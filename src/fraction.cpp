/**************************************************************************/
/*  fraction.cpp                                                          */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             FracLib                                    */
/*                        https://jodadev.com                             */
/**************************************************************************/
/* Copyright (c) 2024-present Edwin J Martinez (aka Ghost/JODA).          */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "../include/fraction.h"
#include <iostream>
#include <limits>
#include <sstream>
#include <cmath>

namespace FracLib {
    const char* Fraction::ZERO_DIVISOR_ERROR = "Division by zero not allowed. Denominator cannot be zero.";
    const char* Fraction::OVERFLOW_ERROR = "Integer overflow detected.";
    const char* Fraction::INVALID_STRING_PARAMETER_ERROR = "Improper format. Accepted fraction form: (ie \"1/2\" or \"25\" or  \"3 1/2\").";

    //\\\\\\\\\\\\\\\\\\\\/
    // Utilities
    //\\\\\\\\\\\\\\\\\\\\/
    bool willMultiplicationOverflow(int a, int b) {
        // Handle zero cases
        if (a == 0 || b == 0) return false;

        // Check for overflow
        if (a == -1 && b == std::numeric_limits<int>::min()) return true;
        if (b == -1 && a == std::numeric_limits<int>::min()) return true;

        int result = a * b;
        return result / b != a;
    }

    bool willAdditionOverflow(int a, int b) {
        if (b > 0 && a > std::numeric_limits<int>::max() - b) return true;
        if (b < 0 && a < std::numeric_limits<int>::min() - b) return true;
        return false;
    }

    bool willSubtractionOverflow(int a, int b) {
        if (b < 0 && a > std::numeric_limits<int>::max() + b) return true;
        if (b > 0 && a < std::numeric_limits<int>::min() + b) return true;
        return false;
    }


    //\\\\\\\\\\\\\\\\\\\\/
    // Constructors
    //\\\\\\\\\\\\\\\\\\\\/
    Fraction::Fraction() : numerator(0), denominator(1) {}
    Fraction::Fraction(int n) : numerator(n), denominator(1) {}
    Fraction::Fraction(int n, int d, bool simplify) : numerator(n), denominator(d) {
        if (denominator == 0){
            throw std::invalid_argument(ZERO_DIVISOR_ERROR);
        }
        // Optional
        if (simplify) Simplify(*this);
    }
    Fraction::Fraction(double decimal){
        toFraction(decimal);
    }
    Fraction::Fraction(const char* fracStr, bool simplify){
        std::istringstream iss(fracStr);
        parseFromStream(iss, simplify);
    }
    Fraction::Fraction(Fraction& other){
        this->numerator = other.numerator;
        this->denominator = other.denominator;
    }


    //\\\\\\\\\\\\\\\\\\\\/
    // Basic Arithmetic Operators
    //\\\\\\\\\\\\\\\\\\\\/
    Fraction Fraction::operator+(const Fraction& other) {
        // Check for overflow in the intermediate calculations
        if (willMultiplicationOverflow(this->numerator, other.denominator) || 
            willMultiplicationOverflow(other.numerator, this->denominator) || 
            willMultiplicationOverflow(this->denominator, other.denominator) || 
            willAdditionOverflow(this->numerator * other.denominator, other.numerator * this->denominator)) {
            throw std::overflow_error(OVERFLOW_ERROR);
        }

        return Fraction((this->numerator * other.denominator) + (other.numerator * this->denominator), 
            this->denominator * other.denominator);
    }
    Fraction Fraction::operator+(int value){
        if(willMultiplicationOverflow(this->denominator, value)){
            throw std::overflow_error(OVERFLOW_ERROR);
        }
        if(willAdditionOverflow(this->numerator, (this->denominator * value))){
            throw std::overflow_error(OVERFLOW_ERROR);
        }
        return Fraction(this->numerator + (this->denominator * value), this->denominator);
    }   
    Fraction Fraction::operator+(double value){
        return *(this) + Fraction(value);
    };
    Fraction Fraction::operator+(const char* value){
        return *(this) + Fraction(value);
    };
    
