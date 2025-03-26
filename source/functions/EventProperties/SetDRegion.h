//
// Created by alons on 05/06/2023.
//

#ifndef SETDREGION_H
#define SETDREGION_H

#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <typeinfo>

#include "../../namespaces/utilities/utilities.h"/
// #include "../GeneralFunctions.h"
// #include "GetParticleName.h"
// #include "GetParticleNameLC.h"
// #include "GetParticleNameShort.h"
// // #include "../findSubstring.h"

using namespace std;
using namespace utilities;

string SetDRegion(const std::string &RecTitle, const std::string &Particle, const std::string &ParticleLC) {
    std::string DRegion;

    if (findSubstring(RecTitle, ", FD)") || findSubstring(RecTitle, "FD " + Particle) || findSubstring(RecTitle, "FD " + ParticleLC)) {
        DRegion = "FD";
    } else if (findSubstring(RecTitle, ", CD)") || findSubstring(RecTitle, "CD " + Particle) || findSubstring(RecTitle, "CD " + ParticleLC)) {
        DRegion = "CD";
    }

    return DRegion;
}

#endif  // SETDREGION_H
