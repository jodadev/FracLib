cmake_minimum_required(VERSION 3.5)
project(FracLib VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the library target
add_library(Fraction STATIC src/Fraction.cpp)

# Include directories for the library target
target_include_directories(Fraction PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Set the output directory for the library
set_target_properties(Fraction PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/$<CONFIG>
)

# Compile definitions based on build type
target_compile_definitions(Fraction PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
)

# Installation commands
include(GNUInstallDirs)

# Install the library
install(TARGETS Fraction EXPORT FractionTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# Install header files
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Export the targets to a script
install(EXPORT FractionTargets
    FILE FractionTargets.cmake
    NAMESPACE FracLib::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/FracLib
)

# Generate the ConfigVersion file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/FracLibConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

# Generate the Config file
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/FracLibConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/FracLibConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/FracLib
)

# Install the Config files
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/FracLibConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/FracLibConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/FracLib
)