    Fraction Fraction::operator-(const Fraction& other){
        if(willMultiplicationOverflow(this->numerator, other.denominator) || 
            willMultiplicationOverflow(this->denominator, other.numerator) || 
            willMultiplicationOverflow(this->denominator, other.denominator)){
                throw std::overflow_error(OVERFLOW_ERROR);
            }
        if(willSubtractionOverflow((this->numerator * other.denominator), (this->denominator * other.numerator))){
            throw std::overflow_error(OVERFLOW_ERROR);
        }
        return Fraction((this->numerator * other.denominator) - (this->denominator * other.numerator), (this->denominator * other.denominator));
    }
    Fraction Fraction::operator-(int value){
        if(willMultiplicationOverflow(this->denominator, value)){
            throw std::overflow_error(OVERFLOW_ERROR);
        }
        if(willSubtractionOverflow(this->numerator, (this->denominator * value))){
            throw std::overflow_error(OVERFLOW_ERROR);
        }
        return Fraction(this->numerator - (this->denominator * value), this->denominator);
    }   
    Fraction Fraction::operator-(double value){
        return *(this) - Fraction(value);
    };
    Fraction Fraction::operator-(const char* value){
        return *(this) - Fraction(value);
    };
    
    Fraction Fraction::operator*(const Fraction& other){
        if(willMultiplicationOverflow(this->numerator, other.numerator) || 
            willMultiplicationOverflow(this->denominator, other.denominator)){
                throw std::overflow_error(OVERFLOW_ERROR);
            }
        return Fraction(this->numerator * other.numerator, this->denominator * other.denominator);
    }
    Fraction Fraction::operator*(int value){
        if(willMultiplicationOverflow(this->numerator, value)){
            throw std::overflow_error(OVERFLOW_ERROR);
        }
        return Fraction((this->numerator * value), this->denominator);
    }
    Fraction Fraction::operator*(double value){
        return *(this) * Fraction(value);
    };
    Fraction Fraction::operator*(const char* value){
        return *(this) * Fraction(value);
    };
    
    Fraction Fraction::operator/(const Fraction& other){
        if (other.numerator == 0 || this->denominator == 0) {
            throw std::invalid_argument(ZERO_DIVISOR_ERROR);
        }
        if(willMultiplicationOverflow(this->numerator, other.denominator) || 
            willMultiplicationOverflow(this->denominator, other.numerator)){
                throw std::overflow_error(OVERFLOW_ERROR);
            }
        // reciprocal of other than multiply
        return Fraction(this->numerator * other.denominator, this->denominator * other.numerator);
    }
    Fraction Fraction::operator/(int value){
        if (this->numerator == 0){
            throw std::invalid_argument(ZERO_DIVISOR_ERROR);
        }
        if(willMultiplicationOverflow(this->denominator, value)){
            throw std::overflow_error(OVERFLOW_ERROR);
        }
        // reciprocal
        return Fraction((this->denominator * value), this->numerator);
    }
    Fraction Fraction::operator/(double value){
        return *(this) / Fraction(value);
    };
    Fraction Fraction::operator/(const char* value){
        return *(this) / Fraction(value);
    };

    // Reversed order
    Fraction operator+(int value, const Fraction& frac) {
        // Check for overflow in the intermediate calculations
        if (willMultiplicationOverflow(value, frac.denominator)) {
            throw std::overflow_error(Fraction::OVERFLOW_ERROR);
        }
        if (willAdditionOverflow(frac.numerator, value * frac.denominator)) {
            throw std::overflow_error(Fraction::OVERFLOW_ERROR);
        }

        // Perform the addition
        int newNumerator = frac.numerator + (value * frac.denominator);
        int newDenominator = frac.denominator;

        return Fraction(newNumerator, newDenominator);
    }
    Fraction operator+(double value, Fraction& frac){
        return Fraction(value) + frac;
    };
    Fraction operator+(const char* value, Fraction& frac){
        return Fraction(value) + frac;
    };
    
