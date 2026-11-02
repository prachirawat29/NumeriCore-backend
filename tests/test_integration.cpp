// Unit tests for the numerical integration rules.
// Assert-based, no framework: exits non-zero on first failed assertion.

#include "services/Integration.h"

#include <vector>
#include <cmath>
#include <cassert>
#include <stdexcept>
#include <iostream>

using namespace numericore;

static bool nearlyEqual(double a, double b, double eps = 1e-9) {
    return std::fabs(a - b) <= eps * std::max(1.0, std::fabs(b));
}

int main() {

    // f(x) = x^2 sampled on [0, 4] with h = 1.
    const std::vector<double> y    = {0, 1, 4, 9, 16};
    const std::vector<double> mids = {0.25, 2.25, 6.25, 12.25};
    const double h = 1.0;

    // Known composite results for this data.
    assert(nearlyEqual(trapezoidalRule(y, h), 22.0));
    assert(nearlyEqual(midpointRule(mids, h), 21.0));
    assert(nearlyEqual(simpsonsRule(y, h), 64.0 / 3.0)); // exact for quadratics

    // Too few points must throw.
    bool threw = false;
    try { trapezoidalRule({1.0}, h); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    // Simpson needs an even number of intervals (odd point count).
    threw = false;
    try { simpsonsRule({0, 1, 4, 9}, h); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    // Non-positive step size must throw.
    threw = false;
    try { trapezoidalRule(y, 0.0); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    std::cout << "test_integration: all assertions passed\n";
    return 0;
}
