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
 */
class Material
{
private:
    void init(void);    ///< initialize material values
    int parseRGB(float rgb[3], const std::string val);
    int parseFloat(float &param, const std::string val);
    int validateRGB(const float rgb[3], float target[3]);
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

    int  Load(const std::string &name);
    // XXX - no clear benefit to the Save command
    // int  Save(const std::string &name);

    void SetName(const std::string &name);
    const std::string &GetName(void);

    int  SetDiffuse(const float rgb[3]);
    void GetDiffuse(float rgb[3]);

    int  SetEmissivity(const float rgb[3]);
    void GetEmissivity(float rgb[3]);

    int  SetSpecular(const float rgb[3]);
    void GetSpecular(float rgb[3]);

    int  SetAmbientIntensity(float coeff);
    float GetAmbientIntensity(void);

    int  SetTransparency(float coeff);
    float GetTransparency(void);

    int  SetShininess(float coeff);
    float GetShininess(void);
};

#endif  // MATERIAL_H
