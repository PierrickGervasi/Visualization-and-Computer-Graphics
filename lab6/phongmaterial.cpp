/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Tuesday, October 24, 2017 - 17:17:44
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <labraytracer/phongmaterial.h>
#include <labraytracer/util.h>

namespace inviwo {

PhongMaterial::PhongMaterial(const vec3& color, const double reflectance, const double shininess,
    const vec3& ambientMaterialColor, const vec3& diffuseMaterialColor, const vec3& specularMatierialColor)
    : Material(color, reflectance) {

    constexpr double LightIntensity = 100.0;



    shininess_ = shininess;
    ambientMaterialColor_   = Util::scalarMult(LightIntensity, ambientMaterialColor);
    diffuseMaterialColor_   = Util::scalarMult(LightIntensity, diffuseMaterialColor);
    specularMatierialColor_ = Util::scalarMult(LightIntensity, specularMatierialColor);
}


vec4 PhongMaterial::shade(const RayIntersection& intersection, const Light& light) const {
    // get normal and light direction
    vec3 N = intersection.getNormal();
    vec3 L = Util::normalize(light.getPosition() - intersection.getPosition());

    double cosNL = std::max(double(dot(N, L)), double(0)); //to avoid zero dot products

    //DIFFUSE part
    vec3 vec_distance = light.getPosition()-intersection.getPosition();//look at 2. below

    double dist = sqrt(pow(vec_distance[0],2) + pow(vec_distance[1],2) + pow(vec_distance[2],2));

    vec3 c_diffuse = vec3((diffuseMaterialColor_[0]*light.getDiffuseColor()[0]*cosNL)/pow(dist,2),
    (diffuseMaterialColor_[1]*light.getDiffuseColor()[1]*cosNL)/pow(dist,2),
    (diffuseMaterialColor_[2]*light.getDiffuseColor()[2]*cosNL)/pow(dist,2));

    //vec3 c_diffuse = (diffuseMaterialColor_*light.getDiffuseColor()*cosNL)/pow(dist,2);

    //SPECULAR part
    //vec3 N2 = vec3(N[0]*2,N[1]*2, N[2]*2);
    vec3 N_hat = Util::normalize(N);
    vec3 N2 = Util::scalarMult(2, N_hat);

    double temp = std::max(double(dot(N_hat, L)), double(0));

    vec3 R0 = Util::scalarMult(temp,N2);
    vec3 R = -(R0-L);
    // double Rsum = sqrt(pow(R[0],2) + pow(R[1],2) + pow(R[2],2));
    // vec3 Rn = vec3(R[0] / Rsum,
    // R[1] / Rsum,
    // R[2] / Rsum);

    vec3 Rn = Util::normalize(R);

    vec3 V = intersection.getRay().getDirection();
    // double Vsum = sqrt(pow(V[0],2) + pow(V[1],2) + pow(V[2],2));
    // vec3 Vn = vec3(V[0] / Vsum,
    // V[1] / Vsum,
    // V[2] / Vsum);

    vec3 Vn = Util::normalize(V);

    double temp2 = std::max(double(dot(Rn, Vn)), double(0));
    vec3 c_specular = vec3(specularMatierialColor_[0]*light.getSpecularColor()[0]*pow(temp2,shininess_),
    specularMatierialColor_[1]*light.getSpecularColor()[1]*pow(temp2,shininess_),
    specularMatierialColor_[2]*light.getSpecularColor()[2]*pow(temp2,shininess_));

    // vec3 c_specular = vec3(light.getSpecularColor()[0]*pow(dot(Rn,Vn),shininess_),
    // light.getSpecularColor()[1]*pow(dot(Rn,Vn),shininess_),
    // light.getSpecularColor()[2]*pow(dot(Rn,Vn),shininess_));

    //vec3 c_specular = Util::scalarMult(pow(dot(Rn,Vn),shininess_),light.getSpecularColor());

    using std::cout;
    using std::cerr;
    using std::endl;
    //vec3 c_specular = light.getSpecularColor()*pow(dot(Rn,Vn),shininess)

    vec3 c_phong = c_specular+c_diffuse;
    //vec3 c_phong = c_specular;
    //cerr <<"specular " << c_specular <<endl;
    return vec4(c_phong[0],c_phong[1],c_phong[2],1);
    //return vec4(0,0,c_phong[2],1);



    // Programming Task 2: Extend this method.
    // This method currently implements a Lambert's material with ideal
    // diffuse reflection.
    // Your task is to implement a Phong shading model.
    //
    // Hints:
    //
    // 1. This function should return the sum of diffuse and specular parts (no ambient part)
    // 2. The used light color for each type (diffuse/specular) from the light source
    //    is the light color divided by the quadratic distance of the light source from
    //    the point of intersection. (quadratic falloff)
    // 3. The view vector V is the direction of the ray which intersects the object.
    // 4. The rest of the terms as per the slides are as follows
    // 5. You have to take into account shininess_ (p), material colors, light colors
    //    light, view, reflection and normal vector.
    //
    //
    //return vec4(Util::scalarMult(cosNL, this->color()), 1.0);

}


} // namespace
