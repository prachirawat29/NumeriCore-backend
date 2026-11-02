# NumeriCore Backend

A small C++ numerical-integration backend. It reads a JSON request on **stdin**,
estimates a definite integral three ways (trapezoidal, midpoint, Simpson's 1/3),
optionally compares them against a known reference value, and writes a JSON
response on **stdout**.

It is a batch/CLI program, not an HTTP server — one request in, one response out.

## Layout

```
include/   public headers (models/, services/, utils/)
src/       implementation + main.cpp (entrypoint) + api.cpp (request pipeline)
tests/     assert-based unit tests + test_cases/ fixtures
data/      sample_input.json / sample_output.json
docs/      algorithms, API reference, testing notes
```

## Dependencies

Only [nlohmann/json](https://github.com/nlohmann/json) (header-only). It is
pulled automatically — via vcpkg for the native build (`vcpkg.json`), or via the
`nlohmann-json3-dev` apt package inside Docker.

> `third_party/json.hpp` is an unused placeholder kept to match the repo layout;
> the build uses the packaged nlohmann/json, not that file.

## Build & run (native, vcpkg)

Requires CMake, a C++17 compiler, Ninja, and vcpkg.

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"
```

```bash
cmake --build build
```

```bash
./build/numericore < data/sample_input.json
```

On Windows with the MinGW toolchain, add
`-DVCPKG_TARGET_TRIPLET=x64-mingw-dynamic -DVCPKG_HOST_TRIPLET=x64-mingw-dynamic`
to the configure command (the binary is `numericore.exe`).

## Build & run (Docker)

```bash
docker build -t numericore .
```

```bash
docker run --rm -i numericore < data/sample_input.json
```

## Tests

```bash
ctest --test-dir build --output-on-failure
```

See [docs/testing.md](docs/testing.md) for what is covered. The full request /
response contract is in [docs/api-documentation.md](docs/api-documentation.md),
and the maths in [docs/algorithms.md](docs/algorithms.md).

## Example

Request (`data/sample_input.json`):

```json
{
    "operation": "calculate_area",
    "start": 0,
    "end": 4,
    "x_values": [0, 1, 2, 3, 4],
    "y_values": [0, 1, 4, 9, 16],
    "midpoint_values": [0.25, 2.25, 6.25, 12.25],
    "actual_area": 21.3333333333333
}
```

Response (abridged): `success: true`, Simpson estimates `21.3333…` with the
smallest error, so `most_accurate_methods: ["simpson"]`.
