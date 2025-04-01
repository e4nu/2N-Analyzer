//
// Created by Alon Sportes on 01/04/2025.
//

#ifndef ACCEPTANCEMAPSSETTINGS_H
#define ACCEPTANCEMAPSSETTINGS_H

#include <iostream>

struct MomentumResolutionSettings {
    bool plot_and_fit_MomRes;
    bool Calculate_momResS2;
    const double DeltaSlices;
    const bool VaryingDelta;
    bool ForceSmallpResLimits;
    const std::string SmearMode;
    const std::string CorrMode;
    bool Run_with_momResS2;
    bool momRes_test;

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
        if (plot_and_fit_MomRes && (Calculate_momResS2 || Run_with_momResS2)) {
            ForceSmallpResLimits = false;
        }
    }
};

#endif  // ACCEPTANCEMAPSSETTINGS_H
