# Changelog
This file shows this current version only. To view past changes, please navigate to past versions via GitHub.

## Version 1.1.0
This version concentrates on adding important features like supporting decimal and string in multiple operators. Also, to fix important issues like improper subtraction logic, FracLib output generation(.a file and headers).

### Addition
- An example project in `/example`. Open terminal in that directory and run the `build.bat` then the `run.bat` to see results. The FracLib library must be built in the main directory `out/FracLib` in order for the example project to build correctly.
- `CHANGELOG` Markdown file for tracking current changes.
- Assignment operator now handles decimal and string fraction representations.
- Comparison operators now handles decimal and string fraction representations.
- Arithmetic operators now handles decimal and string fraction representations. 
- Compound arithmetic operators now handles decimal and string fraction representations.
- Reversed parameter for Comparison, Arithmetic and Compound operators for Fraction to Fraction supported. Decimal and string fraction representations also supported.
- A reciprocal converter which accepts and fraction and returns a new one that is the reciprocal of the fraction passed.

### Changes
- String Fraction construction data type changed from String object to C-String.
- Removed decimal converting to fraction from constructor `Fraction(double decimal)` to its own function `void toFraction(double decimal)`.
- Simplified increment/decrement logic - handles numerical increment/decrement only.
- Input stream handles mix fractions, decimals and more exceptions for incorrect format.
- `void Simplify(Fraction&)` logic is now moved to a private function and renamed to `void SimplifyFraction(Fraction&)`. `Fraction Simplify(Fraction)` is a new static function meant for inline math operations for Fractions passed by value while `void SimplifyFraction(Fraction&)` is meant for straight simplifications of Fractions passed by reference.

### Fixes
- The FracLib library output was not being generated correctly. Modifications to the CMakeLists, build.bat and FracLibConfig files were made to ensure the FracLib output directory can be generated to the main output directory as `out/FracLib`.
- Subtracting Fractions `operator-(const Fraction other)` formula was incorrect, updated using Common Denominator method. Same applies to the compound `-=`overload.
- Addition formula was incorrect outputting incorrect fraction results - corrected on all addition operations.
- Simplify now correctly handles negative fractions by making components absolute.