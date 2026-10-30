//
// Created by Peter Zdankin on 13.11.24.
//


#include "Scene.hpp"

void Scene::addSphere(Sphere object){
    spheres.push_back(object);
}


const vec3 Scene::getBackgroundColor() const{
    return backgroundColor;
}

std::optional<Intersection> Scene::intersect(const Ray& ray) const{

    std::optional<Intersection> result = {};

    for(auto object : spheres){

        std::optional<Intersection> i = ray.intersects(object);

        if( !i.has_value()){
            continue;
        }
        if(!result.has_value() || i->_t < result->_t){
            result = i;
        }
    }

    return result;
}

//For the brave souls of you that try to understand this: I've made some comments
vec3 Scene::traceRay(const Ray& ray, double IoR, int recDepth) const {

    // In Ray-tracing we shoot rays in a scene and they bounce around. How many times we bounce affects the performance
    // and realism. Try setting different recursion depths in main.cpp to see the result.
    // If we finish, we return black
    if (recDepth == 0) {
        return vec3(0, 0, 0);
    }

    std::optional<Intersection> intersection = intersect(ray);
    //Nothing hit, return background colour
    if (!intersection.has_value()) {
        return backgroundColor;
    }

    // We hit something. Quick, get the intersection point and the surface normal
    // A normal vector is a vector that is perpendicular to a surface, i.e. it's the up vector even if you twist and turn
    // the surface
    vec3 intersectionPoint = ray.point_at(intersection->_t - epsilon);
    vec3 normal = intersection->_normal;

    // Once we hit a surface we may need to send out up to 2 more rays. A reflection ray, for e.g. a mirror, and a
    // refraction ray for e.g. glass. There are mixtures like partially opaque metallic objects, play around with it
    vec3 reflection;
    if(intersection->_material.reflects()) {
        Ray reflectionRay(intersectionPoint + normal * epsilon, ray._direction.reflection(normal));
        reflection = traceRay(reflectionRay, IoR, recDepth - 1);
    }else {
        reflection = vec3();
    }

    // The index of refraction (IoR) gives us an indication how much slower light is in a medium in comparison to air.
    // Air usually has an IoR of 1.0, and most materials have an IoR > 1.0. This affects how light is refracted in a medium.
    // Same if you look into a pond and see a fish a couple inches away from where it ought to be
    vec3 refraction;
    if(intersection->_material.refracts()) {
        // total internal refraction may occur, i.e. the ray is lost in that medium. It's just a thing that can happen
        // in light physics
        std::optional<vec3> refractionDir = ray._direction.refraction(normal, intersection->_material.getIndexOfRefraction());
        //Get direction in which the curvature ray points
        if(refractionDir.has_value())
            { // i.e. our refraction is not total internal refraction
            // THIS IS YOUR SECOND PRAKTIKUM TASK, FILL IN THE MISSING SOURCE CODE TO SHOOT REFRACTION RAYS
            // We need to check whether we're in the air or the medium.
            if(IoR == 1.0)
            {
                //Air -> medium
                double nextIoR = intersection->getMaterial().getIndexOfRefraction(); //was given

                //Line 62, 63 guide:
                Ray refractionRay(intersectionPoint - normal * epsilon, refractionDir.value());
                refraction = traceRay(refractionRay, nextIoR, recDepth - 1);
            }
            else
            {
                //Medium -> air
                double nextIoR = 1.0; //was given
                Ray refractionRay(intersectionPoint + normal * epsilon, refractionDir.value());
                refraction = traceRay(refractionRay, nextIoR, recDepth - 1);
            }
        }
    }

    /* We use the Phong lighting model (not to be confused with Phong shading). Ambient + Diffuse + Specular gives us the
     * local colour. Ambient is the always present colour of an object, diffuse is the light-orientation dependent colour
     * of an object. Specular is the shiny highlight on top of an object. That's a simple illumination model and we
     * will use a better one in the coming weeks.
     *
     * For diffuse, we multiply the diffuse colour of an object with a factor. That factor is the dot product between
     * the up vector and the surface normal ( technically both should be inverted, as we want to check the light direction
     * against the surface orientation.
     */
    vec3 diffuse = intersection->_material.getDiffuse() * dot(vec3(0.0,1.0,0.0),normal);
    double val = dot(vec3(0.0,1.0,0.0), ray._direction.reflection(normal));
    if(val < 0) {
        //see what happens if you remove the following line.
        val = 0;
    }

    // specular is somewhat of a hack. (tbh in phong lighting everything is a hack but it looks good right?)
    // We take val ^ exponent which is between 0 and 1. We multiply that with the specular colour of the material.
    // Result? The higher the exponent, the smaller the bright shiny surface. If you want rougher surfaces, lower exponent.
    // Shinier surfaces -> higher exponent
    vec3 specular = intersection->_material.getSpecular() * pow(val, intersection->_material.getExponent());

    // The local_color is the ambient colour (we multiply it with 1/2) + diffuse colour + specular colour
    vec3 local_color = intersection->_material.getAmbient() * vec3(0.5,0.5,0.5) + diffuse + specular;

    local_color.clamp(0.0,1.0);

    double cosI = dot(ray._direction,intersection->getNormal());
    double l = 0, r = 0, t = 0;

    if (intersection->getMaterial().refracts()) {
        l = intersection->getMaterial().getLocalReflectivity();
        r = intersection->getMaterial().getReflectivity(cosI);
        t = 1 - r;
        r = (1 - l) * r;
        t = (1 - l) * t;
    } else if (intersection->getMaterial().reflects()) {
        r = intersection->getMaterial().getReflectivity(cosI);
        l = 1 - r;
    } else {
        l = 1;
    }

    // COMMENT THIS TO TRY OUT ONE OF THE OTHER OPTIONS
    //return local_color * l;
    // UNCOMMENT THE FOLLOWING LINE TO SEE IF YOUR REFLECTIONS WORK
    //return local_color * l +  reflection * r;
    // UNCOMMENT THE FOLLOWING LINE TO SEE IF YOUR REFRACTIONS WORK
    //return local_color * l + refraction * t;
    // UNCOMMENT THE FOLLOWING LINE TO SEE IF REFLECTIONS AND REFRACTIONS WORK
    return local_color * l +  reflection * r + refraction * t;
}