#pragma once
#ifndef PATHTRACING_CAMERA_HPP
#define PATHTRACING_CAMERA_HPP

#include "Eigen/Dense"
#include "Ray.hpp"


class Camera{
protected:
    Eigen::Vector3f position;
    Eigen::Vector2i resolution;
    float focal_length;
    float vertical_fov;
    Eigen::Matrix3f view;




public:
    std::vector<Eigen::Vector3f> pixel_array;

    Ray  generate_ray(float dx, float dy);

    Camera(const Eigen::Vector3f &pos,const Eigen::Vector2i &res,float focal_length = 1.0f,float vertical_fov = 45);

    void lookat(Eigen::Vector3f lookat_pos = Eigen::Vector3f (0,0,0), Eigen::Vector3f ref_up  =Eigen::Vector3f  (0,1,0));

    Eigen::Vector2i get_resolution();

    void set_pixel(int dx, int dy,Eigen::Vector3f value);
     std::vector<Eigen::Vector3f> getFilm() ;




};












#endif //PATHTRACING_CAMERA_HPP
