#pragma once 
#include <iostream>
#include <random>
#include "distributions.hpp"
#include <Eigen/Dense>

class RandomSampling
{
    public: 
        RandomSampling(const std::shared_ptr<ProbabilityDistribution>& dist, int N, int M); 
        virtual ~RandomSampling() = default; 

        void run(); 

        std::shared_ptr<ProbabilityDistribution> getDistribution() const; 
        int getN() const; 
        int getM() const; 
        double getTimeTaken() const; 
        Eigen::MatrixXd getSample(); 
        double getSampleMean(); 
        double getSampleVariance(); 
        double getSampleSkewness(); 
        double getSampleKurtosis(); 
        bool isExecuted() const; 

    protected: 
        virtual void _run() = 0; 
        void setSample(const Eigen::MatrixXd& sample);

    private: 
        bool executed_; 
        const std::shared_ptr<ProbabilityDistribution> dist_;
        const int N_; const int M_; 
        double timeTaken_; 
        Eigen::MatrixXd sample_; 

};

class UniformSampling final : public RandomSampling
{
    public: 
        UniformSampling(std::shared_ptr<Uniform> uniform, int N, int M); 
        UniformSampling(int N, int M); 
        ~UniformSampling() = default; 
    
    protected: 
        void _run() override;
    
    private:
        std::uniform_real_distribution<double> cuniform_;
        std::mt19937 gen_;

}; 

namespace GaussianSampling
{
    class Base : public RandomSampling
    {
        public:
            Base(std::shared_ptr<Gaussian> gaussian, int N, int M); 
            Base(int N, int M); 
            virtual ~Base() = default;
        
        protected: 
            virtual void _run() override = 0;
    };

    class Inverse final: public Base
    {
        public:
            Inverse(std::shared_ptr<Gaussian> gaussian, int N, int M); 
            Inverse(int N, int M); 
            ~Inverse() = default;
        protected: 
            void _run() override;

    };
};

