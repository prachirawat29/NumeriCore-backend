
#include <iostream>
#include <string>

#include "utils/JsonHandler.h"
#include "utils/Constants.h"

namespace numericore {
    std::string processRequest(const std::string& jsonInput);
}

int main() {

    std::string jsonInput;
    char character;

    // Read the complete JSON request
    // Limit input size to prevent excessive memory usage

    while (std::cin.get(character)) {

        if (jsonInput.size() >= numericore::MAX_JSON_SIZE) {

            numericore::ErrorResult error(
                "INPUT_TOO_LARGE",
                "JSON request exceeds the maximum allowed size."
            );

            std::cout
                << numericore::createErrorResponse(error)
                << std::endl;

            return 1;
        }

        jsonInput += character;

    }

    // Handle empty input

    if (jsonInput.empty()) {

        numericore::ErrorResult error(
            "EMPTY_REQUEST",
            "No JSON request was provided."
        );

        std::cout
            << numericore::createErrorResponse(error)
            << std::endl;

        return 1;
    }

    try {

        // Process request through API

        std::string response =
            numericore::processRequest(jsonInput);

        // Print JSON response

        std::cout << response << std::endl;

        return 0;

    }
    catch (const std::exception& exception) {

        // Diagnostic information goes to stderr

        std::cerr
            << "NumeriCore error: "
            << exception.what()
            << std::endl;

        numericore::ErrorResult error(
            "INTERNAL_ERROR",
            "An unexpected backend error occurred."
        );

        std::cout
            << numericore::createErrorResponse(error)
            << std::endl;

        return 1;

    }

}