    Fraction operator-(int value, const Fraction& frac){
        return Fraction(frac.numerator - (frac.denominator * value), frac.denominator);
    };
    Fraction operator-(double value, Fraction& frac){
        return Fraction(value) - frac;
    };
    Fraction operator-(const char* value, Fraction& frac){
        return Fraction(value) - frac;
    };
    
    Fraction operator*(int value, const Fraction& frac){
        return Fraction(frac.numerator * value, frac.denominator);
    };
    Fraction operator*(double value, Fraction& frac){
        return Fraction(value) * frac;
    };
    Fraction operator*(const char* value, Fraction& frac){
        return Fraction(value) * frac;
    };
    
    Fraction operator/(int value, const Fraction& frac){
        if (frac.numerator == 0) {
            throw std::invalid_argument("Division by zero not allowed. Denominator cannot be zero.");
        }
        if(willMultiplicationOverflow(value, frac.denominator)){
            throw std::overflow_error(Fraction::OVERFLOW_ERROR);
        }
        return Fraction(value * frac.denominator, frac.numerator);
    };
    Fraction operator/(double value, Fraction& frac){
        return Fraction(value) / frac;
    };
    Fraction operator/(const char* value, Fraction& frac){
        return Fraction(value) / frac;
    };
    

    //\\\\\\\\\\\\\\\\\\\\/
    // Compound Operators
    //\\\\\\\\\\\\\\\\\\\\/
    void Fraction::operator+=(const Fraction& other) {
        // Check for overflow in intermediate calculations
        if (willMultiplicationOverflow(this->numerator, other.denominator) ||
            willMultiplicationOverflow(other.numerator, this->denominator) ||
            willMultiplicationOverflow(this->denominator, other.denominator) ||
            willAdditionOverflow(this->numerator * other.denominator, other.numerator * this->denominator)) {
            throw std::overflow_error(OVERFLOW_ERROR);
        }

        // Calculate new numerator and denominator
        int newNumerator = (this->numerator * other.denominator) + (other.numerator * this->denominator);
        int newDenominator = this->denominator * other.denominator;

        // Update the fraction
        this->numerator = newNumerator;
        this->denominator = newDenominator;
    }
    void Fraction::operator+=(int value){
        if(willMultiplicationOverflow(this->denominator, value)){
            throw std::overflow_error(OVERFLOW_ERROR);
        }
        if(willAdditionOverflow(this->numerator, (this->denominator * value))){
            throw std::overflow_error(OVERFLOW_ERROR);
        }
        this->numerator += (this->denominator * value);
    }
    void Fraction::operator+=(double value){
        (*this) = *(this) + Fraction(value);
    }
    void Fraction::operator+=(const char* value){
        (*this) = *(this) + Fraction(value);
    }

    void Fraction::operator-=(const Fraction& other){
        if(willMultiplicationOverflow(this->numerator, other.denominator) || 
            willMultiplicationOverflow(this->denominator, other.numerator) || 
            willMultiplicationOverflow(this->denominator, other.denominator)){
                throw std::overflow_error(OVERFLOW_ERROR);
            }
        if(willSubtractionOverflow((this->numerator * other.denominator), (this->denominator * other.numerator))){
            throw std::overflow_error(OVERFLOW_ERROR);
        }
        this->numerator = (this->numerator * other.denominator) - (this->denominator * other.numerator);
        this->denominator *= other.denominator;
    }
    void Fraction::operator-=(int value){
        if(willMultiplicationOverflow(this->denominator, value)){
            throw std::overflow_error(OVERFLOW_ERROR);
        }
        if(willSubtractionOverflow(this->numerator, (this->denominator * value))){
            throw std::overflow_error(OVERFLOW_ERROR);
        }
        this->numerator = this->numerator - (this->denominator * value);
    }
    void Fraction::operator-=(double value){
        (*this) = *(this) - Fraction(value);
    }
    void Fraction::operator-=(const char* value){
        (*this) = *(this) - Fraction(value);
    }
    
