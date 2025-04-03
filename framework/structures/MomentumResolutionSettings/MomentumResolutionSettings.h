//
// Created by Alon Sportes on 01/04/2025.
//

#ifndef ACCEPTANCEMAPSSETTINGS_H
#define ACCEPTANCEMAPSSETTINGS_H

#include <iostream>

struct MomentumResolutionSettings {
    // TODO: align neutron and proton momRes calculations!
    bool plot_and_fit_MomRes;  // Generate nRes plots
    bool Calculate_momResS2;   // Calculate momResS2 variables
    const double DeltaSlices;
    const bool VaryingDelta;    // 1st momResS1 w/ VaryingDelta = false
    bool ForceSmallpResLimits;  // 1st momResS1 w/ VaryingDelta = false
    const std::string SmearMode;
    const std::string CorrMode;
    bool Run_with_momResS2;  // Smear w/ momResS2 & correct w/ momResS1
    bool momRes_test;        // false by default
    /*
    MomRes run order guide:
    1. momResS1 calculation 1:
                           1a:  VaryingDelta = false  , plot_and_fit_MomRes = true  , Calculate_momResS2 = false , Run_with_momResS2 = false
                           1b:  VaryingDelta = true , plot_and_fit_MomRes = true  , Calculate_momResS2 = false , Run_with_momResS2 = false
    3. momResS2 calculation:    VaryingDelta = true , plot_and_fit_MomRes = true  , Calculate_momResS2 = true  , Run_with_momResS2 = false
    4. momResS2 run:            VaryingDelta = true , plot_and_fit_MomRes = false , Calculate_momResS2 = false , Run_with_momResS2 = true
    */

    MomentumResolutionSettings()
        : plot_and_fit_MomRes(false),
          Calculate_momResS2(false),
          DeltaSlices(0.05),
          VaryingDelta(true),
          ForceSmallpResLimits(true),
          SmearMode("pol1_wKC"),
          CorrMode("pol1_wKC"),
          Run_with_momResS2(true),
          momRes_test(false) {}

    void RefreshSettings() {
        if (plot_and_fit_MomRes && (Calculate_momResS2 || Run_with_momResS2)) { ForceSmallpResLimits = false; }
    }
};

#endif  // ACCEPTANCEMAPSSETTINGS_H
