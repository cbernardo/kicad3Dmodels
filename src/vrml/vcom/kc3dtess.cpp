/*
 *      file: kc3dtess.cpp
 *
 *      Copyright 2012-2014 Dr. Cirilo Bernardo (cjh.bernardo@gmail.com)
 *
 *      This program is free software: you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation, either version 3 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program.  If not, see <http://www.gnu.org/licenses/>
 *
 *      This class creates a tessellated surface given a solid outline in the
 *      XY plane and a list of cutouts.
 *
 */

#include <sstream>
#include <string>
#include <iomanip>
#include <cmath>
#include <kc3dtess.h>
#include <polygon.h>
#include <transform.h>
#include <vcom.h>

#ifndef CALLBACK
#define CALLBACK
#endif

#define GLCALLBACK(x) (( void (CALLBACK*)() )&(x))

using namespace std;
using namespace KC3D;

namespace KC3D
{

static void FormatDoublet( double x, double y, int precision, std::string& strx, std::string& stry )
{
    std::ostringstream ostr;

    ostr << std::fixed << std::setprecision( precision );

    ostr << x;
    strx = ostr.str();

    ostr.str( "" );
    ostr << y;
    stry = ostr.str();

    while( *strx.rbegin() == '0' )
        strx.erase( strx.size() - 1 );

    while( *stry.rbegin() == '0' )
        stry.erase( stry.size() - 1 );
}


static void FormatSinglet( double x, int precision, std::string& strx )
{
    std::ostringstream ostr;

    ostr << std::fixed << std::setprecision( precision );

    ostr << x;
    strx = ostr.str();

    while( *strx.rbegin() == '0' )
        strx.erase( strx.size() - 1 );
}


static void CALLBACK vrml_tess_begin( GLenum cmd, void* user_data )
{
    TESSELATOR* lp = (TESSELATOR*) user_data;

    lp->glStart( cmd );
}


static void CALLBACK vrml_tess_end( void* user_data )
{
    TESSELATOR* lp = (TESSELATOR*) user_data;

    lp->glEnd();
}


static void CALLBACK vrml_tess_vertex( void* vertex_data, void* user_data )
{
    TESSELATOR* lp = (TESSELATOR*) user_data;

    lp->glPushVertex( (VERTEX_3D*) vertex_data );
}


static void CALLBACK vrml_tess_err( GLenum errorID, void* user_data )
{
    TESSELATOR* lp = (TESSELATOR*) user_data;

    lp->Fault = true;
    lp->SetGLError( errorID );
}


static void CALLBACK vrml_tess_combine( GLdouble coords[3], VERTEX_3D* vertex_data[4],
                                        GLfloat weight[4], void** outData, void* user_data )
{
    TESSELATOR* lp = (TESSELATOR*) user_data;
    *outData = lp->AddExtraVertex( coords[0], coords[1] );

    return;
}


TESSELATOR::TESSELATOR()
{
    // arc parameters suitable to mm measurements
    maxArcSeg = 48;
    minSegLength = 0.1;
    maxSegLength = 0.5;

    fix = false;
    Fault = false;
    idx = 0;
    ord = 0;
    glcmd   = 0;

    tess = gluNewTess();

    if( !tess )
        return;

    // set up the tesselator callbacks
    gluTessCallback( tess, GLU_TESS_BEGIN_DATA, GLCALLBACK( vrml_tess_begin ) );

    gluTessCallback( tess, GLU_TESS_VERTEX_DATA, GLCALLBACK( vrml_tess_vertex ) );

    gluTessCallback( tess, GLU_TESS_END_DATA, GLCALLBACK( vrml_tess_end ) );

    gluTessCallback( tess, GLU_TESS_ERROR_DATA, GLCALLBACK( vrml_tess_err ) );

    gluTessCallback( tess, GLU_TESS_COMBINE_DATA, GLCALLBACK( vrml_tess_combine ) );

    gluTessProperty( tess, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_POSITIVE );

    gluTessNormal( tess, 0, 0, 1 );
}


TESSELATOR::~TESSELATOR()
{
    Clear();

    if( tess )
    {
        gluDeleteTess( tess );
        tess = NULL;
    }
}


// clear all data
void TESSELATOR::Clear( void )
{
    int i;

    fix = false;
    idx = 0;

    for( i = contours.size(); i > 0; --i )
    {
        delete contours.back();
        contours.pop_back();
    }

    areas.clear();

    for( i = vertices.size(); i > 0; --i )
    {
        delete vertices.back();
        vertices.pop_back();
    }

    clearTmp();
}


// clear ephemeral data in between invocations of the tesselation routine
void TESSELATOR::clearTmp( void )
{
    unsigned int i;

    Fault   = false;
    eidx    = 0;
    ord = 0;
    glcmd = 0;

    triplets.clear();

    for( i = outline.size(); i > 0; --i )
    {
        delete outline.back();
        outline.pop_back();
    }

    ordmap.clear();

    for( i = extra_verts.size(); i > 0; --i )
    {
        delete extra_verts.back();
        extra_verts.pop_back();
    }

    // note: unlike outline and extra_verts,
    // vlist is not responsible for memory management
    vlist.clear();

    // go through the vertex list and reset ephemeral parameters
    for( i = 0; i < vertices.size(); ++i )
    {
        vertices[i]->o = -1;
    }
}


// create a new contour to be populated; returns an index
// into the contour list or -1 if there are problems
int TESSELATOR::NewContour( void )
{
    if( fix )
        return -1;

    std::list<int>* contour = new std::list<int>;

    if( !contour )
        return -1;

    contours.push_back( contour );
    areas.push_back( 0.0 );

    return contours.size() - 1;
}


// adds a vertex to the existing list and places its index in
// an existing contour; returns true if OK,
// false otherwise (indexed contour does not exist)
bool TESSELATOR::AddVertex( int aContourID, double aXpos, double aYpos )
{
    if( fix )
    {
        error = "AddVertex(): no more vertices may be added (Tesselate was previously executed)";
        return false;
    }

    if( aContourID < 0 || (unsigned int) aContourID >= contours.size() )
    {
        error = "AddVertex(): aContour is not within a valid range";
        return false;
    }

    VERTEX_3D* vertex = new VERTEX_3D;

    if( !vertex )
    {
        error = "AddVertex(): a new vertex could not be allocated";
        return false;
    }

    vertex->x   = aXpos;
    vertex->y   = aYpos;
    vertex->i   = idx++;
    vertex->o   = -1;

    VERTEX_3D* v2 = NULL;

    if( contours[aContourID]->size() > 0 )
        v2 = vertices[ contours[aContourID]->back() ];

    vertices.push_back( vertex );
    contours[aContourID]->push_back( vertex->i );

    if( v2 )
        areas[aContourID] += ( aXpos - v2->x ) * ( aYpos + v2->y );

    return true;
}


bool TESSELATOR::AddPolygon( const KC3D::POLYGON& aPolygon, bool aHoleFlag )
{
    if( fix )
    {
        error = "AddPolygon(): no more vertices may be added (Tesselate was previously executed)";
        return false;
    }

    double* px;
    double* py;
    double* pz;

    int nVerts = aPolygon.GetVertices( &px, &py, &pz );

    if( nVerts < 3 )
    {
        error = "AddPolygon(): polygon has < 3 vertices";
        return false;
    }

    int poly = NewContour();

    if( poly < 0 )
        return false;

    for( int i = 0; i < nVerts; ++ i)
    {
        VERTEX_3D* vertex = new VERTEX_3D;

        if( !vertex )
        {
            error = "AddPolygon(): a new vertex could not be allocated";
            return false;
        }

        vertex->x   = px[i];
        vertex->y   = py[i];
        vertex->i   = idx++;
        vertex->o   = -1;

        VERTEX_3D* v2 = NULL;

        if( contours[poly]->size() > 0 )
            v2 = vertices[ contours[poly]->back() ];

        vertices.push_back( vertex );
        contours[poly]->push_back( vertex->i );

        if( v2 )
            areas[poly] += ( px[i] - v2->x ) * ( py[i] + v2->y );
    }

    EnsureWinding( poly, aHoleFlag );

    return true;
}


// ensure the winding of a contour with respect to the normal (0, 0, 1);
// set 'hole' to true to ensure a hole (clockwise winding)
bool TESSELATOR::EnsureWinding( int aContourID, bool aHoleFlag )
{
    if( aContourID < 0 || (unsigned int) aContourID >= contours.size() )
    {
        error = "EnsureWinding(): aContour is outside the valid range";
        return false;
    }

    std::list<int>* cp = contours[aContourID];

    if( cp->size() < 3 )
    {
        error = "EnsureWinding(): there are fewer than 3 vertices";
        return false;
    }

    double dir = areas[aContourID];

    VERTEX_3D* vp0 = vertices[ cp->back() ];
    VERTEX_3D* vp1 = vertices[ cp->front() ];

    dir += ( vp1->x - vp0->x ) * ( vp1->y + vp0->y );

    // if dir is positive, winding is CW
    if( ( aHoleFlag && dir < 0 ) || ( !aHoleFlag && dir > 0 ) )
    {
        cp->reverse();
        areas[aContourID] = -areas[aContourID];
    }

    return true;
}


// tesselates the contours in preparation for a 3D output;
// returns true if all was fine, false otherwise
bool TESSELATOR::Tesselate( void )
{
    if( !tess )
    {
        error = "Tesselate(): GLU tesselator was not initialized";
        return false;
    }

    Fault   = false;

    gluTessProperty( tess, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_POSITIVE );

    if( contours.size() < 1 || vertices.size() < 3 )
    {
        error = __FUNCTION__;
        error.append( "(): not enough vertices" );
        return false;
    }

    // finish the winding calculation on all vertices prior to setting 'fix'
    if( !fix )
    {
        for( unsigned int i = 0; i < contours.size(); ++i )
        {
            if( contours[i]->size() < 3 )
                continue;

            VERTEX_3D* vp0 = vertices[ contours[i]->back() ];
            VERTEX_3D* vp1 = vertices[ contours[i]->front() ];
            areas[i] += ( vp1->x - vp0->x ) * ( vp1->y + vp0->y );
        }
    }

    // prevent the addition of any further contours and contour vertices
    fix = true;

    // clear temporary internals which may have been used in a previous run
    clearTmp();

    // request an outline
    gluTessProperty( tess, GLU_TESS_BOUNDARY_ONLY, GL_TRUE );

    eidx = idx;

    if( checkNContours( false ) == 0 )
    {
        error = "tesselate(): no solid contours";
        return false;
    }

    // open the polygon
    gluTessBeginPolygon( tess, this );

    // add solid outlines
    pushVertices( false );

    // close the polygon
    gluTessEndPolygon( tess );

    if( Fault )
        return false;

    // if there are no outlines we cannot proceed
    if( outline.empty() )
    {
        error = "tesselate(): no points in result";
        return false;
    }

    // at this point we have a solid outline; add it to the tesselator
    gluTessBeginPolygon( tess, this );

    if( !pushOutline( ) )
        return false;

    // add the holes contained by this object
    pushVertices( true );

    if( Fault )
        return false;

    // erase the previous outline data and vertex order
    // but preserve the extra vertices
    while( !outline.empty() )
    {
        delete outline.back();
        outline.pop_back();
    }

    ordmap.clear();
    ord = 0;

    // go through the vertex lists and reset ephemeral parameters
    for( unsigned int i = 0; i < vertices.size(); ++i )
    {
        vertices[i]->o = -1;
    }

    for( unsigned int i = 0; i < extra_verts.size(); ++i )
    {
        extra_verts[i]->o = -1;
    }

    // close the polygon; this creates the outline points
    // and the point ordering list 'ordmap'
    gluTessEndPolygon( tess );

    // repeat the last operation but request a tesselated surface
    // rather than an outline; this creates the triangles list.
    gluTessProperty( tess, GLU_TESS_BOUNDARY_ONLY, GL_FALSE );

    gluTessBeginPolygon( tess, this );

    if( !pushOutline() )
        return false;

    gluTessEndPolygon( tess );

    if( Fault )
        return false;

    return true;
}


bool TESSELATOR::pushOutline( void )
{
    // traverse the outline list to push all used vertices
    if( outline.size() < 1 )
    {
        error = "pushOutline() failed: no vertices to push";
        return false;
    }

    std::list<std::list<int>*>::const_iterator obeg = outline.begin();
    std::list<std::list<int>*>::const_iterator oend = outline.end();

    int nc = 0; // number of contours pushed

    int pi;
    std::list<int>::const_iterator  begin;
    std::list<int>::const_iterator  end;
    GLdouble pt[3];
    VERTEX_3D* vp;

    while( obeg != oend )
    {
        if( (*obeg)->size() < 3 )
        {
            ++obeg;
            continue;
        }

        gluTessBeginContour( tess );

        begin = (*obeg)->begin();
        end = (*obeg)->end();

        while( begin != end )
        {
            pi = *begin;

            if( pi < 0 || (unsigned int) pi > ordmap.size() )
            {
                gluTessEndContour( tess );
                error = "pushOutline():BUG: *outline.begin() is not a valid index to ordmap";
                return false;
            }

            // retrieve the actual index
            pi = ordmap[pi];

            vp = getVertexByIndex( pi );

            if( !vp )
            {
                gluTessEndContour( tess );
                error = "pushOutline():: BUG: ordmap[n] is not a valid index to vertices[]";
                return false;
            }

            pt[0]   = vp->x;
            pt[1]   = vp->y;
            pt[2]   = 0.0;
            gluTessVertex( tess, pt, vp );
            ++begin;
        }

        gluTessEndContour( tess );
        ++obeg;
        ++nc;
    }

    if( !nc )
    {
        error = "pushOutline():: no valid contours available";
        return false;
    }

    return true;
}


// writes out the vertex list for a planar feature
bool TESSELATOR::WriteVertices( std::ofstream& aOutFile, int aPrecision,
                                KC3D::TRANSFORM& aTransform, int aTabDepth )
{
    if( ordmap.size() < 3 )
    {
        error = "WriteVertices(): not enough vertices";
        return false;
    }

    if( aPrecision < 4 )
        aPrecision = 4;

    int i, j;

    QUAT q0;

    VERTEX_3D* vp = getVertexByIndex( ordmap[0] );

    if( !vp )
        return false;

    q0 = QUAT( 0, vp->x, vp->y, 0 );
    aTransform.Transform( q0 );

    std::string strx, stry, strz;
    FormatDoublet( q0.x, q0.y, aPrecision, strx, stry );
    FormatSinglet( q0.z, aPrecision, strz );

    if( aTabDepth < 1 )
        aTabDepth = 1;
    else if( aTabDepth > 5 )
        aTabDepth = 5;

    string fmt( (aTabDepth) * 4, ' ' );

    aOutFile << fmt << strx << " " << stry << " " << strz;

    for( i = 1, j = ordmap.size(); i < j; ++i )
    {
        vp = getVertexByIndex( ordmap[i] );

        if( !vp )
            return false;

        q0 = QUAT( 0, vp->x, vp->y, 0 );
        aTransform.Transform( q0 );

        FormatDoublet( q0.x, q0.y, aPrecision, strx, stry );
        FormatSinglet( q0.z, aPrecision, strz );

        if( i & 1 )
            aOutFile << ", " << strx << " " << stry << " " << strz;
        else
            aOutFile << ",\n" << fmt << strx << " " << stry << " " << strz;
    }

    return !aOutFile.fail();
}


// writes out the index list;
// 'top' indicates the vertex ordering and should be
// true for a polygon visible from above the PCB
bool TESSELATOR::WriteIndices( bool aTopFlag, std::ofstream& aOutFile, int aTabDepth )
{
    if( triplets.empty() )
    {
        error = "WriteIndices(): no triplets (triangular facets) to write";
        return false;
    }

    // go through the triplet list and write out the indices based on order
    std::list<TRIPLET_3D>::const_iterator   tbeg    = triplets.begin();
    std::list<TRIPLET_3D>::const_iterator   tend    = triplets.end();

    string fmt( (aTabDepth + 1) * 4, ' ' );
    aOutFile << fmt << "   ";

    int i = 1;

    if( aTopFlag )
        aOutFile << fmt << tbeg->i1 << ", " << tbeg->i2 << ", " << tbeg->i3  << ", -1";
    else
        aOutFile << fmt << tbeg->i2 << ", " << tbeg->i1 << ", " << tbeg->i3  << ", -1";

    ++tbeg;

    while( tbeg != tend )
    {
        if( (i++ & 7) == 4 )
        {
            i = 1;

            if( aTopFlag )
            {
                aOutFile << ",\n" << fmt << tbeg->i1 << ", " << tbeg->i2 << ", ";
                aOutFile << tbeg->i3  << ", -1";
            }
            else
            {
                aOutFile << ",\n" << fmt << tbeg->i2 << ", " << tbeg->i1 << ", ";
                aOutFile << tbeg->i3  << ", -1";
            }
        }
        else
        {
            if( aTopFlag )
                aOutFile << ", " << tbeg->i1 << ", " << tbeg->i2 << ", " << tbeg->i3  << ", -1";
            else
                aOutFile << ", " << tbeg->i2 << ", " << tbeg->i1 << ", " << tbeg->i3  << ", -1";
        }

        ++tbeg;
    }

    return !aOutFile.fail();
}


// add a triangular facet (triplet) to the ouptut index list
bool TESSELATOR::addTriplet( VERTEX_3D* p0, VERTEX_3D* p1, VERTEX_3D* p2 )
{
    double  dx0 = p1->x - p0->x;
    double  dx1 = p2->x - p0->x;

    double  dy0 = p1->y - p0->y;
    double  dy1 = p2->y - p0->y;

    // this number is chosen because we shall only write 9 decimal places
    // at most on the VRML output
    double err = 0.000000001;

    // test if the triangles are degenerate (parallel sides)

    if( dx0 < err && dx0 > -err && dx1 < err && dx1 > -err )
        return false;

    if( dy0 < err && dy0 > -err && dy1 < err && dy1 > -err )
        return false;

    double  sl0 = dy0 / dx0;
    double  sl1 = dy1 / dx1;

    double dsl = sl1 - sl0;

    if( dsl < err && dsl > -err )
        return false;

    triplets.push_back( TRIPLET_3D( p0->o, p1->o, p2->o ) );

    return true;
}


// add an extra vertex (to be called only by the COMBINE callback)
VERTEX_3D* TESSELATOR::AddExtraVertex( double aXpos, double aYpos )
{
    VERTEX_3D* vertex = new VERTEX_3D;

    if( !vertex )
    {
        error = "AddExtraVertex(): could not allocate a new vertex";
        return NULL;
    }

    if( eidx == 0 )
        eidx = idx;

    vertex->x   = aXpos;
    vertex->y   = aYpos;
    vertex->i   = eidx++;
    vertex->o   = -1;

    extra_verts.push_back( vertex );

    return vertex;
}


// start a GL command list
void TESSELATOR::glStart( GLenum cmd )
{
    glcmd = cmd;

    while( !vlist.empty() )
        vlist.pop_back();
}


// process a vertex
void TESSELATOR::glPushVertex( VERTEX_3D* vertex )
{
    if( vertex->o < 0 )
    {
        vertex->o = ord++;
        ordmap.push_back( vertex->i );
    }

    vlist.push_back( vertex );
}


// end a GL command list
void TESSELATOR::glEnd( void )
{
    switch( glcmd )
    {
        case GL_LINE_LOOP:
        {
            // add the loop to the list of outlines
            std::list<int>* loop = new std::list<int>;

            if( !loop )
                break;

            double firstX = 0.0;
            double firstY = 0.0;
            double lastX = 0.0;
            double lastY = 0.0;
            double curX, curY;
            double area = 0.0;

            if( vlist.size() > 0 )
            {
                loop->push_back( vlist[0]->o );
                firstX = vlist[0]->x;
                firstY = vlist[0]->y;
                lastX = firstX;
                lastY = firstY;
            }

            for( size_t i = 1; i < vlist.size(); ++i )
            {
                loop->push_back( vlist[i]->o );
                curX = vlist[i]->x;
                curY = vlist[i]->y;
                area += ( curX - lastX ) * ( curY + lastY );
                lastX = curX;
                lastY = curY;
            }

            area += ( firstX - lastX ) * ( firstY + lastY );

            outline.push_back( loop );
        }
        break;

        case GL_TRIANGLE_FAN:
            processFan();
            break;

        case GL_TRIANGLE_STRIP:
            processStrip();
            break;

        case GL_TRIANGLES:
            processTri();
            break;

        default:
            break;
    }

    while( !vlist.empty() )
        vlist.pop_back();

    glcmd = 0;
}


// set the error message
void TESSELATOR::SetGLError( GLenum errorID )
{
    error = "";
    error = (const char*)gluGetString( errorID );

    if( error.empty() )
    {
        std::ostringstream ostr;
        ostr << "Unknown OpenGL error: " << errorID;
        error = ostr.str();
    }
}


// process a GL_TRIANGLE_FAN list
void TESSELATOR::processFan( void )
{
    if( vlist.size() < 3 )
        return;

    VERTEX_3D* p0 = vlist[0];

    int i;
    int end = vlist.size();

    for( i = 2; i < end; ++i )
    {
        addTriplet( p0, vlist[i - 1], vlist[i] );
    }
}


// process a GL_TRIANGLE_STRIP list
void TESSELATOR::processStrip( void )
{
    // note: (source: http://www.opengl.org/wiki/Primitive)
    // GL_TRIANGLE_STRIP​: Every group of 3 adjacent vertices forms a triangle.
    // The face direction of the strip is determined by the winding of the
    // first triangle. Each successive triangle will have its effective face
    // order reverse, so the system compensates for that by testing it in the
    // opposite way. A vertex stream of n length will generate n-2 triangles.

    if( vlist.size() < 3 )
        return;

    int i;
    int end = vlist.size();
    bool flip = false;

    for( i = 2; i < end; ++i )
    {
        if( flip )
        {
            addTriplet( vlist[i - 1], vlist[i - 2], vlist[i] );
            flip = false;
        }
        else
        {
            addTriplet( vlist[i - 2], vlist[i - 1], vlist[i] );
            flip = true;
        }
    }
}


// process a GL_TRIANGLES list
void TESSELATOR::processTri( void )
{
    // notes:
    // 1. each successive group of 3 vertices is a triangle
    // 2. as per OpenGL specification, any incomplete triangles are to be ignored

    if( vlist.size() < 3 )
        return;

    int i;
    int end = vlist.size();

    for( i = 2; i < end; i += 3 )
        addTriplet( vlist[i - 2], vlist[i - 1], vlist[i] );
}


int TESSELATOR::checkNContours( bool holes )
{
    int nc = 0;     // number of contours

    if( contours.empty() )
        return 0;

    std::list<int>::const_iterator  begin;
    std::list<int>::const_iterator  end;

    for( size_t i = 0; i < contours.size(); ++i )
    {
        if( contours[i]->size() < 3 )
            continue;

        if( ( holes && areas[i] <= 0.0 ) || ( !holes && areas[i] > 0.0 ) )
            continue;

        ++nc;
    }

    return nc;
}


// push the internally held vertices
void TESSELATOR::pushVertices( bool holes )
{
    // push the internally held vertices
    unsigned int i;

    std::list<int>::const_iterator  begin;
    std::list<int>::const_iterator  end;
    GLdouble pt[3];
    VERTEX_3D* vp;

    for( i = 0; i < contours.size(); ++i )
    {
        if( contours[i]->size() < 3 )
            continue;

        if( ( holes && areas[i] <= 0.0 ) || ( !holes && areas[i] > 0.0 ) )
            continue;

        gluTessBeginContour( tess );

        begin = contours[i]->begin();
        end = contours[i]->end();

        while( begin != end )
        {
            vp = vertices[ *begin ];
            pt[0]   = vp->x;
            pt[1]   = vp->y;
            pt[2]   = 0.0;
            gluTessVertex( tess, pt, vp );
            ++begin;
        }

        gluTessEndContour( tess );
    }

    return;
}


VERTEX_3D* TESSELATOR::getVertexByIndex( int aPointIndex )
{
    if( aPointIndex < 0 || (unsigned int) aPointIndex >= ( idx + extra_verts.size() ) )
    {
        error = "getVertexByIndex():BUG: invalid index";
        return NULL;
    }

    if( aPointIndex < idx )
    {
        // vertex is in the vertices[] list
        return vertices[ aPointIndex ];
    }

    // vertex is in the extra_verts[] list
    return extra_verts[aPointIndex - idx];
}


// retrieve the total number of vertices
int TESSELATOR::GetSize( void )
{
    return vertices.size();
}


// return the vertex identified by index
VERTEX_3D* TESSELATOR::GetVertexByIndex( int aPointIndex )
{
    int i0 = vertices[0]->i;

    if( aPointIndex < i0 || aPointIndex >= ( i0 + (int) vertices.size() ) )
    {
        error = "GetVertexByIndex(): invalid index";
        return NULL;
    }

    return vertices[aPointIndex - i0];
}


// return the error string
const std::string& TESSELATOR::GetError( void )
{
    return error;
}


bool TESSELATOR::WriteVRML( bool isCCW, TRANSFORM& aTransform, VRMLMAT& aMaterial,
                            bool reuseMaterial, std::ofstream& aVRMLFile, int aTabDepth )
{
    if( !fix && !Tesselate() )
        return false;

    // set up VRML Shape
    SetupShape( aMaterial, reuseMaterial, aVRMLFile, aTabDepth );

    string fmt( (aTabDepth + 1) * 4, ' ' );

    // enumerate vertices
    // WriteCoord( lx, ly, lz, nv, aVRMLFile, aTabDepth + 1 );
    aVRMLFile << fmt << "coord Coordinate { point [\n";
    WriteVertices( aVRMLFile, 5, aTransform, aTabDepth + 1 );
    aVRMLFile << "]\n";
    aVRMLFile << fmt << "}\n";

    //WriteIndices
    SetupCoordIndex( aVRMLFile, aTabDepth + 1 );
    WriteIndices( isCCW, aVRMLFile, aTabDepth );
    CloseCoordIndex( aVRMLFile, aTabDepth + 1 );

    CloseShape( aVRMLFile, aTabDepth );

    return aVRMLFile.good();
}

}   // namespace KC3D
