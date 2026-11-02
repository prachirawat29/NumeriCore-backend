
#ifndef NUMERICORE_CALCULATION_SERVICE_H
#define NUMERICORE_CALCULATION_SERVICE_H

#include "models/Measurement.h"
#include "models/CalculationResult.h"
#include "models/ErrorResult.h"

namespace numericore {

class CalculationService {

public:

    // Calculate area or volume using numerical integration
    bool calculate(
        const Measurement& measurement,
        CalculationResult& result,
        ErrorResult& error
    );

private:

    // Calculate interval width
    double calculateStepSize(
        double start,
        double end,
        std::size_t intervals
    );

};

}

#endif
