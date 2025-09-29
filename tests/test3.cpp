#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <cassert>
#include "../include/cpp-math/optim.hpp"

// 🔹 Test function for Newton-Raphson: f(x) = x^2 - 4
double testFunctionNR(double x) {
    return x * x - 4;  // Roots at x = ±2
}

// 🔹 Derivative of the function: df(x) = 2x
double testFunctionDerivativeNR(double x) {
    return 2 * x;
}

// 🔹 Test Newton-Raphson Optimization
void testNewtonRaphson() {
    std::cout << "Testing Newton-Raphson Method...\n";
    
    NewtonRaphson optimizer(1.0, testFunctionNR, testFunctionDerivativeNR); // Initial guess x0 = 1.0
    optimizer.setToleranceThreshold(1e-6);
    optimizer.setMaximumIterations(100);
    
    
    std::cout << "Newton-Raphson Result:\n";
    std::cout << "Optimal x: " << optimizer.getResult() << "\n";
    std::cout << "Function Value at x: " << optimizer.getFunctionResult() << "\n";
    std::cout << "Iterations: " << optimizer.getNumberIterations() << "\n";
    std::cout << "Time taken: " << optimizer.getTimeTaken() << "\n";
    
    // ✅ Verify that we are close to the root (either +2 or -2)
    assert(std::abs(optimizer.getResult() - 2.0) < 1e-4 || std::abs(optimizer.getResult()+ 2.0) < 1e-4);
    std::cout << "✅ Newton-Raphson Test Passed!\n\n";
}

int main()
{
    testNewtonRaphson();;
    return 0;
}