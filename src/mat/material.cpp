/*
 *      file: material.cpp
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

#include <sstream>
#include <iostream>
#include <iomanip>

#include "vdefs.h"
#include "material.h"
#include "keyvalparser.h"

using namespace std;

Material::Material()
{
    init();
    return;
}

Material::~Material()
{
    material.clear();
    return;
}

void Material::init(void)
{
    material.clear();
    int i;
    for (i = 0; i < 3; ++i)
    {
        diffuse[i] = 0.0f;
        emissive[i] = 0.0f;
        specular[i] = 0.0f;
    }
    ambient = 0.0f;
    transparency = 1.0f;
    shininess = 0.0f;

    return;
}

int Material::Load(const std::string &name)
{
    KeyValParser kp;
    int nl = 0;     // number of required keys parsed

    if (kp.LoadKeys(name) <= 0) return -1;

    const map<string, string> &keys = kp.GetKeys();
    map<string, string>::const_iterator ki;

    init();

    // name
    ki = keys.find("name");
    if (ki != keys.end())
    {
        material = ki->second;
    }
    else
    {
        cerr << "No material name found in file '" << name << "'\n";
        return -1;
    }
    // cerr << "Material file: '" << name << "'\n";

    // diffusivity
    ki = keys.find("diffuse");
    if (ki != keys.end())
    {
        if (parseRGB(diffuse, ki->second))
        {
            cerr << "\tfailed to parse diffuse values in string '" << ki->second << "'\n";
        }
        else
        {
            ++nl;
        }
    }
    else
    {
        cerr << "\tno diffuse coefficients specified\n";
    }

    // emissivity (optional)
    ki = keys.find("emissive");
    if (ki != keys.end())
    {
        if (parseRGB(emissive, ki->second))
        {
            cerr << "\tfailed to parse emissive values in string '" << ki->second << "'\n";
        }
    }

    // specular
    ki = keys.find("specular");
    if (ki != keys.end())
    {
        if (parseRGB(specular, ki->second))
        {
            cerr << "\tfailed to parse specular values in string '" << ki->second << "'\n";
        }
        else
        {
            ++nl;
        }
    }
    else
    {
        cerr << "\tno specular coefficients specified\n";
    }
    if (nl < 1)
    {
        cerr << "\t**WARNING** neither diffuse nor specular reflection were specified\n";
    }

    // ambient (optional)
    ki = keys.find("ambient");
    if (ki != keys.end())
    {
        if (parseFloat(ambient, ki->second))
        {
            cerr << "\tfailed to parse ambient intensity value in string '" << ki->second << "'\n";
        }
    }

    // transparency (optional)
    ki = keys.find("transparency");
    if (ki != keys.end())
    {
        if (parseFloat(transparency, ki->second))
        {
            cerr << "\tfailed to parse transparency value in string '" << ki->second << "'\n";
        }
    }

    // shininess (optional)
    ki = keys.find("shininess");
    if (ki != keys.end())
    {
        if (parseFloat(shininess, ki->second))
        {
            cerr << "\tfailed to parse shininess value in string '" << ki->second << "'\n";
        }
    }

    return 0;
}


int Material::parseRGB(float rgb[3], const std::string val)
{
    istringstream istr(val);
    float r = -1.0;
    float g = -1.0;
    float b = -1.0;

    istr >> r >> g >> b;
    if ((r < 0.0)||(r > 1.0)||(g < 0.0)||(g > 1.0)||(b < 0.0)||(b > 1.0))
    {
        cerr << "\tinvalid RGB values (range is 0 .. 1); see message below\n";
        return -1;
    }
    rgb[0] = r;
    rgb[1] = g;
    rgb[2] = b;

    return 0;
}

int Material::parseFloat(float &param, const std::string val)
{
    istringstream istr(val);
    float v = -1.0;

    istr >> v;
    if ((v < 0.0)||(v > 1.0))
    {
        cerr << "\tinvalid coefficient (range is 0 .. 1); see message below\n";
        return -1;
    }
    param = v;

    return 0;
}

int Material::validateRGB(const float rgb[3], float target[3])
{

    if ((rgb[0] < 0.0) || (rgb[0] > 1.0)
            || (rgb[1] < 0.0) || (rgb[1] > 1.0)
            || (rgb[2] < 0.0) || (rgb[2] > 1.0))
    {
        ERRBLURB;
        cerr << "Invalid RGB value (range is 0.0 .. 1.0): " << setprecision(2) << rgb[0] <<
                ", " << rgb[1] << ", " << rgb[2] << "\n";
        return -1;
    }
    target[0] = rgb[0];
    target[1] = rgb[1];
    target[2] = rgb[2];

    return 0;
}


void Material::SetName(const std::string &name)
{
    material = name;
    return;
}

const std::string &Material::GetName(void)
{
    return material;
}


int Material::SetDiffuse(const float rgb[3])
{
    if (validateRGB(rgb, diffuse))
    {
        cerr << "Error setting diffuse values; see message above\n";
        return -1;
    }

    return 0;
}

void Material::GetDiffuse(float rgb[3])
{
    rgb[0] = diffuse[0];
    rgb[1] = diffuse[1];
    rgb[2] = diffuse[2];
    return;
}


int Material::SetEmissivity(const float rgb[3])
{
    if (validateRGB(rgb, emissive))
    {
        cerr << "Error setting emissivity values; see message above\n";
        return -1;
    }

    return 0;
}

void Material::GetEmissivity(float rgb[3])
{
    rgb[0] = emissive[0];
    rgb[1] = emissive[1];
    rgb[2] = emissive[2];
    return;
}


int Material::SetSpecular(const float rgb[3])
{
    if (validateRGB(rgb, specular))
    {
        cerr << "Error setting specular values; see message above\n";
        return -1;
    }

    return 0;
}

void Material::GetSpecular(float rgb[3])
{
    rgb[0] = specular[0];
    rgb[1] = specular[1];
    rgb[2] = specular[2];
    return;
}


int Material::SetAmbientIntensity(float coeff)
{
    if ((coeff < 0.0) || (coeff > 1.0))
    {
        ERRBLURB;
        cerr << "Invalid value for Ambient Intensity (" << coeff <<
                "). Valid range is 0.0 .. 1.0\n";
        return -1;
    }
    ambient = coeff;
    return 0;
}

float Material::GetAmbientIntensity(void)
{
    return ambient;
}

int Material::SetTransparency(float coeff)
{
    if ((coeff < 0.0) || (coeff > 1.0))
    {
        ERRBLURB;
        cerr << "Invalid value for transparency (" << coeff <<
                "). Valid range is 0.0 .. 1.0\n";
        return -1;
    }
    transparency = coeff;
    return 0;
}

float Material::GetTransparency(void)
{
    return transparency;
}

int Material::SetShininess(float coeff)
{
    if ((coeff < 0.0) || (coeff > 1.0))
    {
        ERRBLURB;
        cerr << "Invalid value for shininess (" << coeff <<
                "). Valid range is 0.0 .. 1.0\n";
        return -1;
    }
    shininess = coeff;
    return 0;
}

float Material::GetShininess(void)
{
    return shininess;
}
