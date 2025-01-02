/**************************************************************************/
/*  fraction.h                                                            */
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

#pragma once
#include <string>
#include <stdexcept>

namespace FracLib {
    class Fraction {
    public:
        static const char* ZERO_DIVISOR_ERROR;
        static const char* OVERFLOW_ERROR;
        static const char* INVALID_STRING_PARAMETER_ERROR;
    public:
        int numerator;
        int denominator;

    public: // CONSTRUCTORS
        /// @brief Default constructor. Initializes the fraction to `0/1`.
        /// @example Fraction f; // Represents 0/1
        Fraction();
        /// @brief Constructs a Fraction object with the given integer numerator.
        /// @param n The integer numerator (denominator is set to 1).
        /// @param simplify Determines whether the fraction will attempt to simplify or not.
        /// @example Fraction f(5); // Creates a fraction representing 5/1
        Fraction(int n);
        /// @brief Constructs a Fraction object with the specified numerator and denominator.
        /// @param n The numerator.
        /// @param d The denominator.
        /// @param simplify Determines whether the fraction will attempt to simplify or not.
        /// @throws std::invalid_argument If the denominator is zero.
        /// @example Fraction f(3, 4); // Creates a fraction representing 3/4
        Fraction(int n, int d, bool simplify = false);
        /// @brief Constructs a Fraction object from a decimal number by approximating its fractional equivalent.
        /// The fraction is then simplified.
        /// @param decimal The decimal number to convert to a fraction.
        /// @example Fraction f(0.75); // Creates a fraction representing 3/4
        Fraction(double decimal);
        /// @brief Constructs a Fraction object by parsing a string representation.
        /// @param fracStr The string representing the fraction, in the format "numerator/denominator", "numerator" or "whole numerator/denominator".
        /// @param simplify Determines whether the fraction will attempt to simplify or not.
        /// @throws std::invalid_argument If the string is not properly formatted or if the denominator is zero.
        /// @example Fraction f("3/4"); // Creates a fraction representing 3/4
        Fraction(const char* fracStr, bool simplify = false);
        /// @brief Copy constructor. Creates a new Fraction object as a copy of an existing Fraction.
        /// @param other The Fraction object to copy.
        /// @example Fraction f2(f1); // f2 is a copy of f1
        Fraction(Fraction& other);

    public: // OPERATORS
        
        // Basic Arithmetic
        Fraction operator+(const Fraction& other);
        Fraction operator+(int value);
        Fraction operator+(double value);
        Fraction operator+(const char* value);

        Fraction operator-(const Fraction& other);
        Fraction operator-(int value);
        Fraction operator-(double value);
        Fraction operator-(const char* value);

        Fraction operator*(const Fraction& other);
        Fraction operator*(int value);
        Fraction operator*(double value);
        Fraction operator*(const char* value);

        Fraction operator/(const Fraction& other);
        Fraction operator/(int value);
        Fraction operator/(double value);
        Fraction operator/(const char* value);
        
        // Reversed order - ie 2 + Fraction
        friend Fraction operator+(int value, const Fraction& frac);
        friend Fraction operator+(double value, Fraction& frac);
        friend Fraction operator+(const char* value, Fraction& frac);

        friend Fraction operator-(int value, const Fraction& frac);
        friend Fraction operator-(double value, Fraction& frac);
        friend Fraction operator-(const char* value, Fraction& frac);

        friend Fraction operator*(int value, const Fraction& frac);
        friend Fraction operator*(double value, Fraction& frac);
        friend Fraction operator*(const char* value, Fraction& frac);

        friend Fraction operator/(int value, const Fraction& frac);
        friend Fraction operator/(double value, Fraction& frac);
        friend Fraction operator/(const char* value, Fraction& frac);
        
        // Compound
        void operator+=(const Fraction& other);
        void operator+=(int value);
        void operator+=(double value);
        void operator+=(const char* value);

