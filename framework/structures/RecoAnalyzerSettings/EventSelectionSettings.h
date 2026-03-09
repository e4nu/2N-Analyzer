//
// Created by Alon Sportes on 01/04/2025.
//

#ifndef EVENTSELECTIONSETTINGS_H
#define EVENTSELECTIONSETTINGS_H

#include <iostream>

/**
 * @struct EventSelectionSettings
 * @brief Settings related to event selection, including which final states to analyze, whether to calculate truth level plots, and settings related to FD neutrals.
 * @details These settings are used in the main code to control the event selection process, including which final states to analyze (1p, 1n, 2p, pFDpCD, nFDpCD), whether to calculate truth
 * level plots, and settings related to FD neutrals (whether to enable FD photons and neutrons, and whether to count FD neutron and photon hits). The RefreshSettings function is used to
 * update the settings based on other settings, such as whether to enable FD neutrons based on the ES_by_leading_FDneutron setting.
 * @note The settings related to calculating truth level plots and FD neutrals can have a significant impact on the analysis, so it is important to set these settings appropriately based on
 * the goals of the analysis and the characteristics of the data being analyzed. For example, enabling FD neutrons can increase the efficiency of the analysis, but may also increase the
 * background from RES and DIS events, so it is important to consider these trade-offs when setting these options.
 */
struct EventSelectionSettings {
    // Settings to enable/disable specific FS plot calculations (Rec only):

    // Final states to analyze (1N & 2N):
    const bool calculate_1p;
    const bool calculate_1n;
    const bool calculate_2p;
    const bool calculate_pFDpCD;
    const bool calculate_nFDpCD;

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

    const bool limless_mom_eff_plots;

    /* FD neutrals settings */
    const bool Enable_FD_photons;  // keep as false to decrease RES & DIS
    bool Enable_FD_neutrons;       // keep as false to increase eff. plots
    const bool Count_FD_neurton_and_photon_hits;

    const bool ES_by_leading_FDneutron;

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

    void RefreshSettings() {
        if (!ES_by_leading_FDneutron) { Enable_FD_neutrons = false; }
    }
};

#endif  // EVENTSELECTIONSETTINGS_H
