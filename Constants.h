
#ifndef NUMERICORE_CONSTANTS_H
#define NUMERICORE_CONSTANTS_H

#include <cstddef>

namespace numericore {

// Maximum number of endpoint measurements
static const std::size_t MAX_MEASUREMENTS = 100000;

// Tolerance for checking equal spacing
static const double SPACING_TOLERANCE = 1e-9;

// Tolerance for comparing numerical errors
static const double ERROR_TOLERANCE = 1e-12;

// Maximum supported input line size (in bytes)
static const std::size_t MAX_JSON_SIZE = 1024 * 1024;

// Minimum number of endpoint measurements
static const std::size_t MIN_MEASUREMENTS = 3;

}

#endif
