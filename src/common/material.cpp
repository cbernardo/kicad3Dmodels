/*
 *      file: material.cpp
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
 */

#include <sstream>
#include <iostream>
#include <iomanip>

#include <vdefs.h>
#include <material.h>
#include <keyvalparser.h>

using namespace std;
using namespace KC3D;

MATERIAL::MATERIAL()
{
    init();
}


MATERIAL::~MATERIAL()
{
    material.clear();
}


void MATERIAL::init( void )
{
    material.clear();
    int i;

    for( i = 0; i < 3; ++i )
    {
        diffuse[i]  = 0.0f;
        emissive[i] = 0.0f;
        specular[i] = 0.0f;
    }

    ambient = 0.0f;
    transparency = 1.0f;
    shininess = 0.0f;
}


int MATERIAL::Load( const std::string& aMaterialFile )
{
    KEYVAL_PARSER kp;
    int nl = 0;             // number of required keys parsed
    MATERIAL::fname.clear();

    if( kp.LoadKeys( aMaterialFile ) <= 0 )
        return -1;

    const map<string, string>& keys = kp.GetKeys();
    map<string, string>::const_iterator ki;

    init();

    // name
    ki = keys.find( "name" );

    if( ki != keys.end() )
    {
        material = ki->second;
    }
    else
    {
        ERRBLURB;
        cerr << "No material name found in file '" << aMaterialFile << "'\n";
        return -1;
    }

    // diffusivity
    ki = keys.find( "diffuse" );

    if( ki != keys.end() )
    {
        if( parseRGB( diffuse, ki->second ) )
        {
            ERRBLURB;
            cerr << "file: " << aMaterialFile << "\n";
            cerr << "\tfailed to parse diffuse values in string '" << ki->second << "'\n";
        }
        else
        {
            ++nl;
        }
    }
    else
    {
        ERRBLURB;
        cerr << "file: " << aMaterialFile << "\n";
        cerr << "\t[info] no diffuse coefficients specified\n";
    }

    // emissivity (optional)
    ki = keys.find( "emissive" );

    if( ki != keys.end() )
    {
        if( parseRGB( emissive, ki->second ) )
        {
            ERRBLURB;
            cerr << "file: " << aMaterialFile << "\n";
            cerr << "\tfailed to parse emissive values in string '" << ki->second << "'\n";
        }
    }

    // specular
    ki = keys.find( "specular" );

    if( ki != keys.end() )
    {
        if( parseRGB( specular, ki->second ) )
        {
            ERRBLURB;
            cerr << "file: " << aMaterialFile << "\n";
            cerr << "\tfailed to parse specular values in string '" << ki->second << "'\n";
        }
        else
        {
            ++nl;
        }
    }
    else
    {
        ERRBLURB;
        cerr << "file: " << aMaterialFile << "\n";
        cerr << "\t[info] no specular coefficients specified\n";
    }

    if( nl < 1 )
    {
        ERRBLURB;
        cerr << "file: " << aMaterialFile << "\n";
        cerr << "\t[warn] neither diffuse nor specular reflection were specified\n";
    }

    // ambient (optional)
    ki = keys.find( "ambient" );

    if( ki != keys.end() )
    {
        if( parseFloat( ambient, ki->second ) )
        {
            ERRBLURB;
            cerr << "file: " << aMaterialFile << "\n";
            cerr << "\tfailed to parse ambient intensity value in string '" << ki->second << "'\n";
        }
    }

    // transparency (optional)
    ki = keys.find( "transparency" );

    if( ki != keys.end() )
    {
        if( parseFloat( transparency, ki->second ) )
        {
            ERRBLURB;
            cerr << "file: " << aMaterialFile << "\n";
            cerr << "\tfailed to parse transparency value in string '" << ki->second << "'\n";
        }
    }

    // shininess (optional)
    ki = keys.find( "shininess" );

    if( ki != keys.end() )
    {
        if( parseFloat( shininess, ki->second ) )
        {
            ERRBLURB;
            cerr << "file: " << aMaterialFile << "\n";
            cerr << "\tfailed to parse shininess value in string '" << ki->second << "'\n";
        }
    }

    MATERIAL::fname = aMaterialFile;
    return 0;
}


int MATERIAL::parseRGB( float aRGB[3], const std::string aRGBString )
{
    istringstream istr( aRGBString );
    float   r   = -1.0;
    float   g   = -1.0;
    float   b   = -1.0;

    istr >> r >> g >> b;

    if( (r < 0.0)||(r > 1.0)||(g < 0.0)||(g > 1.0)||(b < 0.0)||(b > 1.0) )
    {
        ERRBLURB;
        cerr << "file: " << fname << "\n";
        cerr << "\tinvalid RGB values (range is 0 .. 1); see message below\n";
        return -1;
    }

    aRGB[0]  = r;
    aRGB[1]  = g;
    aRGB[2]  = b;

    return 0;
}


