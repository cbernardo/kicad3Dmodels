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

/**
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
class Material
{
private:
    void init(void);                                        ///< initialize material values
    int parseRGB(float rgb[3], const std::string val);      ///< read an R G B triplet
    int parseFloat(float &param, const std::string val);    ///< read a floating point
    int validateRGB(const float rgb[3], float target[3]);   ///< validate
    std::string name;

protected:
    std::string material;   ///< user given material name
    float diffuse[3];   ///< coefficients of diffuse reflection for R,G,B, range 0..1
    float emissive[3];  ///< coefficients of emission for R,G,B; generally zero, range 0..1
    float specular[3];  ///< coefficients of specular reflection for R,G,B, range 0..1
    float ambient;      ///< coefficient for ambient intensity contribution. range 0..1
    float transparency; ///< coefficient of transparency, range 0 (transparent) .. 1 (opaque)
    float shininess;    ///< shininess of an object, range 0 (no glint) .. 1 (reflection at oblique angles)

public:
    Material();
    virtual ~Material();

    /**
     * Load a material appearance specification file.
     *
     * @param name [in] path to material specification file
     * @return 0 for success, -1 for failure
     */
    int  Load(const std::string &name);

    /**
     * Set the name of the material appearance.  This name may be used when writing out the
     * material information.
     *
     * @param name [in] material name
     */
    void SetName(const std::string &name);
    /**
     * Retrieve the material name.
     *
     * @return material name.
     */
    const std::string &GetName(void);

    /**
     * Set the material diffuse reflectance values
     *
     * @param rgb [in] VRML2.0 RGB values for diffuse reflectance (range: 0..1)
     * @return 0 for success, -1 for failure
     */
    int  SetDiffuse(const float rgb[3]);
    /**
     * Retrieve the material's diffuse reflectance parameters
     *
     * @param rgb [out] RGB diffuse reflectance values (range: 0..1)
     */
    void GetDiffuse(float rgb[3]);

    /**
     * Set the material emissivity values
     *
     * @param rgb [in] VRML2.0 RGB values for emissivity (range: 0..1)
     * @return 0 for success, -1 for failure
     */
    int  SetEmissivity(const float rgb[3]);
    /**
     * Retrieve the material emissivity values
     *
     * @param rgb [out] RGB emissivity values (range: 0..1)
     */
    void GetEmissivity(float rgb[3]);

    /**
     * Set the material specular reflectance values.
     *
     * @param rgb [in] VRML2.0 RGB values for specular reflectance (range: 0..1)
     * @return 0 for success, -1 for failure
     */
    int  SetSpecular(const float rgb[3]);
    /**
     * Retrieve the material specular reflectance values
     *
     * @param rgb [out] RGB values for specular reflectance (range: 0..1)
     */
    void GetSpecular(float rgb[3]);

    /**
     * Set the material ambient intensity value
     *
     * @param coeff [in] VRML2.0 Ambient Intensity coefficient
     * @return 0 for success, -1 for failure
     */
    int  SetAmbientIntensity(float coeff);
    /**
     * Retrieve the material ambient intensity value
     *
     * @return ambient intensity (as per VRML2.0 specification)
     */
    float GetAmbientIntensity(void);

    /**
     * Set the material transparency
     *
     * @param coeff [in] transparency; range is 0 (opaque) to 1 (completely transparent)
     * @return 0 for success, -1 for failure
     */
    int  SetTransparency(float coeff);
    /**
     * Retrieve the material transparency
     *
     * @return material transparency; range 0 (opaque) to 1 (transparent)
     */
    float GetTransparency(void);

    /**
     * Set the material shininess parameter
     *
     * @param coeff [in] range 0 (no glint) to 1 (mirror surface)
     * @return 0 for success, -1 for failure
     */
    int  SetShininess(float coeff);
    /**
     * Retrieve the material shininess
     *
     * @return shininess; 0 (no glint) to 1 (mirror)
     */
    float GetShininess(void);
};

#endif  // MATERIAL_H
