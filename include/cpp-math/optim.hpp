#pragma once
#include <iostream>
#include <vector>
#include <chrono>
#include <set>
#include <iterator>
#include "errors.hpp"

class Optimizer
{
    public: 
        Optimizer(); 
        virtual ~Optimizer(){}; 

        void optimize();

        bool isOptimized() const; 
        double getToleranceThreshold() const; 
        int getMaximumIterations() const; 
        double getTimeTaken() const; 
        std::exception_ptr getError() const;

        void setToleranceThreshold(double value); 
        void setMaximumIterations(int value); 

    protected: 
        void setOptimized(bool value);
        virtual void _optimize() = 0;
    
    private: 
        bool optimized_;
        double toleranceThreshold_; 
        int maximumIterations_; 
        double timeTaken_; 
        std::exception_ptr optimizeError;
};

class NewtonRaphson final: public Optimizer
{
    public: 
        NewtonRaphson(double x0, const std::function<double(double)>& f, const std::function<double(double)>& fDeriv);
        ~NewtonRaphson(){}; 

        double getResult();
        double getFunctionResult();
        int getNumberIterations(); 
        double getStartValue() const; 
        double evaluateFunction(double value) const; 
        double evaluateFunctionDerivative(double value) const; 

        void setStartValue(double x0);

    protected: 
        void _optimize() override;
    
    private:
        double x0_; 
        const std::function<double(double)>& f_; 
        const std::function<double(double)>& fDeriv_;
        double result_;
        double fResult_; 
        int numberIterations_; 

};