int MATERIAL::parseFloat( float& aFloat, const std::string aFloatString )
{
    istringstream istr( aFloatString );
    float v = -1.0;

    istr >> v;

    if( (v < 0.0)||(v > 1.0) )
    {
        ERRBLURB;
        cerr << "file: " << fname << "\n";
        cerr << "\tinvalid coefficient (range is 0 .. 1); see message below\n";
        return -1;
    }

    aFloat = v;

    return 0;
}


int MATERIAL::validateRGB( const float aSrcRGB[3], float aDestRGB[3] )
{
    if( (aSrcRGB[0] < 0.0) || (aSrcRGB[0] > 1.0)
        || (aSrcRGB[1] < 0.0) || (aSrcRGB[1] > 1.0)
        || (aSrcRGB[2] < 0.0) || (aSrcRGB[2] > 1.0) )
    {
        ERRBLURB;
        cerr << "Invalid RGB value (range is 0.0 .. 1.0): " << setprecision( 2 );
        cerr << aSrcRGB[0] << ", " << aSrcRGB[1] << ", " << aSrcRGB[2] << "\n";
        return -1;
    }

    aDestRGB[0]   = aSrcRGB[0];
    aDestRGB[1]   = aSrcRGB[1];
    aDestRGB[2]   = aSrcRGB[2];

    return 0;
}


void MATERIAL::SetName( const std::string& aMaterialName )
{
    material = aMaterialName;
}


const std::string& MATERIAL::GetName( void )
{
    return material;
}


int MATERIAL::SetDiffuse( const float aRGB[3] )
{
    if( validateRGB( aRGB, diffuse ) )
    {
        ERRBLURB;
        cerr << "Error setting diffuse values; see message above\n";
        return -1;
    }

    return 0;
}


void MATERIAL::GetDiffuse( float aRGB[3] )
{
    aRGB[0]  = diffuse[0];
    aRGB[1]  = diffuse[1];
    aRGB[2]  = diffuse[2];
}


int MATERIAL::SetEmissivity( const float aRGB[3] )
{
    if( validateRGB( aRGB, emissive ) )
    {
        ERRBLURB;
        cerr << "Error setting emissivity values; see message above\n";
        return -1;
    }

    return 0;
}


void MATERIAL::GetEmissivity( float aRGB[3] )
{
    aRGB[0]  = emissive[0];
    aRGB[1]  = emissive[1];
    aRGB[2]  = emissive[2];
}


int MATERIAL::SetSpecular( const float aRGB[3] )
{
    if( validateRGB( aRGB, specular ) )
    {
        ERRBLURB;
        cerr << "Error setting specular values; see message above\n";
        return -1;
    }

    return 0;
}


void MATERIAL::GetSpecular( float aRGB[3] )
{
    aRGB[0]  = specular[0];
    aRGB[1]  = specular[1];
    aRGB[2]  = specular[2];
}


int MATERIAL::SetAmbientIntensity( float aAmbientCoefficient )
{
    if( (aAmbientCoefficient < 0.0) || (aAmbientCoefficient > 1.0) )
    {
        ERRBLURB;
        cerr << "Invalid value for Ambient Intensity (" << aAmbientCoefficient;
        cerr << "). Valid range is 0.0 .. 1.0\n";
        return -1;
    }

    ambient = aAmbientCoefficient;
    return 0;
}


float MATERIAL::GetAmbientIntensity( void )
{
    return ambient;
}


int MATERIAL::SetTransparency( float aTransparency )
{
    if( (aTransparency < 0.0) || (aTransparency > 1.0) )
    {
        ERRBLURB;
        cerr << "Invalid value for transparency (" << aTransparency;
        cerr << "). Valid range is 0.0 .. 1.0\n";
        return -1;
    }

    transparency = aTransparency;
    return 0;
}


float MATERIAL::GetTransparency( void )
{
    return transparency;
}


int MATERIAL::SetShininess( float aShininess )
{
    if( (aShininess < 0.0) || (aShininess > 1.0) )
    {
        ERRBLURB;
        cerr << "Invalid value for shininess (" << aShininess;
        cerr << "). Valid range is 0.0 .. 1.0\n";
        return -1;
    }

    shininess = aShininess;
    return 0;
}


float MATERIAL::GetShininess( void )
{
    return shininess;
}