    void Fraction::operator*=(const Fraction& other){
        if(willMultiplicationOverflow(this->numerator, other.numerator) || 
            willMultiplicationOverflow(this->denominator, other.denominator)){
                throw std::overflow_error(OVERFLOW_ERROR);
            }
        this->numerator *= other.numerator;
        this->denominator *= other.denominator;
    }
    void Fraction::operator*=(int value){
        if(willMultiplicationOverflow(this->numerator, value)){
            throw std::overflow_error(OVERFLOW_ERROR);
        }
        this->numerator *= value;
    }
    void Fraction::operator*=(double value){
        (*this) = *(this) * Fraction(value);
    }
    void Fraction::operator*=(const char* value){
        (*this) = *(this) * Fraction(value);
    }
    
    void Fraction::operator/=(const Fraction& other){
        if (other.numerator == 0 || this->denominator == 0) {
            throw std::invalid_argument(ZERO_DIVISOR_ERROR);
        }
        if(willMultiplicationOverflow(this->numerator, other.denominator) || 
            willMultiplicationOverflow(this->denominator, other.numerator)){
                throw std::overflow_error(OVERFLOW_ERROR);
            }
        // reciprocal of other than multiply
        this->numerator *= other.denominator;
        this->denominator *= other.numerator;
    }
    void Fraction::operator/=(int value){
        if (this->numerator == 0){
            throw std::invalid_argument(ZERO_DIVISOR_ERROR);
        }
        if(willMultiplicationOverflow(value, this->denominator)){
            throw std::overflow_error(Fraction::OVERFLOW_ERROR);
        }
        // reciprocal of other than multiply
        this->numerator = this->denominator * value;
        this->denominator = this->numerator;
    }
    void Fraction::operator/=(double value){
        (*this) = *(this) / Fraction(value);
    }
    void Fraction::operator/=(const char* value){
        (*this) = *(this) / Fraction(value);
    }

    //\\\\\\\\\\\\\\\\\\\\/
    // Increment Decrement Operators
    //\\\\\\\\\\\\\\\\\\\\/
    Fraction& Fraction::operator++(){
        if (willAdditionOverflow(this->numerator, 1)) {
            throw std::overflow_error(OVERFLOW_ERROR);
        }
        ++numerator;
        return *this;
    }
    Fraction& Fraction::operator--(){
        if (willSubtractionOverflow(this->numerator, 1)) {
            throw std::overflow_error(OVERFLOW_ERROR);
        }
        --numerator;
        return *this;
    }
    Fraction Fraction::operator++(int){
        if (willAdditionOverflow(this->numerator, 1)) {
            throw std::overflow_error(OVERFLOW_ERROR);
        }
        Fraction temp = *this;
        numerator++;
        return temp; 
    }
    Fraction Fraction::operator--(int){
        if (willSubtractionOverflow(this->numerator, 1)) {
            throw std::overflow_error(OVERFLOW_ERROR);
        }
        Fraction temp = *this;
        numerator--; 
        return temp;
    }
    

    //\\\\\\\\\\\\\\\\\\\\/
    // Unary Operators
    //\\\\\\\\\\\\\\\\\\\\/
    Fraction Fraction::operator-(){
        // Handles negating numerator (+/-)
        return Fraction(-this->numerator, this->denominator);
    }


    //\\\\\\\\\\\\\\\\\\\\/
    // Comparision Operators
    //\\\\\\\\\\\\\\\\\\\\/
    bool Fraction::operator==(const Fraction& other) const {
        // This cross-multiplication avoids the need to reduce the fractions to their simplest forms.
        return (this->numerator * other.denominator) == (other.numerator * this->denominator);
    }
    bool Fraction::operator==(double other) const {
        return (*this == Fraction(other));
    }
    bool Fraction::operator==(const char* other) const {
        return (*this == Fraction(other));
    }
    bool operator==(double other, Fraction& frac) {
        return (Fraction(other) == frac);
    }
    bool operator==(const char* other, Fraction& frac) {
        return (Fraction(other) == frac);
    }

