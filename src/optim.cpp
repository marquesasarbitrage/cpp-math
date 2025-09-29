#include "../include/cpp-math/optim.hpp"

Optimizer::Optimizer(): optimized_(false), toleranceThreshold_(1e-9), maximumIterations_(100), timeTaken_(0.0), optimizeError(nullptr){};

void Optimizer::optimize()
{
    if (!optimized_)
    {
        auto start = std::chrono::high_resolution_clock::now();
        try{
            _optimize();
        } catch (const std::exception& e){
            optimizeError = std::current_exception(); 
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        optimized_ = true;
        std::chrono::duration<double> elapsed = end - start;
        timeTaken_ = elapsed.count();
    }
}

void Optimizer::setOptimized(bool value){optimized_ = value;}

bool Optimizer::isOptimized() const{return optimized_;}
double Optimizer::getToleranceThreshold() const{return toleranceThreshold_;}
int Optimizer::getMaximumIterations() const{return maximumIterations_;}
double Optimizer::getTimeTaken() const{return timeTaken_;}
std::exception_ptr Optimizer::getError() const{return optimizeError;}

void Optimizer::setToleranceThreshold(double value)
{
    toleranceThreshold_ = value; 
    optimized_ = false; 
}

void Optimizer::setMaximumIterations(int value)
{
    maximumIterations_ = value; 
    optimized_ = false; 
}

NewtonRaphson::NewtonRaphson(double x0, const std::function<double(double)>& f, const std::function<double(double)>& fDeriv): x0_(x0), f_(f), fDeriv_(fDeriv){};

double NewtonRaphson::getResult()
{
    optimize(); 
    if (getError())
    {
        return nan("");
    }else{
        return result_;
    }
    
}

double NewtonRaphson::getFunctionResult()
{
    optimize(); 
    if (getError())
    {
        return nan("");
    }else{
        return fResult_;
    }
}

int NewtonRaphson::getNumberIterations(){optimize(); return numberIterations_;}
double NewtonRaphson::getStartValue() const{return x0_;}
double NewtonRaphson::evaluateFunction(double value) const{return f_(value);}
double NewtonRaphson::evaluateFunctionDerivative(double value) const{return fDeriv_(value);}

void NewtonRaphson::setStartValue(double x0){x0_ = x0; setOptimized(false);}

void NewtonRaphson::_optimize()
{
    double x = x0_;
    double fX; 
    double dfX; 
    double xStep = 0;
    numberIterations_ = 0;
    for (int i = 1; i <= getMaximumIterations(); ++i) {

        numberIterations_ += 1;
        fX = f_(x);
        dfX = fDeriv_(x);
        if (std::abs(fX) < getToleranceThreshold()){
            break;
        }
        
        if (std::abs(dfX) < 1e-12) {throw MathErrorRegistry::Optim::NewtonRaphsonDerivativeZeroError();}
        
        double xNew = x - fX / dfX;
        xStep = std::abs(xNew - x);
        if (xStep < getToleranceThreshold()) {x = xNew; break;}

        if (i==getMaximumIterations()) break;
        
        x = xNew;
    }
    result_ = x; 
    fResult_ = fX; 
}