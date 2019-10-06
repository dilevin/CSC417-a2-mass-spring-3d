#include <Eigen/Dense>

//Input:
//  q - generalized coordinate of a particle
//  mass - the mass of particles in the mass spring system
//  g - the gravity acceleration vector
//Output:
//  V - the potential energy due to gravity acting on this particle
void V_gravity_particle(double &V, Eigen::Ref<const Eigen::Vector3d> q,  double mass, Eigen::Ref<const Eigen::Vector3d> g);