    bool Fraction::operator!=(const Fraction& other) const {
        return !(*this == other);  // implement != by negating ==
    }
    bool Fraction::operator!=(double other) const {
        return !(*this == Fraction(other));  // implement != by negating ==
    }
    bool Fraction::operator!=(const char* other) const {
        return !(*this == Fraction(other));  // implement != by negating ==
    }
    bool operator!=(double other, Fraction& frac) {
        return (Fraction(other) != frac);
    }
    bool operator!=(const char* other, Fraction& frac) {
        return (Fraction(other) != frac);
    }

    bool Fraction::operator>=(const Fraction& other) const {
        // This cross-multiplication avoids the need to reduce the fractions to their simplest forms.
        return (this->numerator * other.denominator) >= (other.numerator * this->denominator);
    }
    bool Fraction::operator>=(double other) const {
        return (*this >= Fraction(other));
    }
    bool Fraction::operator>=(const char* other) const {
        return (*this >= Fraction(other));
    }
    bool operator>=(double other, Fraction& frac) {
        return (Fraction(other) >= frac);
    }
    bool operator>=(const char* other, Fraction& frac) {
        return (Fraction(other) >= frac);
    }

    bool Fraction::operator<=(const Fraction& other) const {
        // This cross-multiplication avoids the need to reduce the fractions to their simplest forms.
        return (this->numerator * other.denominator) <= (other.numerator * this->denominator);
    }
    bool Fraction::operator<=(double other) const {
        return (*this <= Fraction(other));
    }
    bool Fraction::operator<=(const char* other) const {
        return (*this <= Fraction(other));
    }
    bool operator<=(double other, Fraction& frac) {
        return (Fraction(other) <= frac);
    }
    bool operator<=(const char* other, Fraction& frac) {
        return (Fraction(other) <= frac);
    }

    bool Fraction::operator>(const Fraction& other) const {
        // This cross-multiplication avoids the need to reduce the fractions to their simplest forms.
        return (this->numerator * other.denominator) > (other.numerator * this->denominator);
    }
    bool Fraction::operator>(double other) const {
        return (*this > Fraction(other));
    }
    bool Fraction::operator>(const char* other) const {
        return (*this > Fraction(other));
    }
    bool operator>(double other, Fraction& frac) {
        return (Fraction(other) > frac);
    }
    bool operator>(const char* other, Fraction& frac) {
        return (Fraction(other) > frac);
    }

    bool Fraction::operator<(const Fraction& other) const {
        // This cross-multiplication avoids the need to reduce the fractions to their simplest forms.
        return (this->numerator * other.denominator) < (other.numerator * this->denominator);
    }
    bool Fraction::operator<(double other) const {
        return (*this < Fraction(other));
    }
    bool Fraction::operator<(const char* other) const {
        return (*this < Fraction(other));
    }
    bool operator<(double other, Fraction& frac) {
        return (Fraction(other) < frac);
    }
    bool operator<(const char* other, Fraction& frac) {
        return (Fraction(other) < frac);
    }

    //\\\\\\\\\\\\\\\\\\\\/
    // Misc Operators
    //\\\\\\\\\\\\\\\\\\\\/
    std::ostream& operator<<(std::ostream& os, const Fraction& frac) {
        os << frac.numerator << "/" << frac.denominator;
        return os;
    }

