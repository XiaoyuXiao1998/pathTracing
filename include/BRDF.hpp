#pragma once

#ifndef PATHTRACING_BRDF_HPP
#define PATHTRACING_BRDF_HPP

#include "utils.hpp"
#include "Interaction.hpp"

class BRDF{
public:
    /* sample an reflective direction from an interaction point and returns a PDF value */
    virtual float sample(Interaction &interact) = 0;
    virtual Eigen::Vector3f eval(Interaction &interaction) = 0;


};

class Idealdiffusion: public BRDF{

protected:
    Eigen::Vector3f reflectivity;

public:
    Idealdiffusion(Eigen::Vector3f ref):reflectivity(ref){}
    virtual float sample(Interaction &interaction) override;
    virtual Eigen::Vector3f eval(Interaction &interaction) override ;
     Eigen::Vector3f get_reflectivity();

};



#endif //PATHTRACING_BRDF_HPP
