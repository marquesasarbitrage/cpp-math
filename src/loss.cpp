#include "../include/core-math/loss.hpp"

EstimatorLoss::EstimatorLoss(const std::vector<double>& estimates, const std::vector<double>& trueValues): 
estimates_(EigenTool::getEigenVector(estimates)), trueValues_(EigenTool::getEigenVector(trueValues))
{
    if (estimates.size() != trueValues.size()) throw MathErrorRegistry::Loss::MismatchVectorSizeError(); 
    if (estimates.empty()) throw MathErrorRegistry::Loss::EmptyVectorError(); 
    residuals_ = estimates_ - trueValues_;
}

EstimatorLoss::EstimatorLoss(const Eigen::VectorXd& estimates, const Eigen::VectorXd& trueValues): estimates_(estimates), trueValues_(trueValues)
{
    if (estimates.size() != trueValues.size()) throw MathErrorRegistry::Loss::MismatchVectorSizeError(); 
    if (estimates.size()==0) throw MathErrorRegistry::Loss::EmptyVectorError(); 
    residuals_ = estimates_ - trueValues_;
}

Eigen::VectorXd EstimatorLoss::getResiduals() const {return residuals_;} 
std::vector<double> EstimatorLoss::getResidualsAsVector() const 
{
    std::vector<double> stdVec(residuals_.data(), residuals_.data() + residuals_.size()); 
    return stdVec;
}

void EstimatorLoss::setEstimates(const std::vector<double>& estimates)
{
    if (estimates.size() != trueValues_.size()) throw MathErrorRegistry::Loss::MismatchVectorSizeError(); 
    estimates_ = EigenTool::getEigenVector(estimates);
    residuals_ = estimates_ - trueValues_;
}
double EstimatorLoss::getMAE() const {return residuals_.cwiseAbs().mean();}
double EstimatorLoss::getMSE() const {return residuals_.squaredNorm() / estimates_.size();}
double EstimatorLoss::getRMSE() const {return std::sqrt(getMSE());}

double EstimatorLoss::getMAPE() const 
{
    try {
        Eigen::ArrayXd denom = trueValues_.array();
        Eigen::ArrayXd errors = (residuals_.array() / denom).abs();
        return errors.mean() * 100.0;
    }catch (const std::exception& e){
        return NAN;
    }
}

double EstimatorLoss::getMSLE() const 
{
    try {
        Eigen::ArrayXd logEst = (estimates_.array() + 1.0).log();
        Eigen::ArrayXd logTrue = (trueValues_.array() + 1.0).log();
        Eigen::ArrayXd diff = logEst - logTrue;
        return diff.square().mean();
    }catch (const std::exception& e){
        return NAN;
    }
}

double EstimatorLoss::getMSPE() const 
{
    try {
        Eigen::ArrayXd denom = trueValues_.array();
        Eigen::ArrayXd errors = (residuals_.array() / denom).square();
        return errors.mean() * 100.0;
    }catch (const std::exception& e){
        return NAN;
    }
}

double EstimatorLoss::getRMPSE() const {return std::sqrt(getMSPE());}

