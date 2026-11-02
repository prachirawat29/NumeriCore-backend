# Build stage: compile numericore against the system nlohmann-json
# (header-only, from apt — no vcpkg needed inside the image).
FROM debian:bookworm-slim AS build

RUN apt-get update && apt-get install -y --no-install-recommends \
        g++ cmake ninja-build nlohmann-json3-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src
COPY CMakeLists.txt .
COPY include/ include/
COPY src/ src/

RUN cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF \
    && cmake --build build

# Runtime stage: just the binary + C++ runtime.
FROM debian:bookworm-slim AS runtime

RUN apt-get update && apt-get install -y --no-install-recommends \
        libstdc++6 \
    && rm -rf /var/lib/apt/lists/*

COPY --from=build /src/build/numericore /usr/local/bin/numericore

# Reads a JSON request on stdin, writes the JSON response on stdout.
#   docker run --rm -i numericore < data/sample_input.json
ENTRYPOINT ["numericore"]
