# Testing

Tests are plain assert-based programs (no framework) registered with CTest.
Each exits non-zero on the first failed assertion.

## Running

```bash
cmake -S . -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"
```

```bash
cmake --build build
```

```bash
ctest --test-dir build --output-on-failure
```

Tests build only when `BUILD_TESTING=ON` (the default). The Docker image
configures with `-DBUILD_TESTING=OFF` so the runtime image stays minimal.

## Coverage

**`tests/test_integration.cpp`** — the three integration rules on
f(x)=x² over [0, 4], h = 1:

- trapezoidal = 22, midpoint = 21, Simpson = 64/3 (exact for a quadratic)
- throws on too few points, on an odd interval count (Simpson), and on a
  non-positive step size

**`tests/test_validation.cpp`** — `validateInput` and `isValidNumber`:

- a well-formed sample passes
- rejects: unknown operation, `end ≤ start`, odd interval count, wrong midpoint
  count, and unequal x-spacing — each with its expected error code
- `isValidNumber` rejects infinity and NaN

## Fixtures

`tests/test_cases/` holds JSON fixtures used for manual / integration checks:

- `valid_input.json` — a complete valid request
- `invalid_input.json` — 4 points (odd interval count) → `INVALID_INTERVAL_COUNT`
- `expected_output.json` — the response produced for `valid_input.json`

Regenerate the expected output after an intended behaviour change:

```bash
./build/numericore < tests/test_cases/valid_input.json > tests/test_cases/expected_output.json
```
