#pragma once
#ifndef PATHTRACING_GEOMETRY_HPP
#define PATHTRACING_GEOMETRY_HPP

#include "Eigen/Dense"
#include "utils.hpp"
#include "BRDF.hpp"
#include "Ray.hpp"
#include "AABB.hpp"

bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1);

class object{
protected:


public:
    virtual bool intersect( const Ray &ray, Interaction &interaction) = 0;


};


class rectangle:public object{
protected:
    BRDF *brdf;
    Eigen::Vector3f  position;
    Eigen::Vector3f edge1;
    Eigen::Vector3f edge2;
    float edge1_len;
    float edge2_len;
    Eigen::Vector3f normal;





public:
    rectangle(Eigen::Vector3f pos,Eigen::Vector3f e1, Eigen::Vector3f e2,Eigen::Vector3f normal,BRDF *brdf);
    rectangle(Eigen::Vector3f pos,Eigen::Vector3f e1, Eigen::Vector3f e2,Eigen::Vector3f normal);

    virtual bool intersect( const Ray &ray, Interaction &interaction) override ;
};

class sphere: public object{
protected:
    BRDF *brdf;
    Eigen::Vector3f position;
    float r;
    AABB aabb;

public:
    sphere(Eigen::Vector3f pos,float radius, BRDF *brdf);
    virtual bool intersect( const Ray &ray, Interaction &interaction) override ;


};

class TriangleMesh: public object{
protected:
    BRDF *brdf;
    /* Number of triangles */
    int num_triangles;
    /* List of verteices and vertex indices */
    std::vector<Eigen::Vector3f> vertices;
    std::vector<int> vertices_index;
    /* List of normals and normal indices */
    std::vector<Eigen::Vector3f> normals;
    std::vector<int> normals_index;
    /* Is uniform grid established */
    bool has_grid;
    /* stores information of triangle interacted with mesh*/
    std::vector<std::vector<Eigen::Vector4i>> grids;
    Eigen::Vector3i resolution;
    Eigen::Vector3f cellDimension;
    AABB aabb;

    /* Test whether the given ray is intersected with the triangle (v0_idx, v1_idx, v2_idx) */
    bool raySingleTriangleIntersection(Interaction& interaction, const Ray& ray, int v0_idx, int v1_idx, int v2_idx,int normal_idx) const;

public:
    TriangleMesh(std::string file_path, BRDF* mat);
    virtual bool intersect(const Ray &ray, Interaction &interaction) override;
    void buildUniformGrid();
    void applyTransformation(const Eigen::Affine3f& t);
    void buildBoundingBox();

};



#endif //PATHTRACING_GEOMETRY_HPP
