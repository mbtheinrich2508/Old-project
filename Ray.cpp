//
// Created by Peter Zdankin on 13.11.24.
//

#include "Ray.hpp"

#include "Intersection.hpp"
#include "Sphere.hpp"

Ray::Ray(vec3 origin, vec3 direction) : _origin(origin), _direction(direction) {}

std::optional<Intersection> Ray::intersects(Sphere& sphere) const {
    vec3 sphere_to_ray = sphere._center - _origin;
    double tcenter = dot(sphere_to_ray, _direction);
    if(tcenter < 0 ) {
        return {};
    }
    double d_Squared = dot(sphere_to_ray, sphere_to_ray) - tcenter * tcenter;
    if (d_Squared > sphere._radius_squared) {
        return {};
    }

    double distance = sqrt(sphere._radius_squared - d_Squared);
    double t = tcenter - distance;

    if( t < 0) {
        t = tcenter + distance;
    }

    vec3 normal = unit_vector((point_at(t) - sphere._center));

    return std::optional<Intersection>(Intersection(sphere._material, normal, t));

}

vec3 Ray::point_at(double t) const {
    return _origin + t * _direction;
}