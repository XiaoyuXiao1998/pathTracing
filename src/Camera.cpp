//
// Created by PC on 2021/6/10.
//
#include "Camera.hpp"

Camera::Camera(const Eigen::Vector3f &pos,const Eigen::Vector2i &res,float focal_length,float vertical_fov)
{
    position = pos;
    resolution = res;
    this->focal_length = focal_length;
    this->vertical_fov = vertical_fov;
    pixel_array.resize(resolution[0]*resolution[1],Eigen::Vector3f(1, 1, 1));

}


 Ray Camera::generate_ray(float dx, float dy) {
     //// TODO
     //dx&dy are in raster space; // film space;
     //xNDC = dx/Imagewidth;
     //yNDC = dy/Imageheigt;
     //rasterization space-----> NDC space(0,1)----->screen space(-1,1)

     float half_fov = vertical_fov/2;
     float aspect_ratio = resolution.x()/resolution.y();
     float yCam = ( 2*(dy/resolution.y())-1.0f)*focal_length*tan(half_fov/180.0f*PI);
     float xCam = (2*(dx/resolution.x()) - 1.0f)*focal_length*tan(half_fov/180.0f*PI)*aspect_ratio;
     Eigen::Vector3f  camdir(xCam,yCam,-focal_length);
     Eigen::Vector3f dir_world = (view * camdir).normalized();
     return Ray(position,dir_world);

 }

 void Camera::lookat(Eigen::Vector3f lookat_pos, Eigen::Vector3f ref_up) {
    Eigen::Vector3f forward = (position- lookat_pos).normalized();
    Eigen::Vector3f right = ref_up.cross(forward).normalized();
    Eigen::Vector3f up = forward.cross(right).normalized();

    view.col(0)<<right;
    view.col(1)<<up;
    view.col(2)<<forward;

}

Eigen::Vector2i Camera::get_resolution() {
    return resolution;
}

void Camera::set_pixel(int dx, int dy, Eigen::Vector3f value){
    pixel_array[dy*resolution[0]+dx] = value;


}
std::vector<Eigen::Vector3f> Camera::getFilm() {
    return pixel_array;
}