        void operator-=(const Fraction& other);
        void operator-=(int value);
        void operator-=(double value);
        void operator-=(const char* value);

        void operator*=(const Fraction& other);
        void operator*=(int value);
        void operator*=(double value);
        void operator*=(const char* value);

        void operator/=(const Fraction& other);
        void operator/=(int value);
        void operator/=(double value);
        void operator/=(const char* value);
        
        // Increment/Decrement Post/Pre
        Fraction& operator++();
        Fraction& operator--();
        Fraction operator++(int);
        Fraction operator--(int);

        // Unary
        Fraction operator-();

        // Comparision
        bool operator==(const Fraction& other) const;
        bool operator==(double other) const;
        bool operator==(const char* other) const;
        friend bool operator==(double other, Fraction& frac);
        friend bool operator==(const char* other, Fraction& frac);

        bool operator!=(const Fraction& other) const;
        bool operator!=(double other) const;
        bool operator!=(const char* other) const;
        friend bool operator!=(double other, Fraction& frac);
        friend bool operator!=(const char* other, Fraction& frac);

        bool operator>=(const Fraction& other) const;
        bool operator>=(double other) const;
        bool operator>=(const char* other) const;
        friend bool operator>=(double other, Fraction& frac);
        friend bool operator>=(const char* other, Fraction& frac);

        bool operator<=(const Fraction& other) const;
        bool operator<=(double other) const;
        bool operator<=(const char* other) const;
        friend bool operator<=(double other, Fraction& frac);
        friend bool operator<=(const char* other, Fraction& frac);

        bool operator>(const Fraction& other) const;
        bool operator>(double other) const;
        bool operator>(const char* other) const;
        friend bool operator>(double other, Fraction& frac);
        friend bool operator>(const char* other, Fraction& frac);

        bool operator<(const Fraction& other) const;
        bool operator<(double other) const;
        bool operator<(const char* other) const;
        friend bool operator<(double other, Fraction& frac);
        friend bool operator<(const char* other, Fraction& frac);

        // Assignment
        Fraction& operator=(const Fraction& other);
        Fraction& operator=(const char* str);
        Fraction& operator=(double decimal);

        // Insertion/Extraction
        friend std::ostream& operator<<(std::ostream& os, const Fraction& frac);
        friend std::istream& operator>>(std::istream& is, Fraction& frac);

    public: // STATIC METHODS
        
        /// @brief Best use is for inline math operations. Simplifies a Fraction object using GCD(Greatest Common Divisor).
        /// @param frac Fraction value object.
        static Fraction Simplify(Fraction frac);
        /// @brief Best use is for straight-forward simplification. Simplifies a Fraction object using GCD(Greatest Common Divisor).
        /// @param frac Fraction reference object.
        static void SimplifyFraction(Fraction& frac);
        /// @brief Converts a Fraction object to a string (ie. "1/2").
        /// @param frac Fraction object
        /// @return fraction as string
        static std::string toString(const Fraction& frac);
        static float toFloat(const Fraction& frac);
        /// @brief Converts a fraction to a floating decimal.
        /// @param frac Fraction object.
        /// @return fraction as float.
        static double toDouble(const Fraction& frac);
        /// @brief Returns the reciprocal of a fraction as a new Fraction object.
        /// @param frac fraction to get reciprocal of
        /// @return fraction reciprocal
        static Fraction toReciprocal(const Fraction& frac);

    private: // PRIVATE FUNCTIONS
        void simplify();

        /// @brief Parses input stream fractions to Fraction members numerator and denominator.
        /// @param is input stream.
        /// @param simplify optional, simplify fraction.
        void parseFromStream(std::istream& is, bool simplify = false);
        /// @brief Converts a decimal into a Fraction by assigning the numerator and denominator
        /// to this Fraction object. The Fraction is simplified.
        /// @param decimal decimal to convert to fraction.
        void toFraction(double decimal);
    };
}


