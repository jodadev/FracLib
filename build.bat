@echo off

set OUT_DIR=out
set BUILD_DIR=%OUT_DIR%\bin

rem Set the installation prefix
set INSTALL_PREFIX=%CD%\%OUT_DIR%\FracLib

rem Create directories
mkdir %OUT_DIR%
mkdir %BUILD_DIR%
cd %BUILD_DIR%

cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="%CD%\..\FracLib" ../..
rem Build and install the project
cmake --build .
cmake --install .
