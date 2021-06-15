//
// Created by PC on 2021/6/11.
//
#pragma once
#include "geometry.hpp"
#include "Ray.hpp"
#include "light.hpp"
class Scene{
protected:
    std::vector<object*> objects;
    Light* scene_light;

public:
    Scene(Light *light):scene_light(light){}

    void add_object(object* obj){
        objects.push_back(obj);
    }

    void set_light(Light* light){
        scene_light = light;
    }

    bool intersect(const Ray &ray, Interaction &interaction);

    bool isShadowed(const Ray& ray);

    Light* get_light();


};
