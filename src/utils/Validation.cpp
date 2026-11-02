
#include "utils/Validation.h"
#include "utils/Constants.h"

#include <cmath>
#include <algorithm>
#include <string>

namespace numericore {

// -----------------------------------------
// 1. Check Finite Numbers
// -----------------------------------------

bool isValidNumber(double value) {

    return std::isfinite(value);

}


// -----------------------------------------
// 2. Validate Equal Spacing
// -----------------------------------------

bool validateEqualSpacing(
    const Measurement& measurement
) {

    const std::vector<double>& x =
        measurement.xValues;

    if (x.size() < 2) {
        return false;
    }

    double expectedStep =
        (measurement.end - measurement.start) /
        static_cast<double>(x.size() - 1);

    double tolerance = SPACING_TOLERANCE *
        std::max(1.0, std::fabs(expectedStep));

    for (std::size_t i = 1; i < x.size(); i++) {

        double actualStep = x[i] - x[i - 1];

        if (std::fabs(actualStep - expectedStep) > tolerance) {
            return false;
        }

    }

    return true;

}


// -----------------------------------------
// 3. Complete Input Validation
// -----------------------------------------

bool validateInput(
    const Measurement& measurement,
    ErrorResult& error
) {

    error = ErrorResult();

    // Validate operation

    if (measurement.operation != "calculate_area" &&
        measurement.operation != "calculate_volume") {

        error = ErrorResult(
            "INVALID_OPERATION",
            "Operation must be calculate_area or calculate_volume."
        );

        return false;
    }


    // Validate start and end positions

    if (!isValidNumber(measurement.start) ||
        !isValidNumber(measurement.end)) {

        error = ErrorResult(
            "INVALID_RANGE",
            "Starting and ending positions must be finite numbers."
        );

        return false;
    }

    if (measurement.end <= measurement.start) {

        error = ErrorResult(
            "INVALID_RANGE",
            "Ending position must be greater than starting position."
        );

        return false;
    }


    // Validate measurement count

    if (measurement.xValues.size() < 3 ||
        measurement.xValues.size() > MAX_MEASUREMENTS) {

        error = ErrorResult(
            "INVALID_MEASUREMENT_COUNT",
            "At least 3 and at most MAX_MEASUREMENTS are required."
        );

        return false;
    }


    // Validate matching x and y arrays

    if (measurement.xValues.size() !=
        measurement.yValues.size()) {

        error = ErrorResult(
            "MISMATCHED_MEASUREMENTS",
            "X and Y measurement arrays must have equal lengths."
        );

        return false;
    }


    // Calculate number of intervals

    std::size_t intervals =
        measurement.xValues.size() - 1;


    // Simpson's Rule requires even intervals

    if (intervals % 2 != 0) {

        error = ErrorResult(
            "INVALID_INTERVAL_COUNT",
            "Simpson's 1/3 Rule requires an even number of intervals."
        );

        return false;
    }


    // Validate midpoint measurement count

    if (measurement.midpointValues.size() != intervals) {

        error = ErrorResult(
            "INVALID_MIDPOINT_COUNT",
            "Exactly one midpoint measurement is required per interval."
        );

        return false;
    }


    // Validate x coordinates

    for (std::size_t i = 0;
         i < measurement.xValues.size();
         i++) {

        if (!isValidNumber(measurement.xValues[i])) {

            error = ErrorResult(
                "INVALID_X_VALUE",
                "X coordinates must contain finite numbers."
            );

            return false;
        }

    }


    // Validate y measurements

    for (std::size_t i = 0;
         i < measurement.yValues.size();
         i++) {

        if (!isValidNumber(measurement.yValues[i])) {

            error = ErrorResult(
                "INVALID_Y_VALUE",
                "Y measurements must contain finite numbers."
            );

            return false;
        }

    }


    // Validate midpoint measurements

    for (std::size_t i = 0;
         i < measurement.midpointValues.size();
         i++) {

        if (!isValidNumber(measurement.midpointValues[i])) {

            error = ErrorResult(
                "INVALID_MIDPOINT_VALUE",
                "Midpoint measurements must contain finite numbers."
            );

            return false;
        }

    }


    // Validate first and last positions

    double tolerance = SPACING_TOLERANCE *
        std::max(1.0, std::fabs(measurement.end -
                               measurement.start));

    if (std::fabs(measurement.xValues.front() -
                  measurement.start) > tolerance ||
        std::fabs(measurement.xValues.back() -
                  measurement.end) > tolerance) {

        error = ErrorResult(
            "RANGE_MISMATCH",
            "X coordinates must begin at start and end at end."
        );

        return false;
    }


    // Validate equal spacing

    if (!validateEqualSpacing(measurement)) {

        error = ErrorResult(
            "UNEQUAL_SPACING",
            "X coordinates must be equally spaced."
        );

        return false;
    }


    // Validate optional actual value

    if (measurement.hasActualValue &&
        !isValidNumber(measurement.actualValue)) {

        error = ErrorResult(
            "INVALID_ACTUAL_VALUE",
            "Actual value must be a finite number."
        );

        return false;
    }


    return true;

}

}
