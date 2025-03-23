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

// GetBeamEnergyFromString function -------------------------------------------------------------------------------------------------------------------------------------

double GetBeamEnergyFromString(const string &sn) {
    double be = 0.;

    if (FindSubstring(sn, "598636MeV") || FindSubstring(sn, "598636mev") || FindSubstring(sn, "598636") || FindSubstring(sn, "5986MeV") || FindSubstring(sn, "5986mev") ||
        FindSubstring(sn, "5986") || FindSubstring(sn, "6GeV") || FindSubstring(sn, "6Gev") || FindSubstring(sn, "6gev")) {
        be = 5.98636;
    } else if (FindSubstring(sn, "402962MeV") || FindSubstring(sn, "402962mev") || FindSubstring(sn, "402962") || FindSubstring(sn, "4029MeV") || FindSubstring(sn, "4029mev") ||
               FindSubstring(sn, "4029") || FindSubstring(sn, "4GeV") || FindSubstring(sn, "4Gev") || FindSubstring(sn, "4gev")) {
        be = 4.02962;
    } else if (FindSubstring(sn, "207052MeV") || FindSubstring(sn, "207052mev") || FindSubstring(sn, "207052") || FindSubstring(sn, "2070MeV") || FindSubstring(sn, "2070mev") ||
               FindSubstring(sn, "2070") || FindSubstring(sn, "2GeV") || FindSubstring(sn, "2Gev") || FindSubstring(sn, "2gev")) {
        be = 2.07052;
    }

    return be;
}

}  // namespace basic_tools

#endif  // BASIC_TOOLS_H
