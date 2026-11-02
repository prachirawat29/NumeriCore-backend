
#include "models/CalculationResult.h"

namespace numericore {

MethodResult::MethodResult() {

    methodName = "";

    estimatedValue = 0.0;

    hasAbsoluteError = false;
    absoluteError = 0.0;

    hasPercentageError = false;
    percentageError = 0.0;

}

CalculationResult::CalculationResult() {

    stepSize = 0.0;

    hasActualValue = false;
    actualValue = 0.0;

    comparisonAvailable = false;

}

}
