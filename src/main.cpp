#include "utils.hpp"
#include "scene.hpp"
#include "Camera.hpp"
#include "Integrator.hpp"
#include "config.hpp"
#include "test_componet.hpp"
#include "BRDF.hpp"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <chrono>
#define clock_t std::chrono::time_point<std::chrono::system_clock>

Config conf;



int main(int argc, char* argv[])
{
    if(argc>1){
    conf.parse(argv[1]);
    conf.printConfig();
    }

    //// --------------------------set camera---------------------------------------------
    Eigen::Vector3f camera_position;
    if(conf.camera_id == 1){
        camera_position = Eigen::Vector3f(0,0,10);
    }

    Camera camera(camera_position,Eigen::Vector2i(conf.output_resolution,conf.output_resolution));
    camera.lookat();

    ////-------------------------set light-------------------------------------------------
    Eigen::Vector3f light_pos;
    Eigen::Vector3f light_radiance;
    Eigen::Vector2f light_size;
    if (conf.light_id == 1)
    {
        // Light setting 1
        light_pos = Eigen::Vector3f(0, 5.95, -6);
        light_radiance = Eigen::Vector3f(1.0, 1.0, 1.0) * 60;
        light_size = Eigen::Vector2f(2.0, 2.0);
    }

    AreaLight light(light_pos, light_radiance, light_size);

    Scene scene(&light);
   // test_camera_light(camera,light);

   ////----------------------set material-----------------------------------------------------
    Idealdiffusion mat_front_wall(Eigen::Vector3f(0.7, 0.7, 0.7));
    Idealdiffusion mat_back_wall(Eigen::Vector3f(0.7, 0.7, 0.7));
    Idealdiffusion mat_left_wall(Eigen::Vector3f(0.5, 0, 0));
    Idealdiffusion mat_right_wall(Eigen::Vector3f(0, 0.5, 0));
    Idealdiffusion mat_floor_wall(Eigen::Vector3f(0.7, 0.7, 0.7));
    Idealdiffusion mat_ceiling_wall(Eigen::Vector3f(0.7, 0.7, 0.7));

    Idealdiffusion mat_sphere1(Eigen::Vector3f(0.0, 0.2, 0.5));
    Idealdiffusion mat_sphere2_diff(Eigen::Vector3f(0.4, 0.4, 0.4));

    Idealdiffusion mat_bunny_diff(Eigen::Vector3f(0.3, 0.3, 0.5));
    Idealdiffusion mat_dragon(Eigen::Vector3f(0.2, 0.2, 0.2));

    ////--------------set cornell box--------------------------------------------------------
    Eigen::Vector3f cbox_lb(-6, -6, -10);
    Eigen::Vector3f cbox_size(12, 12, 30);

    Eigen::Vector3f p_p0(cbox_lb);
    Eigen::Vector3f p_s0(cbox_size.x(), 0, 0);
    Eigen::Vector3f p_s1(0, cbox_size.y(), 0);
    Eigen::Vector3f p_normal(0, 0, 1);
    rectangle back_wall(cbox_lb, p_s0, p_s1, p_normal, &mat_front_wall);

    p_p0 = Eigen::Vector3f(cbox_lb.x(), cbox_lb.y(), cbox_lb.z() + cbox_size.z());
    p_s0 = Eigen::Vector3f(cbox_size.x(), 0, 0);
    p_s1 = Eigen::Vector3f(0, cbox_size.y(), 0);
    p_normal = Eigen::Vector3f(0, 0, -1);
    rectangle front_wall(p_p0, p_s0 , p_s1, p_normal, &mat_back_wall);

    p_p0 = Eigen::Vector3f(cbox_lb);
    p_s0 = Eigen::Vector3f(0, 0, cbox_size.z());
    p_s1 = Eigen::Vector3f(0, cbox_size.y(), 0);
    p_normal = Eigen::Vector3f(1, 0, 0);
    rectangle left_wall(p_p0, p_s0, p_s1, p_normal, &mat_left_wall);

    p_p0 = Eigen::Vector3f(cbox_lb.x() + cbox_size.x(), cbox_lb.y(), cbox_lb.z());
    p_s0 = Eigen::Vector3f(0, 0, cbox_size.z());
    p_s1 = Eigen::Vector3f(0, cbox_size.y(), 0);
    p_normal = Eigen::Vector3f(-1, 0, 0);
    rectangle right_wall(p_p0, p_s0, p_s1, p_normal, &mat_right_wall);

    p_p0 = Eigen::Vector3f(cbox_lb);
    p_s0 = Eigen::Vector3f(cbox_size.x(), 0, 0);
    p_s1 = Eigen::Vector3f(0, 0, cbox_size.z());
    p_normal = Eigen::Vector3f(0, 1, 0);
    rectangle floor(p_p0, p_s0, p_s1, p_normal, &mat_floor_wall);

    p_p0 = Eigen::Vector3f(cbox_lb.x(), cbox_lb.y() + cbox_size.y(), cbox_lb.z());
    p_s0 = Eigen::Vector3f(cbox_size.x(), 0, 0);
    p_s1 = Eigen::Vector3f(0, 0, cbox_size.z());
    p_normal = Eigen::Vector3f(0, -1, 0);
    rectangle ceiling(p_p0, p_s0, p_s1, p_normal, &mat_ceiling_wall);

    scene.add_object(&back_wall);
    scene.add_object(&front_wall);
    scene.add_object(&left_wall);
    scene.add_object(&right_wall);
    scene.add_object(&floor);
    scene.add_object(&ceiling);

    sphere sphere1(Eigen::Vector3f(3, 1, -7), 1.5f, &mat_sphere1);
    sphere sphere2(Eigen::Vector3f(-4, 2, -5), 1.8f, &mat_sphere2_diff);

    scene.add_object(&sphere1);
    scene.add_object(&sphere2);

    TriangleMesh mesh_dragon("../resources/stanford_dragon.obj", &mat_dragon);
    mesh_dragon.applyTransformation(Eigen::Translation3f(2.5, -7.0, -6.5) * Eigen::Scaling(25.0f));
    mesh_dragon.buildUniformGrid();

    TriangleMesh mesh_bunny("../resources/stanford_bunny.obj", &mat_bunny_diff);
    mesh_bunny.applyTransformation(Eigen::Translation3f(-2, -6.5, -6.0) * Eigen::Scaling(25.0f));
    mesh_bunny.buildUniformGrid();
    scene.add_object(&mesh_dragon);
    scene.add_object(&mesh_bunny);





    std::cout << "\n\nRendering..." << std::endl;

    clock_t start_time = std::chrono::system_clock::now();

    Integrator integrator(&scene, &camera);
    integrator.render();

    clock_t end_time = std::chrono::system_clock::now();
    double time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    std::cout << "\nTime elapsed: " << time_elapsed << " ms" << std::endl;

    /////////////////////////////////////////////////////////////////////////
    // Output the image to a file
    /////////////////////////////////////////////////////////////////////////

    std::string output_path = conf.output_file;
    std::vector<unsigned char> output_data;
    output_data.reserve(camera.get_resolution().x() * camera.get_resolution().y() * 3);

    for (Eigen::Vector3f v : camera.getFilm())
    {

        for (int i = 0; i < 3; i++)
        {
            output_data.push_back(mathutils::gamma_correction(v[i]));

        }
    }

    stbi_flip_vertically_on_write(true);
    stbi_write_png(output_path.c_str(), camera.get_resolution().x() , camera.get_resolution().y() , 3, output_data.data(), 0);

    return 0;


}

