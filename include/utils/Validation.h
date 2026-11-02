
#ifndef NUMERICORE_VALIDATION_H
#define NUMERICORE_VALIDATION_H

#include "models/Measurement.h"
#include "models/ErrorResult.h"

namespace numericore {

// Validate complete measurement input
bool validateInput(
    const Measurement& measurement,
    ErrorResult& error
);

// Check whether a number is finite
bool isValidNumber(double value);

// Check whether x coordinates are equally spaced
bool validateEqualSpacing(
    const Measurement& measurement
);

}

#endif
