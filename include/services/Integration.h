
#ifndef NUMERICORE_INTEGRATION_H
#define NUMERICORE_INTEGRATION_H

#include <vector>

namespace numericore {

// Composite Trapezoidal Rule
double trapezoidalRule(
    const std::vector<double>& yValues,
    double stepSize
);

// Composite Midpoint Rule
double midpointRule(
    const std::vector<double>& midpointValues,
    double stepSize
);

// Composite Simpson's 1/3 Rule
double simpsonsRule(
    const std::vector<double>& yValues,
    double stepSize
);

}

#endif
