#pragma once
#include <iostream>
#include <cmath>
#include <cfloat>

namespace GaussianTool
{

   // ALGORITHM AS241  APPL. STATIST. (1988) VOL. 37, NO. 3
   //
   // Produces the normal deviate Z corresponding to a given lower
   // tail area of u; Z is accurate to about 1 part in 10**16.
   // see http://lib.stat.cmu.edu/apstat/241
    double acklamStandardGaussianInverseCdf(double p); 

    double standardGaussianPdf(double z);

    constexpr double ONE_OVER_SQRT_TWO_PI = 0.3989422804014326779399460599343818684758586311649;

    // Code source from https://github.com/vollib/lets_be_rational/
    double d_int(const double x);
    // Code source from https://github.com/vollib/lets_be_rational/
    double erfCody(double x);
    // Code source from https://github.com/vollib/lets_be_rational/
    double erfcCody(double x);
    // Code source from https://github.com/vollib/lets_be_rational/
    double erfcxCody(double x);
    // Code source from https://github.com/vollib/lets_be_rational/
    double standardGaussianCdf(double z); 
}

