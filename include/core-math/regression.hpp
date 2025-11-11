#pragma once 
#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include "loss.hpp"
#include "errors.hpp"
#include "tools.hpp"

class Regression 
{
    public: 
        Regression(const std::vector<double>& target, const std::vector<std::vector<double>>& features); 
        Regression(const Eigen::VectorXd& target, const Eigen::MatrixXd& features); 
        virtual ~Regression() = default; 

        Eigen::VectorXd getTarget() const; 
        Eigen::MatrixXd getFeatures() const; 
        double getTimeTaken() const; 
        std::exception_ptr getRegressionError() const;
        
    protected: 
        void fit(); 
        virtual void _fit() = 0;
        bool isExecuted() const;

    private: 
        const Eigen::VectorXd target_; 
        const Eigen::MatrixXd features_; 
        bool executed_; 
        double timeTaken_; 
        std::exception_ptr regressionError_;

}; 

class OrdinaryLeastSquare final : public Regression
{
    public: 
        OrdinaryLeastSquare(const std::vector<double>& target, const std::vector<std::vector<double>>& features, bool fitIntercept); 
        OrdinaryLeastSquare(const Eigen::VectorXd& target, const Eigen::MatrixXd& features, bool fitIntercept); 
        ~OrdinaryLeastSquare() = default; 

        double getIntercept() const;
        std::vector<double> getCoefficients() const;
        EstimatorLoss getResidualsObject() const; 
        std::vector<double> getResiduals() const; 
        double getRSquared() const;
        double getPrediction(const std::vector<double> featureValues);
    
    protected: 
        void _fit() override;

    private:
        bool fitIntercept_;
        double intercept_; 
        std::vector<double> coefficients_; 
        EstimatorLoss residualsObject_; 
        Eigen::MatrixXd getProcessedFeatures(const Eigen::MatrixXd& features, bool fitIntercept);
};

