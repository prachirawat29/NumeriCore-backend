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
COPY third_party/ third_party/

RUN cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF \
    && cmake --build build

# Runtime stage: just the binaries + C++ runtime.
FROM debian:bookworm-slim AS runtime

RUN apt-get update && apt-get install -y --no-install-recommends \
        libstdc++6 \
    && rm -rf /var/lib/apt/lists/*

COPY --from=build /src/build/numericore /usr/local/bin/numericore
COPY --from=build /src/build/numericore_server /usr/local/bin/numericore_server

# HTTP server: listens on $PORT (default 8080), POST /api/calculate.
# The stdin/stdout CLI is still available as `numericore`.
EXPOSE 8080
ENTRYPOINT ["numericore_server"]
