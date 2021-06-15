//
// Created by PC on 2021/6/11.
//
#include "geometry.hpp"
#include "objloader.hpp"

rectangle::rectangle(Eigen::Vector3f pos,Eigen::Vector3f e1, Eigen::Vector3f e2,Eigen::Vector3f normal):
position(pos),normal(normal){

    edge1  = e1.normalized();
    edge2 = e2.normalized();
    edge1_len = e1.norm();
    edge2_len = e2.norm();

}

rectangle::rectangle(Eigen::Vector3f pos,Eigen::Vector3f e1, Eigen::Vector3f e2,Eigen::Vector3f normal,BRDF *brdf):
position(pos),normal(normal),brdf(brdf){
    edge1  = e1.normalized();
    edge2 = e2.normalized();
    edge1_len = e1.norm();
    edge2_len = e2.norm();

}

sphere::sphere(Eigen::Vector3f pos,float radius, BRDF *brdf):
position(pos), r(radius),brdf(brdf),aabb(pos,radius){

}


bool rectangle::intersect( const Ray &ray, Interaction &interaction) {
    if (ray.direction.dot(normal) == 0)
    {
        return false;
    }



    float t = (position-ray.origin).dot(normal)/ray.direction.dot(normal);

    if(t<ray.range_min || t>ray.range_max) return false;
    Eigen::Vector3f p0_p = ray.getpoint(t) - position;
    if(p0_p.dot(edge1)>0 && p0_p.dot(edge1)<edge1_len && p0_p.dot(edge2)>0 && p0_p.dot(edge2)<edge2_len){
        interaction.normal = normal;
        interaction.entry_point  = ray.getpoint(t);
        interaction.entry_dis  = t;
        interaction.exit_dis = t;
        interaction.type = Interaction::Type::GEOMETRY;
        if(brdf!=nullptr){
            interaction.material = (Idealdiffusion*) brdf;

        }
        return true;
    }
    return false;
}

bool sphere::intersect( const Ray &ray, Interaction &interaction) {


        float t0,t1;
        ////analytic solution
        Eigen::Vector3f dis = ray.origin - position;
        float a = ray.direction.dot(ray.direction);
        float b = 2*ray.direction.dot(dis);
        float c = dis.dot(dis)  - pow(r,2);
        if(!solveQuadratic(a,b,c,t0,t1)) return false;
        if(t0 < 0){
            t0 = t1;
            if(t0 <0) return false;
        }
        float t = t0;
     if(t<ray.range_min || t>ray.range_max) return false;
        interaction.entry_dis = t;
        interaction.exit_dis = t;
        interaction.entry_point = ray.getpoint(t);
        interaction.normal = (interaction.entry_point - position).normalized();
        interaction.type = Interaction::Type::GEOMETRY;
        if(brdf!=nullptr){
        interaction.material = (Idealdiffusion*) brdf;

    }


        return true;
    }

    bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1){
        float discr = b * b - 4 * a * c;
        if (discr < 0) return false;
        else if (discr == 0) x0 = x1 = - 0.5 * b / a;
        else {
            float q = (b > 0) ?
                      -0.5 * (b + sqrt(discr)) :
                      -0.5 * (b - sqrt(discr));
            x0 = q / a;
            x1 = c / q;
        }
        if (x0 > x1) std::swap(x0, x1);

        return true;
    }

bool TriangleMesh::raySingleTriangleIntersection(Interaction& interaction, const Ray& ray, int v0_idx, int v1_idx, int v2_idx, int normal_idx) const
{
    /**
     * TODO: Ray intersection test with single triangle
     * Note: Remember that normals are interpolated using barycentric coordinates.
     */
    float kEpsilon = 1e-5;

    Eigen::Vector3f v0v1 = vertices[v1_idx] - vertices[v0_idx];
    Eigen::Vector3f v0v2 = vertices[v2_idx] - vertices[v0_idx];
    Eigen::Vector3f n_un = v0v1.cross(v0v2).normalized();
    if(n_un.dot(normals[normal_idx])<0) n_un = -n_un;


    Eigen::Vector3f pvec = ray.direction.cross(v0v2);
    float det = v0v1.dot(pvec);

    //ray and triangle are parallel if det is close to zero;
    if(abs(det)<kEpsilon) return false;

    float invDet = 1.0f/det;
    Eigen::Vector3f tvec = ray.origin - vertices[v0_idx];
    float u = tvec.dot(pvec)*invDet;
    if(u<0 || u>1 )return false;

    Eigen::Vector3f qvec = tvec.cross(v0v1);
    float v = ray.direction.dot(qvec)*invDet;
    if(v<0||u+v>1) return false;

    float t = v0v2.dot(qvec)*invDet;
    if(t<ray.range_min || t>ray.range_max) return false;

    interaction.entry_dis = t;
    interaction.exit_dis = t;
    interaction.entry_point = ray.getpoint(t);
    interaction.normal = n_un;
    if (brdf != nullptr)
    {
        interaction.material = (void*)brdf;
    }
    interaction.type = Interaction::Type::GEOMETRY;
    return true;


}

