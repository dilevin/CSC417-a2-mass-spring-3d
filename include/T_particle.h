#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <EigenTypes.h>

//Input:
//  qdot - generalized velocity for the mass spring system
//  mass - the mass of a particle
//Output:
//  T - kinetic energy of the all particles in the mass-spring system
void T_particle(double &T, Eigen::Ref<const Eigen::VectorXd> qdot, double mass);