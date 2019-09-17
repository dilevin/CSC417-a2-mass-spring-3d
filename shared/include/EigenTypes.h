#ifndef EIGENTYPES_H
#define EIGENTYPES_H

#include <Eigen/Dense>
#include <Eigen/Sparse>

namespace Eigen {

    //dense types
    using Vector6d = Eigen::Matrix<double, 6,1>;
    using Matrix36d = Eigen::Matrix<double, 3,6>;
    using Matrix66d  = Eigen::Matrix<double, 6,6>;
    using Matrix44f = Eigen::Matrix<float, 4,4>;
    //sparse types
    using SparseMatrixd = Eigen::SparseMatrix<double>;

}

#endif 
