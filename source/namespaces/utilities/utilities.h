//
// Created by Alon Sportes on 24/02/2025.
//

#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>

using namespace std;

namespace utilities {

// ToStringWithPrecision function ------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
std::string ToStringWithPrecision(const T a_value, const int n = 2) {
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return out.str();
}

// FindSubstring function -----------------------------------------------------------------------------------------------------------------------------------------------

bool FindSubstring(string string1, string string2) {
    if (string1.find(string2) != string::npos) {
        return true;
    } else {
        return false;
    }
}

// RadCalc function -----------------------------------------------------------------------------------------------------------------------------------------------------

double RadCalc(double x, double y, double z) {
    double r = sqrt(x * x + y * y + z * z);
    return r;
}

}  // namespace utilities

#endif  // UTILITIES_H