bool TriangleMesh::intersect(const Ray &ray, Interaction &interaction)
{
    Interaction final_interaction;

    if (has_grid) {
        /**
         * TODO: Use uniform grid to handle triangle intersection here
         * Note: Grid traversal algorithm must be used here.
         */
        // test whether ray interacts with the scene coordinate
        //assume that camera is out of all triangular object

        float t_in,tout;
        if(!aabb.intersect(ray,t_in,tout)) return false;

        //converting ray startpoint to cell coordinates;
        Eigen::Vector3f originGridCoordinate = ray.origin + t_in*ray.direction-aabb.lb;
        Eigen::Vector3f originCellCoordinate = originGridCoordinate.cwiseQuotient(cellDimension);
        Eigen::Vector3i nextGridCell;
        Eigen::Vector3f deltaT,nextCrossingT;
        Eigen::Vector3i exit,step;
        for(unsigned int i =0;i<3;i++){
            nextGridCell[i] = mathutils::clamp((int)floor(originCellCoordinate[i]),0,resolution[i]-1);


            /* direction of ray is negative againt grid*/
            if(ray.direction[i]<0){
                deltaT[i] = -cellDimension[i] / ray.direction[i];
                nextCrossingT[i] = t_in +
                                   (nextGridCell[i]*cellDimension[i]-originGridCoordinate[i])/ray.direction[i];
                exit[i] = -1;
                step[i] = -1;


            }
            else{

                deltaT[i] = cellDimension[i] / ray.direction[i];
                nextCrossingT[i] = t_in +
                                   ( (nextGridCell[i]+1)*cellDimension[i]-originGridCoordinate[i])/ray.direction[i];
                exit[i] = resolution[i];
                step[i] = 1;


            }

        }

        //walk through each cell of the grid and test for an interaction  if current cell contains geometry


        while(true) {
            Interaction CurrentCellInteraction;
            unsigned int o = nextGridCell[2]*resolution[1]*resolution[0] +nextGridCell[1]*resolution[0] + nextGridCell[0];



            if (grids[o].size() != 0) {
                for (int i = 0; i < grids[o].size(); i++) {

                    if (raySingleTriangleIntersection(CurrentCellInteraction, ray, grids[o][i][0], grids[o][i][1],
                                                      grids[o][i][2],grids[o][i][3])) {
                        if(interaction.entry_dis ==-1.f || interaction.entry_dis>CurrentCellInteraction.entry_dis){
                            interaction = CurrentCellInteraction;
                        }
                    }
                }
            }


            //number: 0 1 2 3 4 5 6 7
            //axis:   2 1 2 1 2 2 0 0
            uint8_t k =
                    ((nextCrossingT[0] < nextCrossingT[1]) << 2) +
                    ((nextCrossingT[0] < nextCrossingT[2]) << 1) +
                    ((nextCrossingT[1] < nextCrossingT[2]));
            static const unsigned int map[8] = {2, 1, 2, 1, 2, 2, 0, 0};
            unsigned int nextAxisT = map[k];
            if (interaction.entry_dis <nextCrossingT[nextAxisT] && interaction.entry_dis!=-1.f) break;
            nextGridCell[nextAxisT] += step[nextAxisT];
            if(nextGridCell[nextAxisT] == exit[nextAxisT]) return false;
            nextCrossingT[nextAxisT]+=deltaT[nextAxisT];

        }
        return true;


    } else {
        for (int i = 0; i < num_triangles; i++) {
            Interaction cur_it;
            if (raySingleTriangleIntersection(cur_it, ray, 3 * i, 3 * i + 1, 3 * i + 2,0)) {
                if (final_interaction.entry_dis == -1 || cur_it.entry_dis < final_interaction.entry_dis) {
                    final_interaction = cur_it;
                }
            }
        }
    }

    if (final_interaction.entry_dis != -1)
    {
        interaction = final_interaction;
        if (brdf != nullptr)
        {
            interaction.material = brdf;
        }
        interaction.type = Interaction::Type::GEOMETRY;

        return true;
    }

    return false;
}

