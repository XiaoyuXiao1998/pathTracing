//
// Created by PC on 2021/6/14.
//
#include "AABB.hpp"
AABB::AABB(Eigen::Vector3f pos,float r){
    lb = pos -Eigen::Vector3f(r,r,r);
    ub = pos +Eigen::Vector3f(r,r,r);

}
AABB::AABB(Eigen::Vector3f A,Eigen::Vector3f B,Eigen::Vector3f C){

    lb = A.cwiseMin(B).cwiseMin(C);
    ub = A.cwiseMax(B).cwiseMax(C);

}
AABB::AABB(){

}




bool AABB::intersect(const struct Ray & ray,float& t_in, float& t_out) {
    float txmin,txmax,tymin,tymax,tzmin,tzmax;
    Eigen::Vector3f invdir;
    for(int i = 0;i<3;i++){
        invdir[i] = (ray.direction[i] == 0)?1.0e32:1.0f/ray.direction[i];
    }

    txmin = (lb.x() - ray.origin.x())*invdir.x();
    txmax = (ub.x() - ray.origin.x())*invdir.x();

    tymin = (lb.y() - ray.origin.y())*invdir.y();
    tymax = (ub.y() - ray.origin.y())*invdir.y();



    tzmin = (lb.z() - ray.origin.z())*invdir.z();
    tzmax = (ub.z() - ray.origin.z())*invdir.z();

    t_in  = std::max(std::max(std::min(txmin, txmax), std::min(tymin, tymax)), std::min(tzmin, tzmax));
    t_out = std::min(std::min(std::max(txmin, txmax), std::max(tymin, tymax)), std::max(tzmin, tzmax));


    if(t_in<0) return false;
    return t_out >= t_in;



}