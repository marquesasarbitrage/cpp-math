#include "../include/cpp-math/errors.hpp"

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
        std::string WrongVectorSizeError::getErrorMessage() const {return "The data must have a size greater than 3 to construct interpolation object";}
        std::string OutOfRangeCurveInterpolationError::getErrorMessage() const {return "Cannot interpolate a value out of interpolatin range.";}
    };
}