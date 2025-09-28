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
            public:
                explicit WrongVectorSizeError(){};
                virtual ~WrongVectorSizeError() = default;
            protected: 
                std::string getErrorMessage() const override; 
        };

        class OutOfRangeCurveInterpolationError final: public MathLibraryError
        {
            public:
                explicit OutOfRangeCurveInterpolationError(){};
                virtual ~OutOfRangeCurveInterpolationError() = default;
            protected: 
                std::string getErrorMessage() const override; 
        };
    };
}