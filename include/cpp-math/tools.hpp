#pragma once 
#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include "errors.hpp"

namespace EigenTool
{
    Eigen::MatrixXd getEigenMatrix(const std::vector<std::vector<double>>& data);

    Eigen::VectorXd getEigenVector(const std::vector<double>& data);
}