#include "../include/cpp-math/probability/distributions.hpp"

Uniform::Uniform(double a, double b): a_(a), b_(b){if (b_<=a_) throw MathErrorRegistry::Probability::InvalidUniformBoundError();}
Uniform::Uniform(): a_(0.0), b_(1.0){}
double Uniform::getLowerBound() const {return a_;}
double Uniform::getUpperBound() const {return b_;}
double Uniform::getMean() const {return .5*(a_ + b_);}
double Uniform::getVariance() const {return (b_+a_)*(b_+a_)/12.0;}
double Uniform::getSkewness() const {return 0.0;}
double Uniform::getKurtosis() const {return -6/5;}
std::complex<double> Uniform::cf(double t) const 
{
    if (t == 0.0) return std::complex<double>(1.0, 0.0);
    std::complex<double> i(0.0, 1.0); 
    return (exp(i * t * b_) - exp(i * t * a_)) / (i * t * (b_ - a_));
}
double Uniform::cdf(double x) const 
{
    if (x<a_) return 0.0;
    if (x>b_) return 1.0;
    return (x-a_)/(b_-a_);

}
double Uniform::pdf(double x) const 
{
    if (x<a_ or x>b_) return 0.0;
    return 1.0/(b_-a_);
}

Gaussian::Gaussian(double mu, double sigma): mu_(mu), sigma_(sigma){if (sigma<=0) throw MathErrorRegistry::Probability::InvalidSigmaGaussianError();}
Gaussian::Gaussian(): mu_(0.0), sigma_(1.0){}
double Gaussian::getMu() const {return mu_;} 
double Gaussian::getSigma() const {return sigma_;} 
double Gaussian::getMean() const {return mu_;}
double Gaussian::getVariance() const {return sigma_*sigma_;}
double Gaussian::getSkewness() const {return 0.0;}
double Gaussian::getKurtosis() const {return 0.0;}
std::complex<double> Gaussian::cf(double t) const
{
    std::complex<double> i(0.0, 1.0); // Imaginary unit i
    return exp(i * t * mu_ - 0.5 * sigma_ * sigma_ * t * t);
}

// Code source copied from https://github.com/vollib/lets_be_rational/
double Gaussian::cdf(double x) const
{
    double z = (x - mu_)/sigma_;
    return GaussianTool::standardGaussianCdf(z);
}

double Gaussian::pdf(double x) const
{
    double z = (x - mu_)/sigma_;
    return GaussianTool::standardGaussianPdf(z)/sigma_;
}

double Gaussian::quantile(double p) const
{
    return mu_ + sigma_ * GaussianTool::acklamStandardGaussianInverseCdf(p);
}