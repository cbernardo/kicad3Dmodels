/*
 *      file: material.h
 *
 *      Copyright 2012 Dr. Cirilo Bernardo (cjh.bernardo@gmail.com)
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


#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>

namespace KC3D
{
/**
 * \ingroup common_tools
 * \brief Material appearance
 *
 *  Class Material represents the material appearance according to the
 *  VRML2.0 specification.  KiCAD only supports the most basic material
 *  color definitions and does not support textures. The appearance of
 *  a material will be determined by the diffuse reflection, specular
 *  reflection, transparency, shininess, ambient intensity, and on occasion
 *  the emission. Each light source consists of a direct lighting component
 *  and an ambient component; the ambient intensity value in the material
 *  specification affects the contribution of the ambient component of the
 *  lighting to the appearance of the model; for example if the material's
 *  ambient intensity is set to zero then the ambient component of the
 *  lighting does not affect the material appearance at all. The shininess
 *  controls the reflection of light at oblique angles and simulates the
 *  glint of objects. The emissivity controls the amount of light emitted
 *  by an object; generally this is set to zero but it can be used for
 *  unusual effects; for example the object may have an RGB emissivity of
 *  (0, 0.2, 0) and an RGB specular reflection of (1, 0, 0) - under a bright
 *  white light the object would appear red while in a dim light the object
 *  will glow green. See the VRML2.0 specification for more details of the
 *  lighting calculations.
 *
 *  It is anticipated that the VRML material appearance can be used to
 *  create material appearance specifications for models other than
 *  VRML and that reusing the VRML material appearance specifications
 *  may promote a more uniform appearance of models regardless of the
 *  type.
 */
class MATERIAL
{
private:
    /// initialize material values
    void init( void );
    /// read an R G B triplet from a key string
    int parseRGB( float aRGB[3], const std::string aRGBString );
    /// read a floating point value from a key string
    int parseFloat( float& aFloat, const std::string aFloatString );
    /// validate a SourceRGB set and copy to a DestinationRGB set
    int validateRGB( const float aSrcRGB[3], float aDstRGB[3] );

    std::string fname;  // name of input file (if any)

protected:
    /// user given material name
    std::string material;
    /// coefficients of diffuse reflection for R,G,B, range 0..1
    float   diffuse[3];
    /// coefficients of emission for R,G,B; generally zero, range 0..1
    float   emissive[3];
    /// coefficients of specular reflection for R,G,B, range 0..1
    float   specular[3];
    /// coefficient for ambient intensity contribution. range 0..1
    float   ambient;
    /// coefficient of transparency, range 0 (transparent) .. 1 (opaque)
    float   transparency;
    /// shininess of an object, range 0 (no glint) .. 1 (reflection at oblique angles)
    float   shininess;

public:
    MATERIAL();
    virtual ~MATERIAL();

    /**
     * Load a material appearance specification file.
     *
     * @param aMaterialFile [in] path to material specification file
     * @return 0 for success, -1 for failure
     */
    int Load( const std::string& aMaterialFile );

    /**
     * Set the name of the material appearance.  This name may be used when writing out the
     * material information.
     *
     * @param aMaterialName [in] material name
     */
    void SetName( const std::string& aMaterialName );

    /**
     * Retrieve the material name.
     *
     * @return material name.
     */
    const std::string& GetName( void );

    /**
     * Set the material diffuse reflectance values
     *
     * @param aRGB [in] VRML2.0 RGB values for diffuse reflectance (range: 0..1)
     * @return 0 for success, -1 for failure
     */
    int SetDiffuse( const float aRGB[3] );

    /**
     * Retrieve the material's diffuse reflectance parameters
     *
     * @param aRGB [out] RGB diffuse reflectance values (range: 0..1)
     */
    void GetDiffuse( float aRGB[3] );

    /**
     * Set the material emissivity values
     *
     * @param aRGB [in] VRML2.0 RGB values for emissivity (range: 0..1)
     * @return 0 for success, -1 for failure
     */
    int SetEmissivity( const float aRGB[3] );

    /**
     * Retrieve the material emissivity values
     *
     * @param aRGB [out] RGB emissivity values (range: 0..1)
     */
    void GetEmissivity( float aRGB[3] );

    /**
     * Set the material specular reflectance values.
     *
     * @param aRGB [in] VRML2.0 RGB values for specular reflectance (range: 0..1)
     * @return 0 for success, -1 for failure
     */
    int SetSpecular( const float aRGB[3] );

    /**
     * Retrieve the material specular reflectance values
     *
     * @param aRGB [out] RGB values for specular reflectance (range: 0..1)
     */
    void GetSpecular( float aRGB[3] );

    /**
     * Set the material ambient intensity value
     *
     * @param aAmbientCoefficient [in] VRML2.0 Ambient Intensity coefficient
     * @return 0 for success, -1 for failure
     */
    int SetAmbientIntensity( float aAmbientCoefficient );

    /**
     * Retrieve the material ambient intensity value
     *
     * @return ambient intensity (as per VRML2.0 specification)
     */
    float GetAmbientIntensity( void );

    /**
     * Set the material transparency
     *
     * @param aTransparency [in] transparency; range is 0 (opaque) to 1 (completely transparent)
     * @return 0 for success, -1 for failure
     */
    int SetTransparency( float aTransparency );

    /**
     * Retrieve the material transparency
     *
     * @return material transparency; range 0 (opaque) to 1 (transparent)
     */
    float GetTransparency( void );

    /**
     * Set the material shininess parameter
     *
     * @param aShininess [in] range 0 (no glint) to 1 (mirror surface)
     * @return 0 for success, -1 for failure
     */
    int SetShininess( float aShininess );

    /**
     * Retrieve the material shininess
     *
     * @return shininess; 0 (no glint) to 1 (mirror)
     */
    float GetShininess( void );
};


}       // namespace KC3D

#endif  // MATERIAL_H
