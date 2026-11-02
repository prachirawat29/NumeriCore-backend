
#ifndef NUMERICORE_ERROR_ANALYSIS_H
#define NUMERICORE_ERROR_ANALYSIS_H

#include "models/CalculationResult.h"

namespace numericore {

// Calculate absolute error
double calculateAbsoluteError(
    double actualValue,
    double estimatedValue
);

// Calculate percentage error
// Returns false if actualValue is zero.
bool calculatePercentageError(
    double actualValue,
    double estimatedValue,
    double& percentageError
);

// Analyze one numerical method
void analyzeMethodResult(
    MethodResult& result,
    double actualValue
);

// Compare accuracy of all three methods
void compareAccuracy(
    CalculationResult& result
);

}

#endif
