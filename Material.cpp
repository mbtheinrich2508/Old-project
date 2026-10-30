//
// Created by Peter Zdankin on 27.11.24.
//
#include <cmath>
#include "Material.hpp"

#include "Vector3.hpp"

double Material::getExponent() const{
    return _exponent;
}

bool Material::reflects() const{
    return _local < 1.0;
}
bool Material::refracts() const{
    return _local < 1.0 && _index_of_refraction > 0.0;
}
double Material::getLocalReflectivity() const{
    return _local;
}
double Material::getIndexOfRefraction() const{
    return _index_of_refraction;
}
const vec3& Material::getAmbient() const{
    return _ambient;
}
const vec3& Material::getDiffuse() const{
    return _diffuse;
}
const vec3& Material::getSpecular() const{
    return _specular;
}

double Material::getReflectivity(double cosI) const {

    double R0 = 1 - _local;

    int sign = (cosI < 0) ? -1 : 1;

    if (_index_of_refraction != 0.0) {
        double n = (sign == 1) ? _index_of_refraction : 1.0 / _index_of_refraction;
        double R0sqrt = (n - 1) / (n + 1);
        R0 = R0sqrt * R0sqrt;
    }

    return R0 + (1 - R0) * pow(1 - sign * cosI, 5);
}

