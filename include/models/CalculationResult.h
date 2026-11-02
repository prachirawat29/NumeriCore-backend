
#ifndef NUMERICORE_CALCULATION_RESULT_H
#define NUMERICORE_CALCULATION_RESULT_H

#include <string>
#include <vector>

namespace numericore {

struct MethodResult {

    std::string methodName;

    double estimatedValue;

    bool hasAbsoluteError;
    double absoluteError;

    bool hasPercentageError;
    double percentageError;

    MethodResult();

};

struct CalculationResult {

    double stepSize;

    MethodResult trapezoidal;
    MethodResult midpoint;
    MethodResult simpson;

    bool hasActualValue;
    double actualValue;

    bool comparisonAvailable;

    std::vector<std::string> mostAccurateMethods;

    CalculationResult();

};

}

#endif