void TriangleMesh::buildUniformGrid() {
    /**
     * TODO: Build uniform grid here
     * Note: you may need to build your own data structures in the accel_struct.hpp and accel_struct.cpp
     */
    // build scene bounding box;
    Eigen::Vector3f size = aabb.ub - aabb.lb;


    float lamda = 5.0f;
    float cuberoot = pow(lamda * num_triangles / size[0]*size[1]*size[2],1/3.0f);
    for(unsigned int i = 0; i<3;i++){
        resolution[i] = floor(size[i]*cuberoot);
        resolution[i] = fmax(uint32_t(1),fmin(resolution[i],uint32_t(128)));
    }
    grids.resize(resolution[0] * resolution[1] *resolution[2]);
    //std::cout<<"size: "<<grids.size()<<std::endl;

    for(uint32_t i = 0; i<3;i++){
        cellDimension[i] = size[i]/resolution[i];
    }
    // insert all triangles into the cells;
    // cells need to store the information of
    for(uint32_t i = 0; i< num_triangles;i+=3){
        Eigen::Vector3f A = vertices[vertices_index[i]];
        Eigen::Vector3f B = vertices[vertices_index[i+1]];
        Eigen::Vector3f C = vertices[vertices_index[i+2]];

        // build a bounding box for picked triangle
        AABB  triangle_box(A,B,C);

        //convert to cell coodrinates;
        Eigen::Vector3f min = (triangle_box.lb - aabb.lb).cwiseQuotient(cellDimension);
        Eigen::Vector3f max = (triangle_box.ub - aabb.lb).cwiseQuotient(cellDimension);

        uint32_t zmin = mathutils::clamp(std::floor(min[2]), 0, resolution[2] - 1);
        uint32_t zmax = mathutils::clamp(std::floor(max[2]), 0, resolution[2] - 1);
        uint32_t ymin = mathutils::clamp(std::floor(min[1]), 0, resolution[1] - 1);
        uint32_t ymax =mathutils:: clamp(std::floor(max[1]), 0, resolution[1] - 1);
        uint32_t xmin = mathutils::clamp(std::floor(min[0]), 0, resolution[0] - 1);
        uint32_t xmax = mathutils::clamp(std::floor(max[0]), 0, resolution[0] - 1);

        for (uint32_t z = zmin; z <= zmax; ++z) {
            for (uint32_t y = ymin; y <= ymax; ++y) {
                for (uint32_t x = xmin; x <= xmax; ++x) {
                    uint32_t o = z * resolution[0] * resolution[1] + y * resolution[0] + x;
                    grids[o].push_back(Eigen::Vector4i(vertices_index[i],vertices_index[i+1],vertices_index[i+2],normals_index[i]));

                }
            }
        }
    }
    has_grid = true;
}

TriangleMesh::TriangleMesh(std::string file_path, BRDF* mat):brdf(mat)
{


    std::vector<Eigen::Vector2f> out_uvs;
    std::vector<int> out_vt_index;
    loadObj(file_path.c_str(), vertices, out_uvs, normals, vertices_index, out_vt_index, normals_index);

    num_triangles = vertices_index.size() / 3;


    has_grid = false;

    buildBoundingBox();
}
void TriangleMesh::buildBoundingBox()
{

    aabb.lb = vertices[0].cwiseMin(vertices[1]);
    aabb.ub = vertices[0].cwiseMax(vertices[1]);
    for (int i = 2; i < vertices.size(); i++)
    {
        aabb.lb = aabb.lb.cwiseMin(vertices[i]);
        aabb.ub = aabb.ub.cwiseMax(vertices[i]);
    }
}

void TriangleMesh::applyTransformation(const Eigen::Affine3f& t)
{
    for (int i = 0; i < vertices.size(); i++)
    {
        vertices[i] = t * vertices[i];
    }

    Eigen::Matrix3f t_inv_tr = t.linear().inverse().transpose();
    for (int i = 0; i < normals.size(); i++)
    {
        normals[i] = (t_inv_tr * normals[i]).normalized();
    }

    buildBoundingBox();
}