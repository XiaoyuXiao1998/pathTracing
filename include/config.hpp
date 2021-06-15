#pragma once

#include "utils.hpp"

class Config{
public:
    int output_resolution;
    std::string output_file;
    int camera_id;
    int light_id;
    bool delta_mat;
    int spp;
    int max_depth;
    std::string integral_method;
    int num_threads = 6;

    Config(){
        output_resolution = 256;
        output_file = "./output_cbox_all_diff_with_undirect_light.png";
        camera_id = 1;
        light_id = 1;
        delta_mat = false;
        spp = 1024;
        max_depth = 5;
        integral_method = "mis";
    }

    bool parse(std::string filepath);
    void printConfig();

};