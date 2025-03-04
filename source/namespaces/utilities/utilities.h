//
// Created by Alon Sportes on 24/02/2025.
//

#ifndef UTILITIES_H
#define UTILITIES_H

#include <TH1.h>

#include <iostream>

using namespace std;

namespace utilities {

// BoolToChar function --------------------------------------------------------------------------------------------------------------------------------------------------

/* Usage: convert bool variables to char */
inline const char *const BoolToChar(bool b) { return b ? "true" : "false"; }

// BoolToString function ------------------------------------------------------------------------------------------------------------------------------------------------

/* Usage: convert bool variables to string */
string BoolToString(bool b) { return b ? "true" : "false"; }

// ToStringWithPrecision function ---------------------------------------------------------------------------------------------------------------------------------------

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

// replaceSubstring function --------------------------------------------------------------------------------------------------------------------------------------------

// Function to replace one substring with another
std::string replaceSubstring(const std::string &input, const std::string &toReplace, const std::string &replaceWith) {
    size_t pos = input.find(toReplace);

    if (pos == std::string::npos) {
        // If 'toReplace' is not found, return the original string
        return input;
    }
    return input.substr(0, pos) + replaceWith + input.substr(pos + toReplace.length());
}

// RadCalc function -----------------------------------------------------------------------------------------------------------------------------------------------------

double RadCalc(double x, double y, double z) {
    double r = sqrt(x * x + y * y + z * z);
    return r;
}

// GetHistogramFromVector function --------------------------------------------------------------------------------------------------------------------------------------

TH1 *GetHistogramFromVector(const std::vector<TH1 *> &HistoList, const std::string &searchString, const std::string &searchStringOption = "name") {
    for (const auto &hist : HistoList) {
        if (hist) {
            if (FindSubstring(searchStringOption, "Name") || FindSubstring(searchStringOption, "name")) {
                if (hist->GetName() == searchString) { return hist; }
            } else if (FindSubstring(searchStringOption, "Title") || FindSubstring(searchStringOption, "title")) {
                if (hist->GetTitle() == searchString) { return hist; }
            }
        }
    }

    return nullptr;  // Return nullptr if no match is found
}

// GetHistogramEntries function -----------------------------------------------------------------------------------------------------------------------------------------

double GetHistogramEntriesFromVector(const std::vector<TH1 *> &HistoList, const std::string &searchString, const std::string &searchStringOption = "name") {
    TH1 *Histogram = GetHistogramFromVector(HistoList, searchString, searchStringOption);

    // Return the number of entries
    if (Histogram) { return Histogram->GetEntries(); }

    return -1;  // Return -1 if no match is found
}

}  // namespace utilities

#endif  // UTILITIES_H
