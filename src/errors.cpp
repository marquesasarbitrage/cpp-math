#include "../include/core-math/errors.hpp"

const char* MathLibraryError::what() const noexcept
{
    if (cachedMessage_.empty()) {
            cachedMessage_ = getErrorMessage();  
        }
    return cachedMessage_.c_str();
}

namespace MathErrorRegistry
{
    namespace CurveInterpolation 
    {
        std::string WrongVectorSizeError::getErrorMessage() const {return "The data must have a size greater than 3 to construct interpolation object.";}
        std::string OutOfRangeCurveInterpolationError::getErrorMessage() const {return "Cannot interpolate a value out of interpolatin range.";}
    };

    namespace Optim
    {
        std::string NewtonRaphsonDerivativeZeroError::getErrorMessage() const {return "The derivative value in the Newton-Raphson optimizer is getting close to zero which makes the optimizer unstable.";}
    };

    namespace Probability
    {
        std::string InvalidUniformBoundError::getErrorMessage() const {return "The bounds given for the Uniform distribution are invalid (b>a is the condition).";}
        std::string InvalidSigmaGaussianError::getErrorMessage() const {return "Sigma parameter must be a positive value.";}
    }

    namespace Loss
    {
        std::string MismatchVectorSizeError::getErrorMessage() const {return "Both vector must have equal size to apply estimator loss calculation.";}
        std::string EmptyVectorError::getErrorMessage() const {return "Vector cannot be emppty for estimator loss calculation.";}
    }

    namespace Tools
    {
        std::string InvalidMatrixSize::getErrorMessage() const {return "The data passed is either empty or does not have valid dimensions for matrix construction.";}
    }

    namespace Regression
    {
        std::string MismatchTargetFeaturesSizeError::getErrorMessage() const {return "The number of rows in the matrix of features must match the number of targets.";}
    }
}