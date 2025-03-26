//
// Created by alons on 09/05/2023.
//

#ifndef GETPARTICLENAMELC_H
#define GETPARTICLENAMELC_H

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

string GetParticleNameLC(string Source, bool ForDir = false) {
    std::string ParticleName;

    if (findSubstring(Source, "neutrals") || findSubstring(Source, "Neutrals") || findSubstring(Source, "neut.") || findSubstring(Source, "Neut.")) {
        ParticleName = "neut";
    } else if (findSubstring(Source, "Electron") || findSubstring(Source, "electron")) {
        ParticleName = "electron";
    } else if (findSubstring(Source, "Proton") || findSubstring(Source, "proton")) {
        ParticleName = "proton";
    } else if (findSubstring(Source, "Neutron") || findSubstring(Source, "neutron")) {
        if (!(findSubstring(Source, "Leading") || findSubstring(Source, "leading"))) {
            ParticleName = "neutron";
        } else {
            if (ForDir) {
                ParticleName = "leading_neutron";
            } else {
                ParticleName = "leading neutron";
            }
        }
    } else if (findSubstring(Source, "#pi^{+}")) {
        ParticleName = "piplus";
    } else if (findSubstring(Source, "#pi^{-}")) {
        ParticleName = "piminus";
    } else if (findSubstring(Source, "#K^{+}")) {
        ParticleName = "kplus";
    } else if (findSubstring(Source, "#K^{-}")) {
        ParticleName = "kminus";
    } else if (findSubstring(Source, "#gamma") || findSubstring(Source, "photon") || findSubstring(Source, "Photon")) {
        ParticleName = "photon";
    } else {
        ParticleName = "unknown";
    }

    return ParticleName;
}

#endif  // GETPARTICLENAMELC_H
