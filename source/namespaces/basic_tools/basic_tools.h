//
// Created by Alon Sportes on 23/03/2025.
//

#ifndef BASIC_TOOLS_H
#define BASIC_TOOLS_H

#include <iostream>
#include <string>

using namespace std;

namespace basic_tools {
// GetCurrentDirectory function ------------------------------------------------------------------------------------------------------------------------------------------

string GetCurrentDirectory() {
    char pwd[PATH_MAX];
    getcwd(pwd, sizeof(pwd));

    string WorkingDirectory = pwd;

    return WorkingDirectory;
}

// BoolToChar function --------------------------------------------------------------------------------------------------------------------------------------------------

/* Usage: convert bool variables to char */
inline const char *const BoolToChar(bool b) { return b ? "true" : "false"; }

// BoolToString function ------------------------------------------------------------------------------------------------------------------------------------------------

/* Usage: convert bool variables to string */
string BoolToString(bool b) { return b ? "true" : "false"; }

// ToStringWithPrecision function ---------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
string ToStringWithPrecision(const T a_value, const int n = 2) {
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return out.str();
}

// FindSubstring function -----------------------------------------------------------------------------------------------------------------------------------------------

inline bool FindSubstring(const string &string1, const string &string2) { return string1.find(string2) != string::npos; }

}  // namespace basic_tools

#endif  // BASIC_TOOLS_H
