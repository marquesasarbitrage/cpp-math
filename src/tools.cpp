#include "../include/cpp-math/tools.hpp"

namespace EigenTool
{
    Eigen::MatrixXd getEigenMatrix(const std::vector<std::vector<double>>& data)
    {
        int rows = data.size();
        int cols = data[0].size();

        if (rows == 0) throw MathErrorRegistry::Tools::InvalidMatrixSize();
        if (cols == 0) throw MathErrorRegistry::Tools::InvalidMatrixSize();

        // Create Eigen matrix
        Eigen::MatrixXd mat(rows, cols);

        // Fill Eigen matrix
        for (int i = 0; i < rows; ++i) {
            if (data[i].size() != cols) throw MathErrorRegistry::Tools::InvalidMatrixSize(); ; 
            for (int j = 0; j < cols; ++j) {
                mat(i, j) = data[i][j];
            }
        }
        return mat;
    }

    Eigen::VectorXd getEigenVector(const std::vector<double>& data)
    {
        return Eigen::Map<const Eigen::VectorXd>(data.data(), data.size());
    }
}