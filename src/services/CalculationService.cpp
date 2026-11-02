
#include "services/CalculationService.h"

#include "services/Integration.h"
#include "services/ErrorAnalysis.h"
#include "utils/Validation.h"

#include <stdexcept>
#include <cmath>
#include <limits>

namespace numericore {

// -----------------------------------------
// Calculate Step Size
// -----------------------------------------

double CalculationService::calculateStepSize(
    double start,
    double end,
    std::size_t intervals
) {

    if (intervals == 0) {
        throw std::invalid_argument(
            "Number of intervals cannot be zero."
        );
    }

    double stepSize =
        (end - start) / static_cast<double>(intervals);

    if (!std::isfinite(stepSize) || stepSize <= 0.0) {
        throw std::invalid_argument(
            "Invalid step size."
        );
    }

    return stepSize;
}


// -----------------------------------------
// Main Calculation Function
// -----------------------------------------

bool CalculationService::calculate(
    const Measurement& measurement,
    CalculationResult& result,
    ErrorResult& error
) {

    // Reset previous error information
    error = ErrorResult();

    // Validate incoming measurements
    if (!validateInput(measurement, error)) {
        return false;
    }
    result = CalculationResult();

    try {

        std::size_t intervals =
            measurement.xValues.size() - 1;

        // Calculate interval width
        result.stepSize = calculateStepSize(
            measurement.start,
            measurement.end,
            intervals
        );

        // Store reference value if available
        result.hasActualValue =
            measurement.hasActualValue;

        if (measurement.hasActualValue) {
            result.actualValue = measurement.actualValue;
        }

        // ---------------------------------
        // Trapezoidal Rule
        // ---------------------------------

        result.trapezoidal.methodName = "trapezoidal";

        result.trapezoidal.estimatedValue =
            trapezoidalRule(
                measurement.yValues,
                result.stepSize
            );

        // ---------------------------------
        // Midpoint Rule
        // ---------------------------------

        result.midpoint.methodName = "midpoint";

        result.midpoint.estimatedValue =
            midpointRule(
                measurement.midpointValues,
                result.stepSize
            );

        // ---------------------------------
        // Simpson's 1/3 Rule
        // ---------------------------------

        result.simpson.methodName = "simpson";

        result.simpson.estimatedValue =
            simpsonsRule(
                measurement.yValues,
                result.stepSize
            );

        // Check calculated values
        if (!std::isfinite(result.trapezoidal.estimatedValue) ||
            !std::isfinite(result.midpoint.estimatedValue) ||
            !std::isfinite(result.simpson.estimatedValue)) {

            throw std::overflow_error(
                "Calculation produced a non-finite result."
            );

        }

        // ---------------------------------
        // Accuracy Analysis
        // ---------------------------------

        if (measurement.hasActualValue) {

            compareAccuracy(result);

        }
        else {

            result.comparisonAvailable = false;
            result.mostAccurateMethods.clear();

        }

        return true;

    }
    catch (const std::exception& exception) {

        error = ErrorResult(
            "CALCULATION_ERROR",
            exception.what()
        );

        return false;

    }

}

}
