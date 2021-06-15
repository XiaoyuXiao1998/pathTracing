
#pragma once
#ifndef PATHTRACING_RAY_HPP
#define PATHTRACING_RAY_HPP

#include <Eigen/dense>
#include <utils.hpp>

struct Ray{
    Eigen::Vector3f origin;
    Eigen::Vector3f direction;
    float range_min;
    float range_max;

    Ray(Eigen::Vector3f ori, Eigen::Vector3f dir,float min = 1e-6,float max = 1e10){
        origin = ori;
        direction = dir.normalized();
        range_max = max;
        range_min = min;
    }
    Eigen::Vector3f getpoint(float t ) const {
        return origin + direction*t;
    }



};


#endif //PATHTRACING_RAY_HPP
