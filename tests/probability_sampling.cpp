#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <cassert>
#include "../include/core-math/probability/sampling.hpp"

void testUniformSampling()
{
    UniformSampling sampler(10000, 1); 
    std::shared_ptr<Uniform> uniform = std::dynamic_pointer_cast<Uniform>(sampler.getDistribution());
    std::cout << "Time taken for sampling 10000 values: " << sampler.getTimeTaken() << std::endl;
    assert((uniform->getMean() - sampler.getSampleMean())<1e-2); 
    assert((uniform->getVariance() - sampler.getSampleVariance())<1e-2); 
    assert((uniform->getSkewness() - sampler.getSampleSkewness())<1e-1); 
    assert((uniform->getKurtosis() - sampler.getSampleKurtosis())<1e-2); 

    sampler.run();
    std::cout << "Time taken for sampling 10000 values: " << sampler.getTimeTaken() << std::endl;
    assert((uniform->getMean() - sampler.getSampleMean())<1e-2); 
    assert((uniform->getVariance() - sampler.getSampleVariance())<1e-2); 
    assert((uniform->getSkewness() - sampler.getSampleSkewness())<1e-2); 
    assert((uniform->getKurtosis() - sampler.getSampleKurtosis())<1e-2); 

    std::cout << "All tests passed for uniform sampling!" << std::endl;
}

void testInverseGaussianSampling()
{
    GaussianSampling::Inverse sampler(10000, 1); 
    std::shared_ptr<Gaussian> gaussian = std::dynamic_pointer_cast<Gaussian>(sampler.getDistribution());
    std::cout << "Time taken for sampling 10000 values: " << sampler.getTimeTaken() << std::endl;
    assert((gaussian->getMean() - sampler.getSampleMean())<5e-2); 
    assert((gaussian->getVariance() - sampler.getSampleVariance())<5e-2); 
    assert((gaussian->getSkewness() - sampler.getSampleSkewness())<5e-2); 
    assert((gaussian->getKurtosis() - sampler.getSampleKurtosis())<5e-2); 

    sampler.run();
    std::cout << "Time taken for sampling 10000 values: " << sampler.getTimeTaken() << std::endl;
    assert((gaussian->getMean() - sampler.getSampleMean())<5e-2); 
    assert((gaussian->getVariance() - sampler.getSampleVariance())<5e-2); 
    assert((gaussian->getSkewness() - sampler.getSampleSkewness())<5e-2); 
    assert((gaussian->getKurtosis() - sampler.getSampleKurtosis())<5e-2); 

    std::cout << "All tests passed for inverse standard gaussian sampling!" << std::endl;
}

int main()
{
    testUniformSampling(); 
    testInverseGaussianSampling();
    return 0;
}