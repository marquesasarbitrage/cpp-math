#include "../../include/cpp-math/probability/sampling.hpp"

RandomSampling::RandomSampling(const std::shared_ptr<ProbabilityDistribution>& dist, int N, int M): 
dist_(dist), N_(std::max(std::abs(N),1)), M_(std::max(std::abs(M),1)), timeTaken_(0.0), sample_(Eigen::MatrixXd::Zero(N_,M_)) {}

void RandomSampling::run()
{
    auto start = std::chrono::high_resolution_clock::now(); 
    setSample(Eigen::MatrixXd::Zero(N_,M_));
    _run();
    auto end = std::chrono::high_resolution_clock::now(); 
    timeTaken_ = std::chrono::duration<double>(end - start).count();
}

int RandomSampling::getN() const{return N_;}
int RandomSampling::getM() const{return M_;} 
double RandomSampling::getTimeTaken() const{return timeTaken_;}
Eigen::MatrixXd RandomSampling::getSample() const {return sample_;}
std::shared_ptr<ProbabilityDistribution> RandomSampling::getDistribution() const{return dist_;}
double RandomSampling::getSampleMean() const {return sample_.mean();}
double RandomSampling::getSampleVariance() const
{
    double mean = sample_.mean();
    Eigen::ArrayXd centered = sample_.array() - mean;
    return (centered.square().sum()) / (sample_.size() - 1);
}
double RandomSampling::getSampleSkewness() const
{
    int n = sample_.size();
    double mean = sample_.mean();
    Eigen::ArrayXd centered = sample_.array() - mean;
    double m2 = centered.square().mean();
    double m3 = (centered.pow(3)).mean();
    return m3 / std::pow(m2, 1.5);
}
double RandomSampling::getSampleKurtosis() const
{
    int n = sample_.size();
    double mean = sample_.mean();
    Eigen::ArrayXd centered = sample_.array() - mean;
    double m2 = centered.square().mean();
    double m4 = (centered.pow(4)).mean();
    return m4 / (m2 * m2);
}

void RandomSampling::setSample(const Eigen::MatrixXd& sample) {sample_ = sample;}


UniformSampling::UniformSampling(std::shared_ptr<Uniform> uniform, int N, int M): 
RandomSampling(uniform,N,M), cuniform_(uniform->getLowerBound(), uniform->getUpperBound()), gen_(std::random_device{}()) {run();}
UniformSampling::UniformSampling(int N, int M): 
RandomSampling(std::make_shared<Uniform>(),N,M), cuniform_(0.0,1.0), gen_(std::random_device{}()){run();}

void UniformSampling::_run()
{
    setSample(Eigen::MatrixXd::NullaryExpr(getN(), getM(),[&]() { return cuniform_(gen_); }));
}

namespace GaussianSampling 
{
    Base::Base(std::shared_ptr<Gaussian> gaussian, int N, int M): RandomSampling(gaussian,N,M){};
    Base::Base(int N, int M): RandomSampling(std::make_shared<Gaussian>(),N,M){}; 

    Inverse::Inverse(std::shared_ptr<Gaussian> gaussian, int N, int M): Base(gaussian,N,M){run();};
    Inverse::Inverse(int N, int M): Base(std::make_shared<Gaussian>(),N,M){run();}; 

    void Inverse::_run()
    {
        UniformSampling uniformSampling(getN(), getM()); 
        Eigen::MatrixXd u = uniformSampling.getSample();
        setSample(u.unaryExpr([this](double x) { return std::dynamic_pointer_cast<Gaussian>(getDistribution())->quantile(x); }));
    }

}
