//
// Created by PC on 2021/6/10.
//
#include <test_componet.hpp>
#include <config.hpp>

extern Config conf;

void test_Camera(){
    Eigen::Vector3f position(0,0,30);
    Eigen::Vector2i res(1024,1024);
    Camera camera(position,res);
    Eigen::Vector3f origin(0,0,0 );

    camera.lookat();

    Ray ray = camera.generate_ray(res[0]/2,res[1]/2);
    if(ray.direction.dot(Eigen::Vector3f(0,0,-1))>0.95) std::cout<<"camera componet works fine"<<std::endl;

    else std::cout<<"camera componet works wrong"<<std::endl;

}

void test_camera_light(Camera &camera,AreaLight &light){
    int dx;
    int dy;
    for(dy = 0 ;dy<conf.output_resolution;dy++) {
        Ray ray = camera.generate_ray(conf.output_resolution / 2, dy);
        Interaction interaction;
        if (light.intersect(ray, interaction)) std::cout << "camera and light works properly at "<<dy<<std::endl;
    }


}

