//
// Created by alons on 05/06/2023.
//

#ifndef SETTITLE_H
#define SETTITLE_H

#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <typeinfo>

#include "GetParticleNameShort_rm.h"
#include "GetParticleName_rm.h"
#include "SetType.h"

using namespace std;

string SetTitle(const std::string &RecTitle, const std::string &PlotsT, const std::string &DRegion) {
    std::string Title;

    std::string Particle = GetParticleName(RecTitle);
    std::string ParticleShort = GetParticleNameShort(RecTitle);
    std::string Type = SetType(RecTitle);

    if (PlotsT == "FSRatio") {
        if (Particle == "Electron") {
            if (Type == "momentum") {
                Title = Particle + " " + Type + " ratio";
            } else {
                Title = Particle + " #" + Type + " ratio";
            }
        } else {
            if (DRegion == "FD") {
                if (Type == "momentum") {
                    Title = DRegion + " nucleon " + Type + " ratio";
                } else {
                    Title = DRegion + " nucleon #" + Type + " ratio";
                }
            } else if (DRegion == "CD") {
                if (Type == "momentum") {
                    Title = DRegion + " proton " + Type + " ratio";
                } else {
                    Title = DRegion + " proton #" + Type + " ratio";
                }
            }
        }
    } else {
        if (Type == "momentum") {
            Title = Particle + " " + Type;
        } else {
            Title = "#" + Type + "_{" + ParticleShort + "}";
        }
    }

    return Title;
}

#endif  // SETTITLE_H
