cmake_minimum_required(VERSION 3.20)
project(NumeriCore CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(nlohmann_json CONFIG REQUIRED)

# Core logic as a library so the executable and the tests share one build.
add_library(numericore_core STATIC
    src/api.cpp
    src/models/Measurement.cpp
    src/models/CalculationResult.cpp
    src/models/ErrorResult.cpp
    src/services/Integration.cpp
    src/services/ErrorAnalysis.cpp
    src/services/CalculationService.cpp
    src/utils/Validation.cpp
    src/utils/JsonHandler.cpp
)
target_include_directories(numericore_core PUBLIC include)
target_link_libraries(numericore_core PUBLIC nlohmann_json::nlohmann_json)

add_executable(numericore src/main.cpp)
target_link_libraries(numericore PRIVATE numericore_core)

option(BUILD_TESTING "Build the unit tests" ON)
if(BUILD_TESTING)
    enable_testing()

    add_executable(test_integration tests/test_integration.cpp)
    target_link_libraries(test_integration PRIVATE numericore_core)
    add_test(NAME integration COMMAND test_integration)

    add_executable(test_validation tests/test_validation.cpp)
    target_link_libraries(test_validation PRIVATE numericore_core)
    add_test(NAME validation COMMAND test_validation)
endif()