    std::istream& operator>>(std::istream& is, Fraction& frac){
        std::string input;

        // Read the entire input
        std::getline(is, input);

        // Trim any leading/trailing whitespace
        input.erase(input.find_last_not_of(" \t\n\r\f\v") + 1);
        input.erase(0, input.find_first_not_of(" \t\n\r\f\v"));

        // Check if the input is empty or invalid
        if (input.empty() || (!std::isdigit(input[0]) && input[0] != '-')) {
            is.setstate(std::ios::failbit);
            throw std::invalid_argument(
                "Invalid format: use decimal (0.5, 1.2) or string fractions (1/2, 2 1/2).");
        }

        // Attempt to parse as a double and convert to Fraction object
        {
            std::istringstream iss(input);
            double value;

            if (iss >> value && iss.eof()) { 
                frac.toFraction(value);
                return is;
            }
        }

        // Attempt to parse string and convert to Fraction object
        {
            std::istringstream iss(input);
            try {
                frac.parseFromStream(iss);
                return is;
            } catch (const std::invalid_argument& e) {
                is.setstate(std::ios::failbit);
                throw std::invalid_argument(e.what());
            } catch (const std::overflow_error& e) {
                is.setstate(std::ios::failbit); 
                throw std::overflow_error(Fraction::OVERFLOW_ERROR);
            }
        }
        is.setstate(std::ios::failbit); // Mark stream as failed for invalid input
        return is;
    }

    Fraction& Fraction::operator=(const Fraction& other){
        if (this != &other) {  // Check for self-assignment
            this->numerator = other.numerator;
            this->denominator = other.denominator;
        }
        return *this;
    }

    Fraction& Fraction::operator=(const char* str){
        std::istringstream iss(str);
        parseFromStream(iss);
        return *this;
    }

    Fraction& Fraction::operator=(double decimal){
        toFraction(decimal);
        return *this;
    }


    //\\\\\\\\\\\\\\\\\\\\/
    // Methods
    //\\\\\\\\\\\\\\\\\\\\/
    Fraction Fraction::toReciprocal(const Fraction& frac){
        if (frac.numerator == 0){
            throw std::invalid_argument(ZERO_DIVISOR_ERROR);
        }
        return Fraction(frac.denominator, frac.numerator);
    }

    void Fraction::SimplifyFraction(Fraction& frac){
        frac.simplify();
    }

    Fraction Fraction::Simplify(Fraction frac){
        frac.simplify();
        return frac;
    }
    
    void Fraction::simplify(){
        if(denominator == 0) return; // quick fix for 0

        int a = std::abs(numerator); // Use absolute values
        int b = std::abs(denominator);
        int gcd = 0;
        while (true)
        {
            if (a > b){
                a = (a % b);
                if(a == 0) {
                    gcd = b;
                    break;
                }
            }
            else {
                b = (b % a);
                if(b == 0) {
                    gcd = a;
                    break;
                }
            }
        }
        
        numerator /= gcd;
        denominator /= gcd;

        // Ensure the denominator is always positive
        if (denominator < 0) {
            numerator = -numerator;
            denominator = -denominator;
        }
    }

    std::string Fraction::toString(const Fraction& frac){
        return std::string(std::to_string(frac.numerator) + "/" + std::to_string(frac.denominator));
    }
    
    float Fraction::toFloat(const Fraction& frac){
        return (float)frac.numerator / (float)frac.denominator;
    }
    
    double Fraction::toDouble(const Fraction& frac){
        return (double)frac.numerator / (double)frac.denominator;
    }

    void Fraction::toFraction(double decimal){
        // Save sign information and make decimal absolute value
        int sign = (decimal < 0) ? -1 : 1;
        decimal = std::abs(decimal);

        // Convert decimal to string to count decimal places
        std::string decimalStr = std::to_string(decimal);
        size_t decimalPointPos = decimalStr.find('.');
        size_t decimalPlaces = decimalStr.size() - decimalPointPos - 1;

        // Remove trailing zeros
        while (decimalStr.back() == '0') {
            decimalStr.pop_back();
            decimalPlaces--;
        }

        // Build numerator and denominator
        denominator = static_cast<int>(std::pow(10, decimalPlaces));
        numerator = static_cast<int>(decimal * denominator + 0.5); // Rounding

        if(denominator == 0){
            throw std::invalid_argument(ZERO_DIVISOR_ERROR);
        }
        
        // Required
        Simplify(*this);
    }

