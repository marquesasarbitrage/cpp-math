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

double testFunctionNM(std::vector<double> x) {
    double a = 1.0, b = 100.0;
    return pow(a - x[0], 2) + b * pow(x[1] - x[0] * x[0], 2);
}

double himmelblauFunction(std::vector<double> x) {
    double a = x[0] * x[0] + x[1] - 11;
    double b = x[0] + x[1] * x[1] - 7;
    return a * a + b * b;
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

void testNelderMead() {
    std::cout << "Testing Nelder-Mead Method...\n";

    // Initial guess for 2D problem (Rosenbrock function)
    std::vector<double> x0 = {-1.2, 1.0};  

    NelderMead optimizer(x0, testFunctionNM);
    optimizer.setToleranceThreshold(1e-10);
    optimizer.setMaximumIterations(500);
    optimizer.setReflectionParam(1.0);
    optimizer.setExpansionParam(2.0);
    optimizer.setContractionParam(0.5);
    optimizer.setShrinkParam(0.75);

    optimizer.optimize();
    std::vector<double> result = optimizer.getResult();

    std::cout << "Nelder-Mead Result:\n";
    std::cout << "Optimal x: (" << result[0] << ", " << result[1] << ")\n";
    std::cout << "Function Value at x: " << optimizer.getFunctionResult() << "\n";
    std::cout << "Iterations: " << optimizer.getNumberIterations() << "\n";
    std::cout << "Time taken: " << optimizer.getTimeTaken() << "\n";

    // ✅ Verify convergence close to (1,1) for Rosenbrock function
    assert(std::abs(result[0] - 1.0) < 1e-3);
    assert(std::abs(result[1] - 1.0) < 1e-3);
    std::cout << "✅ Nelder-Mead Test Passed!\n\n";
}


// ✅ Test Nelder-Mead on Himmelblau's Function
void testNelderMeadHimmelblau() {
    std::cout << "Testing Nelder-Mead on Himmelblau's Function...\n";

    // Initial guess (random, not near any known minimum)
    std::vector<double> x0 = {-5.0, 5.0};  

    NelderMead optimizer(x0, himmelblauFunction);
    optimizer.setToleranceThreshold(1e-6);
    optimizer.setMaximumIterations(1000);
    optimizer.setReflectionParam(1.0);
    optimizer.setExpansionParam(2.0);
    optimizer.setContractionParam(0.5);
    optimizer.setShrinkParam(0.5);

    optimizer.optimize();
    std::vector<double> result = optimizer.getResult();

    std::cout << "Nelder-Mead Result:\n";
    std::cout << "Optimal x: (" << result[0] << ", " << result[1] << ")\n";
    std::cout << "Function Value at x: " << optimizer.getFunctionResult() << "\n";
    std::cout << "Iterations: " << optimizer.getNumberIterations() << "\n";
    std::cout << "Time taken: " << optimizer.getTimeTaken() << "\n";

    // ✅ Check if the result is near any of the four minima
    bool near_minimum = (
        (std::abs(result[0] - 3.0) < 1e-3 && std::abs(result[1] - 2.0) < 1e-3) ||
        (std::abs(result[0] + 2.805) < 1e-3 && std::abs(result[1] - 3.131) < 1e-3) ||
        (std::abs(result[0] + 3.779) < 1e-3 && std::abs(result[1] + 3.283) < 1e-3) ||
        (std::abs(result[0] - 3.584) < 1e-3 && std::abs(result[1] + 1.848) < 1e-3)
    );

    assert(near_minimum);
    std::cout << "✅ Nelder-Mead Himmelblau Test Passed!\n\n";
}


int main()
{
    testNewtonRaphson();
    testNelderMead();
    testNelderMeadHimmelblau();
    return 0;
}