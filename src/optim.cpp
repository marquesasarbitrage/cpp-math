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
void Optimizer::setToleranceThreshold(double value){toleranceThreshold_ = value; optimized_ = false; }
void Optimizer::setMaximumIterations(int value){maximumIterations_ = value; optimized_ = false;}

NewtonRaphson::NewtonRaphson(double x0, const std::function<double(double)>& f, const std::function<double(double)>& fDeriv): x0_(x0), f_(f), fDeriv_(fDeriv){};

double NewtonRaphson::getResult(){optimize(); return getError() ? NAN : result_;}
double NewtonRaphson::getFunctionResult(){optimize(); return getError() ? NAN : fResult_;}
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
    numberIterations_ = 1;
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

void NelderMead::setStartValues(std::vector<double> x0){x0_ = x0; setOptimized(false);}
void NelderMead::setInitSimplexMethod(const NelderMead::InitSimplexMethod& method){initSimplexMethod_=method;setOptimized(false);}
void NelderMead::setPerturbationParam(double epsilon){epsilon_=epsilon;setOptimized(false);}
void NelderMead::setReflectionParam(double alpha){alpha_=alpha;setOptimized(false);}
void NelderMead::setExpansionParam(double beta){beta_=beta;setOptimized(false);}
void NelderMead::setContractionParam(double gamma){gamma_=gamma;setOptimized(false);}
void NelderMead::setShrinkParam(double delta){delta_=delta;setOptimized(false);}

std::vector<double> NelderMead::getStartValues() const{return x0_;}
NelderMead::InitSimplexMethod NelderMead::getInitSimplexMethod()const{return initSimplexMethod_;}
double NelderMead::getPerturbationParam()const{return epsilon_;}
double NelderMead::getReflectionParam()const{return alpha_;}
double NelderMead::getExpansionParam()const{return beta_;}
double NelderMead::getContractionParam()const{return gamma_;}
double NelderMead::getShrinkParam()const{return delta_;}
double NelderMead::getFunctionResult(){optimize(); return getError() ? NAN : fResult_;}
int NelderMead::getNumberIterations(){optimize(); return numberIterations_;}
std::vector<double> NelderMead::getResult(){optimize(); return getError() ? std::vector<double>(n_, NAN):result_;}

NelderMead::NelderMead(const std::vector<double>& x0,const std::function<double(std::vector<double>)>& f): 
x0_(x0), n_(x0.size()), f_(f) 
{
    setPerturbationParam(0.05); 
    setReflectionParam(1.0); 
    setExpansionParam(2.0); 
    setContractionParam(0.5); 
    setShrinkParam(0.5); 
    setInitSimplexMethod(NelderMead::InitSimplexMethod::BASIC);
};

NelderMead::Vertex::Vertex(const std::vector<double>& points, double value): points_(points), value_(value){}; 
NelderMead::Simplex::Simplex(const std::vector<Vertex>& vertices): vertices_(vertices){}; 

void NelderMead::Simplex::sortVertices()
{
    std::sort(vertices_.begin(), vertices_.end(), 
    [](const NelderMead::Vertex& a, const NelderMead::Vertex& b) {
        return a.value_ < b.value_;  // Compare values directly
    });
}

void NelderMead::Simplex::setVertex(const Vertex& vertex_, int i) {vertices_[i] = vertex_; sortVertices();}
NelderMead::Vertex NelderMead::Simplex::getBest() const {return vertices_[0];}
NelderMead::Vertex NelderMead::Simplex::getWorst() const {return vertices_.back();}
NelderMead::Vertex NelderMead::Simplex::getSecondWorst() const {int n = vertices_.size(); return vertices_[n-2];}

double NelderMead::getSymAlpha() const {return epsilon_/(2*sqrt(n_));}
NelderMead::Vertex NelderMead::getVertex(std::vector<double> x_input) const {return NelderMead::Vertex(x_input, f_(x_input));}

NelderMead::Simplex NelderMead::getInitialSimplex() const
{
    std::vector<NelderMead::Vertex> simplex;
    double a = getSymAlpha();
    simplex.push_back(getVertex(x0_));
    for (int i = 0; i < n_; ++i) {
        std::vector<double> x = x0_; 
        switch (initSimplexMethod_)
        {
        case NelderMead::InitSimplexMethod::BASIC: x[i] += epsilon_; break;
        case NelderMead::InitSimplexMethod::SCALED: x[i] += epsilon_ * (1 + fabs(x0_[i])); break;
        case NelderMead::InitSimplexMethod::SYMMETRIC: 
            for (int j = 0; j < n_; ++j) {
                x[j] += (i == j) ? a : (-a / (n_ - 1));
            };
            break;
        default: break;
        }
        simplex.push_back(getVertex(x));
    };
    return NelderMead::Simplex(simplex);
}

