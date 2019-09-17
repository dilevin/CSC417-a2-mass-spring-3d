#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <EigenTypes.h>

//Input:
//  q - generalized coordiantes for the mass-spring system
//  mass - the mass of each particle in the mass-spring system.
//Output:
//  M - sparse mass matrix for mass-spring system
void mass_matrix_particles(Eigen::SparseMatrixd &M, Eigen::Ref<const Eigen::VectorXd> q, double mass);