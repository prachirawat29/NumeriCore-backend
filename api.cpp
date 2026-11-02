
#include "utils/JsonHandler.h"
#include "services/CalculationService.h"

#include <string>
#include <exception>

namespace numericore {

// -----------------------------------------
// Process One API Request
// -----------------------------------------

std::string processRequest(
    const std::string& jsonInput
) {

    Measurement measurement;
    CalculationResult result;
    ErrorResult error;

    // Step 1: Parse JSON request

    if (!parseJsonRequest(
            jsonInput,
            measurement,
            error)) {

        return createErrorResponse(error);

    }

    // Step 2: Perform calculations

    CalculationService service;

    bool success = service.calculate(
        measurement,
        result,
        error
    );

    // Step 3: Return response

    if (!success) {
        return createErrorResponse(error);
    }

    return createJsonResponse(result, error);

}

}
