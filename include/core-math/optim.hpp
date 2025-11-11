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
        virtual ~Optimizer() = default; 

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
        ~NewtonRaphson() = default; 

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


class NelderMead final: public Optimizer
{
    public: 
        NelderMead(const std::vector<double>& x0,const std::function<double(std::vector<double>)>& f);
        ~NelderMead() = default ;  

        enum class InitSimplexMethod {BASIC, SCALED, SYMMETRIC};
        
        void setStartValues(std::vector<double> x0);
        void setInitSimplexMethod(const InitSimplexMethod& method);
        void setPerturbationParam(double epsilon);
        void setReflectionParam(double alpha);
        void setExpansionParam(double beta);
        void setContractionParam(double gamma);
        void setShrinkParam(double delta);

        std::vector<double> getStartValues() const;
        InitSimplexMethod getInitSimplexMethod() const;
        double getPerturbationParam() const;
        double getReflectionParam() const;
        double getExpansionParam() const;
        double getContractionParam() const;
        double getShrinkParam() const;
        std::vector<double> getResult();
        double getFunctionResult();
        int getNumberIterations(); 
    
    protected: 
        void _optimize() override;

    private: 
        std::vector<double> x0_; 
        const int n_;
        const std::function<double(std::vector<double>)> f_;
        double epsilon_; 
        double alpha_; 
        double beta_; 
        double gamma_; 
        double delta_;
        InitSimplexMethod initSimplexMethod_;

        struct Vertex
        {
            Vertex(const std::vector<double>& points, double value); 
            ~Vertex() = default;
            std::vector<double> points_; 
            double value_; 
        }; 

        struct Simplex
        {
            Simplex(const std::vector<Vertex>& vertices); 
            ~Simplex() = default;
            
            std::vector<Vertex> vertices_; 

            void sortVertices();
            void setVertex(const Vertex& vertex_, int i);
            Vertex getBest() const;
            Vertex getWorst() const;
            Vertex getSecondWorst() const;

        };

        double getSymAlpha() const;
        Vertex getVertex(std::vector<double> x_input) const;
        Simplex getInitialSimplex() const; 
        Vertex getCentroid(const Simplex& simplex_) const;
        Vertex getReflection(const Vertex& centroid, const Vertex& worst) const; 
        Vertex getExpansion(const Vertex& centroid, const Vertex& reflection) const; 
        Vertex getContraction(const Vertex& centroid, const Vertex& worst) const; 
        Simplex getShrink(const Simplex& simplex_) const; 
        bool checkConvergence(const Simplex& simplex_) const;

        std::vector<double> result_;
        double fResult_; 
        int numberIterations_; 
};