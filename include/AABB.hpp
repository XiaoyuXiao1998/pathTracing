//
// Created by PC on 2021/6/13.
//

#ifndef PATHTRACING_AABB_HPP
#define PATHTRACING_AABB_HPP

#include "utils.hpp"
#include "Ray.hpp"


struct AABB{
    Eigen::Vector3f lb;
    Eigen::Vector3f ub;
    AABB();
    AABB(Eigen::Vector3f pos,float r);
    AABB(Eigen::Vector3f A,Eigen::Vector3f B,Eigen::Vector3f C);

    bool intersect(const Ray& ray,float& t_in, float& t_out);
};

#endif //PATHTRACING_AABB_HPP

