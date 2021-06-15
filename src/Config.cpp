//
// Created by PC on 2021/6/12.
//
#include "config.hpp"
#include "fstream"
void Config::parse(std::string filepath) {
    std::ifstream config_file(filepath);
    std::string line;
    while(std::getline(config_file,line)){

    }

}

