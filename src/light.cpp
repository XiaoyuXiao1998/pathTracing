//
// Created by PC on 2021/6/11.
//
#include "light.hpp"

Eigen::Vector3f AreaLight::sample(float &pdf) {
    float w = dis(random_generator)*area_size[0];
    float h = dis(random_generator)*area_size[1];
    pdf =1.0f/( area_size[0] * area_size[1]);


    Eigen::Vector3f a = position + Eigen::Vector3f(w,0,h)-Eigen::Vector3f(area_size[0], 0, area_size[1])/2.0f;

    return position + Eigen::Vector3f(w,0,h)-Eigen::Vector3f(area_size[0], 0, area_size[1])/2.0f;
}

bool AreaLight::intersect(const Ray &ray,Interaction & interaction) {
    if(rectangle_light.intersect(ray,interaction)){
        interaction.type = Interaction::LIGHT;
        return true;
    }
    return false;


}

Eigen::Vector3f AreaLight::emission(Eigen::Vector3f pos, Eigen::Vector3f dir)
{

    return radiance*(normal.dot(dir));

}

AreaLight::AreaLight(Eigen::Vector3f pos, Eigen::Vector3f rad, Eigen::Vector2f size,Eigen::Vector3f normal ):
Light(rad),
position(pos),
area_size(size),
rectangle_light(pos -Eigen::Vector3f (size[0],0,size[1])/2.0f,Eigen::Vector3f (size[0],0,0),
                Eigen::Vector3f (0,0,size[1]), normal),
                normal(normal)
{

}