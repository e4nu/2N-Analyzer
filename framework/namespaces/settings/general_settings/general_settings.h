//
// Created by Alon Sportes on 14/03/2025.
//

#ifndef GENERAL_SETTINGS_H
#define GENERAL_SETTINGS_H

#include <map>

namespace general_settings {
std::map<std::string, bool> CodeSettings = {
    {"isMC", false},
    {"keep_good", true},
};

};  // namespace settings

#endif  // GENERAL_SETTINGS_H
