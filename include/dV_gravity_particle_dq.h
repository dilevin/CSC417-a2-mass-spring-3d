#include <Eigen/Dense>

//Input:
//  q - the generalized coordinates of the particle
//  mass - mass of this particle
//  g - 3x1 gravity acceleration vector
//Output:
//  f - the 3x1 force of gravity vector acting on the particle
void dV_gravity_particle_dq(Eigen::Ref<Eigen::Vector3d> f, Eigen::Ref<const Eigen::Vector3d> q,  double mass, Eigen::Ref<const Eigen::Vector3d> g);