//
// Created by Peter Zdankin on 13.11.24.
//

#include "Intersection.hpp"

Intersection::Intersection(Material& material, vec3 normal, double t) : _material(material), _normal(normal), _t(t) {}

const Material& Intersection::getMaterial() const {
    return _material;
}
const vec3& Intersection::getNormal() const {
    return _normal;
}
double Intersection::getT() const {
    return _t;
}
