#include "../include/cpp-math/regression.hpp"


Regression::Regression(const std::vector<double>& target, const std::vector<std::vector<double>>& features): 
target_(EigenTool::getEigenVector(target)), features_(EigenTool::getEigenMatrix(features)), executed_(false), timeTaken_(0.0), regressionError_(nullptr)
{
    if (target_.size() != features_.rows()) throw MathErrorRegistry::Regression::MismatchTargetFeaturesSizeError();
}

Regression::Regression(const Eigen::VectorXd& target, const Eigen::MatrixXd& features):
target_(target), features_(features), executed_(false), timeTaken_(0.0), regressionError_(nullptr)
{
    if (target_.size() != features_.rows()) throw MathErrorRegistry::Regression::MismatchTargetFeaturesSizeError();
}
Eigen::VectorXd Regression::getTarget() const {return target_;}
Eigen::MatrixXd Regression::getFeatures() const {return features_;}
double Regression::getTimeTaken() const {return timeTaken_;}
std::exception_ptr Regression::getRegressionError() const {return regressionError_;}
bool Regression::isExecuted() const{return executed_;}

void Regression::fit()
{
    auto start = std::chrono::high_resolution_clock::now();
    try{
        _fit();
    } catch (const std::exception& e){
        regressionError_ = std::current_exception(); 
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    executed_ = true;
    std::chrono::duration<double> elapsed = end - start;
    timeTaken_ = elapsed.count();
}

OrdinaryLeastSquare::OrdinaryLeastSquare(const std::vector<double>& target, const std::vector<std::vector<double>>& features, bool fitIntercept): 
Regression(EigenTool::getEigenVector(target), getProcessedFeatures(EigenTool::getEigenMatrix(features), fitIntercept)), 
fitIntercept_(fitIntercept),intercept_(0.0), coefficients_({}), residualsObject_{{0.0}, {0.0}}
{
    fit();
};

OrdinaryLeastSquare::OrdinaryLeastSquare(const Eigen::VectorXd& target, const Eigen::MatrixXd& features, bool fitIntercept): 
Regression(target, getProcessedFeatures(features, fitIntercept)), fitIntercept_(fitIntercept),
intercept_(0.0), coefficients_({}), residualsObject_{{0.0}, {0.0}}
{
    fit();
};

double OrdinaryLeastSquare::getIntercept() const {return intercept_;}
std::vector<double> OrdinaryLeastSquare::getCoefficients() const {return coefficients_;}
EstimatorLoss OrdinaryLeastSquare::getResidualsObject() const {return residualsObject_;}
std::vector<double> OrdinaryLeastSquare::getResiduals() const {return residualsObject_.getResidualsAsVector();}
double OrdinaryLeastSquare::getRSquared() const
{
    double residualsSumOfSquares = residualsObject_.getResiduals().squaredNorm();
    Eigen::VectorXd target = getTarget();
    double totalSumOfSquares = (target.array() - target.mean()).matrix().squaredNorm();
    return 1.0 - (residualsSumOfSquares / totalSumOfSquares);
}

Eigen::MatrixXd OrdinaryLeastSquare::getProcessedFeatures(const Eigen::MatrixXd& features, bool fitIntercept)
{
    if (!fitIntercept) return features;
    Eigen::MatrixXd newFeatures(features.rows(), features.cols() + 1);
    newFeatures.col(0) = Eigen::VectorXd::Ones(features.rows()); // first column = ones
    newFeatures.block(0, 1, features.rows(), features.cols()) = features;
    return newFeatures;
}

void OrdinaryLeastSquare::_fit()
{
    Eigen::MatrixXd F = getFeatures(); 
    Eigen::VectorXd T = getTarget(); 
    Eigen::VectorXd X = (F.transpose() * F).ldlt().solve(F.transpose() * T);
    intercept_ = fitIntercept_ ? X(0) : 0.0;
    Eigen::VectorXd estimators = F * X;
    Eigen::VectorXd coeffs = X.segment(fitIntercept_ ? 1 : 0, X.size() - 1); 
    coefficients_ = std::vector<double>(coeffs.data(), coeffs.data() + coeffs.size());
    residualsObject_ = EstimatorLoss(estimators, T);
}

double OrdinaryLeastSquare::getPrediction(const std::vector<double> featureValues)
{
    std::vector<double> coeffs = getCoefficients();
    if (featureValues.size() != coeffs.size()) throw MathErrorRegistry::Regression::MismatchTargetFeaturesSizeError();
    double predicted = intercept_;
    for (size_t i = 0; i < coeffs.size(); ++i) 
    {
        predicted += coeffs[i] * featureValues[i];
    }
    return predicted;
}


