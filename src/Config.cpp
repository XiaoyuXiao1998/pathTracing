//
// Created by PC on 2021/6/12.
//
#include "config.hpp"
#include "fstream"
bool Config::parse(std::string file_path)
{
    std::string token;
    std::ifstream file(file_path);

    std::string line;
    int line_num = 1;
    while (std::getline(file, line))
    {
        strutils::trim(line);

        if (line.find("#") == 0 || line.empty())
        {
            continue;
        }

        std::stringstream line_stream(line);
        std::string key, val;
        line_stream >> key;
        if (!(line_stream >> val)) {
            std::cout << "Config: Wrong syntax at " << file_path << ":" << line_num << ". Value field not found." << std::endl;
            return false;
        }

        if (key == "output_resolution")
        {
            output_resolution = std::stoi(val);
        }
        else if (key == "output_file")
        {
            output_file = val;
        }
        else if (key == "camera_id")
        {
            camera_id = std::stoi(val);
        }
        else if (key == "light_id")
        {
            light_id = std::stoi(val);
        }
        else if (key == "delta_mat")
        {
            delta_mat = (val == "true" || val == "1");
        }
        else if (key == "num_threads")
        {
            num_threads = std::stoi(val);
        }
        else if (key == "spp")
        {
            spp = std::stoi(val);
        }
        else if (key == "max_depth")
        {
            max_depth = std::stoi(val);
        }
        else if (key == "integral_method")
        {
            integral_method = val;
        }

        if (line_stream >> key)
        {
            std::cout << "Config: Wrong syntax at " << file_path << ":" << line_num << ". Multiple values are not supported." << std::endl;
            return false;
        }

        ++line_num;
    }

    return true;
}

void Config::printConfig()
{
    std::cout << "=====================" << std::endl;
#ifndef NO_OMP
    std::cout << "max number of threads: " << omp_get_max_threads() << std::endl;
    omp_set_num_threads(num_threads);
#pragma omp parallel
    {
#pragma omp single
        {
            std::cout << "number of using threads: " << omp_get_num_threads() << std::endl;
        }
    }
#endif
    std::cout << "output file: " << output_file << std::endl;
    std::cout << "output resolution: " << output_resolution << " x " << output_resolution << std::endl;
    std::cout << "samples per pixel: " << spp << std::endl;
    std::cout << "max depth: " << max_depth << std::endl;
    std::cout << "integral method: " << integral_method << std::endl;
    std::cout << "camera setting: " << camera_id << std::endl;
    std::cout << "light setting: " << light_id << std::endl;
    std::cout << "enable delta materials: " << delta_mat << std::endl;
    std::cout << "=====================" << std::endl;
    std::cout << std::endl;
}