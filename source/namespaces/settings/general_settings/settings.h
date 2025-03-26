//
// Created by Alon Sportes on 14/03/2025.
//

#ifndef SETTINGS_H
#define SETTINGS_H

#include <map>

namespace settings {
std::map<std::string, bool> CodeSettings = {
    {"isMC", false},
    {"keep_good", true},
};

}  // namespace settings

#endif  // SETTINGS_H
