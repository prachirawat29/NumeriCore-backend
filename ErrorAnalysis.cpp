
#include "services/ErrorAnalysis.h"

#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <vector>

namespace numericore {

// -----------------------------------------
// 1. Calculate Absolute Error
// -----------------------------------------

double calculateAbsoluteError(
    double actualValue,
    double estimatedValue
) {

    if (!std::isfinite(actualValue) ||
        !std::isfinite(estimatedValue)) {

        throw std::invalid_argument(
            "Values must be finite."
        );
    }

    return std::fabs(actualValue - estimatedValue);
}


// -----------------------------------------
// 2. Calculate Percentage Error
// -----------------------------------------

bool calculatePercentageError(
    double actualValue,
    double estimatedValue,
    double& percentageError
) {

    if (!std::isfinite(actualValue) ||
        !std::isfinite(estimatedValue)) {

        throw std::invalid_argument(
            "Values must be finite."
        );
    }

    if (actualValue == 0.0) {
        percentageError = 0.0;
        return false;
    }

    double absoluteError = calculateAbsoluteError(
        actualValue,
        estimatedValue
    );

    percentageError =
        (absoluteError / std::fabs(actualValue)) * 100.0;

    if (!std::isfinite(percentageError)) {
        throw std::overflow_error(
            "Percentage error calculation overflowed."
        );
    }

    return true;
}


// -----------------------------------------
// 3. Analyze One Method
// -----------------------------------------

void analyzeMethodResult(
    MethodResult& result,
    double actualValue
) {

    result.absoluteError = calculateAbsoluteError(
        actualValue,
        result.estimatedValue
    );

    result.hasAbsoluteError = true;

    result.hasPercentageError = calculatePercentageError(
        actualValue,
        result.estimatedValue,
        result.percentageError
    );

}


// -----------------------------------------
// 4. Compare All Three Methods
// -----------------------------------------

void compareAccuracy(
    CalculationResult& result
) {

    if (!result.hasActualValue) {

        result.comparisonAvailable = false;
        result.mostAccurateMethods.clear();

        return;
    }

    // Calculate errors for all three methods

    analyzeMethodResult(
        result.trapezoidal,
        result.actualValue
    );

    analyzeMethodResult(
        result.midpoint,
        result.actualValue
    );

    analyzeMethodResult(
        result.simpson,
        result.actualValue
    );

    // Store method names and their absolute errors

    std::vector<MethodResult*> methods;

    methods.push_back(&result.trapezoidal);
    methods.push_back(&result.midpoint);
    methods.push_back(&result.simpson);

    double minimumError = result.trapezoidal.absoluteError;

    for (std::size_t i = 1; i < methods.size(); i++) {

        minimumError = std::min(
            minimumError,
            methods[i]->absoluteError
        );

    }

    // Identify all methods with minimum error

    result.mostAccurateMethods.clear();

    for (std::size_t i = 0; i < methods.size(); i++) {

        double error = methods[i]->absoluteError;

        double tolerance = 1e-12 *
            std::max(1.0, std::fabs(minimumError));

        if (std::fabs(error - minimumError) <= tolerance) {

            result.mostAccurateMethods.push_back(
                methods[i]->methodName
            );

        }

    }

    result.comparisonAvailable = true;

}

}
