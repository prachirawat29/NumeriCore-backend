// Unit tests for input validation.
// Assert-based, no framework: exits non-zero on first failed assertion.

#include "utils/Validation.h"
#include "models/Measurement.h"
#include "models/ErrorResult.h"

#include <cmath>
#include <cassert>
#include <limits>
#include <iostream>

using namespace numericore;

static Measurement validSample() {
    Measurement m;
    m.operation = "calculate_area";
    m.start = 0.0;
    m.end = 4.0;
    m.xValues = {0, 1, 2, 3, 4};
    m.yValues = {0, 1, 4, 9, 16};
    m.midpointValues = {0.25, 2.25, 6.25, 12.25};
    m.hasActualValue = false;
    m.actualValue = 0.0;
    return m;
}

int main() {

    // isValidNumber
    assert(isValidNumber(3.14));
    assert(!isValidNumber(std::numeric_limits<double>::infinity()));
    assert(!isValidNumber(std::nan("")));

    ErrorResult err;

    // A well-formed sample passes.
    assert(validateInput(validSample(), err));
    assert(err.success);

    // Unknown operation.
    { Measurement m = validSample(); m.operation = "nope";
      assert(!validateInput(m, err)); assert(err.code == "INVALID_OPERATION"); }

    // end must be greater than start.
    { Measurement m = validSample(); m.end = m.start;
      assert(!validateInput(m, err)); assert(err.code == "INVALID_RANGE"); }

    // Odd number of intervals (4 points -> 3 intervals).
    { Measurement m = validSample();
      m.xValues = {0, 1, 2, 3};
      m.yValues = {0, 1, 4, 9};
      m.midpointValues = {0.5, 2.5, 6.5};
      assert(!validateInput(m, err)); assert(err.code == "INVALID_INTERVAL_COUNT"); }

    // One midpoint per interval is required.
    { Measurement m = validSample(); m.midpointValues = {0.25, 2.25};
      assert(!validateInput(m, err)); assert(err.code == "INVALID_MIDPOINT_COUNT"); }

    // x must be equally spaced (endpoints still match start/end).
    { Measurement m = validSample(); m.xValues = {0, 0.5, 2, 3, 4};
      assert(!validateInput(m, err)); assert(err.code == "UNEQUAL_SPACING"); }

    std::cout << "test_validation: all assertions passed\n";
    return 0;
}
