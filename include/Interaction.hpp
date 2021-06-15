//
// Created by PC on 2021/6/10.
//
#pragma once
#include "utils.hpp"


struct Interaction{
    enum Type
    {
        NONE,      /* No interaction */
        GEOMETRY,  /* Interaction happens on a geometry */
        LIGHT      /* Interaction happens on a light source */
    };

    Eigen::Vector3f entry_point;
    Eigen::Vector3f normal;
    float entry_dis;
    float exit_dis;
    /*  wi is the direction of light source cast on light point; */
    Eigen::Vector3f wi;
    void* material;
    Type type;
    Interaction(){
        entry_dis = -1.0f;
        type = Type::NONE;
    }
};

