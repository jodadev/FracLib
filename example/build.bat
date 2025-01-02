@echo off
setlocal

set BUILD_DIR=bin\debug
set FracLib=%CD%\..\out\FracLib

rem Normalize the path to FracLib
for /f "delims=" %%i in ("%FracLib%") do set FracLib=%%~fi

rem Ensure the library installation directory exists
if not exist "%FracLib%" (
    echo "Error: Required library directory %FracLib% does not exist."
    exit /b 1
)

mkdir %BUILD_DIR%
cd %BUILD_DIR%

cmake -S ../.. -B . -G "Ninja" -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="%FracLib%"
cmake --build .