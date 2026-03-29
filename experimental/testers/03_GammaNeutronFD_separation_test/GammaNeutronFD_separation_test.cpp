// source gammaNeutronFD_separation_test_run.sh

#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

#include "Ph_nFD_sep_body_function.cpp"

namespace bt = basic_tools;

void GammaNeutronFD_separation_test() {
    auto start = std::chrono::system_clock::now();  // Start counting running time

    std::cout << "\033[33m\n\nInitiating GammaNeutronFD_separation_test.cpp\n\033[0m" << std::flush;

    std::cout << "\033[36m Warning!\033[31m recheck calculation in Calc_nFD_vars!\033[0m" << endl;

    // - General variables ----------------------------------------------------------------------------------------------------------------------------------------------

    int version = 91;    // Version of the analysis framework
    int LoopNumber = 1;  // Loop number for the test

    // - Beam energy settings -------------------------------------------------------------------------------------------------------------------------------------------

    vector<double> Ebeam_v = {2.07052};
    vector<vector<bool>> Ebeam_bool_v = {{true, false, false}};
    // vector<double> Ebeam_v = {4.02962};
    // vector<vector<bool>> Ebeam_bool_v = {{false, true, false}};
    // vector<double> Ebeam_v = {5.98636};
    // vector<vector<bool>> Ebeam_bool_v = {{false, false, true}};
    // vector<double> Ebeam_v = {4.02962, 5.98636};
    // vector<vector<bool>> Ebeam_bool_v = {{false, true, false}, {false, false, true}};
    // vector<double> Ebeam_v = {2.07052, 4.02962, 5.98636};
    // vector<vector<bool>> Ebeam_bool_v = {{true, false, false}, {false, true, false}, {false, false, true}};

    // - Limiter settings -----------------------------------------------------------------------------------------------------------------------------------------------

    // int Limiter = 25000000;  // 2500 files
    // int Limiter = 10000000;  // 1000 files
    // int Limiter = 5000000;  // 500 files
    int Limiter = 1000000;  // 100 files
    // int Limiter = 100000;  // 10 files
    // int Limiter = 10000;  // 1 file

    // - CutSelector_v settings -----------------------------------------------------------------------------------------------------------------------------------------

    /*
    // CutSelector_v = {ConstrainedE, OnlyGood_nFD, OnlyBad_nFD}
    // vector<vector<bool>> CutSelector_v = {{false, false, false}};  // All nFD and no ConstrainedE

    // vector<vector<bool>> CutSelector_v = {{true, false, false}};  // All nFD and with ConstrainedE
    // vector<vector<bool>> CutSelector_v = {{false, false, true}};  // OnlyBad and no ConstrainedE
    // vector<vector<bool>> CutSelector_v = {{true, false, true}};  // OnlyBad and with ConstrainedE
    // vector<vector<bool>> CutSelector_v = {{false, true, false}};  // OnlyGood and no ConstrainedE
    // vector<vector<bool>> CutSelector_v = {{true, true, false}};  // OnlyGood and with ConstrainedE
    // vector<vector<bool>> CutSelector_v = {{false, false, true}, {true, false, true}};  // OnlyBad and with and without ConstrainedE

    // vector<vector<bool>> CutSelector_v = {{false, false, false}, {false, true, false}, {false, false, true}};  // OnlyGood and OnlyBad without ConstrainedE

    // vector<vector<bool>> CutSelector_v = {{true, false, false}, {true, true, false}, {true, false, true}};   // OnlyGood and OnlyBad with ConstrainedE
    // vector<vector<bool>> CutSelector_v = {{false, false, false}, {false, true, false}, {false, false, true},
    //   {true, false, false},  {true, true, false},  {true, false, true}};  // Run all combinations
    */

    /* CutSelector_v = {ConstrainedE, OnlyGood_nFD, OnlyBad_nFD} */
    // vector<vector<bool>> CutSelector_v = {{false, false, false}};  // All nFD and no ConstrainedE
    // vector<vector<bool>> CutSelector_v = {{false, true, false}};  // OnlyGood and no ConstrainedE
    // vector<vector<bool>> CutSelector_v = {{false, false, false}, {false, true, false}, {false, false, true}};  // All nFD, OnlyGood, and OnlyBad without ConstrainedE
    vector<vector<bool>> CutSelector_v = {{false, false, false}, {true, false, false}};  // All nFD and no ConstrainedE

    // - Other settings -------------------------------------------------------------------------------------------------------------------------------------------------

    // vector<bool> use_ConstPn_samples_v = {true, false};  // ConstPn settings
    // vector<bool> use_ConstPn_samples_v = {true};  // ConstPn settings
    vector<bool> use_ConstPn_samples_v = {false};  // ConstPn settings
    // vector<bool> apply_neutFD_redef_v = {true, false};  // nFD redef settings
    // vector<bool> apply_neutFD_redef_v = {true};  // nFD redef settings
    vector<bool> apply_neutFD_redef_v = {false};  // nFD redef settings

    // ECAL veto settings
    vector<bool> apply_cPart_ECAL_veto = {true, false};
    // vector<bool> apply_cPart_ECAL_veto = {true};
    // vector<bool> apply_cPart_ECAL_veto = {false};
    vector<double> cPart_veto_radii_v = {100};

    vector<bool> apply_nPart_ECAL_veto = {true, false};
    // vector<bool> apply_nPart_ECAL_veto = {true};
    // vector<bool> apply_nPart_ECAL_veto = {false};
    vector<double> nPart_veto_radii_v = {125};

    vector<bool> apply_no_same_sector_cuts_v = {false};  // Same sector cuts settings

    // FD neutrals multiplicity cuts settings
    // vector<bool> apply_nFD_multi_cuts_v = {false, true};
    // vector<bool> apply_nFD_multi_cuts_v = {true};
    vector<bool> apply_nFD_multi_cuts_v = {false};
    // vector<bool> apply_no_FD_photons_cuts_v = {true, false};
    // vector<bool> apply_no_FD_photons_cuts_v = {true};
    vector<bool> apply_no_FD_photons_cuts_v = {false};

    // Multiplicity cuts settings for other particles
    // vector<bool> apply_no_other_part_multi_cuts_v = {false, true};
    // vector<bool> apply_no_other_part_multi_cuts_v = {true};
    vector<bool> apply_no_other_part_multi_cuts_v = {false};

    // Angular cuts (for radiated photons) settings
    // vector<bool> apply_theta_nFD_e_vs_dTheta_cuts_v = {false, true};
    // vector<bool> apply_theta_nFD_e_vs_dTheta_cuts_v = {true};
    vector<bool> apply_theta_nFD_e_vs_dTheta_cuts_v = {false};

    // Other angular cuts settings
    // vector<bool> apply_dTheta_cuts_v = {false, true};
    vector<bool> apply_dTheta_cuts_v = {false};
    // // DSCuts dTheta_cuts = DSCuts("dTheta_cuts", "FD", "", "", 0, -1., 1.);
    // // DSCuts dTheta_cuts = DSCuts("dTheta_cuts", "FD", "", "", 0, -2., 2.);
    // vector<DSCuts> dTheta_cuts_v = {DSCuts("dTheta_cuts", "FD", "", "", 0, -5., 5.)};
    bool apply_dPhi_cuts = false;
    DSCuts dPhi_cuts = DSCuts("dPhi_cuts", "FD", "", "", 10., -25., 5.);

    // - Momentum threshold settings ------------------------------------------------------------------------------------------------------------------------------------

    // Momentum threshold settings
    ParticleID pid;
    DSCuts e_mom_th = DSCuts("Momentum_th", "FD", "Electron", "", 0, -9999, 9999);
    DSCuts pCD_mom_th = DSCuts("Momentum_th", "CD", "Protons", "", 0, 0.3, 9999);
    DSCuts pFD_mom_th = DSCuts("Momentum_th", "FD", "Protons", "", 0, 0.4, 9999);
    // vector<DSCuts> phFD_mom_th_v = {DSCuts("Momentum_th", "FD", "Photons", "", 0, 0.0, 9999)};
    // // vector<DSCuts> phFD_mom_th_v = {DSCuts("Momentum_th", "FD", "Photons", "", 0, 0.0, 9999), DSCuts("Momentum_th", "FD", "Photons", "", 0, 0.3, 9999)};
    DSCuts pip_mom_th = DSCuts("Momentum_th", "", "Piplus", "", 0, 0.2, 9999);
    DSCuts pipFD_mom_th = DSCuts("Momentum", "FD", "Piplus", "", 0, pip_mom_th.GetLowerCut(), pip_mom_th.GetUpperCut());
    DSCuts pipCD_mom_th = DSCuts("Momentum", "CD", "Piplus", "", 0, pip_mom_th.GetLowerCut(), pip_mom_th.GetUpperCut());
    DSCuts pim_mom_th = DSCuts("Momentum_th", "", "Piplus", "", 0, 0.2, 9999);
    DSCuts pimFD_mom_th = DSCuts("Momentum", "FD", "Piminus", "", 0, pim_mom_th.GetLowerCut(), pim_mom_th.GetUpperCut());
    DSCuts pimCD_mom_th = DSCuts("Momentum", "CD", "Piminus", "", 0, pim_mom_th.GetLowerCut(), pim_mom_th.GetUpperCut());
    DSCuts nFD_mom_th = DSCuts("Momentum_th", "FD", "Neutrons", "", 0, pFD_mom_th.GetLowerCut(), pFD_mom_th.GetUpperCut());

    // Other settings
    bool apply_PCAL_neutral_veto = false;
    bool ConstrainTLmom = false;

    bool plot_general_histograms = true;

    bool plot_AMaps = false;
    bool plot_and_fit_MomRes = true;
    bool plot_FDNeutralVeto = true;

    std::string General_status0 = "NoStep5_effDebug";

    for (int Ebeam_ind = 0; Ebeam_ind < Ebeam_v.size(); Ebeam_ind++) {
        for (int Selector_ind = 0; Selector_ind < CutSelector_v.size(); Selector_ind++) {
            for (int use_ConstPn_samples_ind = 0; use_ConstPn_samples_ind < use_ConstPn_samples_v.size(); use_ConstPn_samples_ind++) {
                for (int apply_neutFD_redef_ind = 0; apply_neutFD_redef_ind < apply_neutFD_redef_v.size(); apply_neutFD_redef_ind++) {
                    for (int apply_cPart_ECAL_veto_ind = 0; apply_cPart_ECAL_veto_ind < apply_cPart_ECAL_veto.size(); apply_cPart_ECAL_veto_ind++) {
                        for (int apply_nPart_ECAL_veto_ind = 0; apply_nPart_ECAL_veto_ind < apply_nPart_ECAL_veto.size(); apply_nPart_ECAL_veto_ind++) {
                            for (int rc_ind = 0; rc_ind < cPart_veto_radii_v.size(); rc_ind++) {
                                for (int rn_ind = 0; rn_ind < nPart_veto_radii_v.size(); rn_ind++) {
                                    for (int apply_no_same_sector_cuts_ind = 0; apply_no_same_sector_cuts_ind < apply_no_same_sector_cuts_v.size(); apply_no_same_sector_cuts_ind++) {
                                        for (int apply_nFD_multi_cut_ind = 0; apply_nFD_multi_cut_ind < apply_nFD_multi_cuts_v.size(); apply_nFD_multi_cut_ind++) {
                                            for (int apply_no_FD_photons_cut_ind = 0; apply_no_FD_photons_cut_ind < apply_no_FD_photons_cuts_v.size(); apply_no_FD_photons_cut_ind++) {
                                                vector<DSCuts> phFD_mom_th_v;

                                                vector<bool> temp_apply_theta_nFD_e_vs_dTheta_cuts_v;

                                                if (!apply_no_FD_photons_cuts_v.at(apply_no_FD_photons_cut_ind)) {
                                                    phFD_mom_th_v = {DSCuts("Momentum_th", "FD", "Photons", "", 0, 0.0, 9999)};
                                                    // If we do not apply no FD photons cuts, then we do not apply theta_nFD_e vs dTheta cuts
                                                    temp_apply_theta_nFD_e_vs_dTheta_cuts_v = {false};
                                                } else {
                                                    phFD_mom_th_v = {DSCuts("Momentum_th", "FD", "Photons", "", 0, 0.3, 9999)};
                                                    // phFD_mom_th_v = {DSCuts("Momentum_th", "FD", "Photons", "", 0, 0.0, 9999), DSCuts("Momentum_th", "FD", "Photons", "", 0, 0.3, 9999)};
                                                    // If we do not apply no FD photons cuts, then we do not apply theta_nFD_e vs dTheta cuts
                                                    temp_apply_theta_nFD_e_vs_dTheta_cuts_v = apply_theta_nFD_e_vs_dTheta_cuts_v;
                                                }  // If we apply no FD photons cuts, then we use only the first cut

                                                for (int apply_no_other_part_multi_cut_ind = 0; apply_no_other_part_multi_cut_ind < apply_no_other_part_multi_cuts_v.size();
                                                     apply_no_other_part_multi_cut_ind++) {
                                                    for (int apply_theta_nFD_e_vs_dTheta_cuts_ind = 0; apply_theta_nFD_e_vs_dTheta_cuts_ind < temp_apply_theta_nFD_e_vs_dTheta_cuts_v.size();
                                                         apply_theta_nFD_e_vs_dTheta_cuts_ind++) {
                                                        vector<DSCuts> dTheta_cuts_v;

                                                        if (!apply_no_FD_photons_cuts_v.at(apply_no_FD_photons_cut_ind)) {
                                                            dTheta_cuts_v = {DSCuts("dTheta_cuts", "FD", "", "", 0, -5., 5.)};
                                                        } else {
                                                            // If we apply no FD photons cuts, then we use only the first cut
                                                            if (!temp_apply_theta_nFD_e_vs_dTheta_cuts_v.at(apply_theta_nFD_e_vs_dTheta_cuts_ind)) {
                                                                dTheta_cuts_v = {DSCuts("dTheta_cuts", "FD", "", "", 0, -5., 5.)};
                                                            } else {
                                                                dTheta_cuts_v = {DSCuts("dTheta_cuts", "FD", "", "", 0, -1., 1.), DSCuts("dTheta_cuts", "FD", "", "", 0, -2., 2.),
                                                                                 DSCuts("dTheta_cuts", "FD", "", "", 0, -5., 5.)};
                                                            }
                                                        }

                                                        for (int dTheta_cuts_ind = 0; dTheta_cuts_ind < dTheta_cuts_v.size(); dTheta_cuts_ind++) {
                                                            for (int phFD_mom_th_ind = 0; phFD_mom_th_ind < phFD_mom_th_v.size(); phFD_mom_th_ind++) {
                                                                std::string General_status = General_status0 + "_" + bt::ToStringWithPrecision(LoopNumber, 0);

                                                                Ph_nFD_sep_body_function(
                                                                    LoopNumber, version, Ebeam_v.at(Ebeam_ind), Ebeam_bool_v.at(Ebeam_ind), Limiter, CutSelector_v.at(Selector_ind),
                                                                    use_ConstPn_samples_v.at(use_ConstPn_samples_ind), apply_neutFD_redef_v.at(apply_neutFD_redef_ind),
                                                                    apply_cPart_ECAL_veto.at(apply_cPart_ECAL_veto_ind), cPart_veto_radii_v.at(rc_ind),
                                                                    apply_nPart_ECAL_veto.at(apply_nPart_ECAL_veto_ind), nPart_veto_radii_v.at(rn_ind),
                                                                    apply_no_same_sector_cuts_v.at(apply_no_same_sector_cuts_ind), apply_nFD_multi_cuts_v.at(apply_nFD_multi_cut_ind),
                                                                    apply_no_FD_photons_cuts_v.at(apply_no_FD_photons_cut_ind),
                                                                    apply_no_other_part_multi_cuts_v.at(apply_no_other_part_multi_cut_ind),
                                                                    temp_apply_theta_nFD_e_vs_dTheta_cuts_v.at(apply_theta_nFD_e_vs_dTheta_cuts_ind),
                                                                    temp_apply_theta_nFD_e_vs_dTheta_cuts_v.at(apply_theta_nFD_e_vs_dTheta_cuts_ind), dTheta_cuts_v.at(dTheta_cuts_ind), pid,
                                                                    e_mom_th, pCD_mom_th, pFD_mom_th, phFD_mom_th_v.at(phFD_mom_th_ind), pip_mom_th, pipFD_mom_th, pipCD_mom_th, pim_mom_th,
                                                                    pimFD_mom_th, pimCD_mom_th, nFD_mom_th, apply_PCAL_neutral_veto, ConstrainTLmom, plot_general_histograms, General_status,
                                                                    plot_AMaps, plot_and_fit_MomRes, plot_FDNeutralVeto);
                                                            }  // end of loop over phFD_mom_th_v
                                                        }  // end of loop over dTheta_cuts_v
                                                    }  // end of loop over temp_apply_theta_nFD_e_vs_dTheta_cuts_v
                                                }  // end of loop over apply_no_other_part_multi_cuts_v
                                            }  // end of loop over apply_no_other_part_multi_cuts_v
                                        }  // end of loop over apply_nFD_multi_cuts_v
                                    }  // end of loop over apply_no_same_sector_cuts_v
                                }  // end of loop over nPart_veto_radii_v
                            }  // end of loop over cPart_veto_radii_v
                        }  // end of loop over apply_nPart_ECAL_veto
                    }  // end of loop over apply_cPart_ECAL_veto
                }  // end of loop over apply_neutFD_redef_v
            }  // end of loop over use_ConstPn_samples_v
        }  // end of loop over CutSelector_v
    }  // end of loop over Ebeam_v
    // End of main loops

    auto end = std::chrono::system_clock::now();
    auto elapsed_time_seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    double elapsed_time_minutes = elapsed_time_seconds.count() / 60;

    std::cout << "\033[36m Warning!\033[31m recheck calculation in Calc_nFD_vars!\033[0m" << endl;

    if (elapsed_time_seconds.count() < 60) {
        std::cout << "\033[33m" << "Running time:" << "\033[0m " << elapsed_time_seconds.count() << " seconds\n\n";
    } else {
        std::cout << "\033[33m" << "Running time:" << "\033[0m " << bt::ToStringWithPrecision(elapsed_time_minutes, 3) << " minutes\n\n";
    }
}  // End of GammaNeutronFD_separation_test
