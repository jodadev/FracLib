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
        if (denominator == 0){ // validate denominator
            throw std::invalid_argument(ZERO_DENOMINATOR_ERROR);
        }
        // Optional
        if (simplify) Simplify(*this);
    }
    Fraction::Fraction(double decimal){
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
        
        // Required
        Simplify(*this);
    }
    Fraction::Fraction(std::string fracStr, bool simplify){

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
    Fraction Fraction::operator+(const Fraction other){
        return Fraction(this->numerator + other.numerator, this->denominator + other.denominator);
    }
    Fraction Fraction::operator+(const int& whole){
        return Fraction(this->numerator + (this->denominator * whole), this->denominator);
    }   
    Fraction Fraction::operator-(const Fraction other){
        return Fraction(this->numerator - other.numerator, this->denominator - other.denominator);
    }
    Fraction Fraction::operator-(const int& whole){
        return Fraction(this->numerator - (this->denominator * whole), this->denominator);
    }   
    Fraction Fraction::operator*(const Fraction other){
        return Fraction(this->numerator * other.numerator, this->denominator * other.denominator);
    }
    Fraction Fraction::operator*(const int& whole){
        return Fraction((this->numerator * whole), this->denominator);
    }
    Fraction Fraction::operator/(const Fraction other){
        // reciprocal of other than multiply
        return Fraction(this->numerator * other.denominator, this->denominator * other.numerator);
    }
    Fraction Fraction::operator/(const int& whole){
        if (this->numerator == 0){
            throw std::invalid_argument(ZERO_DENOMINATOR_ERROR);
        }
        // reciprocal
        return Fraction((this->denominator * whole), this->numerator);
    }

    Fraction operator+(int value, const Fraction& frac){
        return Fraction(frac.numerator + (value * frac.denominator), frac.denominator);
    };
    Fraction operator-(int value, const Fraction& frac){
        return Fraction(frac.numerator - (frac.denominator * value), frac.denominator);
    };
    Fraction operator*(int value, const Fraction& frac){
        return Fraction(frac.numerator * value, frac.denominator);
    };
    Fraction operator/(int value, const Fraction& frac){
        if (frac.numerator == 0) {
            throw std::invalid_argument("Division by zero not allowed. Denominator cannot be zero.");
        }
        return Fraction(value * frac.denominator, frac.numerator);
    };


    //\\\\\\\\\\\\\\\\\\\\/
    // Compound Operators
    //\\\\\\\\\\\\\\\\\\\\/
    void Fraction::operator+=(const Fraction other){
        this->numerator += other.numerator;
        this->denominator += other.denominator;
    }
    void Fraction::operator+=(const int& value){
        this->numerator += (this->denominator * value);
    }
    void Fraction::operator-=(const Fraction other){
        this->numerator -= other.numerator;
        this->denominator -= other.denominator;
    }
    void Fraction::operator-=(const int& value){
        this->numerator = this->numerator - (this->denominator * value);
    }
    void Fraction::operator*=(const Fraction other){
        this->numerator *= other.numerator;
        this->denominator *= other.denominator;
    }
    void Fraction::operator*=(const int& value){
        this->numerator *= value;
    }
    void Fraction::operator/=(const Fraction other){
        if (this->numerator == 0){
            throw std::invalid_argument(ZERO_DENOMINATOR_ERROR);
        }
        // reciprocal of other than multiply
        this->numerator *= other.denominator;
        this->denominator *= other.numerator;
    }
    void Fraction::operator/=(const int& value){
        if (this->numerator == 0){
            throw std::invalid_argument(ZERO_DENOMINATOR_ERROR);
        }
        // reciprocal of other than multiply
        this->numerator = this->denominator * value;
        this->denominator = this->numerator;
    }
    

    //\\\\\\\\\\\\\\\\\\\\/
    // Increment Decrement Operators
    //\\\\\\\\\\\\\\\\\\\\/
    Fraction& Fraction::operator++(){
        if (willAdditionOverflow(this->numerator, this->denominator)) {
            throw std::overflow_error(OVERFLOW_ERROR);
        }
        numerator += denominator;
        // Simplify(*this);
        return *this;
    }
    Fraction& Fraction::operator--(){
        if (willSubtractionOverflow(this->numerator, this->denominator)) {
            throw std::overflow_error(OVERFLOW_ERROR);
        }
        numerator -= denominator;
        // Simplify(*this);
        return *this;
    }
    Fraction Fraction::operator++(int){
        if (willAdditionOverflow(this->numerator, this->denominator)) {
            throw std::overflow_error(OVERFLOW_ERROR);
        }
        Fraction temp = *this;
        ++(*this); 
        // Simplify(*this);
        return temp;
    }
    Fraction Fraction::operator--(int){
        if (willSubtractionOverflow(this->numerator, this->denominator)) {
            throw std::overflow_error(OVERFLOW_ERROR);
        }
        Fraction temp = *this;
        --(*this); 
        // Simplify(*this);
        return temp;
    }
    

    //\\\\\\\\\\\\\\\\\\\\/
    // Unary Operators
    //\\\\\\\\\\\\\\\\\\\\/
    Fraction Fraction::operator+(){
        return *this;
    }
    Fraction Fraction::operator-(){
        return Fraction(-this->numerator, this->denominator);
    }


    //\\\\\\\\\\\\\\\\\\\\/
    // Comparision Operators
    //\\\\\\\\\\\\\\\\\\\\/
    bool Fraction::operator==(const Fraction& other) const {
        return (this->numerator * other.denominator) == (other.numerator * this->denominator);
    }
    bool Fraction::operator!=(const Fraction& other) const {
        return !(*this == other);  // implement != by negating ==
    }
    bool Fraction::operator>=(const Fraction& other) const {
        return (this->numerator * other.denominator) >= (other.numerator * this->denominator);
    }
    bool Fraction::operator<=(const Fraction& other) const {
        return (this->numerator * other.denominator) <= (other.numerator * this->denominator);
    }
    bool Fraction::operator>(const Fraction& other) const {
        return (this->numerator * other.denominator) > (other.numerator * this->denominator);
    }
    bool Fraction::operator<(const Fraction& other) const {
        return (this->numerator * other.denominator) < (other.numerator * this->denominator);
    }


    //\\\\\\\\\\\\\\\\\\\\/
    // Misc Operators
    //\\\\\\\\\\\\\\\\\\\\/
    std::ostream& operator<<(std::ostream& os, const Fraction& frac) {
        os << frac.numerator << "/" << frac.denominator;
        return os;
    }

    std::istream& operator>>(std::istream& is, Fraction& frac){
         try {
            frac.parseFromStream(is);
            // Clear any error flags if parsing was successful
            is.clear();
        } catch (const std::invalid_argument e) {
            // Set the failbit to indicate extraction failure
            is.setstate(std::ios::failbit);
            std::cerr << "Failed - invalid" << std::endl;
        } catch (const std::overflow_error e) {
            // Set the failbit to indicate extraction failure
            is.setstate(std::ios::failbit);
            std::cerr << "Failed - overflow" << std::endl;
        }

        return is;
    }

    Fraction& Fraction::operator=(const Fraction& other){
        if (this != &other) {  // Check for self-assignment
            this->numerator = other.numerator;
            this->denominator = other.denominator;
        }
        return *this;
    }


    //\\\\\\\\\\\\\\\\\\\\/
    // Methods
    //\\\\\\\\\\\\\\\\\\\\/
    void Fraction::Simplify(Fraction& frac){
        if(frac.denominator == 0) return; // quick fix for 0

        int a = frac.numerator;
        int b = frac.denominator;
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
        
        frac.numerator /= gcd;
        frac.denominator /= gcd;
        
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

    //TODO Find better solution - it is working however.
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
            throw std::invalid_argument(ZERO_DENOMINATOR_ERROR);
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
