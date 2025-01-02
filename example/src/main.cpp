/* Example Project
    
    This example file is meant to experiment with the library to familiarize
    yourself with FracLib features.

    Below in the main function is a basic example, you may run the executable
    by running ./run in your terminal (make sure you build first with ./build 
    and that you've built the FracLib in the main directory with ./build).

    You may change the contents of main to experiment with the library,
    ensure you rebuild using ./build when changes are made.

    Enjoy!

*/

#include "frac.h"
#include <iostream>

void print(const char* msg, Fraction& frac){
    // Support for writing Fraction objects to output stream.
    std::cout << msg << ": " << frac << std::endl;
}

int main(){

    Fraction frac1(0.5); // using decimals works! This fraction is 1/2
    print("Construction by decimal", frac1);

    frac1 = 0.6; // Reassign easily
    print("Reassigned by decimal", frac1);

    frac1 = "1/2"; // Accepts string literal assignments
    print("Reassigned by string", frac1);

    Fraction frac2(1,2); // Construction with numerator, denominator
    frac1 = frac1 + frac2; // Arithmetic of two separate Fraction objects
    print("Arithmetic (fraction + fraction)", frac1);

    frac1 = frac1 + "1/2"; // Arithmetic using string representation
    print("Arithmetic (fraction + string)", frac1);

    frac1 = frac1 * 0.2; // Arithmetic using decimal representation
    print("Arithmetic (fraction * decimal)", frac1);

    frac1 = frac1 * 1.2; // Arithmetic with mix fractions decimal
    print("Arithmetic (fraction * decimal(mixed))", frac1);

    frac1 = frac1 + "2 1/2"; // Arithmetic with mix fractions string
    print("Arithmetic (fraction + string(mixed))", frac1);

    // Arithmetic reversed order 
    frac1 = Fraction::Simplify(0.5 / frac1); // simplified
    print("Arithmetic (decimal / fraction) - simplified", frac1);
    
    frac1 = Fraction::Simplify("1/2" - frac1);
    print("Arithmetic (string / fraction) - simplified", frac1);

    // Increment/Decrement pre/post
    ++frac1;
    print("Prefix Increment", frac1);
    --frac1;
    print("Postfix Decrement", frac1);
    frac1++;
    print("Prefix Increment", frac1);
    frac1--;
    print("Postfix Decrement", frac1);

    frac1 *= -1; // flip negative to positive
    print("Compound Arithmetic (fraction *= -1) - flips sign.", frac1);

    // Comparisons
    if (frac1 == frac1){
        print("Equality", frac1);
    }
    if (frac1 != "2/92"){
        print("Inequality 2/92 not equal to", frac1);
    }
    if (2.6 > frac1){
        print("Relational 2 3/5 greater than", frac1);
    }

    // accepts decimal(0.5, 1.5) or string(2/4, 1 1/2) representations
    std::cin >> frac1;
    Fraction::SimplifyFraction(frac1); // when you can't perform inline or just want to simplify later
    print("User Input display", frac1);

    // Prevent terminal from closing
    system("pause");
    return 0;
}