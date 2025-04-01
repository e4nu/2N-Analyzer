//
// Created by Alon Sportes on 01/04/2025.
//

#ifndef EVENTSELECTIONSETTINGS_H
#define EVENTSELECTIONSETTINGS_H

#include <iostream>

struct EventSelectionSettings {
    const bool calculate_1p;
    const bool calculate_1n;
    const bool calculate_2p;
    const bool calculate_pFDpCD;
    const bool calculate_nFDpCD;

    bool calculate_truth_level;
    bool TL_plots_only_for_NC;
    bool fill_TL_plots;
    bool ZoomIn_On_mom_th_plots;
    bool Eff_calc_with_one_reco_electron;
    bool Calc_inc_eff_with_varying_theta;
    bool Calc_1n_n_eff_with_smaller_theta;
    bool Calc_eff_overlapping_FC;
    bool Rec_wTL_ES;

    const bool limless_mom_eff_plots;

    const bool Enable_FD_photons;
    const bool Enable_FD_neutrons;
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
          Rec_wTL_ES(true),

          limless_mom_eff_plots(false),

          Enable_FD_photons(false),
          Enable_FD_neutrons(true),
          Count_FD_neurton_and_photon_hits(true),
          ES_by_leading_FDneutron(true) {}
};

#endif  // EVENTSELECTIONSETTINGS_H
