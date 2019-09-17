#include <pick_nearest_vertices.h>
#include <iostream>
#include <igl/Hit.h>
#include <igl/ray_mesh_intersect.h>
#include <igl/unproject.h>

bool pick_nearest_vertices(std::vector<unsigned int> &verts, Eigen::Ref<const Eigen::Vector3d> win, 
                           Eigen::Ref<const Eigen::Matrix44f> view, Eigen::Ref<const Eigen::Matrix44f> proj, Eigen::Vector4f viewport,
                           Eigen::Ref<const Eigen::MatrixXd> V, Eigen::Ref<const Eigen::MatrixXi> F, double radius) {

    verts.clear();

    // Source, destination and direction in world
    Eigen::Vector3f start,dir;
    igl::Hit hit;

    //compute start and direction in the world to check for picked vertex
    //YOUR CODE HERE
    
    const auto & shoot_ray = [&V,&F](const Eigen::Vector3f& s, const Eigen::Vector3f& dir, igl::Hit & hit)->bool
    {
        std::vector<igl::Hit> hits;
        
        if(!igl::ray_mesh_intersect(s,dir,V,F,hits))
        {
            return false;
        }
        hit = hits[0];
        return true;
    };

    if(!shoot_ray(start,dir,hit))
    {
        return false;
    }

    //check if any of the hit vertices are within the selection radius 
    //YOUR CODE HERE

    return (verts.size() == 0 ? false : true);
}