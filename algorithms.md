
# NumeriCore - Numerical Integration Algorithms

## 1. Introduction

NumeriCore is a C++ numerical computation backend
designed to estimate definite integrals using
numerical integration techniques.

It implements three methods:

1. Trapezoidal Rule
2. Midpoint Rule
3. Simpson's 1/3 Rule

The results are compared using error analysis
when an actual reference value is provided.

---

## 2. Trapezoidal Rule

### Concept

The Trapezoidal Rule approximates the area under
a curve by dividing it into trapezoids.

### Formula

I = (h/2) [y0 + 2(y1 + y2 + ... + y(n-1)) + yn]

Where:

- h = Width of each interval
- y0 = First function value
- yn = Last function value
- n = Number of intervals

### Algorithm

1. Read the endpoint measurements.
2. Add the first and last values.
3. Multiply each interior value by 2.
4. Add all values.
5. Multiply the sum by h/2.
6. Return the estimated integral.

### Time Complexity

O(n)

---

## 3. Midpoint Rule

### Concept

The Midpoint Rule estimates the area using
rectangles whose heights are determined by
the function values at interval midpoints.

### Formula

I = h [f(m1) + f(m2) + ... + f(mn)]

Where:

- h = Width of each interval
- mi = Midpoint of interval i
- f(mi) = Function value at the midpoint

### Algorithm

1. Read midpoint measurements.
2. Add all midpoint values.
3. Multiply the sum by h.
4. Return the estimated integral.

### Time Complexity

O(n)

---

## 4. Simpson's 1/3 Rule

### Concept

Simpson's 1/3 Rule approximates a curve using
parabolic segments.

It requires an even number of intervals.

### Formula

I = (h/3) [y0 + 4y1 + 2y2 + 4y3
           + ... + 4y(n-1) + yn]

Where:

- h = Width of each interval
- y0 = First function value
- yn = Last function value
- Interior values receive alternating
  coefficients of 4 and 2.

### Algorithm

1. Read endpoint measurements.
2. Add the first and last values.
3. Multiply odd-indexed interior values by 4.
4. Multiply even-indexed interior values by 2.
5. Add all values.
6. Multiply the sum by h/3.
7. Return the estimated integral.

### Time Complexity

O(n)

---

## 5. Error Analysis

NumeriCore calculates two types of errors.

### Absolute Error

Absolute Error = |Actual Value - Estimated Value|

### Percentage Error

Percentage Error =
(Absolute Error / |Actual Value|) x 100

Percentage error is unavailable when the
actual reference value is zero.

---

## 6. Accuracy Comparison

The backend compares the absolute errors
of all three integration methods.

The method with the smallest absolute error
is considered the most accurate for the
given dataset.

If multiple methods have equal minimum
errors within the comparison tolerance,
all are reported.

---

## 7. Input Validation

Before performing calculations, NumeriCore checks:

- Valid operation type
- Valid numerical values
- Matching measurement arrays
- Equal spacing between coordinates
- Valid measurement count
- Even interval count for Simpson's Rule
- Correct midpoint measurement count

Invalid inputs are rejected with structured
error codes and messages.

---

## 8. Overall Processing Flow

1. Receive JSON input.
2. Parse measurement data.
3. Validate the input.
4. Calculate interval width.
5. Apply numerical integration methods.
6. Perform error analysis if a reference
   value is available.
7. Compare accuracy.
8. Generate JSON response.
