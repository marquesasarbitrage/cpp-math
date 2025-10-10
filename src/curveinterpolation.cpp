#include "../include/cpp-math/curveinterpolation.hpp"

CurveInterpolation::CurveInterpolation(const std::map<double, double>& data){classSetter(data);}

void CurveInterpolation::classSetter(const std::map<double, double>& data)
{
    int n = data.size();
    if (n < 2){throw MathErrorRegistry::CurveInterpolation::WrongVectorSizeError();}
    int i = 0;
    for(auto const& imap: data)
    {
        if (i == 0){lowerBoundX_ = imap.first;}
        if (i == n-1){upperBoundX_ = imap.first;}
        i++;
        xVector_.push_back(imap.first);
        yVector_.push_back(imap.second);
        
    }    
}

double CurveInterpolation::evaluate(double x) const
{
    if (x < lowerBoundX_ || x > upperBoundX_) throw MathErrorRegistry::CurveInterpolation::OutOfRangeCurveInterpolationError();
    return _evaluate(x);
}

double CurveInterpolation::evaluateFirstDerivative(double x) const
{
    if (x < lowerBoundX_ || x > upperBoundX_) throw MathErrorRegistry::CurveInterpolation::OutOfRangeCurveInterpolationError();
    return _evaluateFirstDerivative(x);
}

double CurveInterpolation::evaluateSecondDerivative(double x) const
{
    if (x < lowerBoundX_ || x > upperBoundX_) throw MathErrorRegistry::CurveInterpolation::OutOfRangeCurveInterpolationError();
    return _evaluateSecondDerivative(x);
}

int CurveInterpolation::findIndex(double x) const
{
    if (x < lowerBoundX_ || x > upperBoundX_) throw MathErrorRegistry::CurveInterpolation::OutOfRangeCurveInterpolationError();
    if (x==lowerBoundX_) return 1; 
    if (x==upperBoundX_) return xVector_.size();
    auto it = std::lower_bound(xVector_.begin(), xVector_.end(), x);
    int i = std::distance(xVector_.begin(), it);
    return i;
}

double CurveInterpolation::getLowerBoundX() const{return lowerBoundX_;}
double CurveInterpolation::getUpperBoundX() const{return upperBoundX_;}
double CurveInterpolation::getX(int i) const {return xVector_[i];};
double CurveInterpolation::getY(int i) const {return yVector_[i];};
std::vector<double> CurveInterpolation::getVectorX() const {return xVector_;}
std::vector<double> CurveInterpolation::getVectorY() const {return yVector_;}

std::map<double, double> CurveInterpolation::getInitialData() const 
{
    std::map<double, double> result;
    for (std::size_t i = 0; i < xVector_.size(); ++i) {
        result.emplace(xVector_[i], yVector_[i]);
    }

    return result;
}

LinearInterpolation::LinearInterpolation(const std::map<double, double>& data): CurveInterpolation(data){}

double LinearInterpolation::_evaluate(double x) const
{
    int i = findIndex(x);
    double x0 = getX(i - 1), x1 = getX(i);
    double y0 = getY(i - 1), y1 = getY(i);
    double t = (x - x0) / (x1 - x0);
    return y0 + t * (y1 - y0);
}

double LinearInterpolation::_evaluateFirstDerivative(double x) const
{
    int i = findIndex(x);
    double x0 = getX(i - 1), x1 = getX(i);
    double y0 = getY(i - 1), y1 = getY(i);
    return (y1 - y0) / (x1 - x0);
} 

double LinearInterpolation::_evaluateSecondDerivative(double x) const {return 0.0;} 

CubicSpline::CubicSpline(const std::map<double, double>& data): CurveInterpolation(data){classSetter();}

void CubicSpline::classSetter()
{
    x_ = getVectorX();
    int n = x_.size() - 1; 
    a_ = getVectorY();
    
    std::vector<double> h(n);
    std::vector<double> alpha(n, 0.0);

    for (int i = 0; i < n; ++i) {
        h[i] = x_[i + 1] - x_[i];
    }

    for (int i = 1; i < n; ++i) {
        alpha[i] = (3.0 / h[i]) * (a_[i + 1] - a_[i]) - (3.0 / h[i - 1]) * (a_[i] - a_[i - 1]);
    }

    std::vector<double> l(n + 1, 0.0);
    std::vector<double> mu(n, 0.0);
    std::vector<double> z(n + 1, 0.0);
    
    l[0] = 1.0;  
    mu[0] = 0.0;
    z[0] = 0.0;

    for (int i = 1; i < n; ++i) {
        l[i] = 2.0 * (x_[i + 1] - x_[i - 1]) - h[i - 1] * mu[i - 1];
        mu[i] = h[i] / l[i];
        z[i] = (alpha[i] - h[i - 1] * z[i - 1]) / l[i];
    }

    l[n] = 1.0;
    z[n] = 0.0;
    
    c_.resize(n + 1, 0.0);
    b_.resize(n, 0.0);
    d_.resize(n, 0.0);
    
    for (int j = n - 1; j >= 0; --j) {
        c_[j] = z[j] - mu[j] * c_[j + 1];
        b_[j] = (a_[j + 1] - a_[j]) / h[j] - h[j] * (c_[j + 1] + 2.0 * c_[j]) / 3.0;
        d_[j] = (c_[j + 1] - c_[j]) / (3.0 * h[j]);
    }
}

double CubicSpline::_evaluate(double x) const
{
    int i = findIndex(x);
    double dx = x - getX(i-1);
    return a_[i-1] + b_[i-1] * dx + c_[i-1] * dx * dx + d_[i-1] * dx * dx * dx;
}

double CubicSpline::_evaluateFirstDerivative(double x) const
{
    int i = findIndex(x);
    double dx = x - getX(i-1);
    return b_[i-1] + 2.0*c_[i-1] * dx + 3.0*d_[i-1] * dx * dx;
}

double CubicSpline::_evaluateSecondDerivative(double x) const
{
    int i = findIndex(x);
    double dx = x - getX(i-1);
    return 2.0*c_[i-1] + 6.0*d_[i-1] * dx;
}