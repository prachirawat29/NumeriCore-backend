
#include "utils/JsonHandler.h"
#include "utils/Constants.h"

#include <nlohmann/json.hpp>

#include <sstream>
#include <cmath>
#include <stdexcept>

using json = nlohmann::json;

namespace numericore {

// -----------------------------------------
// Helper: Validate JSON numeric values
// -----------------------------------------

static bool isJsonNumber(const json& value) {

    return value.is_number() &&
           std::isfinite(value.get<double>());

}


// -----------------------------------------
// Helper: Read Numeric Array
// -----------------------------------------

static bool readNumberArray(
    const json& request,
    const std::string& key,
    std::vector<double>& output,
    ErrorResult& error
) {

    if (!request.contains(key) ||
        !request[key].is_array()) {

        error = ErrorResult(
            "INVALID_JSON_FIELD",
            key + " must be a JSON array."
        );

        return false;
    }

    const json& array = request[key];

    if (array.size() > MAX_MEASUREMENTS) {

        error = ErrorResult(
            "INPUT_TOO_LARGE",
            key + " contains too many measurements."
        );

        return false;
    }

    output.clear();

    for (std::size_t i = 0; i < array.size(); i++) {

        if (!isJsonNumber(array[i])) {

            error = ErrorResult(
                "INVALID_NUMERIC_VALUE",
                key + " must contain only finite numbers."
            );

            return false;
        }

        output.push_back(array[i].get<double>());

    }

    return true;

}


// -----------------------------------------
// 1. Parse JSON Request
// -----------------------------------------

bool parseJsonRequest(
    const std::string& jsonInput,
    Measurement& measurement,
    ErrorResult& error
) {

    error = ErrorResult();

    if (jsonInput.empty()) {

        error = ErrorResult(
            "EMPTY_REQUEST",
            "JSON request cannot be empty."
        );

        return false;
    }

    if (jsonInput.size() > MAX_JSON_SIZE) {

        error = ErrorResult(
            "INPUT_TOO_LARGE",
            "JSON request exceeds the maximum allowed size."
        );

        return false;
    }

    try {

        json request = json::parse(jsonInput);

        if (!request.is_object()) {

            error = ErrorResult(
                "INVALID_JSON_FORMAT",
                "JSON request must be an object."
            );

            return false;
        }

        // Operation

        if (!request.contains("operation") ||
            !request["operation"].is_string()) {

            error = ErrorResult(
                "MISSING_OPERATION",
                "A valid operation field is required."
            );

            return false;
        }

        measurement.operation =
            request["operation"].get<std::string>();


        // Start and end

        if (!request.contains("start") ||
            !isJsonNumber(request["start"])) {

            error = ErrorResult(
                "INVALID_START",
                "Start must be a finite number."
            );

            return false;
        }

        if (!request.contains("end") ||
            !isJsonNumber(request["end"])) {

            error = ErrorResult(
                "INVALID_END",
                "End must be a finite number."
            );

            return false;
        }

        measurement.start =
            request["start"].get<double>();

        measurement.end =
            request["end"].get<double>();


        // Measurement arrays

        if (!readNumberArray(
                request,
                "x_values",
                measurement.xValues,
                error)) {

            return false;
        }

        if (!readNumberArray(
                request,
                "y_values",
                measurement.yValues,
                error)) {

            return false;
        }

        if (!readNumberArray(
                request,
                "midpoint_values",
                measurement.midpointValues,
                error)) {

            return false;
        }


        // Optional actual value

        measurement.hasActualValue = false;
        measurement.actualValue = 0.0;

        if (request.contains("actual_area") &&
            request.contains("actual_volume")) {

            error = ErrorResult(
                "AMBIGUOUS_ACTUAL_VALUE",
                "Provide only one actual reference value."
            );

            return false;
        }

        std::string actualKey =
            measurement.operation == "calculate_volume"
            ? "actual_volume"
            : "actual_area";

        if (request.contains(actualKey)) {

            if (!isJsonNumber(request[actualKey])) {

                error = ErrorResult(
                    "INVALID_ACTUAL_VALUE",
                    "Actual reference value must be finite."
                );

                return false;
            }

            measurement.hasActualValue = true;

            measurement.actualValue =
                request[actualKey].get<double>();

        }

        return true;

    }
    catch (const std::exception& exception) {

        error = ErrorResult(
            "INVALID_JSON",
            exception.what()
        );

        return false;

    }

}


// -----------------------------------------
// 2. Generate Error JSON
// -----------------------------------------

std::string createErrorResponse(
    const ErrorResult& error
) {

    json response;

    response["success"] = false;
    response["data"] = nullptr;

    response["error"] = {
        {"code", error.code},
        {"message", error.message}
    };

    return response.dump(4);

}


// -----------------------------------------
// 3. Generate Successful JSON
// -----------------------------------------

std::string createJsonResponse(
    const CalculationResult& result,
    const ErrorResult& error
) {

    if (!error.success) {
        return createErrorResponse(error);
    }

    json data;

    data["step_size"] = result.stepSize;

    data["actual_value"] =
        result.hasActualValue
        ? json(result.actualValue)
        : json(nullptr);

    data["comparison_available"] =
        result.comparisonAvailable;

    data["results"] = json::object();

    auto addMethod = [&](const std::string& key,
                         const MethodResult& method) {

        json item;

        item["estimated_value"] = method.estimatedValue;

        item["absolute_error"] =
            method.hasAbsoluteError
            ? json(method.absoluteError)
            : json(nullptr);

        item["percentage_error"] =
            method.hasPercentageError
            ? json(method.percentageError)
            : json(nullptr);

        data["results"][key] = item;

    };

    addMethod("trapezoidal", result.trapezoidal);
    addMethod("midpoint", result.midpoint);
    addMethod("simpson", result.simpson);

    data["most_accurate_methods"] =
        result.mostAccurateMethods;

    json response;

    response["success"] = true;
    response["data"] = data;
    response["error"] = nullptr;

    return response.dump(4);

}

}
