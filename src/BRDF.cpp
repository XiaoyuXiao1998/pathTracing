//
// Created by PC on 2021/6/11.
//
#include "BRDF.hpp"

float  Idealdiffusion::sample(Interaction &interact) {
    // use weighted cosine sample function

    float r1 = dis(random_generator);
    float r2 = dis(random_generator);
    //r1
    float y = sqrtf(1 - r1 * r1);
    float phi = 2 * PI * r2;
    float x = r1 * cosf(phi);
    float z = r1 * sinf(phi);
    // wi is the direction of light source cast on light point;
    interact.wi = Eigen::Vector3f(x,y,z); // needs to be transformed to world coordnate;
    //create local hemisphere coordinate
    Eigen::Vector3f up = interact.normal;
    Eigen::Vector3f right;
    if(abs(up.x())>abs(up.y())){
        right = Eigen::Vector3f (up.z(),0,-up.x());
        right = right.normalized();
    } else{
        right = Eigen::Vector3f (0,-up.z(),up.y());
        right = right.normalized();
    }
    Eigen::Vector3f forward = up.cross(right);
    //transform sampled wi from local coordinatev to world coordiante
    interact.wi =  interact.wi[0]*right + interact.wi[1]*up + interact.wi[2]*forward;
    interact.wi = interact.wi.normalized();
    //calculate pdf(wi) related to cos theta
    return y/PI;

}

Eigen::Vector3f Idealdiffusion::eval(Interaction &interaction){
    return Eigen::Vector3f (1.0f/PI,1.0f/PI,1.0f/PI).cwiseProduct(reflectivity);
}

Eigen::Vector3f Idealdiffusion::get_reflectivity() {
    return reflectivity;
}


