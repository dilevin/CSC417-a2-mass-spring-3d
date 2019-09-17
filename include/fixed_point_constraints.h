#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <EigenTypes.h>

#include <vector>

//Input:
//  q_size - total number of scalar generalized coordinaes (3 times number of vertices in the mesh)
//  indices - indices (row ids in V) for fixed vertices
//Output:
//  P - mxn sparse matrix which projects out fixed vertices
void fixed_point_constraints(Eigen::SparseMatrixd &P, unsigned int q_size, const std::vector<unsigned int> indices);