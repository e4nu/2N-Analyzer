//
// Created by alons on 09/05/2023.
//

#ifndef GETPARTICLENAME_H
#define GETPARTICLENAME_H

#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <typeinfo>

#include "../../namespaces/general_utilities/utilities/utilities.h"
// #include "../GeneralFunctions.h"
// #include "GetParticleName.h"
// #include "GetParticleNameLC.h"
// #include "GetParticleNameShort.h"
// // #include "../findSubstring.h"

using namespace std;
using namespace utilities;

string GetParticleName(string Source, bool ForDir = false) {
    std::string ParticleName;

    if (findSubstring(Source, "neutrals") || findSubstring(Source, "Neutrals") || findSubstring(Source, "neut.") || findSubstring(Source, "Neut.")) {
        ParticleName = "neut";
    } else if (findSubstring(Source, "Electron") || findSubstring(Source, "electron")) {
        ParticleName = "Electron";
    } else if (findSubstring(Source, "Proton") || findSubstring(Source, "proton")) {
        ParticleName = "Proton";
    } else if (findSubstring(Source, "Neutron") || findSubstring(Source, "neutron")) {
        if (!(findSubstring(Source, "Leading") || findSubstring(Source, "leading"))) {
            ParticleName = "Neutron";
        } else {
            if (ForDir) {
                ParticleName = "Leading_neutron";
            } else {
                ParticleName = "Leading neutron";
            }
        }
    } else if (findSubstring(Source, "#pi^{+}")) {
        ParticleName = "Piplus";
    } else if (findSubstring(Source, "#pi^{-}")) {
        ParticleName = "Piminus";
    } else if (findSubstring(Source, "#K^{+}")) {
        ParticleName = "Kplus";
    } else if (findSubstring(Source, "#K^{-}")) {
        ParticleName = "Kminus";
    } else if (findSubstring(Source, "#gamma") || findSubstring(Source, "photon") || findSubstring(Source, "Photon")) {
        ParticleName = "Photon";
    } else {
        ParticleName = "Unknown";
    }

    return ParticleName;
}

////<editor-fold desc="Before LnFD addition">
// std::string GetParticleName(string Source) {
//     std::string ParticleName;
//
//     if (findSubstring(Source, "neutrals") || findSubstring(Source, "Neutrals")
//         || findSubstring(Source, "neut.") || findSubstring(Source, "Neut.")) {
//         ParticleName = "neut";
//     } else if (findSubstring(Source, "Electron") || findSubstring(Source, "electron")) {
//         ParticleName = "Electron";
//     } else if (findSubstring(Source, "Proton") || findSubstring(Source, "proton")) {
//         ParticleName = "Proton";
//     } else if (findSubstring(Source, "Neutron") || findSubstring(Source, "neutron")) {
//         ParticleName = "Neutron";
//     } else if (findSubstring(Source, "#pi^{+}")) {
//         ParticleName = "Piplus";
//     } else if (findSubstring(Source, "#pi^{-}")) {
//         ParticleName = "Piminus";
//     } else if (findSubstring(Source, "#K^{+}")) {
//         ParticleName = "Kplus";
//     } else if (findSubstring(Source, "#K^{-}")) {
//         ParticleName = "Kminus";
//     } else if (findSubstring(Source, "#gamma") || findSubstring(Source, "photon")
//                || findSubstring(Source, "Photon")) {
//         ParticleName = "Photon";
//     } else {
//         ParticleName = "Unknown";
//     }
//
//     return ParticleName;
// }
////</editor-fold>

#endif  // GETPARTICLENAME_H
