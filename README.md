
# FracLib

## Overview

`FracLib` is a fraction library which provides an efficient and user-friendly way to work with fractions in C++. It includes various constructors, operators, and methods for basic arithmetic, comparisons, and simplifications of fractions, along with utilities for converting fractions to strings and floating-point representations.

## Features

- **Fraction Creation**: 
  - Supports fraction creation from integers, strings, and decimals.
  - Handles mixed fractions and improper formats gracefully with error messages.

- **Arithmetic Operations**:
  - Addition, subtraction, multiplication, and division for fractions and integers.
  - Compound assignments (e.g., `+=`, `-=`) with fractions and integers.
  
- **Comparison Operators**:
  - Equality, inequality, and relational operators (`==`, `!=`, `>=`, `<=`, `>`, `<`) for easy comparison.

- **Simplification**:
  - Automatically simplifies fractions using the Greatest Common Divisor (GCD) for readability.

- **Stream Support**:
  - Overloads `<<` and `>>` operators for output/input streams, making it easy to print and read fractions.

- **Static Methods**:
  - `Simplify`, `toString`, `toFloat`, and `toDouble` for easy conversions and utility functions.

## Installation

### Prerequisites
- [CMake](https://cmake.org/download/) (for building)
- [Ninja](https://ninja-build.org/) (for efficient builds)

### Steps

1. **Clone or Download** the repository.

2. **Build and Install** the project by running the following commands:
```sh
./build
```

3. **Use in Your Projects**: Link your application to the `Fraction` library installed in the specified directory.

## Build Output
The build process generates a folder named FracLib within the out directory. This folder contains the necessary files to use the Fraction library as a static library in your projects.

### Steps
1. Locate the FracLib Folder: After running the build, the FracLib folder will be created in the out directory. This folder includes the compiled static library (libFraction.a or Fraction.lib depending on your operating system) and any necessary headers.

2. Using FracLib in Your Projects:
  - Include Path: Add the FracLib folder as an include path in your project's build settings.
  - Link Library: Link the static library file found in the FracLib folder (libFraction.a for Unix-based systems or Fraction.lib for Windows) to your project. This allows you to use Fraction functions and classes by including Fraction.h in your source files.

## Usage

After installation, you can include `frac.h` in your project and start using the library for fraction-based calculations.

### Example
```cpp
#include "frac.h"

int main() {
    Fraction a(3, 4); // 3/4
    Fraction b("1/2"); // 1/2
    Fraction result = a + b;

    std::cout << "Result: " << result << std::endl;
    return 0;
}
```

## License
This library is released under the MIT License.
