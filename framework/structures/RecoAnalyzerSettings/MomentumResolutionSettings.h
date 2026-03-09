//
// Created by Alon Sportes on 01/04/2025.
//

#ifndef MOMENTUMRESOLUTIONSETTINGS_H
#define MOMENTUMRESOLUTIONSETTINGS_H

#include <iostream>

/**
 * @struct MomentumResolutionSettings
 * @brief Settings related to momentum resolution, including options for plotting and fitting momentum resolution, calculating momentum resolution variables, and settings for smearing and
 * correcting momentum.
 * @details These settings are used in the main code to control various aspects of momentum resolution analysis, such as whether to generate momentum resolution plots, whether to calculate
 * specific momentum resolution variables (momResS2), the settings for smearing and correcting momentum, and whether to run with momResS2. The RefreshSettings function is used to update the
 * settings based on other settings, such as whether to force small momentum resolution limits based on the combination of plot_and_fit_MomRes, Calculate_momResS2, and Run_with_momResS2.
 * @note The settings for momentum resolution can have a significant impact on the analysis, so it is important to set these settings appropriately based on the goals of the analysis and the
 * characteristics of the data being analyzed. For example, enabling the calculation of momResS2 variables can provide additional insights into the momentum resolution, but may also increase
 * the complexity of the analysis, so it is important to consider these trade-offs when setting these options. Additionally, the settings for smearing and correcting momentum can affect the
 * results of the analysis, so it is important to choose appropriate smearing and correction modes based on the characteristics of the data and the goals of the analysis.
 */
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
                           1b:  VaryingDelta = true   , plot_and_fit_MomRes = true  , Calculate_momResS2 = false , Run_with_momResS2 = false
    3. momResS2 calculation:    VaryingDelta = true   , plot_and_fit_MomRes = true  , Calculate_momResS2 = true  , Run_with_momResS2 = false
    4. momResS2 run:            VaryingDelta = true   , plot_and_fit_MomRes = false , Calculate_momResS2 = false , Run_with_momResS2 = true
    */

    MomentumResolutionSettings()
        // momResS0:
        : plot_and_fit_MomRes(false),
          Calculate_momResS2(false),
          DeltaSlices(0.05),
          VaryingDelta(true),
          ForceSmallpResLimits(true),
          SmearMode("pol1_wKC"),
          CorrMode("pol1_wKC"),
          Run_with_momResS2(true),
          momRes_test(false) {}

    // // momResS1a:
    // : plot_and_fit_MomRes(true),
    //   Calculate_momResS2(false),
    //   DeltaSlices(0.05),
    //   VaryingDelta(false),
    //   ForceSmallpResLimits(true),
    //   SmearMode("pol1_wKC"),
    //   CorrMode("pol1_wKC"),
    //   Run_with_momResS2(false),
    //   momRes_test(false) {}

    // // momResS1:
    // : plot_and_fit_MomRes(false),
    //   Calculate_momResS2(false),
    //   DeltaSlices(0.05),
    //   VaryingDelta(true),
    //   ForceSmallpResLimits(true),
    //   SmearMode("pol1_wKC"),
    //   CorrMode("pol1_wKC"),
    //   Run_with_momResS2(true),
    //   momRes_test(false) {}

    // : plot_and_fit_MomRes(false),
    //   Calculate_momResS2(false),
    //   DeltaSlices(0.05),
    //   VaryingDelta(true),
    //   ForceSmallpResLimits(true),
    //   SmearMode("pol1_wKC"),
    //   CorrMode("pol1_wKC"),
    //   Run_with_momResS2(true),
    //   momRes_test(false) {}

    void RefreshSettings() {
        if (plot_and_fit_MomRes && (Calculate_momResS2 || Run_with_momResS2)) { ForceSmallpResLimits = false; }
    }
};

#endif  // MOMENTUMRESOLUTIONSETTINGS_H
