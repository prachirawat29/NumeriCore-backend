
#ifndef NUMERICORE_ERROR_RESULT_H
#define NUMERICORE_ERROR_RESULT_H

#include <string>

namespace numericore {

struct ErrorResult {

    bool success;

    std::string code;
    std::string message;

    ErrorResult();

    ErrorResult(
        const std::string& errorCode,
        const std::string& errorMessage
    );

};

}

#endif
