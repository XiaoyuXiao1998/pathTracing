//
// Created by PC on 2021/6/11.
//
#pragma once

#ifndef PATHTRACING_LIGHT_HPP
#define PATHTRACING_LIGHT_HPP

#include "geometry.hpp"

class Light{
protected:
    Eigen::Vector3f radiance;
public:
    /*sample a light position*/
    Light(Eigen::Vector3f rad):radiance(rad){ }
    virtual Eigen::Vector3f sample(float &pdf) = 0;
    virtual bool intersect(const Ray &ray,Interaction& interaction) = 0;
    virtual Eigen::Vector3f emission(Eigen::Vector3f pos, Eigen::Vector3f dir) = 0;
    Eigen::Vector3f get_radiance(){
        return radiance;
    }
};



class AreaLight:public Light{
protected:
    rectangle rectangle_light;
    Eigen::Vector2f area_size;
    Eigen::Vector3f position;
    Eigen::Vector3f normal;
public:
    AreaLight(Eigen::Vector3f pos, Eigen::Vector3f rad, Eigen::Vector2f size,Eigen::Vector3f normal=Eigen::Vector3f(0,1,0));
    virtual Eigen::Vector3f sample(float &pdf) override;
    virtual bool intersect(const Ray &ray,Interaction& interaction) override ;
    virtual Eigen::Vector3f emission(Eigen::Vector3f pos, Eigen::Vector3f dir) override ;


};
#endif //PATHTRACING_LIGHT_HPP
