#pragma once 
#include <iostream>
#include <Eigen/Dense>
#include "errors.hpp"
#include "tools.hpp"

class EstimatorLoss 
{
    public: 
        EstimatorLoss(const std::vector<double>& estimates, const std::vector<double>& trueValues); 
        EstimatorLoss(const Eigen::VectorXd& estimates, const Eigen::VectorXd& trueValues); 
        ~EstimatorLoss() = default;

        void setEstimates(const std::vector<double>& estimates); 
        Eigen::VectorXd getResiduals() const; 
        std::vector<double> getResidualsAsVector() const; 
        
        double getMAE() const; 
        double getMSE() const; 
        double getRMSE() const; 
        double getMAPE() const; 
        double getMSLE() const; 
        double getMSPE() const; 
        double getRMPSE() const;

    private: 
        Eigen::VectorXd estimates_;
        Eigen::VectorXd trueValues_;
        Eigen::VectorXd residuals_;
};