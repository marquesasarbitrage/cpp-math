#pragma once 
#include <iostream>
#include <complex>
#include <cfloat>
#include "../../../include/core-math/errors.hpp"
#include "../../../include/core-math/tools.hpp"

class ProbabilityDistribution
{
    public: 
        ProbabilityDistribution(){}; 
        virtual ~ProbabilityDistribution() = default; 

        virtual double getMean() const = 0;
        virtual double getVariance() const = 0;
        virtual double getSkewness() const = 0;
        virtual double getKurtosis() const = 0;

        virtual std::complex<double> cf(double t) const = 0; 

};

class Uniform final: public ProbabilityDistribution
{
    public: 
        Uniform(double a, double b); 
        Uniform(); 
        ~Uniform() = default; 
        double getLowerBound() const; 
        double getUpperBound() const; 
        double getMean() const override;
        double getVariance() const override;
        double getSkewness() const override;
        double getKurtosis() const override;
        std::complex<double> cf(double t) const override; 
        double cdf(double x) const; 
        double pdf(double x) const; 
    private: 
        const double a_; 
        const double b_; 
}; 

class Gaussian final: public ProbabilityDistribution
{
    public: 
        Gaussian(double mu, double sigma); 
        Gaussian();
        ~Gaussian() = default; 
        double getMu() const; 
        double getSigma() const;
        double getMean() const override;
        double getVariance() const override;
        double getSkewness() const override;
        double getKurtosis() const override;
        std::complex<double> cf(double t) const override; 
        double cdf(double x) const; 
        double pdf(double x) const; 
        double quantile(double p) const; 
    private: 
        const double mu_; 
        const double sigma_; 
};