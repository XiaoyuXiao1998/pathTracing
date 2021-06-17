//
// Created by PC on 2021/6/11.
//

#include "Integrator.hpp"
#include "config.hpp"
#include "progressbar.hpp"
extern Config conf;

void Integrator::render() {
        int dx, dy;
        int res_x = camera->get_resolution().x(), res_y = camera->get_resolution().y();
        int steps = conf.spp;

        /* Initialize a progress bar */
        progressbar progress_bar(res_x * res_y);
        

#ifndef NO_OMP
        //#pragma omp parallel for private(dy)
#endif
        for (dx = 0; dx < res_x; ++dx)
        {
#pragma omp parallel for
            for (dy = 0; dy < res_y; ++dy)
            {

                //generate rays from the camera;
                //steps > 10;

                //set pixel value;
                Eigen::Vector3f color(0.0f,0.0f,0.0f);


                for(int i = 0;i<steps;i++){
                    int max_depth = 0;
                    float offsetx = dis(random_generator);
                    float offsety = dis(random_generator);


                    Ray ray  = camera->generate_ray(dx+offsetx,dy+offsety);

                    Interaction currentInteraction;
                    if(scene->intersect(ray,currentInteraction)){
                    color +=radiance(ray,currentInteraction,++max_depth)/steps;
                    }


                    //pixel radiance += 1/N*shade(p,ray);
                }
                camera->set_pixel(dx,dy,color);



#ifndef NO_OMP
#pragma omp critical
#endif
                progress_bar.update();


            }
        }
}

Eigen::Vector3f Integrator::radiance(Ray ray,Interaction &interaction,int max_depth)
{

    if(interaction.type == Interaction::LIGHT){

        return scene->get_light()->get_radiance();

    }


    Eigen::Vector3f L_dir(0.0f,0.0f,0.f);
    Eigen::Vector3f L_undir(0.0f,0.0f,0.0f);

    //// contribution from the light source
    //initialize light ray( from interaction point to light source)
    float pdf_dir;
    Eigen::Vector3f light_pos = scene->get_light()->sample(pdf_dir);
    Ray light_ray(interaction.entry_point,(light_pos-interaction.entry_point).normalized());
    //if(abs(interaction.entry_point[0] +6)<0.01) std::cout<<L_dir[0]<<std::endl;
    Idealdiffusion *brdf = (Idealdiffusion*)interaction.material;
    //std::cout<<brdf->get_reflectivity();
    Eigen::Vector3f f_r =brdf->eval(interaction);



    if(!scene->isShadowed(light_ray)){

        if(light_ray.direction.dot(interaction.normal) > 0 ) {
            L_dir = scene->get_light()->emission(light_pos, -light_ray.direction).cwiseProduct(f_r) *
                    light_ray.direction.dot(interaction.normal) *
                    light_ray.direction.dot(-scene->get_light()->get_normal()) /
                    (light_pos - interaction.entry_point).dot((light_pos - interaction.entry_point)) / (pdf_dir);
        }

    }




    //// contribution from the undirection light
    /////test russian roulette with probability P_RR
    ////maniually specify a probility P_RR
    float P_RR = 0.8f;
    if(dis(random_generator)>P_RR ||max_depth>=conf.max_depth) return L_dir + L_undir;
    //uniformally samplle the hemisphere toward wi;
    //get a direction wi stored
    float pdf_hemi = brdf->sample(interaction);

    Interaction newinteraction;
    Ray newray(interaction.entry_point,interaction.wi);


    if(!scene->intersect(newray,newinteraction)) return  L_dir + L_undir;

    if(newinteraction.type == Interaction::LIGHT) return  L_dir + L_undir;
    L_undir = radiance(newray,newinteraction,++max_depth).cwiseProduct(f_r)*interaction.normal.dot(newray.direction)/pdf_hemi/P_RR;



    return L_dir + L_undir;

}
