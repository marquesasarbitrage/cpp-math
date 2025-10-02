#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <cassert>
#include "../include/cpp-math/probability/distributions.hpp"


void testSandardGaussian() {
    // Test default constructor
    Gaussian defaultDist;
    assert(defaultDist.getMu() == 0.0);
    assert(defaultDist.getSigma() == 1.0);

    // Test invalid sigma (should throw MathError)
    try {
        Gaussian invalidDist(0.0, -1.0);
        assert(false); // Should not reach here
    } catch (const MathErrorRegistry::Probability::InvalidSigmaGaussianError e) {
        assert(true); 
    }

    // Test CDF 
    double cdfValue = defaultDist.cdf(0.0);
    assert(std::abs(cdfValue - 0.5) < 1e-6); // cdf(0) for standard normal is 0.5
    double cdf1 = defaultDist.cdf(1.0);
    assert(std::abs(cdf1 - 0.841344746) < 1e-6);
    double cdfm1 = defaultDist.cdf(-1.0);
    assert(std::abs(cdfm1 - 0.158655254) < 1e-6);
    double cdf2 = defaultDist.cdf(2.0);
    assert(std::abs(cdf2 - 0.977249868) < 1e-6);
    double cdfm2 = defaultDist.cdf(-2.0);
    assert(std::abs(cdfm2 - 0.022750132) < 1e-6);
    double cdf3 = defaultDist.cdf(3.0);
    assert(std::abs(cdf3 - 0.998650102) < 1e-6);

    // Test Quantile
    double invCdfValue = defaultDist.quantile(0.5);
    assert(std::abs(invCdfValue - 0.0) < 1e-6); // inv_cdf(0.5) for standard normal is 0
    double q1 = defaultDist.quantile(.841344746);
    assert(std::abs(q1 - 1.0) < 1e-6);
    double qm1 = defaultDist.quantile(.158655254);
    assert(std::abs(qm1 + 1.0) < 1e-6);
    double q2 = defaultDist.quantile(.977249868);
    assert(std::abs(q2 - 2.0) < 1e-6);
    double qm2 = defaultDist.quantile(.022750132);
    assert(std::abs(qm2 + 2.0) < 1e-6);
    double q3 = defaultDist.quantile(.998650102);
    assert(std::abs(q3 - 3.0) < 1e-6);

    // Test Characteristc Function
    std::complex<double> cf0 = defaultDist.cf(0.0);
    assert(std::abs(cf0.real() - 1.0) < 1e-12);
    assert(std::abs(cf0.imag()) < 1e-12);

    std::complex<double> cf01 = defaultDist.cf(0.1);
    assert(std::abs(cf01.real() - 0.995012479) < 1e-9);
    assert(std::abs(cf01.imag()) < 1e-12);

    // CF(1) = exp(-0.5 * 1^2) = exp(-0.5) ≈ 0.6065306597
    std::complex<double> cf1 = defaultDist.cf(1.0);
    assert(std::abs(cf1.real() - 0.6065306597) < 1e-9);
    assert(std::abs(cf1.imag()) < 1e-12);

    // Test pdf
    double pdfValue = defaultDist.pdf(0.0);
    assert(std::abs(pdfValue - GaussianTool::ONE_OVER_SQRT_TWO_PI) < 1e-6); // pdf(0) for standard normal

    std::cout << "All tests passed for Standard Gaussian distribution!" << std::endl;
}

void testNonStandardGaussian() {
    // Gaussian with mean=2, sigma=3
    Gaussian dist(2.0, 3.0);
    assert(dist.getMu() == 2.0);
    assert(dist.getSigma() == 3.0);

    // Test CDF: cdf(x) = Phi((x - mu)/sigma)
    double cdf2 = dist.cdf(2.0); // at mean, z=0 => Phi(0)=0.5
    assert(std::abs(cdf2 - 0.5) < 1e-6);

    double cdf5 = dist.cdf(5.0); // z = (5-2)/3 = 1 => Phi(1) ≈ 0.841344746
    assert(std::abs(cdf5 - 0.841344746) < 1e-6);

    double cdfm1 = dist.cdf(-1.0); // z = (-1-2)/3 = -1 => Phi(-1) ≈ 0.158655254
    assert(std::abs(cdfm1 - 0.158655254) < 1e-6);

    // Test Quantile (inverse CDF)
    double q0_5 = dist.quantile(0.5);  // should return mu=2
    assert(std::abs(q0_5 - 2.0) < 1e-6);

    double q8413 = dist.quantile(0.841344746); // Phi^-1(0.8413) = 1 → X = mu + sigma*1 = 5
    assert(std::abs(q8413 - 5.0) < 1e-6);

    double q1586 = dist.quantile(0.158655254); // Phi^-1(0.1586) = -1 → X = mu + sigma*(-1) = -1
    assert(std::abs(q1586 + 1.0) < 1e-6);

    // Test Characteristic Function
    // cf(t) = exp(i*mu*t - 0.5*sigma^2*t^2)
    // For t=0 -> cf(0)=1
    std::complex<double> cf0 = dist.cf(0.0);
    assert(std::abs(cf0.real() - 1.0) < 1e-12);
    assert(std::abs(cf0.imag()) < 1e-12);

    // For t=0.1
    double realExpected01 = std::exp(-0.5 * 9.0 * 0.1 * 0.1) * std::cos(2.0 * 0.1);
    double imagExpected01 = std::exp(-0.5 * 9.0 * 0.1 * 0.1) * std::sin(2.0 * 0.1);
    std::complex<double> cf01 = dist.cf(0.1);
    assert(std::abs(cf01.real() - realExpected01) < 1e-9);
    assert(std::abs(cf01.imag() - imagExpected01) < 1e-9);

    // Test pdf at mean = 2 → should be maximum
    double pdfAtMu = dist.pdf(2.0);
    double expectedPdf = 1.0 / (3.0 * std::sqrt(2 * M_PI)); 
    assert(std::abs(pdfAtMu - expectedPdf) < 1e-6);

    std::cout << "All tests passed for Non-Standard Gaussian distribution!" << std::endl;
}

int main() {
    testSandardGaussian();
    testNonStandardGaussian();
    std::cout << "All tests passed!" << std::endl;
    return 0;
}