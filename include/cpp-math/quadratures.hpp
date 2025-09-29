#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include <Eigen/Dense>

class GaussianQuadrature
{
    public: 
        GaussianQuadrature(int points); 
        virtual ~GaussianQuadrature() = default;
        

        int getPoints() const;
        double integrate(const std::function<double(double)>& f);
        std::vector<double> getRoots();
        std::vector<double> getWeights();
        bool isComputed() const;

        void setPoints(int points); 
        void compute(); 

    protected: 
        void setWeigts(std::vector<double> weights); 
        void setRoots(std::vector<double> roots); 
        void setIsCompute(bool value);
        virtual void _compute() = 0;

    private:
        int points_;
        bool computed_;
        std::vector<double> roots_;
        std::vector<double> weights_;
};

class GaussLaguerreQuadrature final: public GaussianQuadrature
{
    public: 
        GaussLaguerreQuadrature(int points); 
        ~GaussLaguerreQuadrature() = default;
    protected: 
        void _compute() override;

};

