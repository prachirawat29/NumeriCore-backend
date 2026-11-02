
#ifndef NUMERICORE_MEASUREMENT_H
#define NUMERICORE_MEASUREMENT_H

#include <vector>
#include <string>

namespace numericore {

struct Measurement {

    std::string operation;

    double start;
    double end;

    std::vector<double> xValues;
    std::vector<double> yValues;
    std::vector<double> midpointValues;

    bool hasActualValue;
    double actualValue;

    Measurement();

};

}

#endif