NelderMead::Vertex NelderMead::getCentroid(const NelderMead::Simplex& simplex_) const
{
    std::vector<NelderMead::Vertex> simplexVector = simplex_.vertices_;
    std::vector<double> x_c(n_, 0); 
    for (int j = 0; j < n_; ++j){
        for (int i = 0; i < n_; ++i){
            x_c[i] +=  simplexVector[j].points_[i]/n_;
        }
    }
    return getVertex(x_c);
}

NelderMead::Vertex NelderMead::getReflection(const NelderMead::Vertex& centroid, const NelderMead::Vertex& worst) const
{
    std::vector<double> x(n_); 
    for (int j = 0; j < n_; ++j){
        double xw = worst.points_[j];
        double xc = centroid.points_[j]; 
        x[j] = xc + alpha_*(xc-xw);
    }
    return getVertex(x);
}

NelderMead::Vertex NelderMead::getExpansion(const NelderMead::Vertex& centroid, const NelderMead::Vertex& reflection) const
{
    std::vector<double> x(n_); 
    for (int j = 0; j < n_; ++j){
        double xr = reflection.points_[j];
        double xc = centroid.points_[j]; 
        x[j] = xc + beta_*(xr-xc);
    }
    return getVertex(x);
}

NelderMead::Vertex NelderMead::getContraction(const NelderMead::Vertex& centroid, const NelderMead::Vertex& worst) const
{
    std::vector<double> x(n_); 
    for (int j = 0; j < n_; ++j){
        double xw = worst.points_[j];
        double xc = centroid.points_[j]; 
        x[j] = xc + gamma_*(xw-xc);
    }
    return getVertex(x);
}

NelderMead::Simplex NelderMead::getShrink(const NelderMead::Simplex& simplex_) const
{
    std::vector<NelderMead::Vertex> simplexOut;
    std::vector<NelderMead::Vertex> simplexIn = simplex_.vertices_;
    simplexOut.push_back(simplexIn[0]); 
    for (int j = 1; j < n_; ++j){
        std::vector<double> x(n_); 
        for (int i = 0; i < n_; ++i){
            x[i] = simplexIn[0].points_[i] + delta_*(simplexIn[j].points_[i] - simplexIn[0].points_[i]);
        }
        simplexOut.push_back(getVertex(x)); 
    } 
    return NelderMead::Simplex(simplexOut);
}

bool NelderMead::checkConvergence(const NelderMead::Simplex& simplex_) const
{
    double maxDistance = 0.0;
    double maxValue = 0.0;
    for (int i = 1; i < n_; i++) {
        double distance = 0.0;
        for (int j = 0; j < n_; j++) {
            distance += std::pow(simplex_.vertices_[i].points_[j] - simplex_.vertices_[0].points_[j], 2);
        }
        maxDistance = std::max(maxDistance, std::sqrt(distance));
        maxValue = std::max(maxValue, std::abs(simplex_.vertices_[i].value_ - simplex_.vertices_[0].value_));
    }
    return (maxDistance < getToleranceThreshold() || maxValue < getToleranceThreshold());
}

void NelderMead::_optimize()
{
    NelderMead::Simplex simplex = getInitialSimplex(); 
    numberIterations_ = 1;
    for (int iter = 1; iter <= getMaximumIterations(); iter++) {
        if (checkConvergence(simplex)){break;}
        NelderMead::Vertex centroid = getCentroid(simplex); 
        NelderMead::Vertex worst = simplex.getWorst(); 
        NelderMead::Vertex reflection = getReflection(centroid,worst);
        NelderMead::Vertex best = simplex.getBest(); 
        NelderMead::Vertex second_worst = simplex.getSecondWorst(); 

        if (reflection.value_<best.value_){
            NelderMead::Vertex expansion = getExpansion(centroid, reflection); 
            if (expansion.value_<reflection.value_){
                simplex.setVertex(expansion, simplex.vertices_.size()-1);
            }else{
                simplex.setVertex(reflection, simplex.vertices_.size()-1);
            }
        }
        else if (reflection.value_>=best.value_ and reflection.value_<second_worst.value_){
            simplex.setVertex(reflection, simplex.vertices_.size()-1);
        }
        else {
            
            if (reflection.value_<worst.value_){
                NelderMead::Vertex contraction = getContraction(centroid, reflection);
                if (contraction.value_<reflection.value_){
                    simplex.setVertex(contraction, simplex.vertices_.size()-1);
                }else{
                    simplex = getShrink(simplex);
                }
            }else{
                NelderMead::Vertex contraction = getContraction(centroid, worst);
                if (contraction.value_<worst.value_){
                    simplex.setVertex(contraction, simplex.vertices_.size()-1);
                }else{
                    simplex = getShrink(simplex);
                }
            }
        }
        if (numberIterations_==getMaximumIterations()){break;}
        
        numberIterations_++;

    };
    result_ = simplex.vertices_[0].points_; 
    fResult_ = simplex.vertices_[0].value_; 
}