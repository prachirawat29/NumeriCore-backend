
#include "services/Integration.h"

#include <stdexcept>
#include <cmath>

namespace numericore {

// -----------------------------------------
// 1. Composite Trapezoidal Rule
// -----------------------------------------

double trapezoidalRule(
    const std::vector<double>& yValues,
    double stepSize
) {

    if (yValues.size() < 2) {
        throw std::invalid_argument(
            "Trapezoidal Rule requires at least two measurements."
        );
    }

    if (!std::isfinite(stepSize) || stepSize <= 0.0) {
        throw std::invalid_argument(
            "Step size must be positive and finite."
        );
    }

    double sum = yValues.front() + yValues.back();

    for (std::size_t i = 1; i < yValues.size() - 1; i++) {
        sum += 2.0 * yValues[i];
    }

    return (stepSize / 2.0) * sum;
}


// -----------------------------------------
// 2. Composite Midpoint Rule
// -----------------------------------------

double midpointRule(
    const std::vector<double>& midpointValues,
    double stepSize
) {

    if (midpointValues.empty()) {
        throw std::invalid_argument(
            "Midpoint Rule requires midpoint measurements."
        );
    }

    if (!std::isfinite(stepSize) || stepSize <= 0.0) {
        throw std::invalid_argument(
            "Step size must be positive and finite."
        );
    }

    double sum = 0.0;

    for (std::size_t i = 0; i < midpointValues.size(); i++) {
        sum += midpointValues[i];
    }

    return stepSize * sum;
}


// -----------------------------------------
// 3. Composite Simpson's 1/3 Rule
// -----------------------------------------

double simpsonsRule(
    const std::vector<double>& yValues,
    double stepSize
) {

    if (yValues.size() < 3) {
        throw std::invalid_argument(
            "Simpson's Rule requires at least three measurements."
        );
    }

    if (!std::isfinite(stepSize) || stepSize <= 0.0) {
        throw std::invalid_argument(
            "Step size must be positive and finite."
        );
    }

    std::size_t intervals = yValues.size() - 1;

    if (intervals % 2 != 0) {
        throw std::invalid_argument(
            "Simpson's 1/3 Rule requires an even number of intervals."
        );
    }

    double sum = yValues.front() + yValues.back();

    for (std::size_t i = 1; i < intervals; i++) {

        if (i % 2 == 1) {
            sum += 4.0 * yValues[i];
        }
        else {
            sum += 2.0 * yValues[i];
        }

    }

    return (stepSize / 3.0) * sum;
}

}
