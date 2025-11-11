#pragma once 
#include <iostream>
#include <exception>

class MathLibraryError: public std::exception 
{
    public:
        const char* what() const noexcept override;
        explicit MathLibraryError(){};
        virtual ~MathLibraryError() = default;
    protected: 
        virtual std::string getErrorMessage() const = 0; 
    private:
        mutable std::string cachedMessage_;  // must be mutable to modify in const what()
};

namespace MathErrorRegistry
{
    namespace CurveInterpolation 
    {
        class WrongVectorSizeError final: public MathLibraryError
        {
            protected: 
                std::string getErrorMessage() const override; 
        };

        class OutOfRangeCurveInterpolationError final: public MathLibraryError
        {
            protected: 
                std::string getErrorMessage() const override; 
        };
    };

    namespace Optim 
    {
        class NewtonRaphsonDerivativeZeroError final: public MathLibraryError
        {
            protected: 
                std::string getErrorMessage() const override; 
        };
    };

    namespace Probability
    {
        class InvalidUniformBoundError final: public MathLibraryError
        {
            protected: 
                std::string getErrorMessage() const override; 
        };

        class InvalidSigmaGaussianError final: public MathLibraryError
        {
            protected: 
                std::string getErrorMessage() const override; 
        };
    }

    namespace Loss
    {
        class MismatchVectorSizeError final: public MathLibraryError
        {
            protected: 
                std::string getErrorMessage() const override; 
        };

        class EmptyVectorError final: public MathLibraryError
        {
            protected: 
                std::string getErrorMessage() const override; 
        };

    }

    namespace Tools 
    {
        class InvalidMatrixSize final: public MathLibraryError
        {
            protected: 
                std::string getErrorMessage() const override; 
        };
    }

    namespace Regression 
    {
        class MismatchTargetFeaturesSizeError final : public MathLibraryError
        {
            protected: 
                std::string getErrorMessage() const override; 
        };
    }
};