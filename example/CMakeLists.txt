cmake_minimum_required(VERSION 3.5)
project(ExampleProject VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin/debug)

# Locate the FracLib package
find_package(FracLib REQUIRED CONFIG)

# Add the executable or library for this project
add_executable(${PROJECT_NAME} src/main.cpp)

# Set the output directory for the executable
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Link the Fraction library to this project
target_link_libraries(${PROJECT_NAME} PRIVATE FracLib::Fraction)

# Include directories from FracLib
# target_include_directories(${PROJECT_NAME} PRIVATE ${FracLib_INCLUDE_DIRS})
