#pragma once 
#include <iostream>
#include <map>
#include "errors.hpp"

class CurveInterpolation
{
    public:
        CurveInterpolation(const std::map<double, double>& data); 
        virtual ~CurveInterpolation(){};

        double evaluate(double x) const; 
        double evaluateFirstDerivative(double x) const; 
        double evaluateSecondDerivative(double x) const; 

    protected: 
        virtual double _evaluate(double x) const = 0;
        virtual double _evaluateFirstDerivative(double x) const = 0; 
        virtual double _evaluateSecondDerivative(double x) const = 0; 

        int findIndex(double x) const;
        double getLowerBoundX() const;
        double getUpperBoundX() const;
        double getX(int i) const;
        double getY(int i) const;
        std::vector<double> getVectorX() const;
        std::vector<double> getVectorY() const;

    private:
        void classSetter(const std::map<double, double>& data);
        std::vector<double> xVector_; 
        std::vector<double> yVector_; 
        double lowerBoundX_;
        double upperBoundX_ ;
        

};

class LinearInterpolation final: public CurveInterpolation
{
    public: 
        LinearInterpolation(const std::map<double, double>& data);
        ~LinearInterpolation() = default;
    
    protected: 
        double _evaluate(double x) const override;
        double _evaluateFirstDerivative(double x) const override; 
        double _evaluateSecondDerivative(double x) const override; 
};

class CubicSpline final: public CurveInterpolation
{
    public: 
        CubicSpline(const std::map<double, double>& data);
        ~CubicSpline() = default;
    protected: 
        double _evaluate(double x) const override;
        double _evaluateFirstDerivative(double x) const override; 
        double _evaluateSecondDerivative(double x) const override; 
    private:
        void classSetter();
        std::vector<double> a_, b_, c_, d_, x_;
    
};

