//
// Created by Alon Sportes on 01/04/2025.
//

#ifndef EVENTSELECTIONSETTINGS_H
#define EVENTSELECTIONSETTINGS_H

#include <iostream>

// Include YAML loader:
#include "../io/Yaml_loader.h"

/**
 * @struct EventSelectionSettings
 * @brief Settings related to event selection, including which final states to analyze, whether to calculate truth level plots, and settings related to FD neutrals.
 * @details These settings are used in the main code to control the event selection process, including which final states to analyze (1p, 1n, 2p, pFDpCD, nFDpCD), whether to calculate truth
 * level plots, and settings related to FD neutrals (whether to enable FD photons and neutrons, and whether to count FD neutron and photon hits). The UpdateEventSelectionSettings function is used to
 * update the settings based on other settings, such as whether to enable FD neutrons based on the ES_by_leading_FDneutron setting.
 * @note The settings related to calculating truth level plots and FD neutrals can have a significant impact on the analysis, so it is important to set these settings appropriately based on
 * the goals of the analysis and the characteristics of the data being analyzed. For example, enabling FD neutrons can increase the efficiency of the analysis, but may also increase the
 * background from RES and DIS events, so it is important to consider these trade-offs when setting these options.
 */
struct EventSelectionSettings {
    // Settings to enable/disable specific FS plot calculations (Rec only):

    // Final states to analyze (1N & 2N):
    bool calculate_1p;
    bool calculate_1n;
    bool calculate_2p;
    bool calculate_pFDpCD;
    bool calculate_nFDpCD;

    // Truth level calculation setup:
    bool calculate_truth_level;  // TL master ON/OFF switch
    bool TL_plots_only_for_NC;   // TL plots only AFTER beta fit
    bool fill_TL_plots;
    bool ZoomIn_On_mom_th_plots;           // momentum th. efficiencies with zoomin
    bool Eff_calc_with_one_reco_electron;  // keep as true in normal runs
    bool Calc_inc_eff_with_varying_theta;
    bool Calc_1n_n_eff_with_smaller_theta;
    bool Calc_eff_overlapping_FC;  // keep as true in normal runs
    bool Rec_wTL_ES;               // Calculate efficiency - force TL event selection on reco. plots

    bool limless_mom_eff_plots;

    /* FD neutrals settings */
    bool Enable_FD_photons;   // keep as false to decrease RES & DIS
    bool Enable_FD_neutrons;  // keep as false to increase eff. plots
    bool Count_FD_neurton_and_photon_hits;

    bool ES_by_leading_FDneutron;

    EventSelectionSettings()
        : calculate_1p(true),
          calculate_1n(true),
          calculate_2p(true),
          calculate_pFDpCD(true),
          calculate_nFDpCD(true),

          calculate_truth_level(true),
          TL_plots_only_for_NC(false),
          fill_TL_plots(true),
          ZoomIn_On_mom_th_plots(false),
          Eff_calc_with_one_reco_electron(true),
          Calc_inc_eff_with_varying_theta(false),
          Calc_1n_n_eff_with_smaller_theta(false),
          Calc_eff_overlapping_FC(true),
          Rec_wTL_ES(false),

          limless_mom_eff_plots(false),

          Enable_FD_photons(false),
          Enable_FD_neutrons(true),
          Count_FD_neurton_and_photon_hits(true),

          ES_by_leading_FDneutron(false) {}

    void UpdateEventSelectionSettings() {
        if (!ES_by_leading_FDneutron) { Enable_FD_neutrons = false; }
    }
};

// --------------------------------------------------------------------------------------
// YAML adapter specialization for EventSelectionSettings
// --------------------------------------------------------------------------------------

template <>
struct Yaml_loader::SettingsAdapter<EventSelectionSettings> {
    static void Apply(const YAML::Node& node, EventSelectionSettings& s) {
        using namespace Yaml_loader;

        // Final states to analyze
        TryReadKey(node, "calculate_1p", s.calculate_1p);
        TryReadKey(node, "calculate_1n", s.calculate_1n);
        TryReadKey(node, "calculate_2p", s.calculate_2p);
        TryReadKey(node, "calculate_pFDpCD", s.calculate_pFDpCD);
        TryReadKey(node, "calculate_nFDpCD", s.calculate_nFDpCD);

        // Truth-level calculation setup
        TryReadKey(node, "calculate_truth_level", s.calculate_truth_level);
        TryReadKey(node, "TL_plots_only_for_NC", s.TL_plots_only_for_NC);
        TryReadKey(node, "fill_TL_plots", s.fill_TL_plots);
        TryReadKey(node, "ZoomIn_On_mom_th_plots", s.ZoomIn_On_mom_th_plots);
        TryReadKey(node, "Eff_calc_with_one_reco_electron", s.Eff_calc_with_one_reco_electron);
        TryReadKey(node, "Calc_inc_eff_with_varying_theta", s.Calc_inc_eff_with_varying_theta);
        TryReadKey(node, "Calc_1n_n_eff_with_smaller_theta", s.Calc_1n_n_eff_with_smaller_theta);
        TryReadKey(node, "Calc_eff_overlapping_FC", s.Calc_eff_overlapping_FC);
        TryReadKey(node, "Rec_wTL_ES", s.Rec_wTL_ES);
        TryReadKey(node, "limless_mom_eff_plots", s.limless_mom_eff_plots);

        // FD neutrals settings
        TryReadKey(node, "Enable_FD_photons", s.Enable_FD_photons);
        TryReadKey(node, "Enable_FD_neutrons", s.Enable_FD_neutrons);
        TryReadKey(node, "Count_FD_neurton_and_photon_hits", s.Count_FD_neurton_and_photon_hits);
        TryReadKey(node, "ES_by_leading_FDneutron", s.ES_by_leading_FDneutron);

        // Recompute dependent settings after YAML overrides
        s.UpdateEventSelectionSettings();
    }

    static void Validate(const EventSelectionSettings& s) {
        if (!s.calculate_1p && !s.calculate_1n && !s.calculate_2p && !s.calculate_pFDpCD && !s.calculate_nFDpCD) {
            throw std::runtime_error("EventSelectionSettings error: at least one final state must be enabled.");
        }
    }
};

#endif  // EVENTSELECTIONSETTINGS_H
