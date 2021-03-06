/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Tuesday, October 17, 2017 - 10:24:56
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <labraytracer/sphere.h>
#include <labraytracer/util.h>

namespace inviwo {

Sphere::Sphere(const vec3& center, const double& radius) {
    center_ = center;
    radius_ = radius;
}

bool Sphere::closestIntersection(const Ray& ray, double maxLambda,
                                 RayIntersection& intersection) const {
    //Programming TASK 1: implement this method
    //Your code should compute the intersection between a ray and a sphere;

    //If you detect an intersection, the return type should look similar to this:
    //if(rayIntersectsSphere)
    //{
    //  intersection = RayIntersection(ray,shared_from_this(),lambda,normalVec,uvw);
    //  return true;
    //}
    //
    //Hints:
    // lambda is the distance form the ray origin an the intersection point.
    // Ray origin p_r : ray.getOrigin()
    // Ray direction t_r : ray.getDirection()
    // If you need the intersection point, use ray.pointOnRay(lambda)
    // You can ignore the uvw (texture coordinates)

    //enligt http://webcache.googleusercontent.com/search?q=cache:ztAP6hlhHqQJ:viclw17.github.io/2018/07/16/raytracing-ray-sphere-intersection/+&cd=22&hl=sv&ct=clnk&gl=se&client=ubuntu


    const vec3 pr = ray.getOrigin();
    const vec3 tr = ray.getDirection();

    //kvadratisk ekvation
    //a*lambda^2 +b*lambda + c = 0

    const double a = dot(tr,tr);
    const double b = 2*dot(tr,(pr-center_));
    const double c = dot((pr-center_),(pr-center_)) -(radius_*radius_);

    const double discriminant = (b*b)-(4*a*c);

    if (discriminant < 0){
      return false; //träffar inte
    }

    // HUR GÖRA OM TVÅ TRÄFFAR?


    double lambda;
    lambda = (-b - sqrt(discriminant))/(2*a); //dubbelrot

    if (lambda < 0){ //göra detta? FRÅGA
      lambda=(-b +sqrt(discriminant))/(2*a); // vad göra??
    }

    if (lambda < 0 || lambda + Util::epsilon > maxLambda) {
        return false;
    }

    const vec3 p = ray.pointOnRay(lambda);// =  pr+ lambda*tr; var ray träffar sfär
    const vec3 normal_ = p-center_; //normalen.



    const vec3 uvw(0, 0, 0);
    intersection = RayIntersection(ray, shared_from_this(), lambda, normal_, uvw);
    return true;





      //
    	// const vec3 o = ray.getOrigin();
    	// const vec3 d = ray.getDirection();
      //
    	// const double b = dot(o, d);
    	// const double c = dot(o, o);
      //
    	// const double D2 = b * b - c + 1;
      //
    	// if (D2 < 0){
    	// 	return false;
    	// }
      //
    	// double lambda = -b - sqrt(D2);
      //
    	// if (lambda < 0){
    	// 	lambda = -b + sqrt(D2);
    	// }
      //
    	// if (lambda < 0 || lambda > maxLambda){
    	// 	return false;
    	// }
      //
    	// const vec3 uvw(0, 0, 0);
      //
    	// intersection = RayIntersection(ray, shared_from_this(), lambda, ray.pointOnRay(lambda), uvw);
      // return true;

    //
    //
    // double d = dot(ray.getDirection(), normal_);
    //
    // if (fabs(d) < Util::epsilon) {
    //     return false;
    // }
    //
    // double a = dot((point_ - ray.getOrigin()), normal_);
    //
    // double lambda = a / d;
    //
    // if (lambda < 0 || lambda + Util::epsilon > maxLambda) {
    //     return false;
    // }
    //
    // const vec3 uvw(0, 0, 0);
    // intersection = RayIntersection(ray, shared_from_this(), lambda, normal_, uvw);
    // return true;
    //
    //
    return false;
}

bool Sphere::anyIntersection(const Ray& ray, double maxLambda) const {
    RayIntersection temp;
    return closestIntersection(ray, maxLambda, temp);
}

void Sphere::drawGeometry(std::shared_ptr<BasicMesh> mesh,
                          std::vector<BasicMesh::Vertex>& vertices) const {
    auto indexBuffer = mesh->addIndexBuffer(DrawType::Lines, ConnectivityType::None);

    int lat = 8;
    int lon = 10;

    for (int i = 0; i < lat - 1; i++) {
        float theta1 = float(i * M_PI) / (lat - 1);
        float theta2 = float((i + 1) * M_PI) / (lat - 1);

        for (int j = 0; j < lon - 1; j++) {
            float phi1 = float(j * 2 * M_PI) / (lon - 1);
            float phi2 = float((j + 1) * 2 * M_PI) / (lon - 1);

            vec3 v1 = vec3(radius_ * sin(theta1) * cos(phi1), radius_ * sin(theta1) * sin(phi1),
                           radius_ * cos(theta1)) + center_;
            vec3 v2 = vec3(radius_ * sin(theta2) * cos(phi1), radius_ * sin(theta2) * sin(phi1),
                           radius_ * cos(theta2)) + center_;
            vec3 v3 = vec3(radius_ * sin(theta2) * cos(phi2), radius_ * sin(theta2) * sin(phi2),
                           radius_ * cos(theta2)) + center_;

            Util::drawLineSegment(v1, v2, vec4(0.2, 0.2, 0.2, 1), indexBuffer, vertices);
            Util::drawLineSegment(v2, v3, vec4(0.2, 0.2, 0.2, 1), indexBuffer, vertices);
        }
    }
}

} // namespace