    //TODO Find better solution?
    void Fraction::parseFromStream(std::istream& is, bool simplify) {
        is >> std::noskipws;
        int temp = 0, whole = 0, num = 0, denom = 1;
        char ch;

        // Skip leading whitespace
        while (is.peek() == ' ' || is.peek() == '\t') {
            is.get(ch); // Consume the whitespace
        }

        // Try to read numerator or whole number
        // if (!(is >> temp)) {
        //     throw std::invalid_argument(INVALID_STRING_PARAMETER_ERROR);
        // }
        // FIX: for input streams since it was getting ascii number and not actual value
        std::string number;
        if (!std::isdigit(is.peek())){
            throw std::invalid_argument(INVALID_STRING_PARAMETER_ERROR);
        }
        while(std::isdigit(is.peek())){
            number += is.peek();
            is.get(); // consume
        }
        temp = std::stoi(number); // convert to number
        number.clear();

        ch = is.peek();
        is.get();
        // if (is >> ch) {
        if (ch) {
            if (ch == ' ') {
                whole = temp;

                // Skip whitespace after whole number
                while (is.peek() == ' ' || is.peek() == '\t') {
                    is.get(ch); // Consume the whitespace
                }

                // Try to read numerator
                // if (!(is >> num)) {
                //     throw std::invalid_argument(INVALID_STRING_PARAMETER_ERROR);
                // }
                if (!std::isdigit(is.peek())){
                    throw std::invalid_argument(INVALID_STRING_PARAMETER_ERROR);
                }
                while(std::isdigit(is.peek())){
                    number += is.peek();
                    is.get(); // consume
                }
                num = std::stoi(number); // convert to number
                number.clear();

                if (is >> ch) {
                    if (ch != '/') {
                        throw std::invalid_argument(INVALID_STRING_PARAMETER_ERROR);
                    }

                    // Try to read denominator
                    // if (!(is >> denom)) {
                    //     throw std::invalid_argument(INVALID_STRING_PARAMETER_ERROR);
                    // }
                    if (!std::isdigit(is.peek())){
                        throw std::invalid_argument(INVALID_STRING_PARAMETER_ERROR);
                    }
                    while(std::isdigit(is.peek())){
                        number += is.peek();
                        is.get(); // consume
                    }
                    denom = std::stoi(number); // convert to number
                    number.clear();

                } else {
                    throw std::invalid_argument(INVALID_STRING_PARAMETER_ERROR);
                }
            }
            else if (ch == '/') {
                num = temp;

                // Try to read denominator
                // if (!(is >> denom)) {
                //     throw std::invalid_argument(INVALID_STRING_PARAMETER_ERROR);
                // }
                if (!std::isdigit(is.peek())){
                    throw std::invalid_argument(INVALID_STRING_PARAMETER_ERROR);
                }
                while(std::isdigit(is.peek())){
                    number += is.peek();
                    is.get(); // consume
                }
                denom = std::stoi(number); // convert to number
                number.clear();
            }
            else {
                throw std::invalid_argument(INVALID_STRING_PARAMETER_ERROR);
            }
        } else {
            // No slash or space; it's a whole number
            num = temp;
            denom = 1;
            is.clear(); // Clear EOF flag
        }

        // Skip any remaining whitespace
        while (is.peek() == ' ' || is.peek() == '\t') {
            is.get(ch); // Consume the whitespace
        }

        // Check for extra characters - removed, will just ignore any extra characters
        // if (is.peek() != EOF && is.peek() != -1) {
        //     throw std::invalid_argument(INVALID_STRING_PARAMETER_ERROR);
        // }

        // Check for zero denominator
        if (denom == 0) {
            throw std::invalid_argument(ZERO_DIVISOR_ERROR);
        }

        // Assign parsed values to member variables
        denominator = denom;

        // Check for mixed fraction and calculate numerator
        if (whole != 0) {
            if (willMultiplicationOverflow(denominator, whole)) {
                throw std::overflow_error(OVERFLOW_ERROR);
            }
            numerator = denominator * whole;

            if (willAdditionOverflow(numerator, num)) {
                throw std::overflow_error(OVERFLOW_ERROR);
            }
            numerator += num;
        } else {
            numerator = num;
        }

        // Optional simplification
        if (simplify) Simplify(*this);
    }

}
