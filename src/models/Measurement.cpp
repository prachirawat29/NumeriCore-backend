
#include "models/Measurement.h"

namespace numericore {

Measurement::Measurement() {

    operation = "calculate_area";

    start = 0.0;
    end = 0.0;

    hasActualValue = false;
    actualValue = 0.0;

}

}
