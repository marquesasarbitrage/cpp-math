#include "../include/cpp-math/quadratures.hpp"

GaussianQuadrature::GaussianQuadrature(int points): points_(std::max(2, points)), computed_(false), roots_({}), weights_({}){}; 

int GaussianQuadrature::getPoints() const{return points_;}
bool GaussianQuadrature::isComputed() const{return computed_;}
std::vector<double> GaussianQuadrature::getRoots(){compute();return roots_;}
std::vector<double> GaussianQuadrature::getWeights(){compute();return weights_;}

double GaussianQuadrature::integrate(const std::function<double(double)>& f)
{
    compute();
    double integral = 0.0;
    for (int i = 0; i < points_; i++){integral += weights_[i] * f(roots_[i]);}
    return integral;
}

void GaussianQuadrature::setPoints(int points){points_=std::max(2, points); computed_=false; setWeigts({}); setRoots({});}
void GaussianQuadrature::setWeigts(std::vector<double> weights){weights_ = weights;}
void GaussianQuadrature::setRoots(std::vector<double> roots){roots_ = roots;}
void GaussianQuadrature::setIsCompute(bool value){computed_ = value;}

void GaussianQuadrature::compute()
{
    if (!isComputed()){
        _compute(); 
        setIsCompute(true); 
    }
}

GaussLaguerreQuadrature::GaussLaguerreQuadrature(int points): GaussianQuadrature(points){compute();}

void GaussLaguerreQuadrature::_compute()
{
    int points = getPoints();
    Eigen::MatrixXd J = Eigen::MatrixXd::Zero(points, points);

    for (int k = 0; k < points; k++)
    {
        J(k, k) = 2.0 * k + 1.0;
    }

    for (int k = 0; k < points - 1; k++)
    {
        double b = k + 1.0;
        J(k, k + 1) = b;
        J(k + 1, k) = b;
    }

    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> solver(J);
    Eigen::VectorXd roots = solver.eigenvalues();
    std::vector<double> weights(points);
    Eigen::MatrixXd V = solver.eigenvectors();
    for (int i = 0; i < points; i++)
    {
        weights[i] = std::pow(V(0, i), 2);
    }
    setWeigts(weights); 
    setRoots(std::vector<double>(roots.data(), roots.data() + roots.size()));
}


