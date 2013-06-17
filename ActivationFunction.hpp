#ifndef ACTIVATIONFUNCTION_HPP
#define ACTIVATIONFUNCTION_HPP

#include <cmath>

struct SigmoidFunction {
    double operator() ( const double t ) const;
};

struct LogisticFunction : SigmoidFunction {
    inline double operator() ( const double t ) const {
        return ( 1.0/( 1.0 + exp(-t) ) );
    }
};

struct TanhFunction : SigmoidFunction {
    inline double operator() ( const double t ) const {
        return std::tanh( t );
    }
};


struct SigmoidFunctionDerivative {
    double operator() ( const double t ) const;
};

struct LogisticFunctionDerivative : SigmoidFunctionDerivative {
    double operator() ( const double t ) const {
        return t * ( 1.0 - t );
    }
};

struct TanhFunctionDerivative {
    inline double operator() ( const double t ) const {
        return ( 1.0 - t );
    }
};

#endif // ACTIVATIONFUNCTION_HPP
