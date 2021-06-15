//
// Created by PC on 2021/6/11.
//
#pragma once

#ifndef PATHTRACING_INTEGRATOR_HPP
#define PATHTRACING_INTEGRATOR_HPP

#include "scene.hpp"
#include "Camera.hpp"

class Integrator{
protected:
    Scene*  scene;
    Camera* camera;
public:
    Integrator(Scene *sce, Camera *cam):scene(sce),camera(cam){ }
    void render();
    Eigen::Vector3f radiance(Ray ray,Interaction &interaction,int max_depth);

};

#endif //PATHTRACING_INTEGRATOR_HPP

