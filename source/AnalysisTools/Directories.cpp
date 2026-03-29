//
// Created by Alon Sportes on 09/04/2023.
//

#include "Directories.h"

// Constructor ----------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* Constructor */
Directories::Directories(const std::string& plots_path, const bool Clear_Old_Directories) {
    ClearOldDirectories = Clear_Old_Directories;

    this->Plots_Folder = plots_path;  // Plots_Folder = Parent_Folder
    const std::string& Plots_Folder = this->Plots_Folder;
    system(("mkdir -p " + Plots_Folder).c_str());  // clear old stuff in Parent_Folder

    if (ClearOldDirectories) {
        system(("rm -r " + Plots_Folder + "/*").c_str());  // clear old stuff in Parent_Folder
    }

    using DirEntry = std::pair<const char*, const char*>;
    auto set_directory_block = [&](std::map<std::string, std::string>& directory_map, const std::string& parent_directory, const std::vector<std::string>& daughter_folders,
                                   std::initializer_list<DirEntry> entries) {
        for (const auto& [map_key, daughter_key] : entries) { SetDirectory(directory_map, map_key, Plots_Folder, parent_directory, daughter_folders, daughter_key); }
    };

#pragma region /* Cut parameters plots directories */

#pragma region /* Preselection cuts directories */

#pragma region /* Vertex plots directories */
    CreateDirectories(create_Vtx_Dir, Vtx_Parent_Directory, Vtx_Daughter_Folders, false, Plots_Folder);
    Vtx_dir_map["Vtx_Parent"] = BuildPath(Plots_Folder, Vtx_Parent_Directory);
    set_directory_block(Vtx_dir_map, Vtx_Parent_Directory, Vtx_Daughter_Folders,
                        {{"Vtx_e_1e_cut", "01_1e_cut/01_Electrons"},
                         {"Vtx_p_1e_cut", "01_1e_cut/02_Protons"},
                         {"Vtx_Kp_1e_cut", "01_1e_cut/03_Kplus"},
                         {"Vtx_Km_1e_cut", "01_1e_cut/04_Kminus"},
                         {"Vtx_pip_1e_cut", "01_1e_cut/05_Piplus"},
                         {"Vtx_pim_1e_cut", "01_1e_cut/06_Piminus"},
                         {"Vtx_d_1e_cut", "01_1e_cut/07_Deuteron"}});
#pragma endregion

#pragma region /* Vertex correlation plots directories */
    CreateDirectories(create_Vtx_corr_Dir, Vtx_corr_Parent_Directory, Vtx_corr_Daughter_Folders, false, Plots_Folder);
    Vtx_corr_dir_map["Vtx_corr_Parent"] = BuildPath(Plots_Folder, Vtx_corr_Parent_Directory);
    set_directory_block(Vtx_corr_dir_map, Vtx_corr_Parent_Directory, Vtx_corr_Daughter_Folders,
                        {{"Vtx_corr_p_1e_cut", "01_1e_cut/01_Protons"},
                         {"Vtx_corr_Kp_1e_cut", "01_1e_cut/02_Kplus"},
                         {"Vtx_corr_Km_1e_cut", "01_1e_cut/03_Kminus"},
                         {"Vtx_corr_pip_1e_cut", "01_1e_cut/04_Piplus"},
                         {"Vtx_corr_pim_1e_cut", "01_1e_cut/05_Piminus"},
                         {"Vtx_corr_d_1e_cut", "01_1e_cut/06_Deuteron"},
                         {"Vtx_corr_1p", "02_1p"},
                         {"Vtx_corr_1n", "03_1n"},
                         {"Vtx_corr_2p", "04_2p"},
                         {"Vtx_corr_pFDpCD", "05_pFDpCD"},
                         {"Vtx_corr_nFDpCD", "06_nFDpCD"}});
#pragma endregion

#pragma region /* DC edge plots directories */
    CreateDirectories(create_DC_edge_Dir, DC_edge_Parent_Directory, DC_edge_Daughter_Folders, false, Plots_Folder);
    DC_edge_dir_map["DC_edge_Parent"] = BuildPath(Plots_Folder, DC_edge_Parent_Directory);
    set_directory_block(DC_edge_dir_map, DC_edge_Parent_Directory, DC_edge_Daughter_Folders,
                        {{"DC_edge_e_1e_cut", "01_1e_cut/01_Electrons"},
                         {"DC_edge_Proton_1e_cut", "01_1e_cut/02_Protons"},
                         {"DC_edge_Kplus_1e_cut", "01_1e_cut/03_Kplus"},
                         {"DC_edge_Kminus_1e_cut", "01_1e_cut/04_Kminus"},
                         {"DC_edge_piplus_1e_cut", "01_1e_cut/05_Piplus"},
                         {"DC_edge_piminus_1e_cut", "01_1e_cut/06_Piminus"},
                         {"DC_edge_deuteron_1e_cut", "01_1e_cut/07_Deuteron"}});
#pragma endregion

#pragma endregion

#pragma region /* Electron quality cuts directories */

#pragma region /* Number of Photo-electrons (Nphe) plots directories */
    CreateDirectories(create_Nphe_Dir, Nphe_Parent_Directory, Nphe_Daughter_Folders, false, Plots_Folder);
    Nphe_dir_map["Nphe_Parent"] = BuildPath(Plots_Folder, Nphe_Parent_Directory);
    set_directory_block(Nphe_dir_map, Nphe_Parent_Directory, Nphe_Daughter_Folders,
                        {{"Nphe_1e_cut_BC", "01_1e_cut"},
                         {"Nphe_1e_cut_AC", "01_1e_cut"},
                         {"Nphe_1p", "02_1p"},
                         {"Nphe_1n", "03_1n"},
                         {"Nphe_2p", "04_2p"},
                         {"Nphe_pFDpCD", "05_pFDpCD"},
                         {"Nphe_nFDpCD", "06_nFDpCD"}});
#pragma endregion

#pragma region /* Deposited energy plots directories */
    CreateDirectories(create_Edep_Dir, Edep_Parent_Directory, Edep_Daughter_Folders, false, Plots_Folder);
    Edep_dir_map["Edep_Parent"] = BuildPath(Plots_Folder, Edep_Parent_Directory);
    set_directory_block(Edep_dir_map, Edep_Parent_Directory, Edep_Daughter_Folders, {{"Edep_1e_cut", "01_1e_cut"}});
#pragma endregion

#pragma region /* Sampling Fraction (SF) plots directories */
    CreateDirectories(create_SF_Dir, SF_Parent_Directory, SF_Daughter_Folders, false, Plots_Folder);
    SF_dir_map["SF_Parent"] = BuildPath(Plots_Folder, SF_Parent_Directory);
    set_directory_block(SF_dir_map, SF_Parent_Directory, SF_Daughter_Folders,
                        {{"SF_1e_cut", "01_1e_cut/01_SF_plots"},
                         {"SF_VS_P_e_1e_cut", "01_1e_cut/02_SF_VS_P_e_plots"},
                         {"SF_VS_Edep_PCAL_1e_cut", "01_1e_cut/03_SF_VS_Edep_PCAL_plots"},
                         {"SF_1p", "02_1p/01_SF_plots"},
                         {"SF_VS_P_e_1p", "02_1p/02_SF_VS_P_e_plots"},
                         {"SF_VS_Edep_PCAL_1p", "02_1p/03_SF_VS_Edep_PCAL_plots"},
                         {"SF_1n", "03_1n/01_SF_plots"},
                         {"SF_VS_P_e_1n", "03_1n/02_SF_VS_P_e_plots"},
                         {"SF_VS_Edep_PCAL_1n", "03_1n/03_SF_VS_Edep_PCAL_plots"},
                         {"SF_2p", "04_2p/01_SF_plots"},
                         {"SF_VS_P_e_2p", "04_2p/02_SF_VS_P_e_plots"},
                         {"SF_VS_Edep_PCAL_2p", "04_2p/03_SF_VS_Edep_PCAL_plots"},
                         {"SF_pFDpCD", "05_pFDpCD/01_SF_plots"},
                         {"SF_VS_P_e_pFDpCD", "05_pFDpCD/02_SF_VS_P_e_plots"},
                         {"SF_VS_Edep_PCAL_pFDpCD", "05_pFDpCD/03_SF_VS_Edep_PCAL_plots"},
                         {"SF_nFDpCD", "06_nFDpCD/01_SF_plots"},
                         {"SF_VS_P_e_nFDpCD", "06_nFDpCD/02_SF_VS_P_e_plots"},
                         {"SF_VS_Edep_PCAL_nFDpCD", "06_nFDpCD/03_SF_VS_Edep_PCAL_plots"}});
#pragma endregion

#pragma region /* Edge cuts histograms plots directories */
    CreateDirectories(create_fiducial_Dir, ECAL_Fiducial_Parent_Directory, ECAL_Fiducial_Daughter_Folders, false, Plots_Folder);
    ECAL_fiducial_dir_map["ECAL_Fiducial_Parent"] = BuildPath(Plots_Folder, ECAL_Fiducial_Parent_Directory);
    set_directory_block(ECAL_fiducial_dir_map, ECAL_Fiducial_Parent_Directory, ECAL_Fiducial_Daughter_Folders,
                        {{"Edge_1e_BC_PCAL", "01_1e_cut/01_PCAL"},
                         {"Edge_1e_BC_PCAL", "01_1e_cut/01_PCAL"},
                         {"Edge_1p_PCAL", "02_1p/01_PCAL"},
                         {"Edge_1n_PCAL", "03_1n/01_PCAL"},
                         {"Edge_2p_PCAL", "04_2p/01_PCAL"},
                         {"Edge_pFDpCD_PCAL", "05_pFDpCD/01_PCAL"},
                         {"Edge_nFDpCD_PCAL", "06_nFDpCD/01_PCAL"}});
#pragma endregion

#pragma endregion

#pragma region /* Hadron PID plots directories */

#pragma region /* Chi2 plots directories */
    CreateDirectories(create_chi2_Dir, Chi2_Parent_Directory, Chi2_Daughter_Folders, false, Plots_Folder);
    Chi2_dir_map["Chi2_Parent"] = BuildPath(Plots_Folder, Chi2_Parent_Directory);
    set_directory_block(Chi2_dir_map, Chi2_Parent_Directory, Chi2_Daughter_Folders,
                        {{"Chi2_All_e", "00_All_e"},
                         {"Chi2_1e_cut", "01_1e_cut"},
                         {"Chi2_1p", "02_1p"},
                         {"Chi2_1n", "03_1n"},
                         {"Chi2_2p", "04_2p"},
                         {"Chi2_pFDpCD", "05_pFDpCD"},
                         {"Chi2_nFDpCD", "06_nFDpCD"}});
#pragma endregion

#pragma region /* CD proton PID plots directories */
    CreateDirectories(create_pCD_PID_Dir, pCD_PID_Parent_Directory, pCD_PID_Daughter_Folders, false, Plots_Folder);
    pCD_PID_dir_map["pCD_PID_Parent"] = BuildPath(Plots_Folder, pCD_PID_Parent_Directory);
    set_directory_block(pCD_PID_dir_map, pCD_PID_Parent_Directory, pCD_PID_Daughter_Folders, {{"pCD_PID_1e_cut", "01_1e_cut"}});
#pragma endregion

#pragma region /* CD edge plots directories */
    CreateDirectories(create_CD_edge_Dir, CD_edge_Parent_Directory, CD_edge_Daughter_Folders, false, Plots_Folder);
    CD_edge_dir_map["CD_edge_Parent"] = BuildPath(Plots_Folder, CD_edge_Parent_Directory);
    set_directory_block(CD_edge_dir_map, CD_edge_Parent_Directory, CD_edge_Daughter_Folders, {{"CD_edge_1e_cut", "01_1e_cut"}});
#pragma endregion

#pragma endregion

#pragma region /* Momentum plots directories */
    CreateDirectories(create_Momentum_Dir, Momentum_Parent_Directory, Momentum_Daughter_Folders, false, Plots_Folder);
    Momentum_dir_map["Momentum_Parent"] = BuildPath(Plots_Folder, Momentum_Parent_Directory);
    set_directory_block(Momentum_dir_map, Momentum_Parent_Directory, Momentum_Daughter_Folders,
                        {{"Momentum_1e_cut", "01_1e_cut"},
                         {"Momentum_th_reco_1e_cut", "01_1e_cut/01_Momentum_th_plots_1e_cut"},
                         {"Momentum_1p", "02_1p"},
                         {"Analysis_plots_momentum_1p", "02_1p/Analysis_plots_1p"},
                         {"Momentum_1n", "03_1n"},
                         {"Analysis_plots_momentum_1n", "03_1n/Analysis_plots_1n"},
                         {"Momentum_2p", "04_2p"},
                         {"Momentum_pFDpCD", "05_pFDpCD"},
                         {"Analysis_plots_momentum_pFDpCD", "05_pFDpCD/Analysis_plots_pFDpCD"},
                         {"Momentum_nFDpCD", "06_nFDpCD"},
                         {"Analysis_plots_momentum_nFDpCD", "06_nFDpCD/Analysis_plots_nFDpCD"}});
#pragma endregion

#pragma endregion

#pragma region /* Reaction monitoring plots directories */
    CreateDirectories(create_ReacMon_Dir, ReacMon_Parent_Directory, ReacMon_Daughter_Folders, false, Plots_Folder);
    ReacMon_dir_map["ReacMon_Parent"] = BuildPath(Plots_Folder, ReacMon_Parent_Directory);
    set_directory_block(ReacMon_dir_map, ReacMon_Parent_Directory, ReacMon_Daughter_Folders,
                        {{"ReacMon_1e_cut", "01_1e_cut"},
                         {"ReacMon_1p", "02_1p"},
                         {"ReacMon_1p_P_miss_1N", "02_1p/01_P_miss_1N_plots_1p"},
                         {"ReacMon_1p_E_miss_1N", "02_1p/02_E_miss_1N_plots_1p"},
                         {"ReacMon_1p_Q2", "02_1p/03_Q2_plots_1p"},
                         {"ReacMon_1p_xB", "02_1p/04_xB_plots_1p"},
                         {"ReacMon_1p_theta_q", "02_1p/05_theta_q_plots_1p"},
                         {"ReacMon_1p_theta_q_pFD", "02_1p/06_theta_q_pFD_plots_1p"},
                         {"ReacMon_1n", "03_1n"},
                         {"ReacMon_1n_P_miss_1N", "03_1n/01_P_miss_1N_plots_1n"},
                         {"ReacMon_1n_E_miss_1N", "03_1n/02_E_miss_1N_plots_1n"},
                         {"ReacMon_1n_Q2", "03_1n/03_Q2_plots_1n"},
                         {"ReacMon_1n_xB", "03_1n/04_xB_plots_1n"},
                         {"ReacMon_1n_theta_q", "03_1n/05_theta_q_plots_1n"},
                         {"ReacMon_1n_theta_q_pFD", "03_1n/06_theta_q_nFD_plots_1n"},
                         {"ReacMon_2p", "04_2p"},
                         {"ReacMon_2p_P_miss_1N", "04_2p/01_P_miss_1N_plots_2p"},
                         {"ReacMon_2p_E_miss_1N", "04_2p/02_E_miss_1N_plots_2p"},
                         {"ReacMon_2p_Q2", "04_2p/03_Q2_plots_2p"},
                         {"ReacMon_2p_xB", "04_2p/04_xB_plots_2p"},
                         {"ReacMon_2p_theta_q", "04_2p/05_theta_q_plots_2p"},
                         {"ReacMon_2p_theta_q_pFD", "04_2p/06_theta_q_pFD_plots_2p"},
                         {"ReacMon_pFDpCD", "05_pFDpCD"},
                         {"ReacMon_pFDpCD_P_miss_1N", "05_pFDpCD/01_P_miss_1N_plots_pFDpCD"},
                         {"ReacMon_pFDpCD_E_miss_1N", "05_pFDpCD/02_E_miss_1N_plots_pFDpCD"},
                         {"ReacMon_pFDpCD_P_miss_2N", "05_pFDpCD/03_P_miss_2N_plots_pFDpCD"},
                         {"ReacMon_pFDpCD_E_miss_2N", "05_pFDpCD/04_E_miss_2N_plots_pFDpCD"},
                         {"ReacMon_pFDpCD_Q2", "05_pFDpCD/05_Q2_plots_pFDpCD"},
                         {"ReacMon_pFDpCD_xB", "05_pFDpCD/06_xB_plots_pFDpCD"},
                         {"ReacMon_pFDpCD_theta_q", "05_pFDpCD/07_theta_q_plots_pFDpCD"},
                         {"ReacMon_pFDpCD_theta_q_N", "05_pFDpCD/08_theta_q_N_plots_pFDpCD"},
                         {"ReacMon_pFDpCD_theta_q_r", "05_pFDpCD/09_theta_q_r_plots_pFDpCD"},
                         {"ReacMon_nFDpCD", "06_nFDpCD"},
                         {"ReacMon_nFDpCD_P_miss_1N", "06_nFDpCD/01_P_miss_1N_plots_nFDpCD"},
                         {"ReacMon_nFDpCD_E_miss_1N", "06_nFDpCD/02_E_miss_1N_plots_nFDpCD"},
                         {"ReacMon_nFDpCD_P_miss_2N", "06_nFDpCD/03_P_miss_2N_plots_nFDpCD"},
                         {"ReacMon_nFDpCD_E_miss_2N", "06_nFDpCD/04_E_miss_2N_plots_nFDpCD"},
                         {"ReacMon_nFDpCD_Q2", "06_nFDpCD/05_Q2_plots_nFDpCD"},
                         {"ReacMon_nFDpCD_xB", "06_nFDpCD/06_xB_plots_nFDpCD"},
                         {"ReacMon_nFDpCD_theta_q", "06_nFDpCD/07_theta_q_plots_nFDpCD"},
                         {"ReacMon_nFDpCD_theta_q_N", "06_nFDpCD/08_theta_q_N_plots_nFDpCD"},
                         {"ReacMon_nFDpCD_theta_q_r", "06_nFDpCD/09_theta_q_r_plots_nFDpCD"}});
#pragma endregion

#pragma region /* W plots directories */
    CreateDirectories(create_W_Dir, W_Parent_Directory, W_Daughter_Folders, false, Plots_Folder);
    W_dir_map["W_Parent"] = BuildPath(Plots_Folder, W_Parent_Directory);
    set_directory_block(W_dir_map, W_Parent_Directory, W_Daughter_Folders,
                        {{"W_1e_cut", "01_1e_cut"}, {"W_1p", "02_1p"}, {"W_1n", "03_1n"}, {"W_2p", "04_2p"}, {"W_pFDpCD", "05_pFDpCD"}, {"W_nFDpCD", "06_nFDpCD"}});
#pragma endregion

#pragma region /* Beta plots directories */
    CreateDirectories(create_Beta_Dir, Beta_Parent_Directory, Beta_Daughter_Folders, false, Plots_Folder);
    Beta_dir_map["Beta_Parent"] = BuildPath(Plots_Folder, Beta_Parent_Directory);
    set_directory_block(Beta_dir_map, Beta_Parent_Directory, Beta_Daughter_Folders,
                        {{"Beta_1n", "01_Beta_fit_plots/03_1n"},
                         {"Neutron_beta_from_ph_1n", "01_Beta_fit_plots/03_1n/01_Neutron_beta_from_fPhotons_plots_1n"},
                         {"Neutron_beta_from_ph_01_1n", "01_Beta_fit_plots/03_1n/01_Neutron_beta_from_fPhotons_plots_1n/01_fPhotons_All_plots_1n"},
                         {"Neutron_beta_from_ph_02_1n", "01_Beta_fit_plots/03_1n/01_Neutron_beta_from_fPhotons_plots_1n/02_fPhotons_noPCAL_hit_plots_1n"},
                         {"Neutron_beta_from_ph_03_1n", "01_Beta_fit_plots/03_1n/01_Neutron_beta_from_fPhotons_plots_1n/03_fPhotons_noPCAL_yesECIN_plots_1n"},
                         {"Neutron_beta_from_ph_04_1n", "01_Beta_fit_plots/03_1n/01_Neutron_beta_from_fPhotons_plots_1n/04_fPhotons_noPCAL_noECIN_yesECOUT_plots_1n"},
                         {"Beta_vs_other_variables_1n", "01_Beta_fit_plots/03_1n/04_Beta_vs_other_variables_plots_1n"},
                         {"Beta_VS_P_All_e", "02_Beta_VS_P_plots/00_All_e"},
                         {"Beta_VS_P_by_charge_All_e", "02_Beta_VS_P_plots/00_All_e/By_charge"},
                         {"Beta_VS_P_1e_cut", "02_Beta_VS_P_plots/01_1e_cut"},
                         {"Beta_VS_P_by_charge_1e_cut", "02_Beta_VS_P_plots/01_1e_cut/By_charge"},
                         {"Beta_VS_P_1p", "02_Beta_VS_P_plots/02_1p"},
                         {"Beta_VS_P_by_charge_1p", "02_Beta_VS_P_plots/02_1p/By_charge"},
                         {"Beta_VS_P_1n", "02_Beta_VS_P_plots/03_1n"},
                         {"Beta_VS_P_by_charge_1n", "02_Beta_VS_P_plots/03_1n/By_charge"},
                         {"Beta_VS_P_2p", "02_Beta_VS_P_plots/04_2p"},
                         {"Beta_VS_P_by_charge_2p", "02_Beta_VS_P_plots/04_2p/By_charge"},
                         {"Beta_VS_P_pFDpCD", "02_Beta_VS_P_plots/05_pFDpCD"},
                         {"Beta_VS_P_by_charge_pFDpCD", "02_Beta_VS_P_plots/05_pFDpCD/By_charge"},
                         {"Beta_nFDpCD", "01_Beta_fit_plots/06_nFDpCD"},
                         {"Beta_VS_P_nFDpCD", "02_Beta_VS_P_plots/06_nFDpCD"},
                         {"Beta_VS_P_by_charge_nFDpCD", "02_Beta_VS_P_plots/06_nFDpCD/By_charge"}});
#pragma endregion

#pragma region /* Angle plots directories */
    CreateDirectories(create_Angle_Dir, Angle_Parent_Directory, Angle_Daughter_Folders, false, Plots_Folder);
    Angle_dir_map["Angle_Parent"] = BuildPath(Plots_Folder, Angle_Parent_Directory);
    set_directory_block(
        Angle_dir_map, Angle_Parent_Directory, Angle_Daughter_Folders,
        {{"Theta_e_All_e", "00_All_e/01_Theta_e_All_e_plots"},
         {"Phi_e_All_e", "00_All_e/02_Phi_e_All_e_plots"},
         {"Theta_e_VS_Phi_e_All_e", "00_All_e/03_Theta_e_VS_Phi_e_All_e_plots"},
         {"Theta_neut_VS_Phi_neut_All_e", "00_All_e/03_Theta_neut_VS_Phi_neut_All_e_plots"},
         {"Theta_e_1e_cut", "01_1e_cut/01_Theta_e_1e_cut_plots"},
         {"Phi_e_1e_cut", "01_1e_cut/02_Phi_e_1e_cut_plots"},
         {"Theta_e_VS_Phi_e_1e_cut", "01_1e_cut/03_Theta_e_VS_Phi_e_1e_cut_plots"},
         {"Neutron_veto_1e_cut", "01_1e_cut/04_Neutron_veto_1e_cut_plots"},
         {"Theta_e_1p", "02_1p/01_Theta_e_1p_plots"},
         {"Phi_e_1p", "02_1p/02_Phi_e_1p_plots"},
         {"Theta_e_VS_Phi_e_1p", "02_1p/03_Theta_e_VS_Phi_e_1p_plots"},
         {"Theta_p_1p", "02_1p/04_Theta_p_1p"},
         {"Phi_p_1p", "02_1p/05_Phi_p_1p"},
         {"Theta_p_VS_Phi_p_1p", "02_1p/06_Theta_p_VS_Phi_p_1p_plots"},
         {"Opening_angle_1p", "02_1p/07_Opening_angles_1p_plots"},
         {"Theta_e_1n", "03_1n/01_Theta_e_1n_plots"},
         {"Phi_e_1n", "03_1n/02_Phi_e_1n_plots"},
         {"Theta_e_VS_Phi_e_1n", "03_1n/03_Theta_e_VS_Phi_e_1n_plots"},
         {"Theta_n_1n", "03_1n/04_Theta_n_1n"},
         {"Phi_n_1n", "03_1n/05_Phi_n_1n"},
         {"Theta_n_VS_Phi_n_1n", "03_1n/06_Theta_n_VS_Phi_n_1n_plots"},
         {"Opening_angle_1n", "03_1n/07_Opening_angles_1n_plots"},
         {"Neutron_veto_1n", "03_1n/08_Neutron_veto_1n_plots"},
         {"Theta_e_2p", "04_2p/01_Theta_e_2p_plots"},
         {"Phi_e_2p", "04_2p/02_Phi_e_2p_plots"},
         {"Theta_e_VS_Phi_e_2p", "04_2p/03_Theta_e_VS_Phi_e_2p_plots"},
         {"Opening_angle_2p", "04_2p/04_Opening_angles_2p_plots"},
         {"Opening_angle_by_interaction_2p", "04_2p/04_Opening_angles_2p_plots/00_Theta_p1_p2_by_interaction"},
         {"Ghost_tracks_handling_2p", "04_2p/05_Ghost_tracks_handling_2p_plots"},
         {"CToF_hits_2p", "04_2p/05_Ghost_tracks_handling_2p_plots/01_CFoF_hits_2p_plots"},
         {"CToF_hits_monitoring_2p", "04_2p/05_Ghost_tracks_handling_2p_plots/01_CFoF_hits_2p_plots/00_CFoF_hits_monitoring_2p_plots"},
         {"Double_detection_2p", "04_2p/05_Ghost_tracks_handling_2p_plots/02_Double_detection_2p_plots"},
         {"Double_detection_monitoring_2p", "04_2p/05_Ghost_tracks_handling_2p_plots/02_Double_detection_2p_plots/00_Double_detection_monitoring_2p_plots"},
         {"Theta_e_pFDpCD", "05_pFDpCD/01_Theta_e_pFDpCD_plots"},
         {"Phi_e_pFDpCD", "05_pFDpCD/02_Phi_e_pFDpCD_plots"},
         {"Theta_e_VS_Phi_e_pFDpCD", "05_pFDpCD/03_Theta_e_VS_Phi_e_pFDpCD_plots"},
         {"Theta_pFD_pFDpCD", "05_pFDpCD/04_Theta_pFD_pFDpCD_plots"},
         {"Phi_pFD_pFDpCD", "05_pFDpCD/05_Phi_pFD_pFDpCD_plots"},
         {"Theta_pFD_VS_Phi_pFD_pFDpCD", "05_pFDpCD/06_Theta_pFD_VS_Phi_pFD_pFDpCD_plots"},
         {"Theta_pCD_pFDpCD", "05_pFDpCD/07_Theta_pCD_pFDpCD_plots"},
         {"Phi_pCD_pFDpCD", "05_pFDpCD/08_Phi_pCD_pFDpCD_plots"},
         {"Theta_pCD_VS_Phi_pCD_pFDpCD", "05_pFDpCD/09_Theta_pCD_VS_Phi_pCD_pFDpCD_plots"},
         {"Theta_tot_pFDpCD", "05_pFDpCD/10_Theta_tot_pFDpCD_plots"},
         {"Phi_tot_pFDpCD", "05_pFDpCD/11_Phi_tot_pFDpCD_plots"},
         {"Theta_tot_VS_Phi_tot_pFDpCD", "05_pFDpCD/12_Theta_tot_VS_Phi_tot_pFDpCD_plots"},
         {"Theta_rel_pFDpCD", "05_pFDpCD/13_Theta_rel_pFDpCD_plots"},
         {"Phi_rel_pFDpCD", "05_pFDpCD/14_Phi_rel_pFDpCD_plots"},
         {"Theta_rel_VS_Phi_rel_pFDpCD", "05_pFDpCD/15_Theta_rel_VS_Phi_rel_pFDpCD_plots"},
         {"Opening_angles_pFDpCD", "05_pFDpCD/16_Opening_angles_pFDpCD_plots"},
         {"Opening_angles_by_interaction_pFDpCD", "05_pFDpCD/16_Opening_angles_pFDpCD_plots/00_Theta_pFD_pCD_by_interaction"},
         {"Ghost_tracks_handling_pFDpCD", "05_pFDpCD/17_Ghost_tracks_handling_pFDpCD_plots"},
         {"CToF_hits_pFDpCD", "05_pFDpCD/17_Ghost_tracks_handling_pFDpCD_plots/01_CFoF_hits_pFDpCD_plots"},
         {"CToF_hits_monitoring_pFDpCD", "05_pFDpCD/17_Ghost_tracks_handling_pFDpCD_plots/01_CFoF_hits_pFDpCD_plots/00_CFoF_hits_Monitoring_pFDpCD_plots"},
         {"Double_detection_pFDpCD", "05_pFDpCD/17_Ghost_tracks_handling_pFDpCD_plots/02_Double_detection_pFDpCD_plots"},
         {"Double_detection_monitoring_pFDpCD", "05_pFDpCD/17_Ghost_tracks_handling_pFDpCD_plots/02_Double_detection_pFDpCD_plots/00_Double_detection_monitoring_pFDpCD_plots"},
         {"Theta_e_nFDpCD", "06_nFDpCD/01_Theta_e_nFDpCD_plots"},
         {"Phi_e_nFDpCD", "06_nFDpCD/02_Phi_e_nFDpCD_plots"},
         {"Theta_e_VS_Phi_e_nFDpCD", "06_nFDpCD/03_Theta_e_VS_Phi_e_nFDpCD_plots"},
         {"Theta_nFD_nFDpCD", "06_nFDpCD/04_Theta_nFD_nFDpCD_plots"},
         {"Phi_nFD_nFDpCD", "06_nFDpCD/05_Phi_nFD_nFDpCD_plots"},
         {"Theta_nFD_VS_Phi_nFD_nFDpCD", "06_nFDpCD/06_Theta_nFD_VS_Phi_nFD_nFDpCD_plots"},
         {"Theta_pCD_nFDpCD", "06_nFDpCD/07_Theta_pCD_nFDpCD_plots"},
         {"Phi_pCD_nFDpCD", "06_nFDpCD/08_Phi_pCD_nFDpCD_plots"},
         {"Theta_pCD_VS_Phi_pCD_nFDpCD", "06_nFDpCD/09_Theta_pCD_VS_Phi_pCD_nFDpCD_plots"},
         {"Theta_tot_nFDpCD", "06_nFDpCD/10_Theta_tot_nFDpCD_plots"},
         {"Phi_tot_nFDpCD", "06_nFDpCD/11_Phi_tot_nFDpCD_plots"},
         {"Theta_tot_VS_Phi_tot_nFDpCD", "06_nFDpCD/12_Theta_tot_VS_Phi_tot_nFDpCD_plots"},
         {"Theta_rel_nFDpCD", "06_nFDpCD/13_Theta_rel_nFDpCD_plots"},
         {"Phi_rel_nFDpCD", "06_nFDpCD/14_Phi_rel_nFDpCD_plots"},
         {"Theta_rel_VS_Phi_rel_nFDpCD", "06_nFDpCD/15_Theta_rel_VS_Phi_rel_nFDpCD_plots"},
         {"Opening_angles_nFDpCD", "06_nFDpCD/16_Opening_angles_nFDpCD_plots"},
         {"Opening_angles_by_interaction_nFDpCD", "06_nFDpCD/16_Opening_angles_nFDpCD_plots/00_Theta_nFD_pCD_by_interaction"},
         {"Double_detection_nFDpCD", "06_nFDpCD/17_Ghost_tracks_handling_nFDpCD_plots/02_Double_detection_nFDpCD_plots"},
         {"Double_detection_monitoring_nFDpCD", "06_nFDpCD/17_Ghost_tracks_handling_nFDpCD_plots/02_Double_detection_nFDpCD_plots/00_Double_detection_monitoring_nFDpCD_plots"},
         {"Neutron_veto_nFDpCD", "06_nFDpCD/18_Neutron_veto_nFDpCD_plots"},
         {"Phi_Proton_1e2pXy", "07_1e2pXy/04_Phi_Proton_1e2pXy_plots"}});
#pragma endregion

#pragma region /* Momentum transfer directories */
    CreateDirectories(create_Momentum_transfer_Dir, Momentum_transfer_Parent_Directory, Momentum_transfer_Daughter_Folders, false, Plots_Folder);
    Momentum_transfer_dir_map["Momentum_transfer_Parent"] = BuildPath(Plots_Folder, Momentum_transfer_Parent_Directory);
    set_directory_block(Momentum_transfer_dir_map, Momentum_transfer_Parent_Directory, Momentum_transfer_Daughter_Folders,
                        {{"Momentum_transfer_All_e", "00_All_e"},
                         {"Momentum_transfer_1e_cut", "01_1e_cut"},
                         {"Momentum_transfer_1p", "02_1p"},
                         {"Momentum_transfer_1n", "03_1n"},
                         {"Momentum_transfer_2p", "04_2p"},
                         {"Momentum_transfer_pFDpCD", "05_pFDpCD"},
                         {"Momentum_transfer_nFDpCD", "06_nFDpCD"}});
#pragma endregion

#pragma region /* E_e plots directories */
    CreateDirectories(create_E_e_Dir, E_e_Parent_Directory, E_e_Daughter_Folders, false, Plots_Folder);
    E_e_dir_map["E_e_Parent"] = BuildPath(Plots_Folder, E_e_Parent_Directory);
    set_directory_block(E_e_dir_map, E_e_Parent_Directory, E_e_Daughter_Folders,
                        {{"E_e_All_Int_1e_cut_stack", "01_1e_cut"},
                         {"E_e_All_Int_1e_cut", "01_1e_cut/E_e_plots_1e_cut"},
                         {"E_e_VS_Theta_e_All_Int_1e_cut", "01_1e_cut/E_e_VS_Theta_e_1e_cut"},
                         {"E_e_All_Int_1p_stack", "02_1p"},
                         {"E_e_All_Int_1p", "02_1p/E_e_plots_1p"},
                         {"E_e_VS_Theta_e_All_Int_1p", "02_1p/E_e_VS_Theta_e_1p"},
                         {"E_e_15_All_Int_1p", "02_1p/E_e_plots_1p/Around_15_deg_1p"},
                         {"E_e_All_Int_1n_stack", "03_1n"},
                         {"E_e_All_Int_1n", "03_1n/E_e_plots_1n"},
                         {"E_e_VS_Theta_e_All_Int_1n", "03_1n/E_e_VS_Theta_e_1n"},
                         {"E_e_15_All_Int_1n", "03_1n/E_e_plots_1n/Around_15_deg_1n"},
                         {"E_e_All_Int_2p_stack", "04_2p"},
                         {"E_e_All_Int_2p", "04_2p/E_e_plots_2p"},
                         {"E_e_VS_Theta_e_All_Int_2p", "04_2p/E_e_VS_Theta_e_2p"},
                         {"E_e_15_All_Int_2p", "04_2p/E_e_plots_2p/Around_15_deg_2p"},
                         {"E_e_All_Int_pFDpCD_stack", "05_pFDpCD"},
                         {"E_e_All_Int_pFDpCD", "05_pFDpCD/E_e_plots_pFDpCD"},
                         {"E_e_VS_Theta_e_All_Int_pFDpCD", "05_pFDpCD/E_e_VS_Theta_e_pFDpCD"},
                         {"E_e_15_All_Int_pFDpCD", "05_pFDpCD/E_e_plots_pFDpCD/Around_15_deg_pFDpCD"},
                         {"E_e_All_Int_nFDpCD_stack", "06_nFDpCD"},
                         {"E_e_All_Int_nFDpCD", "06_nFDpCD/E_e_plots_nFDpCD"},
                         {"E_e_VS_Theta_e_All_Int_nFDpCD", "06_nFDpCD/E_e_VS_Theta_e_nFDpCD"},
                         {"E_e_15_All_Int_nFDpCD", "06_nFDpCD/E_e_plots_nFDpCD/Around_15_deg_nFDpCD"}});
#pragma endregion

#pragma region /* ETrans plots directories */
    CreateDirectories(create_ETrans_Dir, Omega_Parent_Directory, Omega_Daughter_Folders, false, Plots_Folder);
    Omega_dir_map["Omega_Parent"] = BuildPath(Plots_Folder, Omega_Parent_Directory);
    set_directory_block(Omega_dir_map, Omega_Parent_Directory, Omega_Daughter_Folders,
                        {{"ETrans_15_stack_1p", "02_1p/02_Around_15_Deg_1p"},
                         {"ETrans_All_Int_15_1p", "02_1p/02_Around_15_Deg_1p/00_All_interactions"},
                         {"ETrans_QEL_15_1p", "02_1p/02_Around_15_Deg_1p/01_QEL_only"},
                         {"ETrans_MEC_15_1p", "02_1p/02_Around_15_Deg_1p/02_MEC_only"},
                         {"ETrans_RES_15_1p", "02_1p/02_Around_15_Deg_1p/03_RES_only"},
                         {"ETrans_DIS_15_1p", "02_1p/02_Around_15_Deg_1p/04_DIS_only"},
                         {"ETrans_All_Ang_stack_1p", "02_1p/01_Every_theta_e_1p"},
                         {"ETrans_All_Int_All_Ang_1p", "02_1p/01_Every_theta_e_1p/00_All_interactions"},
                         {"ETrans_QEL_All_Ang_1p", "02_1p/01_Every_theta_e_1p/01_QEL_only"},
                         {"ETrans_MEC_All_Ang_1p", "02_1p/01_Every_theta_e_1p/02_MEC_only"},
                         {"ETrans_RES_All_Ang_1p", "02_1p/01_Every_theta_e_1p/03_RES_only"},
                         {"ETrans_DIS_All_Ang_1p", "02_1p/01_Every_theta_e_1p/04_DIS_only"},
                         {"ETrans_15_stack_1n", "03_1n/02_Around_15_Deg_1n"},
                         {"ETrans_All_Int_15_1n", "03_1n/02_Around_15_Deg_1n/00_All_interactions"},
                         {"ETrans_QEL_15_1n", "03_1n/02_Around_15_Deg_1n/01_QEL_only"},
                         {"ETrans_MEC_15_1n", "03_1n/02_Around_15_Deg_1n/02_MEC_only"},
                         {"ETrans_RES_15_1n", "03_1n/02_Around_15_Deg_1n/03_RES_only"},
                         {"ETrans_DIS_15_1n", "03_1n/02_Around_15_Deg_1n/04_DIS_only"},
                         {"ETrans_All_Ang_stack_1n", "03_1n/01_Every_theta_e_1n"},
                         {"ETrans_All_Int_All_Ang_1n", "03_1n/01_Every_theta_e_1n/00_All_interactions"},
                         {"ETrans_QEL_All_Ang_1n", "03_1n/01_Every_theta_e_1n/01_QEL_only"},
                         {"ETrans_MEC_All_Ang_1n", "03_1n/01_Every_theta_e_1n/02_MEC_only"},
                         {"ETrans_RES_All_Ang_1n", "03_1n/01_Every_theta_e_1n/03_RES_only"},
                         {"ETrans_DIS_All_Ang_1n", "03_1n/01_Every_theta_e_1n/04_DIS_only"},
                         {"ETrans_15_stack_2p", "04_2p/02_Around_15_Deg_2p"},
                         {"ETrans_All_Int_15_2p", "04_2p/02_Around_15_Deg_2p/00_All_interactions"},
                         {"ETrans_QEL_15_2p", "04_2p/02_Around_15_Deg_2p/01_QEL_only"},
                         {"ETrans_MEC_15_2p", "04_2p/02_Around_15_Deg_2p/02_MEC_only"},
                         {"ETrans_RES_15_2p", "04_2p/02_Around_15_Deg_2p/03_RES_only"},
                         {"ETrans_DIS_15_2p", "04_2p/02_Around_15_Deg_2p/04_DIS_only"},
                         {"ETrans_All_Ang_stack_2p", "04_2p/01_Every_theta_e_2p"},
                         {"ETrans_All_Int_All_Ang_2p", "04_2p/01_Every_theta_e_2p/00_All_interactions"},
                         {"ETrans_QEL_All_Ang_2p", "04_2p/01_Every_theta_e_2p/01_QEL_only"},
                         {"ETrans_MEC_All_Ang_2p", "04_2p/01_Every_theta_e_2p/02_MEC_only"},
                         {"ETrans_RES_All_Ang_2p", "04_2p/01_Every_theta_e_2p/03_RES_only"},
                         {"ETrans_DIS_All_Ang_2p", "04_2p/01_Every_theta_e_2p/04_DIS_only"},
                         {"ETrans_15_stack_pFDpCD", "05_pFDpCD/02_Around_15_Deg_pFDpCD"},
                         {"ETrans_All_Int_15_pFDpCD", "05_pFDpCD/02_Around_15_Deg_pFDpCD/00_All_interactions"},
                         {"ETrans_QEL_15_pFDpCD", "05_pFDpCD/02_Around_15_Deg_pFDpCD/01_QEL_only"},
                         {"ETrans_MEC_15_pFDpCD", "05_pFDpCD/02_Around_15_Deg_pFDpCD/02_MEC_only"},
                         {"ETrans_RES_15_pFDpCD", "05_pFDpCD/02_Around_15_Deg_pFDpCD/03_RES_only"},
                         {"ETrans_DIS_15_pFDpCD", "05_pFDpCD/02_Around_15_Deg_pFDpCD/04_DIS_only"},
                         {"ETrans_All_Ang_stack_pFDpCD", "05_pFDpCD/01_Every_theta_e_pFDpCD"},
                         {"ETrans_All_Int_All_Ang_pFDpCD", "05_pFDpCD/01_Every_theta_e_pFDpCD/00_All_interactions"},
                         {"ETrans_QEL_All_Ang_pFDpCD", "05_pFDpCD/01_Every_theta_e_pFDpCD/01_QEL_only"},
                         {"ETrans_MEC_All_Ang_pFDpCD", "05_pFDpCD/01_Every_theta_e_pFDpCD/02_MEC_only"},
                         {"ETrans_RES_All_Ang_pFDpCD", "05_pFDpCD/01_Every_theta_e_pFDpCD/03_RES_only"},
                         {"ETrans_DIS_All_Ang_pFDpCD", "05_pFDpCD/01_Every_theta_e_pFDpCD/04_DIS_only"},
                         {"ETrans_15_stack_nFDpCD", "06_nFDpCD/02_Around_15_Deg_nFDpCD"},
                         {"ETrans_All_Int_15_nFDpCD", "06_nFDpCD/02_Around_15_Deg_nFDpCD/00_All_interactions"},
                         {"ETrans_QEL_15_nFDpCD", "06_nFDpCD/02_Around_15_Deg_nFDpCD/01_QEL_only"},
                         {"ETrans_MEC_15_nFDpCD", "06_nFDpCD/02_Around_15_Deg_nFDpCD/02_MEC_only"},
                         {"ETrans_RES_15_nFDpCD", "06_nFDpCD/02_Around_15_Deg_nFDpCD/03_RES_only"},
                         {"ETrans_DIS_15_nFDpCD", "06_nFDpCD/02_Around_15_Deg_nFDpCD/04_DIS_only"},
                         {"ETrans_All_Ang_stack_nFDpCD", "06_nFDpCD/01_Every_theta_e_nFDpCD"},
                         {"ETrans_All_Int_All_Ang_nFDpCD", "06_nFDpCD/01_Every_theta_e_nFDpCD/00_All_interactions"},
                         {"ETrans_QEL_All_Ang_nFDpCD", "06_nFDpCD/01_Every_theta_e_nFDpCD/01_QEL_only"},
                         {"ETrans_MEC_All_Ang_nFDpCD", "06_nFDpCD/01_Every_theta_e_nFDpCD/02_MEC_only"},
                         {"ETrans_RES_All_Ang_nFDpCD", "06_nFDpCD/01_Every_theta_e_nFDpCD/03_RES_only"},
                         {"ETrans_DIS_All_Ang_nFDpCD", "06_nFDpCD/01_Every_theta_e_nFDpCD/04_DIS_only"}});
#pragma endregion

#pragma region /* Ecal plots directories */
    CreateDirectories(create_Ecal_Dir, Ecal_Parent_Directory, Ecal_Daughter_Folders, false, Plots_Folder);
    Ecal_dir_map["Ecal_Parent"] = BuildPath(Plots_Folder, Ecal_Parent_Directory);
    set_directory_block(Ecal_dir_map, Ecal_Parent_Directory, Ecal_Daughter_Folders,
                        {{"Ecal_stack_1p", "02_1p"},
                         {"Ecal_All_Int_1p", "02_1p/01_Ecal_rec_by_reaction_1p/00_All_interactions"},
                         {"Ecal_QEL_1p", "02_1p/01_Ecal_rec_by_reaction_1p/01_QEL_only"},
                         {"Ecal_MEC_1p", "02_1p/01_Ecal_rec_by_reaction_1p/02_MEC_only"},
                         {"Ecal_RES_1p", "02_1p/01_Ecal_rec_by_reaction_1p/03_RES_only"},
                         {"Ecal_DIS_1p", "02_1p/01_Ecal_rec_by_reaction_1p/04_DIS_only"},
                         {"Ecal_rec_vs_Mom_1p", "02_1p/02_Ecal_rec_vs_Mom_1p"},
                         {"Ecal_rec_vs_Ang_1p", "02_1p/03_Ecal_rec_vs_Ang_1p"},
                         {"Ecal_rec_vs_TKI_1p", "02_1p/04_Ecal_rec_vs_TKI_1p"},
                         {"Chi2_Ecal_test_1p", "02_1p/05_Chi2_for_Ecal_above_Ebeam_1p"},
                         {"Ecal_stack_1n", "03_1n"},
                         {"Ecal_All_Int_1n", "03_1n/01_Ecal_rec_by_reaction_1n/00_All_interactions"},
                         {"Ecal_QEL_1n", "03_1n/01_Ecal_rec_by_reaction_1n/01_QEL_only"},
                         {"Ecal_MEC_1n", "03_1n/01_Ecal_rec_by_reaction_1n/02_MEC_only"},
                         {"Ecal_RES_1n", "03_1n/01_Ecal_rec_by_reaction_1n/03_RES_only"},
                         {"Ecal_DIS_1n", "03_1n/01_Ecal_rec_by_reaction_1n/04_DIS_only"},
                         {"Ecal_rec_vs_Mom_1n", "03_1n/02_Ecal_rec_vs_Mom_1n"},
                         {"Ecal_rec_vs_Ang_1n", "03_1n/03_Ecal_rec_vs_Ang_1n"},
                         {"Ecal_rec_vs_TKI_1n", "03_1n/04_Ecal_rec_vs_TKI_1n"},
                         {"Chi2_Ecal_test_1n", "03_1n/05_Chi2_for_Ecal_above_Ebeam_1n"},
                         {"Ecal_stack_2p", "04_2p"},
                         {"Ecal_All_Int_2p", "04_2p/01_Ecal_rec_by_reaction_2p/00_All_interactions"},
                         {"Ecal_QEL_2p", "04_2p/01_Ecal_rec_by_reaction_2p/01_QEL_only"},
                         {"Ecal_MEC_2p", "04_2p/01_Ecal_rec_by_reaction_2p/02_MEC_only"},
                         {"Ecal_RES_2p", "04_2p/01_Ecal_rec_by_reaction_2p/03_RES_only"},
                         {"Ecal_DIS_2p", "04_2p/01_Ecal_rec_by_reaction_2p/04_DIS_only"},
                         {"Ecal_rec_vs_TKI_2p", "04_2p/02_Ecal_rec_vs_TKI_2p"},
                         {"Chi2_Ecal_test_2p", "04_2p/03_Chi2_for_Ecal_above_Ebeam_2p"},
                         {"Ecal_stack_pFDpCD", "05_pFDpCD"},
                         {"Ecal_All_Int_pFDpCD", "05_pFDpCD/01_Ecal_rec_by_reaction_pFDpCD/00_All_interactions"},
                         {"Ecal_QEL_pFDpCD", "05_pFDpCD/01_Ecal_rec_by_reaction_pFDpCD/01_QEL_only"},
                         {"Ecal_MEC_pFDpCD", "05_pFDpCD/01_Ecal_rec_by_reaction_pFDpCD/02_MEC_only"},
                         {"Ecal_RES_pFDpCD", "05_pFDpCD/01_Ecal_rec_by_reaction_pFDpCD/03_RES_only"},
                         {"Ecal_DIS_pFDpCD", "05_pFDpCD/01_Ecal_rec_by_reaction_pFDpCD/04_DIS_only"},
                         {"Ecal_rec_vs_Mom_pFDpCD", "05_pFDpCD/02_Ecal_rec_vs_Mom_pFDpCD"},
                         {"Ecal_rec_vs_Ang_pFDpCD", "05_pFDpCD/03_Ecal_rec_vs_Ang_pFDpCD"},
                         {"Ecal_rec_vs_TKI_pFDpCD", "05_pFDpCD/04_Ecal_rec_vs_TKI_pFDpCD"},
                         {"Chi2_Ecal_test_pFDpCD", "05_pFDpCD/05_Chi2_for_Ecal_above_Ebeam_pFDpCD"},
                         {"Ecal_stack_nFDpCD", "06_nFDpCD"},
                         {"Ecal_All_Int_nFDpCD", "06_nFDpCD/01_Ecal_rec_by_reaction_nFDpCD/00_All_interactions"},
                         {"Ecal_QEL_nFDpCD", "06_nFDpCD/01_Ecal_rec_by_reaction_nFDpCD/01_QEL_only"},
                         {"Ecal_MEC_nFDpCD", "06_nFDpCD/01_Ecal_rec_by_reaction_nFDpCD/02_MEC_only"},
                         {"Ecal_RES_nFDpCD", "06_nFDpCD/01_Ecal_rec_by_reaction_nFDpCD/03_RES_only"},
                         {"Ecal_DIS_nFDpCD", "06_nFDpCD/01_Ecal_rec_by_reaction_nFDpCD/04_DIS_only"},
                         {"Ecal_rec_vs_Mom_nFDpCD", "06_nFDpCD/02_Ecal_rec_vs_Mom_nFDpCD"},
                         {"Ecal_rec_vs_Ang_nFDpCD", "06_nFDpCD/03_Ecal_rec_vs_Ang_nFDpCD"},
                         {"Ecal_rec_vs_TKI_nFDpCD", "06_nFDpCD/04_Ecal_rec_vs_TKI_nFDpCD"},
                         {"Chi2_Ecal_test_nFDpCD", "06_nFDpCD/05_Chi2_for_Ecal_above_Ebeam_nFDpCD"}});
#pragma endregion

#pragma region /* TKI plots directories */
    CreateDirectories(create_TKI_Dir, TKI_Parent_Directory, TKI_Daughter_Folders, false, Plots_Folder);
    TKI_dir_map["TKI_Parent"] = BuildPath(Plots_Folder, TKI_Parent_Directory);
    set_directory_block(TKI_dir_map, TKI_Parent_Directory, Ecal_Daughter_Folders,
                        {{"dP_T_vs_dAlpha_T_1p", "02_1p"},
                         {"dP_T_vs_dAlpha_T_1n", "03_1n"},
                         {"dP_T_vs_dAlpha_T_2p", "04_2p"},
                         {"dP_T_vs_dAlpha_T_pFDpCD", "05_pFDpCD"},
                         {"dP_T_vs_dAlpha_T_nFDpCD", "06_nFDpCD"}});
    set_directory_block(TKI_dir_map, TKI_Parent_Directory, TKI_Daughter_Folders,
                        {{"dP_T_1p", "02_1p/01_dP_T_1p"},
                         {"dAlpha_T_1p", "02_1p/02_dAlpha_T_1p"},
                         {"dPhi_T_1p", "02_1p/03_dPhi_T_1p"},
                         {"dP_T_1n", "03_1n/01_dP_T_1n"},
                         {"dAlpha_T_1n", "03_1n/02_dAlpha_T_1n"},
                         {"dPhi_T_1n", "03_1n/03_dPhi_T_1n"},
                         {"dP_T_2p", "04_2p/01_dP_T_2p"},
                         {"dAlpha_T_2p", "04_2p/02_dAlpha_T_2p"},
                         {"dPhi_T_2p", "04_2p/03_dPhi_T_2p"},
                         {"dP_T_pFDpCD", "05_pFDpCD/01_dP_T_pFDpCD"},
                         {"dAlpha_T_pFDpCD", "05_pFDpCD/02_dAlpha_T_pFDpCD"},
                         {"dPhi_T_pFDpCD", "05_pFDpCD/03_dPhi_T_pFDpCD"},
                         {"dP_T_nFDpCD", "06_nFDpCD/01_dP_T_nFDpCD"},
                         {"dAlpha_T_nFDpCD", "06_nFDpCD/02_dAlpha_T_nFDpCD"},
                         {"dPhi_T_nFDpCD", "06_nFDpCD/03_dPhi_T_nFDpCD"}});
#pragma endregion

#pragma region /* ToF plots directories */
    CreateDirectories(create_ToF_Dir, ToF_Parent_Directory, ToF_Daughter_Folders, false, Plots_Folder);
    ToF_dir_map["ToF_Parent"] = BuildPath(Plots_Folder, ToF_Parent_Directory);
    set_directory_block(ToF_dir_map, ToF_Parent_Directory, ToF_Daughter_Folders,
                        {{"Neutron_vs_cParticles_hits_1e_cut", "01_1e_cut/01_Neutron_vs_cParticles_hits_FTOF_1e_cut"},
                         {"Neutron_vs_cParticles_hits_nFDpCD", "06_nFDpCD/01_Neutron_vs_cParticles_hits_FTOF_nFDpCD"}});
#pragma endregion

#pragma region /* Efficiency & Acceptance Correction plots directories */
    CreateDirectories(create_Eff_and_ACorr_Dir, Eff_and_ACorr_Parent_Directory, Eff_and_ACorr_Daughter_Folders, false, Plots_Folder);
    Eff_and_ACorr_dir_map["Eff_and_ACorr_Parent"] = BuildPath(Plots_Folder, Eff_and_ACorr_Parent_Directory);
    set_directory_block(Eff_and_ACorr_dir_map, Eff_and_ACorr_Parent_Directory, Eff_and_ACorr_Daughter_Folders,
                        {{"Eff_and_ACorr_1e_cut", "01_1e_cut"},
                         {"Mom_Eff_and_ACorr_1e_cut", "01_1e_cut/01_Momentum_Eff_and_ACorr_plots_1e_cut"},
                         {"Theta_Eff_and_ACorr_1e_cut", "01_1e_cut/02_Theta_Eff_and_ACorr_plots_1e_cut"},
                         {"Phi_Eff_and_ACorr_1e_cut", "01_1e_cut/03_Phi_Eff_and_ACorr_plots_1e_cut"},
                         {"TL_Acceptance_Maps_1e_cut", "01_1e_cut/04_Hit_Maps"},
                         {"Loaded_reco_ref_Acceptance_Maps_1e_cut", "01_1e_cut/05_Loaded_reco_ref_Hit_Maps"},
                         {"Momentum_th_TL_1e_cut", "01_1e_cut/05_Eff_plots_for_Momentum_th"},
                         {"Eff_and_ACorr_1p", "02_1p"},
                         {"Mom_Eff_and_ACorr_1p", "02_1p/01_Momentum_Eff_and_ACorr_plots_1p"},
                         {"Theta_Eff_and_ACorr_1p", "02_1p/02_Theta_Eff_and_ACorr_plots_1p"},
                         {"Phi_Eff_and_ACorr_1p", "02_1p/03_Phi_Eff_and_ACorr_plots_1p"},
                         {"Proton_FD_Hit_map_1p", "02_1p/04_Proton_FD_Hit_map_1p"},
                         {"Eff_and_ACorr_1n", "03_1n"},
                         {"Mom_Eff_and_ACorr_1n", "03_1n/01_Momentum_Eff_and_ACorr_plots_1n"},
                         {"Theta_Eff_and_ACorr_1n", "03_1n/02_Theta_Eff_and_ACorr_plots_1n"},
                         {"Phi_Eff_and_ACorr_1n", "03_1n/03_Phi_Eff_and_ACorr_plots_1n"},
                         {"Neutron_FD_Hit_map_1n", "03_1n/04_Neutron_FD_Hit_map_1n"},
                         {"Eff_and_ACorr_2p", "04_2p"},
                         {"Mom_Eff_and_ACorr_2p", "04_2p/01_Momentum_Eff_and_ACorr_plots_2p"},
                         {"Theta_Eff_and_ACorr_2p", "04_2p/02_Theta_Eff_and_ACorr_plots_2p"},
                         {"Phi_Eff_and_ACorr_2p", "04_2p/03_Phi_Eff_and_ACorr_plots_2p"},
                         {"Eff_and_ACorr_pFDpCD", "05_pFDpCD"},
                         {"Mom_Eff_and_ACorr_pFDpCD", "05_pFDpCD/01_Momentum_Eff_and_ACorr_plots_pFDpCD"},
                         {"Theta_Eff_and_ACorr_pFDpCD", "05_pFDpCD/02_Theta_Eff_and_ACorr_plots_pFDpCD"},
                         {"Phi_Eff_and_ACorr_pFDpCD", "05_pFDpCD/03_Phi_Eff_and_ACorr_plots_pFDpCD"},
                         {"Proton_FD_Hit_map_pFDpCD", "05_pFDpCD/04_Proton_FD_Hit_map_pFDpCD"},
                         {"Eff_and_ACorr_nFDpCD", "06_nFDpCD"},
                         {"Mom_Eff_and_ACorr_nFDpCD", "06_nFDpCD/01_Momentum_Eff_and_ACorr_plots_nFDpCD"},
                         {"Theta_Eff_and_ACorr_nFDpCD", "06_nFDpCD/02_Theta_Eff_and_ACorr_plots_nFDpCD"},
                         {"Phi_Eff_and_ACorr_nFDpCD", "06_nFDpCD/03_Phi_Eff_and_ACorr_plots_nFDpCD"},
                         {"Neutron_FD_Hit_map_nFDpCD", "06_nFDpCD/04_Neutron_FD_Hit_map_nFDpCD"}});
#pragma endregion

#pragma region /* Acceptance maps directories */
    CreateDirectories(create_Acceptance_maps_Dir, AMaps_Parent_Directory, Hit_Maps_Daughter_Folders, false, Plots_Folder);
    AMaps_dir_map["AMaps_Parent"] = BuildPath(Plots_Folder, AMaps_Parent_Directory);
    set_directory_block(AMaps_dir_map, AMaps_Parent_Directory, Hit_Maps_Daughter_Folders,
                        {{"AMaps_1e_cut", "01_AMaps_1e_cut"},
                         {"AMaps_BC_1e_cut", "01_AMaps_1e_cut/00a_AMaps_BC_from_file"},
                         {"WMaps_1e_cut", "02_WMaps_1e_cut"},
                         {"WMaps_BC_1e_cut", "02_WMaps_1e_cut/00a_WMaps_BC_from_file"}});
#pragma endregion

#pragma region /* Resolution plots directories */
    CreateDirectories(create_Resolution_Dir, Resolution_Parent_Directory, Resolution_Daughter_Folders, false, Plots_Folder);
    Resolution_dir_map["Resolution_Parent"] = BuildPath(Plots_Folder, Resolution_Parent_Directory);
    set_directory_block(Resolution_dir_map, Resolution_Parent_Directory, Resolution_Daughter_Folders,
                        {{"Resolution_1p", "02_1p"},
                         {"pRes_plots_1p", "02_1p/01_Proton_resolution_plots_1p"},
                         {"Monitoring_1p", "02_1p/02_Match_monitoring_plots_1p"},
                         {"Match_multi_1p", "02_1p/03_Match_multi_plots_1p"},
                         {"Basic_var_1p", "02_1p/04_Fit_justification_plots_1p/01_Basic_variables_1p"},
                         {"Corr_just_1p", "02_1p/04_Fit_justification_plots_1p/02_Correction_1p"},
                         {"Corr_Smear_1p", "02_1p/04_Fit_justification_plots_1p/03_Smearing_1p"},
                         {"Resolution_1n", "03_1n"},
                         {"nRes_plots_1n", "03_1n/01_Neutron_resolution_plots_1n"},
                         {"Monitoring_1n", "03_1n/02_Match_monitoring_plots_1n"},
                         {"Monitoring_Before_matching_cuts_1n", "03_1n/02_Match_monitoring_plots_1n/01_Before_matching_cuts_1n"},
                         {"Monitoring_After_only_dPhi_cuts_1n", "03_1n/02_Match_monitoring_plots_1n/02_After_only_dPhi_cuts_1n"},
                         {"Monitoring_After_matching_cuts_1n", "03_1n/02_Match_monitoring_plots_1n/03_After_matching_cuts_1n"},
                         {"Match_multi_1n", "03_1n/03_Match_multi_plots_1n"},
                         {"Basic_var_1n", "03_1n/04_Fit_justification_plots_1n/01_Basic_variables_1n"},
                         {"Corr_just_1n", "03_1n/04_Fit_justification_plots_1n/02_Correction_1n"},
                         {"Smear_just_1n", "03_1n/04_Fit_justification_plots_1n/03_Smearing_1n"},
                         {"Resolution_mom_plots_1n", "03_1n/05_Resolution_mom_plots_1n"},
                         {"Resolution_status_plots_1n", "03_1n/06_Resolution_status_plots_1n"},
                         {"Resolution_vtx_plots_1n", "03_1n/07_Resolution_vtx_plots_1n"},
                         {"Resolution_Hcorr_plots_1n", "03_1n/08_Resolution_Hcorr_plots_1n"},
                         {"Resolution_matchedHcorr_plots_1n", "03_1n/09_Resolution_matchedHcorr_plots_1n"},
                         {"Resolution_ECALcorr_plots_1n", "03_1n/10_Resolution_ECALcorr_plots_1n"},
                         {"Resolution_shower_width_plots_1n", "03_1n/11_Resolution_shower_width_plots_1n"},
                         {"Resolution_shower_moment_plots_1n", "03_1n/12_Resolution_shower_moment_plots_1n"}});
#pragma endregion

#pragma region /* Multiplicity plots directories */
    CreateDirectories(create_Multiplicity_Dir, Multiplicity_Parent_Directory, Multiplicity_Daughter_Folders, false, Plots_Folder);
    Multiplicity_dir_map["Multiplicity_Parent"] = BuildPath(Plots_Folder, Multiplicity_Parent_Directory);
    set_directory_block(
        Multiplicity_dir_map, Multiplicity_Parent_Directory, Multiplicity_Daughter_Folders,
        {{"Multiplicity_1e_cut", "01_1e_cut"}, {"Multiplicity_1p", "02_1p"}, {"Multiplicity_1n", "03_1n"}, {"Multiplicity_pFDpCD", "05_pFDpCD"}, {"Multiplicity_nFDpCD", "06_nFDpCD"}});
#pragma endregion
}
#pragma endregion

