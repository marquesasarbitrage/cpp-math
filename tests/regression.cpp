#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <cassert>
#include "../include/core-math/regression.hpp"

void testOLS() {

    std::vector<std::vector<double>> X = {{5.0}, {7.0}, {8.0}, {4.0}, {3.0}, {2.0}, {1.0}};
    std::vector<double> Y = {6.0, 7.0, 9.0, 10.0, 6.0, 4.0,-2.0};
    OrdinaryLeastSquare ols(Y, X, true);

    double expectedIntercept = 0.9783;
    double expectedCoeff = 1.1051;
    double expectedRSquared = .52;
    double expectedPredictedValue10 = 12.029;

    assert(std::abs(ols.getIntercept() - expectedIntercept) < 1e-4);
    assert(std::abs(ols.getCoefficients()[0] - expectedCoeff) < 1e-4);
    assert(std::abs(ols.getRSquared() - expectedRSquared) < 1e-2); 
    assert(std::abs(ols.getPrediction({10.0}) - expectedPredictedValue10) < 1e-2);

    std::cout << "Test passed for Ordinary Least Square" << std::endl;
}

int main()
{
    testOLS();
    return 0;
}
