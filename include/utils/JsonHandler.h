
#ifndef NUMERICORE_JSON_HANDLER_H
#define NUMERICORE_JSON_HANDLER_H

#include <string>

#include "models/Measurement.h"
#include "models/CalculationResult.h"
#include "models/ErrorResult.h"

namespace numericore {

// Convert JSON input into a Measurement object
bool parseJsonRequest(
    const std::string& jsonInput,
    Measurement& measurement,
    ErrorResult& error
);

// Convert calculation results into JSON
std::string createJsonResponse(
    const CalculationResult& result,
    const ErrorResult& error
);

// Generate an error response
std::string createErrorResponse(
    const ErrorResult& error
);

}

#endif
