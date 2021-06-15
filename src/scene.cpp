//
// Created by PC on 2021/6/11.
//
#include "scene.hpp"


bool Scene::intersect(const Ray & ray, Interaction& interaction) {
    Interaction final_interaction;
    /* if ray does not intersect with the light, final_interaction.entry_dis == -1.0f*/
    scene_light->intersect(ray,final_interaction);
    for(int i = 0;i<objects.size();i++){
        Interaction current_interaction;
        if(objects[i]->intersect(ray,current_interaction)){
            if(final_interaction.entry_dis==-1.0f || current_interaction.entry_dis < final_interaction.entry_dis){
                final_interaction = current_interaction;

            }
        }


    }
    if(final_interaction.entry_dis == -1){
        for(int i = 0;i<objects.size();i++){
            Interaction current_interaction;

            if(objects[i]->intersect(ray,current_interaction)){
                if(final_interaction.entry_dis==-1.0f || current_interaction.entry_dis < final_interaction.entry_dis){
                    final_interaction = current_interaction;

                }
            }


        }
    }

    if(final_interaction.entry_dis != -1 && final_interaction.entry_dis >= ray.range_min && final_interaction.entry_dis<= ray.range_max){
        interaction  = final_interaction;

        return true;
    }
    return false;
}

bool Scene::isShadowed(const Ray & ray) {
    Interaction interaction;
    intersect(ray,interaction);

    if(interaction.type != Interaction::LIGHT) return true;
    return false;

}

Light* Scene::get_light() {
        return scene_light;
}

