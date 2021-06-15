//
// Created by PC on 2021/6/10.
//
#pragma once
#ifndef PATHTRACING_UTILS_HPP
#define PATHTRACING_UTILS_HPP

#include <vector>
#include <iostream>
#include <math.h>
#include "Eigen/Dense"
#include <random>
#include "omp.h"
#define PI 3.1415926535897932f

static  std::default_random_engine random_generator;
static std::uniform_real_distribution<float> dis(0, 1.0);
namespace strutils {
    /* Trim from left */
    void ltrim(std::string& s);
    /* Trim from right */
    void rtrim(std::string& s);
    /* Trim from both left and right */
    void trim(std::string& s);
};


/**
 * Mathematical utilities
 */

namespace mathutils {
    /* Clamps the input x to the closed range [lo, hi] */
    float clamp(float x, float lo, float hi);
    /* Performs Gamma correction on x and outputs an integer between 0-255. */
    unsigned char gamma_correction(float x);
    /* Uniformly sample N numbers among [a, b] */
    std::vector<float> unif(float a, float b, int N = 1);
};



#endif //PATHTRACING_UTILS_HPP
