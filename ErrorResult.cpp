
#include "models/ErrorResult.h"

namespace numericore {

ErrorResult::ErrorResult() {

    success = true;
    code = "";
    message = "";

}

ErrorResult::ErrorResult(
    const std::string& errorCode,
    const std::string& errorMessage
) {

    success = false;
    code = errorCode;
    message = errorMessage;

}

}