// Private methods ------------------------------------------------------------------------------------------------------------------------------------------------------

bool Directories::FindSubstring(const std::string& string1, const std::string& string2) const { return string1.find(string2) != std::string::npos; }

std::string Directories::Find(const std::vector<std::string>& vector, const std::string& string) const {
    for (const std::string& Element : vector) {
        if (FindSubstring(Element, string)) { return Element; }
    }

    return "";
}

std::string Directories::BuildPath(const std::string& base_folder, const std::string& parent_folder, const std::string& daughter_folder) const {
    std::string path = base_folder + "/" + parent_folder;
    if (!daughter_folder.empty()) { path += "/" + daughter_folder; }
    return path + "/";
}

void Directories::CreateDirectories(const bool Create_Directory, const std::string& Plots_Parent_Folder, const std::vector<std::string>& daughter_folders,
                                    const bool Clear_Parent_Folder_content, const std::string& Parent_Folder) {
    if (!Create_Directory) { return; }
    for (const std::string& folder_name : daughter_folders) { MakeDirectory(Create_Directory, Plots_Parent_Folder, folder_name, Clear_Parent_Folder_content, Parent_Folder); }
}

std::string Directories::FindCached(const std::vector<std::string>& vector, const std::string& string) const {
    auto& vector_cache = find_cache_[&vector];
    auto cached_it = vector_cache.find(string);
    if (cached_it != vector_cache.end()) { return cached_it->second; }

    const std::string found_value = Find(vector, string);
    vector_cache[string] = found_value;
    return found_value;
}

void Directories::SetDirectory(std::map<std::string, std::string>& directory_map, const std::string& map_key, const std::string& base_folder, const std::string& parent_folder,
                               const std::vector<std::string>& daughter_folders, const std::string& daughter_key) const {
    directory_map[map_key] = BuildPath(base_folder, parent_folder, FindCached(daughter_folders, daughter_key));
}

// MakeDirectory function -----------------------------------------------------------------------------------------------------------------------------------------------

void Directories::MakeDirectory(const bool Create_Directory, const std::string& Plots_Parent_Folder, const std::string& Plots_Daughter_Folder, const bool Clear_Parent_Folder_content,
                                const std::string& Parent_Folder) {
    if (!Create_Directory) { return; }

    const std::string make_dir = "mkdir -p " + Parent_Folder + "/" + Plots_Parent_Folder + "/" + Plots_Daughter_Folder;
    if (Clear_Parent_Folder_content) {
        const std::string remove_directory_content = "rm -r " + Parent_Folder + "/" + Plots_Parent_Folder + "/*";
        system(remove_directory_content.c_str());
    }

    system(make_dir.c_str());
}
