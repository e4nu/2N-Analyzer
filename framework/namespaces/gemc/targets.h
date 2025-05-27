//
// Created by Alon Sportes on 11/04/2025.
//

#ifndef TARGETS_H
#define TARGETS_H

#include <TRandom3.h>
#include <TVector3.h>

#include <iostream>

#include "../general_utilities/constants.h"

namespace targets {
// Target parameters
double mass_e = 0.511e-3;
double mass_p = 0.938272;
double mass_n = 0.93957;
double mass_pi = 0.13957;
double mass_pi0 = 0.134977;

int rand_seed = 12345;
TRandom3 ran(rand_seed);

double beamspot_x = 0.;
double beamspot_y = 0.;

double beamspread_x = 0.04;
double beamspread_y = 0.04;

// Targets: liquid, 4-foil, 1-foil, Ar, Ca
double global_z = -3;  // center of hallB in GEMC in cm

std::map<std::string, std::vector<double> > targets{{"4-foil", {-1.875 + global_z, -0.625 + global_z, 0.625 + global_z, 1.875 + global_z}},
                                                    {"1-foil", {global_z + 2.5}},
                                                    {"1-foil-small", {global_z + 2.5}},
                                                    {"1-foil-large", {global_z + 2.5}},
                                                    {"Ar", {global_z - 2.5}},
                                                    {"Ca", {global_z}},
                                                    {"liquid", {global_z - 2.5, global_z + 2.5}}};

TVector3 randomVertex(string target) {
    double x = -999, y = -999, z = -999;

    if (targets.find(target) != targets.end()) {
        if (target == "liquid") {
            x = ran.Gaus(beamspot_x, beamspread_x);
            y = ran.Gaus(beamspot_y, beamspread_y);
            z = ran.Uniform(targets[target].at(0), targets[target].at(1));
            return (TVector3(x, y, z));
        } else {
            if (target == "1-foil-large") {
                // large foil targets
                x = ran.Gaus(0.06, 0.06);
                y = ran.Gaus(0.06, 0.06);
                z = targets[target].at(ran.Integer(targets[target].size()));
                std::cout << "\n\n1-foil-large target is implemented. Exiting.\n\n" << std::endl;
                exit(1);
                return (TVector3(x, y, z));
            } else {
                // any other foil targets
                x = ran.Gaus(beamspot_x, beamspread_x);
                y = ran.Gaus(beamspot_y, beamspread_y);
                z = targets[target].at(ran.Integer(targets[target].size()));
                std::cout << "\n\n1-foil-small target is implemented. Exiting.\n\n" << std::endl;
                exit(1);
                return (TVector3(x, y, z));
            }
        }
    }

    cout << "Error in target initialization. Target not found in map. Vertex will be set to -999,-999,-999 " << endl;

    return (TVector3(-999, -999, -999));
}

TString addParticle(int part_idx, int active, int pid, TVector3 momentum, double mass, TVector3 vtx) {
    // LUND info for each particle in the event
    TString formatstring = "%i \t %.3f \t %i \t %i \t %i \t %i \t %.5f \t %.5f \t %.5f \t %.5f \t %.5f \t %.5f \t %.5f \t %.5f \n";
    double energy = sqrt(momentum.Mag2() + mass * mass);
    TString outstring = Form(formatstring, part_idx, 0.0, active, pid, 0, 0, momentum.Px(), momentum.Py(), momentum.Pz(), energy, mass, vtx.X(), vtx.Y(), vtx.Z());

    return outstring;
}

};  // namespace targets

#endif  // TARGETS_H
