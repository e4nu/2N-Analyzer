//
// Created by Alon Sportes on 01/04/2025.
//

#ifndef MOMENTUMRESOLUTIONSETTINGS_H
#define MOMENTUMRESOLUTIONSETTINGS_H

#include <iostream>

// Include YAML loader:
#include "../io/Yaml_loader.h"

/**
 * @struct MomentumResolutionSettings
 * @brief Settings related to momentum resolution, including options for plotting and fitting momentum resolution, calculating momentum resolution variables, and settings for smearing and
 * correcting momentum.
 * @details These settings are used in the main code to control various aspects of momentum resolution analysis, such as whether to generate momentum resolution plots, whether to calculate
 * specific momentum resolution variables (momResS2), the settings for smearing and correcting momentum, and whether to run with momResS2. The UpdateMomResSettings function is used to update the
 * settings based on other settings, such as whether to force small momentum resolution limits based on the combination of plot_and_fit_MomRes, Calculate_momResS2, and Run_with_momResS2.
 * @note The settings for momentum resolution can have a significant impact on the analysis, so it is important to set these settings appropriately based on the goals of the analysis and the
 * characteristics of the data being analyzed. For example, enabling the calculation of momResS2 variables can provide additional insights into the momentum resolution, but may also increase
 * the complexity of the analysis, so it is important to consider these trade-offs when setting these options. Additionally, the settings for smearing and correcting momentum can affect the
 * results of the analysis, so it is important to choose appropriate smearing and correction modes based on the characteristics of the data and the goals of the analysis.
 */
struct MomentumResolutionSettings {
    // TODO: align neutron and proton momRes calculations!
    bool plot_and_fit_MomRes;  // Generate nRes plots
    bool VaryingDelta;         // 1st momResS1 w/ VaryingDelta = false
    bool Calculate_momResS2;   // Calculate momResS2 variables
    bool Run_with_momResS2;    // Smear w/ momResS2 & correct w/ momResS1
    double DeltaSlices;
    bool ForceSmallpResLimits;  // 1st momResS1 w/ VaryingDelta = false
    bool FitAggregatedHistograms;
    double AggregationEventTh;
    std::string SmearMode;
    std::string CorrMode;
    bool momRes_test;  // false by default
    /*
    MomRes run order guide:
        0. momResS0:                plot_and_fit_MomRes = false  ,  VaryingDelta = false  ,  Calculate_momResS2 = false  ,  Run_with_momResS2 = false

        1. momResS1 calculation 1:  plot_and_fit_MomRes = true   ,  VaryingDelta = false  ,  Calculate_momResS2 = false  ,  Run_with_momResS2 = false
        3. momResS2 calculation 2:  plot_and_fit_MomRes = true   ,  VaryingDelta = true   ,  Calculate_momResS2 = true   ,  Run_with_momResS2 = false
        4. momResS2 run:            plot_and_fit_MomRes = true   ,  VaryingDelta = true   ,  Calculate_momResS2 = false  ,  Run_with_momResS2 = true
    */

    MomentumResolutionSettings()
        // momResS0:
        : plot_and_fit_MomRes(false),
          VaryingDelta(false),
          Calculate_momResS2(false),
          Run_with_momResS2(false),
          DeltaSlices(0.05),
          ForceSmallpResLimits(true),
          FitAggregatedHistograms(false),
          AggregationEventTh(1000.0),
          SmearMode("pol1_wKC"),
          CorrMode("pol1_wKC"),
          momRes_test(false) {}

    void ApplyLimiterScaling(const bool applyLimiter, const long long limiter) {
        if (applyLimiter) { AggregationEventTh *= (static_cast<double>(limiter) / 25000000.0); }
    }

    void UpdateMomResSettings() {
        if (plot_and_fit_MomRes && (Calculate_momResS2 || Run_with_momResS2)) { ForceSmallpResLimits = false; }
    }

    void UpdateMomResSettings(const bool applyLimiter, const long long limiter) {
        UpdateMomResSettings();
        ApplyLimiterScaling(applyLimiter, limiter);
    }
};

// --------------------------------------------------------------------------------------
// YAML adapter specialization for MomentumResolutionSettings
// --------------------------------------------------------------------------------------

template <>
struct Yaml_loader::SettingsAdapter<MomentumResolutionSettings> {
    static void Apply(const YAML::Node& node, MomentumResolutionSettings& s) {
        using namespace Yaml_loader;

        TryReadKey(node, "plot_and_fit_MomRes", s.plot_and_fit_MomRes);
        TryReadKey(node, "VaryingDelta", s.VaryingDelta);
        TryReadKey(node, "Calculate_momResS2", s.Calculate_momResS2);
        TryReadKey(node, "Run_with_momResS2", s.Run_with_momResS2);
        TryReadKey(node, "DeltaSlices", s.DeltaSlices);
        TryReadKey(node, "ForceSmallpResLimits", s.ForceSmallpResLimits);
        TryReadKey(node, "FitAggregatedHistograms", s.FitAggregatedHistograms);
        TryReadKey(node, "AggregationEventTh", s.AggregationEventTh);
        TryReadKey(node, "SmearMode", s.SmearMode);
        TryReadKey(node, "CorrMode", s.CorrMode);
        TryReadKey(node, "momRes_test", s.momRes_test);

        // Recompute dependent settings after YAML overrides.
        s.UpdateMomResSettings();
    }

    static void Validate(const MomentumResolutionSettings& s) {
        if (s.DeltaSlices <= 0.0) { throw std::runtime_error("MomentumResolutionSettings error: DeltaSlices must be positive."); }

        if (s.AggregationEventTh <= 0.0) { throw std::runtime_error("MomentumResolutionSettings error: AggregationEventTh must be positive."); }

        if (s.SmearMode.empty()) { throw std::runtime_error("MomentumResolutionSettings error: SmearMode cannot be empty."); }

        if (s.CorrMode.empty()) { throw std::runtime_error("MomentumResolutionSettings error: CorrMode cannot be empty."); }

        if (s.Calculate_momResS2 && s.Run_with_momResS2) { throw std::runtime_error("MomentumResolutionSettings error: Calculate_momResS2 and Run_with_momResS2 cannot both be true."); }
    }
};

#endif  // MOMENTUMRESOLUTIONSETTINGS_H
