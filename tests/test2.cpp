#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>
#include <chrono>
#include "../include/cpp-math/quadratures.hpp"

void testGaussLaguerre() {
    // Test the computation of roots and weights
    int points = 20;
    GaussLaguerreQuadrature gl(points);
    std::vector<double> roots = gl.getRoots();
    std::vector<double> weights = gl.getWeights();

    std::cout << "Roots:\n";
    for (double root : roots) {
        std::cout << root << " ";
    }
    std::cout << "\nWeights:\n";
    for (double weight : weights) {
        std::cout << weight << " ";
    }
    std::cout << std::endl;

    // Verify the number of roots and weights
    assert(roots.size() == points);
    assert(weights.size() == points);

    // Test the integration method
    auto f = [](double x) { return x; }; // Integral of x * exp(-x) from 0 to infinity is 1
    double result = gl.integrate(f);
    std::cout << "Integration result: " << result << std::endl;

    // Verify the integration result
    assert(std::abs(result - 1.0) < 1e-6);
}

void testGaussLaguerreHighDimension() {
    // Test the computation of roots and weights
    int points = 300;
    GaussLaguerreQuadrature gl(points);
    std::vector<double> roots = gl.getRoots();
    std::vector<double> weights = gl.getWeights();

    // Verify the number of roots and weights
    assert(roots.size() == points);
    assert(weights.size() == points);

    // Test the integration method
    auto f = [](double x) { return x; }; // Integral of x * exp(-x) from 0 to infinity is 1
    double result = gl.integrate(f);
    std::cout << "Integration result: " << result << std::endl;

    // Verify the integration result
    assert(std::abs(result - 1.0) < 1e-6);
}

void testGaussLaguerreTime()
{
    // Test the timing of Gauss-Laguerre quadrature
    int points = 300;
    auto start = std::chrono::high_resolution_clock::now();
    GaussLaguerreQuadrature gl(points);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Time taken to compute Gauss-Laguerre quadrature with " << points << " points: " << elapsed.count() << " seconds" << std::endl;
}

int main() {
    std::cout << "Testing Gauss-Laguerre Quadrature:\n";
    testGaussLaguerre();
    testGaussLaguerreHighDimension();
    testGaussLaguerreTime();
    std::cout << "All tests passed successfully!" << std::endl;
    return 0;
}