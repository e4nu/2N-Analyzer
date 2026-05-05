//
// Created by Alon Sportes on 22/01/2024.
//

#include "AnalysisTools/CLAS12PID/ParticleID.h"

// NeutronECAL_Cut_Veto functions ---------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* NeutronECAL_Cut_Veto function */
/**
 * @method NeutronECAL_Cut_Veto
 *
 * Thin wrapper around `reco_analysis_functions::NeutronECAL_Cut_Veto` (aliased as `raf::...`).
 *
 * Purpose:
 *   Preserve a stable `ParticleID` class API while keeping the implementation in the
 *   `reco_analysis_functions` namespace so it can be debugged and exercised directly
 *   from ROOT macros and lightweight test drivers.
 *
 * Inputs:
 *   - allParticles: full reconstructed particle list for the event.
 *   - electrons:    reconstructed electron candidates (typically FD electron) used for event timing/vertex context.
 *   - beamE:        beam energy used by the underlying veto logic when needed.
 *   - index:        index (in allParticles) of the neutral candidate the veto is evaluated on.
 *   - veto_cut:     threshold parameter passed through to the underlying veto.
 *
 * Output / semantics:
 *   Returns exactly what `raf::NeutronECAL_Cut_Veto(...)` returns.
 *   No additional logic, side effects, or parameter changes are introduced here.
 */
bool ParticleID::NeutronECAL_Cut_Veto(std::vector<clas12::region_part_ptr>& allParticles, std::vector<clas12::region_part_ptr>& electrons, const double& beamE, const int& index,
                                      const double& veto_cut) {
    // Forward all arguments verbatim to the implementation in `reco_analysis_functions`.
    // This preserves the full, current behavior while keeping this class method as a stable entry point.
    return raf::NeutronECAL_Cut_Veto(allParticles, electrons, beamE, index, veto_cut);
}
#pragma endregion

// ChargedECALveto functions --------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ChargedECALveto function */
/**
 * @method ChargedECALveto
 *
 * Thin wrapper around `reco_analysis_functions::ChargedECALveto` (aliased as `raf::...`).
 *
 * Purpose:
 *   Preserve a stable `ParticleID` class API while keeping the implementation in the
 *   `reco_analysis_functions` namespace so it can be debugged and exercised directly
 *   from ROOT macros and lightweight test drivers.
 *
 * Notes on parameters:
 *   - starttime: event start time used by the underlying logic when time-of-flight style variables are computed.
 *   - Recalc_P_nFD: when true, the underlying code may recompute the neutral momentum estimate using timing.
 *   - apply_PCAL_veto: when true, the underlying code may apply an explicit PCAL-related requirement to suppress
 *                      photon contamination when selecting neutral hadrons.
 *
 * Output / semantics:
 *   Returns exactly what `raf::ChargedECALveto(...)` returns.
 *   No additional logic, side effects, or parameter changes are introduced here.
 */
bool ParticleID::ChargedECALveto(std::vector<clas12::region_part_ptr>& allParticles, clas12::region_part_ptr& electron, clas12::region_part_ptr NeutronFD, const double& veto_cut,
                                 const double& starttime = 9999, const bool& Recalc_P_nFD = false, const bool& apply_PCAL_veto = false) {
    // Forward all arguments verbatim to the implementation in `reco_analysis_functions`.
    // Keeping this as a pure wrapper guarantees identical behavior to the namespace implementation.
    return raf::ChargedECALveto(allParticles, electron, NeutronFD, veto_cut, starttime, Recalc_P_nFD, apply_PCAL_veto);
}
#pragma endregion

// NeutralECALveto functions --------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* NeutralECALveto function */
/**
 * @method NeutralECALveto
 *
 * Thin wrapper around `reco_analysis_functions::NeutralECALveto` (aliased as `raf::...`).
 *
 * Purpose:
 *   Preserve a stable `ParticleID` class API while keeping the implementation in the
 *   `reco_analysis_functions` namespace so it can be debugged and exercised directly
 *   from ROOT macros and lightweight test drivers.
 *
 * Output / semantics:
 *   Returns exactly what `raf::NeutralECALveto(...)` returns.
 *   No additional logic, side effects, or parameter changes are introduced here.
 */
bool ParticleID::NeutralECALveto(std::vector<clas12::region_part_ptr>& allParticles, clas12::region_part_ptr& electron, clas12::region_part_ptr neut, const double& veto_cut,
                                 const double& starttime, const bool& Recalc_P_nFD, const bool& apply_PCAL_veto, const double B, const double L) {
    // Forward all arguments verbatim to the implementation in `reco_analysis_functions`.
    return raf::NeutralECALveto(allParticles, electron, neut, veto_cut, starttime, Recalc_P_nFD, apply_PCAL_veto, B, L);
}
#pragma endregion

// ChargedParticleID function -------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ChargedParticleID function */
// The updated function for charged particle PID. Does distinguish between FD and CD particles when applies momentum threshold
// TODO: this assumes that FD and CD are the only options. What should we do for FT?
std::vector<int> ParticleID::ChargedParticleID(std::vector<clas12::region_part_ptr>& ChargedParticles, const DSCuts& FD_mom_th, const DSCuts& CD_mom_th) {
    std::vector<int> ChargedParticles_ind;

    for (int i = 0; i < ChargedParticles.size(); i++) {
        if (ChargedParticles[i]->par()->getCharge() == 0) {  // Check that the ChargedParticles's charge is zero
            bt::ExitWithError("ParticleID::ChargedParticleID", __FILE__, __LINE__, "Unable to identify particle - particle is neutral!");
        }

        double Momentum = ChargedParticles[i]->getP();

        if (ChargedParticles[i]->getRegion() == clas12::FD) {
            if (Momentum >= FD_mom_th.GetLowerCutConst()) { ChargedParticles_ind.push_back(i); }
        } else if (ChargedParticles[i]->getRegion() == clas12::CD) {
            if (Momentum >= CD_mom_th.GetLowerCutConst()) { ChargedParticles_ind.push_back(i); }
        }
    }

    return ChargedParticles_ind;
}

// The original function for charged particle PID. Does not distinguish between FD and CD particles when applies momentum threshold
std::vector<int> ParticleID::ChargedParticleID(std::vector<clas12::region_part_ptr>& ChargedParticles, const DSCuts& Momentum_th) {
    std::vector<int> ChargedParticles_ind;

    for (int i = 0; i < ChargedParticles.size(); i++) {
        if (ChargedParticles[i]->par()->getCharge() == 0) {  // Check that the particle's charge is zero
            bt::ExitWithError("ParticleID::ChargedParticleID", __FILE__, __LINE__, "Unable to identify particle - particle is neutral!");
        }

        double Momentum = ChargedParticles[i]->getP();

        if (Momentum >= Momentum_th.GetLowerCutConst()) { ChargedParticles_ind.push_back(i); }
        //        if ((Momentum >= Momentum_th.GetLowerCutConst()) && (Momentum <= Momentum_th.GetUpperCutConst())) { ChargedParticles_ind.push_back(i); }
    }

    return ChargedParticles_ind;
}
#pragma endregion

// FDNeutralParticleID function -----------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* FDNeutralParticleID function */

#pragma region /* Get neutrals by new definition (with neutron charged and neutral ECAL veto and ECAL edge cuts) */
/* The FDNeutralParticleID function gets neutrons or photons from the FD, according to the redefinition from Larry:
 * Neutron = a neutral particle (i.e., neutron or photon) in the FD with no PCal hit and with an ECal hit.
 * Photon = a neutral particle (i.e., neutron or photon) in the FD with a PCal hit. */
void ParticleID::FDNeutralParticleID(std::vector<clas12::region_part_ptr> allParticles, std::vector<clas12::region_part_ptr> electrons,             // clas12::region_part_ptr elements
                                     std::vector<int>& FD_Neutrons_within_PID_cuts, std::vector<int>& ID_Neutrons_FD, DSCuts& Neutron_momentum_th,  // FD neutrons
                                     std::vector<int>& FD_Photons_within_th, std::vector<int>& ID_Photons_FD, DSCuts& Photon_momentum_th,           // FD photons
                                     const bool& apply_charged_ECAL_veto, DSCuts& ChargedECALveto_veto_cut,                                         // Charged ECAL veto
                                     const bool& apply_neutral_ECAL_veto, DSCuts& NeutralECALveto_veto_cut,                                         // Neutral ECAL veto
                                     const double& starttime, const double& beamE,                                                                  // Other event parameters
                                     const double& ECAL_V_edge_cut, const double& ECAL_W_edge_cut,                                                  // ECAL edge cuts
                                     const bool& apply_nucleon_cuts                                                                                 // Nucleon cuts application flag
) {
    if (electrons.size() == 0) { bt::ExitWithError(__func__, __FILE__, __LINE__, "electrons.size() is zero, can't calculate neutron properties!"); }

    for (int& i : ID_Neutrons_FD) {  // Identify neutron above momentum threshold
        /* Particles that get in here are neutrons. Now we take neutrons who pass momentum cuts. */

        int NeutralPDG = allParticles[i]->par()->getPid();
        double Momentum = this->GetFDNeutronP(allParticles[i], electrons[0], starttime, apply_nucleon_cuts);
        auto [Neutron_with_PCAL_hit, Neutron_with_ECIN_hit, Neutron_with_ECOUT_hit, Neutron_ECAL_detlayer] = raf::CheckForECALHits(allParticles[i]);

#pragma region /* Safety checks */
        if (!((NeutralPDG == 22) || (NeutralPDG == 2112))) {
            bt::ExitWithError("ParticleID::FDNeutralParticleID", __FILE__, __LINE__, "neutron PDG is not 2112 or 22 (" + to_string(NeutralPDG) + ")");
        }

        if (Neutron_with_PCAL_hit) { bt::ExitWithError("ParticleID::FDNeutralParticleID", __FILE__, __LINE__, "redefined neutron is in the PCAL!"); }
#pragma endregion

        bool Neutron_pass_momentum_th = (Momentum >= Neutron_momentum_th.GetLowerCutConst() && Momentum <= Neutron_momentum_th.GetUpperCutConst());
        bool NeutronPassesChargedECALVeto, NeutronPassesNeutralECALVeto, Neutron_pass_ECAL_edge_cuts;

        if (apply_nucleon_cuts) {
            NeutronPassesChargedECALVeto =
                this->ChargedECALveto(allParticles, electrons[0], allParticles[i], ChargedECALveto_veto_cut.GetLowerCutConst(), starttime, apply_nucleon_cuts, apply_nucleon_cuts);
            NeutronPassesNeutralECALVeto =
                this->NeutralECALveto(allParticles, electrons[0], allParticles[i], NeutralECALveto_veto_cut.GetLowerCutConst(), starttime, apply_nucleon_cuts, apply_nucleon_cuts);

            Neutron_pass_ECAL_edge_cuts = (allParticles[i]->cal(Neutron_ECAL_detlayer)->getLv() > ECAL_V_edge_cut && allParticles[i]->cal(Neutron_ECAL_detlayer)->getLw() > ECAL_W_edge_cut);
        } else {
            NeutronPassesChargedECALVeto = NeutronPassesNeutralECALVeto = Neutron_pass_ECAL_edge_cuts = true;
        }

        /* Log neutrons above momentum cuts (given by Momentum_th): */
        if (Neutron_pass_momentum_th                                       // Neutron passes momentum threshold
            && (!apply_charged_ECAL_veto || NeutronPassesChargedECALVeto)  // Neutron passes charged ECAL veto (if applied)
            && (!apply_neutral_ECAL_veto || NeutronPassesNeutralECALVeto)  // Neutron passes neutral ECAL veto (if applied)
            && Neutron_pass_ECAL_edge_cuts                                 // Neutron passes ECAL edge cuts
        ) {
            FD_Neutrons_within_PID_cuts.push_back(i);
        }
    }  // end of loop over ID_Neutrons_FD vector

    for (int& i : ID_Photons_FD) {  // Identify photons above momentum threshold
        /* Particles that get in here are photons. Now we take photons who pass momentum cuts. */

        int NeutralPDG = allParticles[i]->par()->getPid();

#pragma region /* Safety check */
        if (NeutralPDG != 22) { bt::ExitWithError("ParticleID::FDNeutralParticleID", __FILE__, __LINE__, "photon PDG is not 22 (" + to_string(NeutralPDG) + ")"); }
#pragma endregion

        double Momentum = allParticles[i]->getP();

        /* Log photons above momentum cuts (given by Momentum_cuts): */
        if (Momentum >= Photon_momentum_th.GetLowerCutConst() && Momentum <= Photon_momentum_th.GetUpperCutConst()) { FD_Photons_within_th.push_back(i); }
    }  // end of loop over ID_Photons_FD vector
}
#pragma endregion

#pragma region /* Get neutrals by new definition (with neutron ECAL veto and ECAL edge cuts) */
/* The FDNeutralParticleID function gets neutrons or photons from the FD, according to the redefinition from Larry:
 * Neutron = a neutral particle (i.e., neutron or photon) in the FD with no PCal hit and with an ECal hit.
 * Photon = a neutral particle (i.e., neutron or photon) in the FD with a PCal hit. */
void ParticleID::FDNeutralParticleID(std::vector<clas12::region_part_ptr> allParticles, std::vector<clas12::region_part_ptr> electrons, std::vector<int>& FD_Neutrons_within_PID_cuts,
                                     std::vector<int>& ID_Neutrons_FD, DSCuts& Neutron_momentum_th, std::vector<int>& FD_Photons_within_th, std::vector<int>& ID_Photons_FD,
                                     DSCuts& Photon_momentum_th, DSCuts& Neutron_veto_cut, const double& starttime, const double& beamE, const double& ECAL_V_edge_cut,
                                     const double& ECAL_W_edge_cut, const bool& apply_nucleon_cuts) {
    if (electrons.size() == 0) { bt::ExitWithError(__func__, __FILE__, __LINE__, "electrons.size() is zero, can't calculate neutron properties!"); }

    for (int& i : ID_Neutrons_FD) {  // Identify neutron above momentum threshold
        /* Particles that get in here are neutrons. Now we take neutrons who pass momentum cuts. */

        int NeutralPDG = allParticles[i]->par()->getPid();
        double Momentum = this->GetFDNeutronP(allParticles[i], electrons[0], starttime, apply_nucleon_cuts);
        bool Neutron_with_PCAL_hit = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7);    // PCAL hit
        bool Neutron_with_ECIN_hit = (allParticles[i]->cal(clas12::ECIN)->getDetector() == 7);    // ECIN hit
        bool Neutron_with_ECOUT_hit = (allParticles[i]->cal(clas12::ECOUT)->getDetector() == 7);  // ECOUT hit
        auto Neutron_ECAL_detlayer = Neutron_with_ECIN_hit ? clas12::ECIN : clas12::ECOUT;        // find first layer of hit

#pragma region /* Safety checks */
        if (!((NeutralPDG == 22) || (NeutralPDG == 2112))) {
            bt::ExitWithError("ParticleID::FDNeutralParticleID", __FILE__, __LINE__, "neutron PDG is not 2112 or 22 (" + to_string(NeutralPDG) + ")");
        }

        if (Neutron_with_PCAL_hit) { bt::ExitWithError("ParticleID::FDNeutralParticleID", __FILE__, __LINE__, "redefined neutron is in the PCAL!!"); }
#pragma endregion

        bool Neutron_pass_momentum_th = (Momentum >= Neutron_momentum_th.GetLowerCutConst() && Momentum <= Neutron_momentum_th.GetUpperCutConst());
        bool Neutron_pass_ECAL_veto, Neutron_pass_ECAL_edge_cuts;

        if (apply_nucleon_cuts) {
            Neutron_pass_ECAL_veto = this->NeutronECAL_Cut_Veto(allParticles, electrons, beamE, i, Neutron_veto_cut.GetLowerCutConst());
            Neutron_pass_ECAL_edge_cuts = (allParticles[i]->cal(Neutron_ECAL_detlayer)->getLv() > ECAL_V_edge_cut && allParticles[i]->cal(Neutron_ECAL_detlayer)->getLw() > ECAL_W_edge_cut);
        } else {
            Neutron_pass_ECAL_veto = Neutron_pass_ECAL_edge_cuts = true;
        }

        /* Log neutrons above momentum cuts (given by Momentum_th): */
        if (Neutron_pass_momentum_th && Neutron_pass_ECAL_veto && Neutron_pass_ECAL_edge_cuts) { FD_Neutrons_within_PID_cuts.push_back(i); }
    }  // end of loop over ID_Neutrons_FD vector

    for (int& i : ID_Photons_FD) {  // Identify photons above momentum threshold
        /* Particles that get in here are photons. Now we take photons who pass momentum cuts. */

        int NeutralPDG = allParticles[i]->par()->getPid();

#pragma region /* Safety check */
        if (NeutralPDG != 22) { bt::ExitWithError("ParticleID::FDNeutralParticleID", __FILE__, __LINE__, "photon PDG is not 22 (" + to_string(NeutralPDG) + ")"); }
#pragma endregion

        double Momentum = allParticles[i]->getP();

        /* Log photons above momentum cuts (given by Momentum_cuts): */
        if (Momentum >= Photon_momentum_th.GetLowerCutConst() && Momentum <= Photon_momentum_th.GetUpperCutConst()) { FD_Photons_within_th.push_back(i); }
    }  // end of loop over ID_Photons_FD vector
}
#pragma endregion

#pragma region /* Get neutrals by new definition (ORIGINAL!) */
/* The FDNeutralParticleID function gets neutrons or photons from the FD, according to the definition from Larry:
 * Neutron = a neutral particle (i.e., neutron or photon) in the FD with no PCal hit and with an ECal hit.
 * Photon = a neutral particle (i.e., neutron or photon) in the FD with a PCal hit. */
void ParticleID::FDNeutralParticleID(std::vector<clas12::region_part_ptr> allParticles, std::vector<int>& FD_Neutrons_within_th, std::vector<int>& ID_Neutrons_FD,
                                     DSCuts& Neutron_momentum_th, std::vector<int>& FD_Photons_within_th, std::vector<int>& ID_Photons_FD, DSCuts& Photon_momentum_th,
                                     std::vector<clas12::region_part_ptr> electrons, const double& starttime, const bool& apply_nucleon_cuts) {
    if (electrons.size() == 0) { bt::ExitWithError(__func__, __FILE__, __LINE__, "electrons.size() is zero, can't calculate neutron properties!"); }

    for (int& i : ID_Neutrons_FD) {  // Identify neutron above momentum threshold
        /* Particles that get in here are neutrons. Now we take neutrons who pass momentum cuts. */

        int NeutralPDG = allParticles[i]->par()->getPid();

#pragma region /* Safety check */
        if (!((NeutralPDG == 22) || (NeutralPDG == 2112))) {
            bt::ExitWithError("ParticleID::FDNeutralParticleID", __FILE__, __LINE__, "neutron PDG is not 2112 or 22 (" + to_string(NeutralPDG) + ")");
        }
#pragma endregion

        double Momentum = this->GetFDNeutronP(allParticles[i], electrons[0], starttime, apply_nucleon_cuts);

        /* Log neutrons above momentum cuts (given by Momentum_th): */
        if (Momentum >= Neutron_momentum_th.GetLowerCutConst() && Momentum <= Neutron_momentum_th.GetUpperCutConst()) { FD_Neutrons_within_th.push_back(i); }
    }  // end of loop over ID_Neutrons_FD vector

    for (int& i : ID_Photons_FD) {  // Identify photons above momentum threshold
        /* Particles that get in here are photons. Now we take photons who pass momentum cuts. */

        int NeutralPDG = allParticles[i]->par()->getPid();

#pragma region /* Safety check */
        if (NeutralPDG != 22) { bt::ExitWithError("ParticleID::FDNeutralParticleID", __FILE__, __LINE__, "photon PDG is not 22 (" + to_string(NeutralPDG) + ")"); }
#pragma endregion

        double Momentum = allParticles[i]->getP();

        /* Log photons above momentum cuts (given by Momentum_cuts): */
        if (Momentum >= Photon_momentum_th.GetLowerCutConst() && Momentum <= Photon_momentum_th.GetUpperCutConst()) { FD_Photons_within_th.push_back(i); }
    }  // end of loop over ID_Photons_FD vector
}
#pragma endregion

#pragma region /* Get leading neutron (ORIGINAL!) */
int ParticleID::GetLnFDIndex(std::vector<clas12::region_part_ptr> allParticles, std::vector<int>& FD_Neutrons_within_th, std::vector<clas12::region_part_ptr> electrons,
                             const double& starttime, const bool& apply_nucleon_cuts) {
    bool PrintOut = false;

    if (electrons.size() == 0) { bt::ExitWithError(__func__, __FILE__, __LINE__, "electrons.size() is zero, can't calculate neutron properties!"); }

    double P_max = -1;
    int MaxPIndex = -1;

    bool PrintLog = (PrintOut && (FD_Neutrons_within_th.size() > 0));

    for (int& i : FD_Neutrons_within_th) {  // Identify the leading neutron
        double P_temp = this->GetFDNeutronP(allParticles[i], electrons[0], starttime, apply_nucleon_cuts);
        int NeutralPDG_temp = allParticles[i]->par()->getPid();

#pragma region /* Safety check */
        if (!((NeutralPDG_temp == 22) || (NeutralPDG_temp == 2112))) {
            bt::ExitWithError("ParticleID::GetLnFDIndex", __FILE__, __LINE__, "neutron PDG is not 2112 or 22 (" + to_string(NeutralPDG_temp) + ")");
        }
#pragma endregion

        if (P_temp >= P_max) {
            P_max = P_temp;
            MaxPIndex = i;
        }

        if (PrintOut) {
            int ParticlePDG_temp = allParticles[i]->par()->getPid();

            bool PCAL_hit_temp = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7);    // PCAL hit
            bool ECIN_hit_temp = (allParticles[i]->cal(clas12::ECIN)->getDetector() == 7);    // ECIN hit
            bool ECOUT_hit_temp = (allParticles[i]->cal(clas12::ECOUT)->getDetector() == 7);  // ECOUT hit

            cout << "P_temp = " << P_temp << " (i = " << i << ", PDG = " << ParticlePDG_temp << ", PCAL_hit = " << PCAL_hit_temp << ", ECIN_hit = " << ECIN_hit_temp
                 << ", ECOUT_hit = " << ECOUT_hit_temp << ")\n";
        }
    }

    if (PrintLog) {
        cout << "P_max = " << P_max << " (MaxPIndex = " << MaxPIndex << ")\n\n";
        cout << "==========================================================\n\n\n";
    }

    return MaxPIndex;
}
#pragma endregion

#pragma endregion

// GetCorrLnFDIndex function --------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GetCorrLnFDIndex function */
/* A function similar to GetLnFDIndex that selects the leading (LnFD) after correction.
   This function might be usefully if the neutron correction factor (1/(1 - mu)) is not increasing/desreasing monotonically! */
int ParticleID::GetCorrLnFDIndex(MomentumResolution& nRes, std::vector<clas12::region_part_ptr> allParticles, std::vector<int>& FD_Neutrons_within_th, clas12::region_part_ptr electron,
                                 const double& starttime, const bool& apply_nucleon_cuts, const bool& apply_nucleon_SmearAndCorr) {
    bool PrintOut = false;
    bool PrintOutCorr = false;
    bool PrintLog = (PrintOut && (FD_Neutrons_within_th.size() > 0));

    double P_max_ACorr = -1;
    int MaxPIndex_ACorr = -1;
    std::vector<double> FD_Neutrons_within_th_ACorr;

    // Create a vector of corrected neutron momenta, corresponding to the i-th neutron in FD_Neutrons_within_th:
    for (int i = 0; i < FD_Neutrons_within_th.size(); i++) {
        double P_temp = this->GetFDNeutronP(allParticles[FD_Neutrons_within_th.at(i)], electron, starttime, apply_nucleon_cuts);
        int NeutralPDG_temp = allParticles[FD_Neutrons_within_th.at(i)]->par()->getPid();

#pragma region /* Safety check */
        if (!((NeutralPDG_temp == 22) || (NeutralPDG_temp == 2112))) {
            bt::ExitWithError("ParticleID::GetCorrLnFDIndex", __FILE__, __LINE__, "neutron PDG is not 2112 or 22 (" + to_string(NeutralPDG_temp) + ")");
        }
#pragma endregion

        double P_temp_ACorr = nRes.NCorr(apply_nucleon_SmearAndCorr, P_temp);

        FD_Neutrons_within_th_ACorr.push_back(P_temp_ACorr);
    }

#pragma region /* Safety check */
    if (FD_Neutrons_within_th.size() != FD_Neutrons_within_th_ACorr.size()) {
        bt::ExitWithError("ParticleID::GetCorrLnFDIndex", __FILE__, __LINE__, "FD_Neutrons_within_th_ACorr have been filled incorrectly!");
    }
#pragma endregion

    for (int j = 0; j < FD_Neutrons_within_th_ACorr.size(); j++) {  // Identify the leading neutron after correction
        double P_temp_ACorr = FD_Neutrons_within_th_ACorr.at(j);

        if (P_temp_ACorr >= P_max_ACorr) {
            P_max_ACorr = P_temp_ACorr;
            MaxPIndex_ACorr = FD_Neutrons_within_th.at(j);
        }
    }

    if (PrintLog) {
        for (int i = 0; i < FD_Neutrons_within_th.size(); i++) {
            double P_temp0 = this->GetFDNeutronP(allParticles[FD_Neutrons_within_th.at(i)], electron, starttime, apply_nucleon_cuts);

            cout << "P_temp = " << P_temp0 << " (ind = " << FD_Neutrons_within_th.at(i) << ")\n";
        }

        cout << "\n";

        for (int i = 0; i < FD_Neutrons_within_th_ACorr.size(); i++) {
            double P_temp_ACorr0 = FD_Neutrons_within_th_ACorr.at(i);

            cout << "P_temp_ACorr = " << P_temp_ACorr0 << " (ind = " << FD_Neutrons_within_th.at(i) << ")\n";
        }

        cout << "\n";

        cout << "P_max_ACorr = " << P_max_ACorr << " (MaxPIndex_ACorr = " << MaxPIndex_ACorr << ")\n\n";
        cout << "==========================================================\n\n\n";
    }

    return MaxPIndex_ACorr;
}
#pragma endregion

// FDNeutralParticle functions ------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* FDNeutralParticle functions */
/* The NeutralParticleID function gets neutrons or photons from the FD, according to the definition from Larry:
 * Neutron = a neutral particle (i.e., neutron or photon) in the FD with no PCal hit and with an ECal hit.
 * Photon = a neutral particle (i.e., neutron or photon) in the FD with a PCal hit. */

/* FDNeutralParticle without ECAL veto */
void ParticleID::ReDefFDNeutrals(std::vector<clas12::region_part_ptr> allParticles, std::vector<int>& ReDef_FD_neutrons, std::vector<int>& ReDef_FD_photons) {
    for (int i = 0; i < allParticles.size(); i++) {
        int ParticlePDG = allParticles[i]->par()->getPid();

        if ((allParticles[i]->par()->getCharge() == 0) && (allParticles[i]->getRegion() == clas12::FD) && (ParticlePDG != 0)) {  // If particle is neutral and in the FD
            bool ParticleInPCAL = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7);                                      // PCAL hit
            bool ParticleInECIN = (allParticles[i]->cal(clas12::ECIN)->getDetector() == 7);                                      // ECIN hit
            bool ParticleInECOUT = (allParticles[i]->cal(clas12::ECOUT)->getDetector() == 7);                                    // ECOUT hit

            if ((ParticlePDG == 2112) || (ParticlePDG == 22)) {
                if (ParticleInPCAL) {
                    // if there's a 'photon' with a PCAL hit -> photon:
                    if (ParticlePDG == 22) { ReDef_FD_photons.push_back(i); }
                } else if (!ParticleInPCAL) {  // if there is a neutron or a 'photon' without a PCAL hit
                    // if there is either a ECIN or ECOUT hit -> neutron:
                    if (ParticleInECIN || ParticleInECOUT) { ReDef_FD_neutrons.push_back(i); }
                }
            }  // end of clas12root neutron or 'photon' if
        }  // end of neutral and in the FD if
    }  // end of loop over allParticles vector
}
#pragma endregion

// Get neutron momentum functions ---------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* Get neutron momentum functions */
double ParticleID::GetFDNeutronBeta(clas12::region_part_ptr& Neutron, clas12::region_part_ptr electron, const double& starttime, const bool& apply_nucleon_cuts) {
    return raf::CalcBetanFD(Neutron, electron, starttime, apply_nucleon_cuts);
}

// Get neutron momentum functions ---------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* Get neutron momentum functions */
double ParticleID::GetFDNeutronP(clas12::region_part_ptr& Neutron, clas12::region_part_ptr electron, const double& starttime, const bool& apply_nucleon_cuts) {
    return raf::CalcPnFD(Neutron, electron, starttime, apply_nucleon_cuts);
}

TVector3 ParticleID::GetFDNeutronP3v(clas12::region_part_ptr& Neutron, clas12::region_part_ptr electron, const double& starttime, const bool& apply_nucleon_cuts) {
    return raf::CalcPnFD3v(Neutron, electron, starttime, apply_nucleon_cuts);
}
#pragma endregion

// GetFDNeutrons functions ----------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GetFDNeutrons functions */
/* The GetFDNeutrons function gets neutrons from the FD, according to the definition from Larry:
 * Neutron = a neutral particle (i.e., neutron or photon) in the FD with no PCal hit and with an ECal hit. */

std::vector<int> ParticleID::GetFDNeutrons(std::vector<clas12::region_part_ptr>& allParticles, clas12::region_part_ptr electron, const double& starttime, const DSCuts& Momentum_cuts,
                                           const bool& apply_nucleon_cuts) {
    std::vector<int> Neutrons_indices_in_allParticles;

    for (int i = 0; i < allParticles.size(); i++) {
        int ParticlePDG = allParticles[i]->par()->getPid();

        if ((allParticles[i]->getRegion() == clas12::FD) && ((ParticlePDG == 2112) || (ParticlePDG == 22))) {  // if neutron/photon is in the FD
            bool inPCAL = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7);                            // PCAL hit
            bool inECIN = (allParticles[i]->cal(clas12::ECIN)->getDetector() == 7);                            // ECIN hit
            bool inECOUT = (allParticles[i]->cal(clas12::ECOUT)->getDetector() == 7);                          // ECOUT hit

            if (!inPCAL && (inECIN || inECOUT)) {  // if the criteria for a particle to be a neutron is upheld
                /* Particles that get in here are neutrons. Now we take neutrons who pass momentum cuts. */
                double Momentum = this->GetFDNeutronP(allParticles[i], electron, starttime, apply_nucleon_cuts);

                /* Log neutrons above momentum cuts (given by Momentum_cuts): */
                if ((Momentum >= Momentum_cuts.GetLowerCutConst()) && (Momentum <= Momentum_cuts.GetUpperCutConst())) { Neutrons_indices_in_allParticles.push_back(i); }
            }  // end of if neutron/photon hit the EC but not in PCAL
        }  // end of if neutron/photon is in the FD
    }  // end of loop over allparticle vector

    /* Return a vector of the neutrons in allParticles, according to the definition. */
    return Neutrons_indices_in_allParticles;
}
#pragma endregion

// GetFDPhotons functions -----------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GetFDPhotons functions */
/* The GetFDPhotons function gets photons from the FD, according to the definition from Larry:
 * Photon = a photon in the FD with a PCAL hit. */

std::vector<int> ParticleID::GetFDPhotons(std::vector<clas12::region_part_ptr>& allParticles, const DSCuts& Momentum_cuts) {
    std::vector<int> Photons_indices_in_allParticles;

    for (int i = 0; i < allParticles.size(); i++) {
        int ParticlePDG = allParticles[i]->par()->getPid();

        if ((allParticles[i]->getRegion() == clas12::FD) && (ParticlePDG == 22)) {   // if photon is in the FD
            bool inPCAL = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7);  // PCAL hit

            if (inPCAL) {  // if the criteria for a particle to be a photon is upheld
                /* Particles that get in here are photons. Now we take photons who pass momentum cuts. */
                double Momentum = allParticles[i]->getP();

                /* Log photons above momentum cuts (given by Momentum_cuts): */
                if ((Momentum >= Momentum_cuts.GetLowerCutConst()) && (Momentum <= Momentum_cuts.GetUpperCutConst())) { Photons_indices_in_allParticles.push_back(i); }
            }  // end of if photon hit the EC but not in PCAL
        }  // end of if photon is in the FD
    }  // end of loop over allparticle vector

    /* Return a vector of the photons in allParticles, according to the definition. */
    return Photons_indices_in_allParticles;
}
#pragma endregion

// GetGoodParticles functions -------------------------------------------------------------------------------------------------------------------------------------------

#pragma region                                                                                 /* GetGoodParticles functions */
std::vector<int> ParticleID::GetGoodParticles(std::vector<clas12::region_part_ptr>& Particle,  // particle
                                              const DSCuts& Momentum_cuts                      // corresponding momentum cuts
) {
    std::vector<int> GoodParticles;

    for (int i = 0; i < Particle.size(); i++) {
        double Momentum = Particle[i]->getP();

        if (Momentum >= Momentum_cuts.GetLowerCutConst()) { GoodParticles.push_back(i); }
        //        if ((Momentum >= Momentum_cuts.GetLowerCutConst()) && (Momentum <= Momentum_cuts.GetUpperCutConst())) { GoodParticles.push_back(i); }
    }

    return GoodParticles;
}
#pragma endregion

// GetGoodProtons functions ---------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GetGoodProtons functions */

#pragma region /* GetGoodProtons function */
std::vector<int> ParticleID::GetGoodProtons(const bool& apply_nucleon_cuts, std::vector<clas12::region_part_ptr>& protons, const std::vector<int>& IDProtons_ind,
                                            const DSCuts& Theta_p1_cuts_2p, const DSCuts& Theta_p2_cuts_2p, const DSCuts& dphi_p1_p2_2p) {
    std::vector<int> GoodProtons;  // good protons vector after the cuts

    /* Monitoring variables */
    bool TwoCutsPrintout = false;                   // set as true to print a massage when both cuts are applied
    bool Cut_sCTOFhp = false, Cut_dCDaFDd = false;  // variables to indicate which cut is applied

    std::vector<bool> AlreadyLogged(IDProtons_ind.size(), false);  // <<== Added: track if a proton was already logged

    for (int i = 0; i < IDProtons_ind.size(); i++) {
        if (AlreadyLogged[i]) continue;  // <<== Skip if already handled

        bool GoodProt = true;  // when GoodProt == true at the end of the loop, the proton's index will be logged

        if (apply_nucleon_cuts) {
            for (int j = i + 1; j < IDProtons_ind.size(); j++) {
                if ((protons[IDProtons_ind.at(i)]->getRegion() == clas12::CD) && (protons[IDProtons_ind.at(j)]->getRegion() == clas12::CD)) {  // if proton pair is in the CD only
                    /* Set hit positions in the CTOF, and position difference: */
                    TVector3 p1_hit_pos, p2_hit_pos, pos_diff;
                    p1_hit_pos.SetXYZ(protons[IDProtons_ind.at(i)]->sci(clas12::CTOF)->getX(), protons[IDProtons_ind.at(i)]->sci(clas12::CTOF)->getY(),
                                      protons[IDProtons_ind.at(i)]->sci(clas12::CTOF)->getZ());
                    p2_hit_pos.SetXYZ(protons[IDProtons_ind.at(j)]->sci(clas12::CTOF)->getX(), protons[IDProtons_ind.at(j)]->sci(clas12::CTOF)->getY(),
                                      protons[IDProtons_ind.at(j)]->sci(clas12::CTOF)->getZ());
                    pos_diff.SetXYZ(p1_hit_pos.Px() - p2_hit_pos.Px(), p1_hit_pos.Py() - p2_hit_pos.Py(), p1_hit_pos.Pz() - p2_hit_pos.Pz());

                    if (pos_diff.Mag() == 0) {  // if protons have the same hit position
                        /* Same hit position for protons i and j - don't log proton i (j will be logged later as a single proton) */
                        GoodProt = false;    // don't log proton i
                        Cut_sCTOFhp = true;  // monitor sCTOFhp
                    }
                } else if (((protons[IDProtons_ind.at(i)]->getRegion() == clas12::FD) && (protons[IDProtons_ind.at(j)]->getRegion() == clas12::CD)) ||
                           ((protons[IDProtons_ind.at(i)]->getRegion() == clas12::CD) && (protons[IDProtons_ind.at(j)]->getRegion() == clas12::FD))) {  // if proton pair CD and FD
                    double Theta_p_i = am::RadToDeg(protons[IDProtons_ind.at(i)]->getTheta());
                    double Theta_p_j = am::RadToDeg(protons[IDProtons_ind.at(j)]->getTheta());
                    double dPhi = am::CalcdPhi2(protons[IDProtons_ind.at(i)], protons[IDProtons_ind.at(j)]);

                    bool p_i_around_40 = (fabs(Theta_p_i - Theta_p1_cuts_2p.GetMeanConst()) < Theta_p1_cuts_2p.GetUpperCutConst());
                    bool p_j_around_40 = (fabs(Theta_p_j - Theta_p2_cuts_2p.GetMeanConst()) < Theta_p2_cuts_2p.GetUpperCutConst());
                    bool small_dPhi = (fabs(dPhi - dphi_p1_p2_2p.GetMeanConst()) < dphi_p1_p2_2p.GetUpperCutConst());

                    if ((p_i_around_40 && p_j_around_40) && small_dPhi) {
                        /* Same angles for protons i and j on the border of the CD-FD */

                        // <<== Modified part START
                        int FD_index = (protons[IDProtons_ind.at(i)]->getRegion() == clas12::FD) ? i : j;
                        int CD_index = (protons[IDProtons_ind.at(i)]->getRegion() == clas12::CD) ? i : j;

                        if (!AlreadyLogged[FD_index]) {
                            GoodProtons.push_back(IDProtons_ind.at(FD_index));
                            AlreadyLogged[FD_index] = true;
                        }
                        AlreadyLogged[CD_index] = true;
                        GoodProt = false;  // proton i won't be logged manually later
                        // <<== Modified part END

                        Cut_dCDaFDd = true;  // monitor dCDaFDd
                    }
                }
            }
        }

        if (GoodProt && !AlreadyLogged[i]) {
            GoodProtons.push_back(IDProtons_ind.at(i));
            AlreadyLogged[i] = true;
        }
    }

#pragma region /* Safety checks */
    if (!apply_nucleon_cuts && (GoodProtons.size() != IDProtons_ind.size())) {
        bt::ExitWithError("ParticleID::GetGoodProtons", __FILE__, __LINE__, "GoodProtons and IDProtons_ind are not the same withot neucleon cut!");
    }

    if (GoodProtons.size() > IDProtons_ind.size()) { bt::ExitWithError("ParticleID::GetGoodProtons", __FILE__, __LINE__, "GoodProtons.size() can't be greater than IDProtons_ind.size()!"); }
#pragma endregion

#pragma region /* Monitoring printout */
    if (TwoCutsPrintout && Cut_sCTOFhp && Cut_dCDaFDd) {
        cout << "\n\nGetGoodProtons(): We have a duble cut!\n";
        cout << "IDProtons_ind.size() = " << IDProtons_ind.size() << "\n";
        cout << "GoodProtons.size() = " << GoodProtons.size() << "\n\n\n";
    }

#pragma endregion

    return GoodProtons;
}
#pragma endregion

#pragma region /* SetGPMonitoringPlots function */
void ParticleID::SetGPMonitoringPlots(const bool& GoodProtonsMonitorPlots, std::string CToF_hits_monitoring_2p_Directory, std::string Double_detection_monitoring_2p_Directory) {
    if (GoodProtonsMonitorPlots) {
        // Monitoring histograms definitions ----------------------------------------------------------------------------------------------------------------------------

#pragma region /* Monitoring histograms definitions */

        // Theta_p1_p2 vs. TOF1-TOF2 plots (2p, CD only) ----------------------------------------------------------------------------------------------------------------

#pragma region /* Theta_p1_p2 vs. TOF1-TOF2 plots (2p, CD only) */
        hdTheta_pi_pj_VS_ToFi_ToFj_BC_2idp_2p =
            hPlot2D("2p", "CD-CTOF", "#theta_{p_{i},p_{j}} vs. ToF_{i}-ToF_{j} for 2 id. p. BC", "#theta_{p_{i},p_{j}} vs. ToF_{i}-ToF_{j} between proton pairs for 2 id. protons BC",
                    "#theta_{p_{i},p_{j}} [Deg]", "ToF_{i}-ToF_{j} [ns]", CToF_hits_monitoring_2p_Directory, "01a_Theta_pi_pj_VS_ToFi-ToFj_2idp_BC_2p", 0, 180, -3, 3, 150, 150);
        hdTheta_pi_pj_VS_ToFi_ToFj_RE_2idp_2p =
            hPlot2D("2p", "CD-CTOF", "#theta_{p_{i},p_{j}} vs. ToF_{i}-ToF_{j} for 2 id. p. RE", "#theta_{p_{i},p_{j}} vs. ToF_{i}-ToF_{j} between proton pairs for 2 id. protons RE",
                    "#theta_{p_{i},p_{j}} [Deg]", "ToF_{i}-ToF_{j} [ns]", CToF_hits_monitoring_2p_Directory, "01b_Theta_pi_pj_VS_ToFi-ToFj_2idp_RE_2p", 0, 180, -3, 3, 150, 150);
        hdTheta_pi_pj_VS_ToFi_ToFj_BC_3idp_2p =
            hPlot2D("2p", "CD-CTOF", "#theta_{p_{i},p_{j}} vs. ToF_{i}-ToF_{j} for 3 id. p. BC", "#theta_{p_{i},p_{j}} vs. ToF_{i}-ToF_{j} between proton pairs for 3 id. protons BC",
                    "#theta_{p_{i},p_{j}} [Deg]", "ToF_{i}-ToF_{j} [ns]", CToF_hits_monitoring_2p_Directory, "02a_Theta_pi_pj_VS_ToFi-ToFj_3idp_BC_2p", 0, 180, -3, 3, 150, 150);
        hdTheta_pi_pj_VS_ToFi_ToFj_AE_3idp_2p =
            hPlot2D("2p", "CD-CTOF", "#theta_{p_{i},p_{j}} vs. ToF_{i}-ToF_{j} for 3 id. p. AE", "#theta_{p_{i},p_{j}} vs. ToF_{i}-ToF_{j} between proton pairs for 3 id. protons AE",
                    "#theta_{p_{i},p_{j}} [Deg]", "ToF_{i}-ToF_{j} [ns]", CToF_hits_monitoring_2p_Directory, "02b_Theta_pi_pj_VS_ToFi-ToFj_3idp_RE_2p", 0, 180, -3, 3, 150, 150);
        hdTheta_pi_pj_VS_ToFi_ToFj_BC_4idp_2p =
            hPlot2D("2p", "CD-CTOF", "#theta_{p_{i},p_{j}} vs. ToF_{i}-ToF_{j} for 4 id. p. BC", "#theta_{p_{i},p_{j}} vs. ToF_{i}-ToF_{j} between proton pairs for 4 id. protons BC",
                    "#theta_{p_{i},p_{j}} [Deg]", "ToF_{i}-ToF_{j} [ns]", CToF_hits_monitoring_2p_Directory, "03a_Theta_pi_pj_VS_ToFi-ToFj_4idp_BC_2p", 0, 180, -3, 3, 150, 150);
        hdTheta_pi_pj_VS_ToFi_ToFj_AE_4idp_2p =
            hPlot2D("2p", "CD-CTOF", "#theta_{p_{i},p_{j}} vs. ToF_{i}-ToF_{j} for 4 id. p. AE", "#theta_{p_{i},p_{j}} vs. ToF_{i}-ToF_{j} between proton pairs for 4 id. protons AE",
                    "#theta_{p_{i},p_{j}} [Deg]", "ToF_{i}-ToF_{j} [ns]", CToF_hits_monitoring_2p_Directory, "03b_Theta_pi_pj_VS_ToFi-ToFj_4idp_RE_2p", 0, 180, -3, 3, 150, 150);
#pragma endregion

        // Theta_p1_p2 vs. position1-position2 plots (2p, CD only) ------------------------------------------------------------------------------------------------------

#pragma region /* Theta_p1_p2 vs. position1-position2 plots (2p, CD only) */
        hTheta_pi_pj_VS_Posi_Posj_BC_2idp_2p =
            hPlot2D("2p", "CD-CTOF", "#theta_{p_{i},p_{j}} vs. Position_{i}-Position_{j} for 2 id. p. BC",
                    "#theta_{p_{i},p_{j}} vs. Position_{i}-Position_{j} between proton pairs for 2 id. protons BC", "#theta_{p_{i},p_{j}} [Deg]", "Position_{i}-Position_{j} [cm]",
                    CToF_hits_monitoring_2p_Directory, "04a_Theta_pi_pj_VS_Posi-Posj_2idp_BC_2p", 0, 180, 0, 100, 150, 150);
        hTheta_pi_pj_VS_Posi_Posj_RE_2idp_2p =
            hPlot2D("2p", "CD-CTOF", "#theta_{p_{i},p_{j}} vs. Position_{i}-Position_{j} for 2 id. p. RE",
                    "#theta_{p_{i},p_{j}} vs. Position_{i}-Position_{j} between proton pairs for 2 id. protons RE", "#theta_{p_{i},p_{j}} [Deg]", "Position_{i}-Position_{j} [cm]",
                    CToF_hits_monitoring_2p_Directory, "04b_Theta_pi_pj_VS_Posi-Posj_2idp_RE_2p", 0, 180, 0, 100, 150, 150);
        hTheta_pi_pj_VS_Posi_Posj_BC_3idp_2p =
            hPlot2D("2p", "CD-CTOF", "#theta_{p_{i},p_{j}} vs. Position_{i}-Position_{j} for 3 id. p. BC",
                    "#theta_{p_{i},p_{j}} vs. Position_{i}-Position_{j} between proton pairs for 3 id. protons BC", "#theta_{p_{i},p_{j}} [Deg]", "Position_{i}-Position_{j} [cm]",
                    CToF_hits_monitoring_2p_Directory, "05a_Theta_pi_pj_VS_Posi-Posj_3idp_BC_2p", 0, 180, 0, 100, 150, 150);
        hTheta_pi_pj_VS_Posi_Posj_AE_3idp_2p =
            hPlot2D("2p", "CD-CTOF", "#theta_{p_{i},p_{j}} vs. Position_{i}-Position_{j} for 3 id. p. AE",
                    "#theta_{p_{i},p_{j}} vs. Position_{i}-Position_{j} between proton pairs for 3 id. protons AE", "#theta_{p_{i},p_{j}} [Deg]", "Position_{i}-Position_{j} [cm]",
                    CToF_hits_monitoring_2p_Directory, "05b_Theta_pi_pj_VS_Posi-Posj_3idp_RE_2p", 0, 180, 0, 100, 150, 150);
        hTheta_pi_pj_VS_Posi_Posj_BC_4idp_2p =
            hPlot2D("2p", "CD-CTOF", "#theta_{p_{i},p_{j}} vs. Position_{i}-Position_{j} for 4 id. p. BC",
                    "#theta_{p_{i},p_{j}} vs. Position_{i}-Position_{j} between proton pairs for 4 id. protons BC", "#theta_{p_{i},p_{j}} [Deg]", "Position_{i}-Position_{j} [cm]",
                    CToF_hits_monitoring_2p_Directory, "06a_Theta_pi_pj_VS_Posi-Posj_4idp_BC_2p", 0, 180, 0, 100, 150, 150);
        hTheta_pi_pj_VS_Posi_Posj_AE_4idp_2p =
            hPlot2D("2p", "CD-CTOF", "#theta_{p_{i},p_{j}} vs. Position_{i}-Position_{j} for 4 id. p. AE",
                    "#theta_{p_{i},p_{j}} vs. Position_{i}-Position_{j} between proton pairs for 4 id. protons AE", "#theta_{p_{i},p_{j}} [Deg]", "Position_{i}-Position_{j} [cm]",
                    CToF_hits_monitoring_2p_Directory, "06b_Theta_pi_pj_VS_Posi-Posj_4idp_RE_2p", 0, 180, 0, 100, 150, 150);
#pragma endregion

        // Theta_p1_vs_Theta_p2 for Theta_p1_p2 < 20 (2p, CD & FD) ------------------------------------------------------------------------------------------------------

#pragma region /* Theta_p1_vs_Theta_p2 for Theta_p1_p2 < 20 (2p, CD & FD) */
        hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_BC_2idp_2p =
            new TH2D("#theta_{p_{i}} vs. #theta_{p_{j}} for #theta_{p_{i},p_{j}}<20#circ  for 2 id. p. BC (All Int., 2p)",
                     "#theta_{p_{i}} vs. #theta_{p_{j}} for #theta_{p_{i},p_{j}}<20#circ between proton pairs for 2 id. protons BC (All Int., 2p);#theta_{p_{j}} [Deg];#theta_{p_{i}} [Deg];",
                     150, 30, 50, 150, 30, 50);
        hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_BC_2idp_2p_Dir = Double_detection_monitoring_2p_Directory;

        hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_RE_2idp_2p =
            new TH2D("#theta_{p_{i}} vs. #theta_{p_{j}} for #theta_{p_{i},p_{j}}<20#circ  for 2 id. p. RE (All Int., 2p)",
                     "#theta_{p_{i}} vs. #theta_{p_{j}} for #theta_{p_{i},p_{j}}<20#circ between proton pairs for 2 id. protons RE (All Int., 2p);#theta_{p_{j}} [Deg];#theta_{p_{i}} [Deg];",
                     150, 30, 50, 150, 30, 50);
        hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_RE_2idp_2p_Dir = Double_detection_monitoring_2p_Directory;

        hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_BC_3idp_2p =
            new TH2D("#theta_{p_{i}} vs. #theta_{p_{j}} for #theta_{p_{i},p_{j}}<20#circ  for 3 id. p. BC (All Int., 2p)",
                     "#theta_{p_{i}} vs. #theta_{p_{j}} for #theta_{p_{i},p_{j}}<20#circ between proton pairs for 3 id. protons BC (All Int., 2p);#theta_{p_{j}} [Deg];#theta_{p_{i}} [Deg];",
                     150, 30, 50, 150, 30, 50);
        hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_BC_3idp_2p_Dir = Double_detection_monitoring_2p_Directory;

        hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_AE_3idp_2p =
            new TH2D("#theta_{p_{i}} vs. #theta_{p_{j}} for #theta_{p_{i},p_{j}}<20#circ  for 3 id. p. AE (All Int., 2p)",
                     "#theta_{p_{i}} vs. #theta_{p_{j}} for #theta_{p_{i},p_{j}}<20#circ between proton pairs for 3 id. protons AE (All Int., 2p);#theta_{p_{j}} [Deg];#theta_{p_{i}} [Deg];",
                     150, 30, 50, 150, 30, 50);
        hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_AE_3idp_2p_Dir = Double_detection_monitoring_2p_Directory;

        hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_BC_4idp_2p =
            new TH2D("#theta_{p_{i}} vs. #theta_{p_{j}} for #theta_{p_{i},p_{j}}<20#circ  for 4 id. p. BC (All Int., 2p)",
                     "#theta_{p_{i}} vs. #theta_{p_{j}} for #theta_{p_{i},p_{j}}<20#circ between proton pairs for 4 id. protons BC (All Int., 2p);#theta_{p_{j}} [Deg];#theta_{p_{i}} [Deg];",
                     150, 30, 50, 150, 30, 50);
        hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_BC_4idp_2p_Dir = Double_detection_monitoring_2p_Directory;

        hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_AE_4idp_2p =
            new TH2D("#theta_{p_{i}} vs. #theta_{p_{j}} for #theta_{p_{i},p_{j}}<20#circ  for 4 id. p. AE (All Int., 2p)",
                     "#theta_{p_{i}} vs. #theta_{p_{j}} for #theta_{p_{i},p_{j}}<20#circ between proton pairs for 4 id. protons AE (All Int., 2p);#theta_{p_{j}} [Deg];#theta_{p_{i}} [Deg];",
                     150, 30, 50, 150, 30, 50);
        hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_AE_4idp_2p_Dir = Double_detection_monitoring_2p_Directory;
#pragma endregion

        // Theta_p1_vs_Theta_p2 for every Theta_p1_p2 (2p, CD & FD) -----------------------------------------------------------------------------------------------------

#pragma region /* Theta_p1_vs_Theta_p2 for Theta_p1_p2 < 20 (2p, CD & FD) */
        hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_BC_2idp_2p = new TH2D(
            "#theta_{p_{i}} vs. #theta_{p_{j}} #forall#theta_{p_{i},p_{j}} for 2 id. p. BC (All Int., 2p)",
            "#theta_{p_{i}} vs. #theta_{p_{j}} for every #theta_{p_{i},p_{j}}<20#circ between proton pairs for 2 id. protons BC (All Int., 2p);#theta_{p_{j}} [Deg];#theta_{p_{i}} [Deg];",
            150, 30, 50, 150, 30, 50);
        hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_BC_2idp_2p_Dir = Double_detection_monitoring_2p_Directory;

        hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_RE_2idp_2p = new TH2D(
            "#theta_{p_{i}} vs. #theta_{p_{j}} #forall#theta_{p_{i},p_{j}} for 2 id. p. RE (All Int., 2p)",
            "#theta_{p_{i}} vs. #theta_{p_{j}} for every #theta_{p_{i},p_{j}}<20#circ between proton pairs for 2 id. protons RE (All Int., 2p);#theta_{p_{j}} [Deg];#theta_{p_{i}} [Deg];",
            150, 30, 50, 150, 30, 50);
        hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_RE_2idp_2p_Dir = Double_detection_monitoring_2p_Directory;

        hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_BC_3idp_2p = new TH2D(
            "#theta_{p_{i}} vs. #theta_{p_{j}} #forall#theta_{p_{i},p_{j}} for 3 id. p. BC (All Int., 2p)",
            "#theta_{p_{i}} vs. #theta_{p_{j}} for every #theta_{p_{i},p_{j}}<20#circ between proton pairs for 3 id. protons BC (All Int., 2p);#theta_{p_{j}} [Deg];#theta_{p_{i}} [Deg];",
            150, 30, 50, 150, 30, 50);
        hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_BC_3idp_2p_Dir = Double_detection_monitoring_2p_Directory;

        hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_AE_3idp_2p = new TH2D(
            "#theta_{p_{i}} vs. #theta_{p_{j}} #forall#theta_{p_{i},p_{j}} for 3 id. p. AE (All Int., 2p)",
            "#theta_{p_{i}} vs. #theta_{p_{j}} for every #theta_{p_{i},p_{j}}<20#circ between proton pairs for 3 id. protons AE (All Int., 2p);#theta_{p_{j}} [Deg];#theta_{p_{i}} [Deg];",
            150, 30, 50, 150, 30, 50);
        hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_AE_3idp_2p_Dir = Double_detection_monitoring_2p_Directory;

        hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_BC_4idp_2p = new TH2D(
            "#theta_{p_{i}} vs. #theta_{p_{j}} #forall#theta_{p_{i},p_{j}} for 4 id. p. BC (All Int., 2p)",
            "#theta_{p_{i}} vs. #theta_{p_{j}} for every #theta_{p_{i},p_{j}}<20#circ between proton pairs for 4 id. protons BC (All Int., 2p);#theta_{p_{j}} [Deg];#theta_{p_{i}} [Deg];",
            150, 30, 50, 150, 30, 50);
        hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_BC_4idp_2p_Dir = Double_detection_monitoring_2p_Directory;

        hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_AE_4idp_2p = new TH2D(
            "#theta_{p_{i}} vs. #theta_{p_{j}} #forall#theta_{p_{i},p_{j}} for 4 id. p. AE (All Int., 2p)",
            "#theta_{p_{i}} vs. #theta_{p_{j}} for every #theta_{p_{i},p_{j}}<20#circ between proton pairs for 4 id. protons AE (All Int., 2p);#theta_{p_{j}} [Deg];#theta_{p_{i}} [Deg];",
            150, 30, 50, 150, 30, 50);
        hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_AE_4idp_2p_Dir = Double_detection_monitoring_2p_Directory;
#pragma endregion

#pragma endregion
    }
}
#pragma endregion

#pragma region /* GPMonitoring function */
void ParticleID::GPMonitoring(const bool& GoodProtonsMonitorPlots, std::vector<clas12::region_part_ptr>& protons, const std::vector<int>& IDProtons_ind, const std::vector<int>& Protons_ind,
                              const DSCuts& Theta_p1_cuts_2p, const DSCuts& Theta_p2_cuts_2p, const DSCuts& dphi_p1_p2_2p, const double& Weight) {
    if (GoodProtonsMonitorPlots) {
        for (int i = 0; i < IDProtons_ind.size(); i++) {
            bool Is_sCTOFhp = false, Is_dCDaFDd = false;  // variables to indicate which cut is applied

            auto proton_i_2p = protons[IDProtons_ind.at(i)];
            TVector3 proton_i_2p_2p_3v;
            proton_i_2p_2p_3v.SetMagThetaPhi(proton_i_2p->getP(), proton_i_2p->getTheta(), proton_i_2p->getPhi());  // proton i in protons vector
            double Theta_pi = am::RadToDeg(proton_i_2p->getTheta());
            double Phi_pi = am::RadToDeg(proton_i_2p->getPhi());  // Theta_pi; Phi_pi in deg

            for (int j = i + 1; j < IDProtons_ind.size(); j++) {
                auto proton_j_2p = protons[IDProtons_ind.at(j)];
                TVector3 proton_j_2p_2p_3v;
                proton_j_2p_2p_3v.SetMagThetaPhi(proton_j_2p->getP(), proton_j_2p->getTheta(),
                                                 proton_j_2p->getPhi());  // proton j in protons vector
                double Theta_pj = am::RadToDeg(proton_j_2p->getTheta());
                double Phi_pj = am::RadToDeg(proton_j_2p->getPhi());  // Theta_pi; Phi_pi in deg

                double Theta_pi_pj_2p =
                    am::RadToDeg(acos((proton_i_2p_2p_3v.Px() * proton_j_2p_2p_3v.Px() + proton_i_2p_2p_3v.Py() * proton_j_2p_2p_3v.Py() + proton_i_2p_2p_3v.Pz() * proton_j_2p_2p_3v.Pz()) /
                                      (proton_i_2p_2p_3v.Mag() * proton_j_2p_2p_3v.Mag())));  // Theta_pi_pj_2p in deg

                if ((proton_i_2p->getRegion() == clas12::CD) && (proton_j_2p->getRegion() == clas12::CD)) {  // if both 2p protons are in the CD
                    TVector3 pi_hit_pos, pj_hit_pos, pos_diff_ij;
                    pi_hit_pos.SetXYZ(proton_i_2p->sci(clas12::CTOF)->getX(), proton_i_2p->sci(clas12::CTOF)->getY(), proton_i_2p->sci(clas12::CTOF)->getZ());
                    pj_hit_pos.SetXYZ(proton_j_2p->sci(clas12::CTOF)->getX(), proton_j_2p->sci(clas12::CTOF)->getY(), proton_j_2p->sci(clas12::CTOF)->getZ());

                    pos_diff_ij.SetXYZ(pi_hit_pos.Px() - pj_hit_pos.Px(), pi_hit_pos.Py() - pj_hit_pos.Py(), pi_hit_pos.Pz() - pj_hit_pos.Pz());
                    double time_diff_ij = proton_i_2p->getTime() - proton_j_2p->getTime();

                    if (IDProtons_ind.size() == 2) {
                        hdTheta_pi_pj_VS_ToFi_ToFj_BC_2idp_2p.hFill(Theta_pi_pj_2p, time_diff_ij, Weight);
                        hTheta_pi_pj_VS_Posi_Posj_BC_2idp_2p.hFill(Theta_pi_pj_2p, pos_diff_ij.Mag(), Weight);

                        if (pos_diff_ij.Mag() == 0) {
                            ++num_of_RM_2p_events_sCTOFhp;
                            hdTheta_pi_pj_VS_ToFi_ToFj_RE_2idp_2p.hFill(Theta_pi_pj_2p, time_diff_ij, Weight);
                            hTheta_pi_pj_VS_Posi_Posj_RE_2idp_2p.hFill(Theta_pi_pj_2p, pos_diff_ij.Mag(), Weight);
                        }
                    }

                    if (IDProtons_ind.size() == 3 && Protons_ind.size() == 2) {
                        hdTheta_pi_pj_VS_ToFi_ToFj_BC_3idp_2p.hFill(Theta_pi_pj_2p, time_diff_ij, Weight);
                        hTheta_pi_pj_VS_Posi_Posj_BC_3idp_2p.hFill(Theta_pi_pj_2p, pos_diff_ij.Mag(), Weight);

                        if (pos_diff_ij.Mag() == 0) {
                            Is_sCTOFhp = true;
                            ++num_of_AD_2p_events_from_3p_sCTOFhp;
                            hdTheta_pi_pj_VS_ToFi_ToFj_AE_3idp_2p.hFill(Theta_pi_pj_2p, time_diff_ij, Weight);
                            hTheta_pi_pj_VS_Posi_Posj_AE_3idp_2p.hFill(Theta_pi_pj_2p, pos_diff_ij.Mag(), Weight);
                        }
                    }

                    if (IDProtons_ind.size() == 4 && Protons_ind.size() == 2) {
                        hdTheta_pi_pj_VS_ToFi_ToFj_BC_4idp_2p.hFill(Theta_pi_pj_2p, time_diff_ij, Weight);
                        hTheta_pi_pj_VS_Posi_Posj_BC_4idp_2p.hFill(Theta_pi_pj_2p, pos_diff_ij.Mag(), Weight);

                        if (pos_diff_ij.Mag() == 0) {
                            Is_sCTOFhp = true;
                            ++num_of_AD_2p_events_from_4p_sCTOFhp;
                            hdTheta_pi_pj_VS_ToFi_ToFj_AE_4idp_2p.hFill(Theta_pi_pj_2p, time_diff_ij, Weight);
                            hTheta_pi_pj_VS_Posi_Posj_AE_4idp_2p.hFill(Theta_pi_pj_2p, pos_diff_ij.Mag(), Weight);
                        }
                    }

                    if (IDProtons_ind.size() >= 5 && Protons_ind.size() == 2) {
                        if (pos_diff_ij.Mag() == 0) {
                            Is_sCTOFhp = true;
                            ++num_of_AD_2p_events_from_Xp_sCTOFhp;
                        }
                    }
                } else if (((proton_i_2p->getRegion() == clas12::FD) && (proton_j_2p->getRegion() == clas12::CD)) ||
                           ((proton_i_2p->getRegion() == clas12::CD) && (proton_j_2p->getRegion() == clas12::FD))) {
                    double dPhi_ij_2p = am::CalcdPhi2(protons[IDProtons_ind.at(i)], protons[IDProtons_ind.at(j)]);  // dPhi_ij_2p in deg

                    bool p_i_around_40 = (fabs(Theta_pi - Theta_p1_cuts_2p.GetMeanConst()) < Theta_p1_cuts_2p.GetUpperCutConst());
                    bool p_j_around_40 = (fabs(Theta_pj - Theta_p2_cuts_2p.GetMeanConst()) < Theta_p2_cuts_2p.GetUpperCutConst());
                    bool small_dPhi = (fabs(dPhi_ij_2p - dphi_p1_p2_2p.GetMeanConst()) < dphi_p1_p2_2p.GetUpperCutConst());

                    if (IDProtons_ind.size() == 2) {
                        if (Theta_pi_pj_2p < 20) {
                            hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_BC_2idp_2p->Fill(Theta_pj, Theta_pi);

                            if ((p_i_around_40 && p_j_around_40) && small_dPhi) { hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_RE_2idp_2p->Fill(Theta_pj, Theta_pi); }
                        }

                        hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_BC_2idp_2p->Fill(Theta_pj, Theta_pi);

                        if ((p_i_around_40 && p_j_around_40) && small_dPhi) {
                            ++num_of_RM_2p_events_dCDaFDd;
                            hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_RE_2idp_2p->Fill(Theta_pj, Theta_pi);
                        }
                    }

                    if (IDProtons_ind.size() == 3 && Protons_ind.size() == 2) {
                        if (Theta_pi_pj_2p < 20) {
                            hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_BC_3idp_2p->Fill(Theta_pj, Theta_pi);

                            if ((p_i_around_40 && p_j_around_40) && small_dPhi) { hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_AE_3idp_2p->Fill(Theta_pj, Theta_pi); }
                        }

                        hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_BC_3idp_2p->Fill(Theta_pj, Theta_pi);

                        if ((p_i_around_40 && p_j_around_40) && small_dPhi) {
                            Is_dCDaFDd = true;
                            ++num_of_AD_2p_events_from_3p_dCDaFDd;
                            hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_AE_3idp_2p->Fill(Theta_pj, Theta_pi);
                        }
                    }

                    if (IDProtons_ind.size() == 4 && Protons_ind.size() == 2) {
                        if (Theta_pi_pj_2p < 20) {
                            hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_BC_4idp_2p->Fill(Theta_pj, Theta_pi);

                            if ((p_i_around_40 && p_j_around_40) && small_dPhi) { hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_AE_4idp_2p->Fill(Theta_pj, Theta_pi); }
                        }

                        hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_BC_4idp_2p->Fill(Theta_pj, Theta_pi);

                        if ((p_i_around_40 && p_j_around_40) && small_dPhi) {
                            Is_dCDaFDd = true;
                            ++num_of_AD_2p_events_from_4p_dCDaFDd;
                            hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_AE_4idp_2p->Fill(Theta_pj, Theta_pi);
                        }
                    }

                    if (IDProtons_ind.size() >= 5 && Protons_ind.size() == 2) {
                        if ((p_i_around_40 && p_j_around_40) && small_dPhi) {
                            Is_dCDaFDd = true;
                            ++num_of_AD_2p_events_from_Xp_dCDaFDd;
                        }
                    }
                }
            }  // end of second for loop over IDProtons_ind (with j)

            if (Is_sCTOFhp && Is_dCDaFDd) { ++num_of_AD_2p_events_from_mixed_sCTOFhp_dCDaFDd; }
        }  // end of first for loop over IDProtons_ind (with i)
    }
}
#pragma endregion

#pragma endregion

// nParticleID functions ------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* nParticleID functions */
/* The NeutralParticleID function gets neutrons or photons from the FD, according to the definition from Larry:
 * Neutron = a neutral particle (i.e., neutron or photon) in the FD with no PCal hit and with an ECal hit.
 * Photon = a neutral particle (i.e., neutron or photon) in the FD with a PCal hit. */

void ParticleID::nParticleID(std::vector<clas12::region_part_ptr>& allParticles, std::vector<int>& ID_Neutrons_FD, const DSCuts& Neutron_momentum_th, std::vector<int>& ID_Photons_FD,
                             const DSCuts& Photon_momentum_th, clas12::region_part_ptr electron, const double& starttime, const bool& apply_nucleon_cuts) {
    for (int i = 0; i < allParticles.size(); i++) {
        if ((allParticles[i]->par()->getCharge() == 0) && (allParticles[i]->getRegion() == clas12::FD)) {  // If particle is neutral and in the FD
            int ParticlePDG = allParticles[i]->par()->getPid();

            bool ParticleInPCAL = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7);    // PCAL hit
            bool ParticleInECIN = (allParticles[i]->cal(clas12::ECIN)->getDetector() == 7);    // ECIN hit
            bool ParticleInECOUT = (allParticles[i]->cal(clas12::ECOUT)->getDetector() == 7);  // ECOUT hit

            if ((ParticlePDG == 2112) || (ParticlePDG == 22)) {
                if (ParticleInPCAL && (ParticlePDG == 22)) {  // if there's a 'photon' with a PCAL hit -> photon
                                                              //                    ID_Photons_FD.push_back(i);

                    /* Particles that get in here are photons. Now we take photons who pass momentum cuts. */
                    double Momentum = allParticles[i]->getP();

                    /* Log photons above momentum cuts (given by Momentum_cuts): */
                    if (Momentum >= Photon_momentum_th.GetLowerCutConst()) { ID_Photons_FD.push_back(i); }
                } else if (!ParticleInPCAL) {                 // if there is a neutron or a 'photon' without a PCAL hit
                    if (ParticleInECIN || ParticleInECOUT) {  // if there is either a ECIN or ECOUT hit -> neutron
                                                              //                        ID_Neutrons_FD.push_back(i);
                                                              //
                        /* Particles that get in here are neutrons. Now we take neutrons who pass momentum cuts. */
                        double Momentum = this->GetFDNeutronP(allParticles[i], electron, starttime, apply_nucleon_cuts);

                        /* Log neutrons above momentum cuts (given by Momentum_th): */
                        if (Momentum >= Neutron_momentum_th.GetLowerCutConst()) { ID_Neutrons_FD.push_back(i); }
                    }
                }
            }  // end of clas12root neutron or 'photon' if
        }  // end of neutral and in the FD if
    }  // end of loop over allparticle vector
}
#pragma endregion

// SetEventParticles function -------------------------------------------------------------------------------------------------------------------------------------------

ParticleID::ParticleVectorsTuple ParticleID::SetEventParticles(const bool& clas12ana_particles, clas12ana& clasAna, const std::unique_ptr<clas12::clas12reader>& c12) {
    // TODO: change these PDG numbers to those from the constants namespace
    std::vector<clas12::region_part_ptr> allParticles;
    std::vector<clas12::region_part_ptr> neutrons;
    std::vector<clas12::region_part_ptr> protons;
    std::vector<clas12::region_part_ptr> Kplus;
    std::vector<clas12::region_part_ptr> Kminus;
    std::vector<clas12::region_part_ptr> piplus;
    std::vector<clas12::region_part_ptr> piminus;
    std::vector<clas12::region_part_ptr> electrons;
    std::vector<clas12::region_part_ptr> deuterons;
    std::vector<clas12::region_part_ptr> neutrals;
    std::vector<clas12::region_part_ptr> otherpart;

    if (clas12ana_particles) {
        allParticles = clasAna.getParticles();
        neutrons = clasAna.getByPid(2112);
        protons = clasAna.getByPid(2212);
        Kplus = clasAna.getByPid(321);
        Kminus = clasAna.getByPid(-321);
        piplus = clasAna.getByPid(211);
        piminus = clasAna.getByPid(-211);
        electrons = clasAna.getByPid(11);
        deuterons = clasAna.getByPid(45);
        neutrals = clasAna.getByPid(0);
        otherpart = clasAna.getByPid(311);
    } else {
        allParticles = c12->getDetParticles();
        neutrons = c12->getByID(2112);
        protons = c12->getByID(2212);
        Kplus = c12->getByID(321);
        Kminus = c12->getByID(-321);
        piplus = c12->getByID(211);
        piminus = c12->getByID(-211);
        electrons = c12->getByID(11);
        deuterons = c12->getByID(45);
        neutrals = c12->getByID(0);
        otherpart = c12->getByID(311);
    }

    return std::make_tuple(allParticles, neutrons, protons, Kplus, Kminus, piplus, piminus, electrons, deuterons, neutrals, otherpart);
}

// Fill neutron multiplicity plots functions ----------------------------------------------------------------------------------------------------------------------------

// TODO: move from this class

#pragma region /* Neutrons by CLAS12PID */
void ParticleID::FillNeutMultiPlots(std::vector<clas12::region_part_ptr>& allParticles, std::vector<clas12::region_part_ptr>& electrons, const double& Weight, const double& beamE,
                                    const double& ECAL_veto_cut, hPlot1D& hNeutronMulti_BPID_BV, hPlot1D& hNeutronMulti_BPID_AV, std::vector<clas12::region_part_ptr>& Neutrons_BPID,
                                    hPlot1D& hNeutronMulti_APID_BV, hPlot1D& hNeutronMulti_APID_AV, std::vector<clas12::region_part_ptr>& Neutrons_APID) {
    FillMultiPlots(allParticles, electrons, Weight, beamE, ECAL_veto_cut, hNeutronMulti_BPID_BV, hNeutronMulti_BPID_AV, Neutrons_BPID);
    FillMultiPlots(allParticles, electrons, Weight, beamE, ECAL_veto_cut, hNeutronMulti_APID_BV, hNeutronMulti_APID_AV, Neutrons_APID);
}

void ParticleID::FillMultiPlots(std::vector<clas12::region_part_ptr>& allParticles, std::vector<clas12::region_part_ptr>& electrons, const double& Weight, const double& beamE,
                                const double& ECAL_veto_cut, hPlot1D& hNeutronMulti_BV, hPlot1D& hNeutronMulti_AV, std::vector<clas12::region_part_ptr>& Neutrons) {
    int Multiplicity_BV = Neutrons.size();
    int Multiplicity_AV = 0;

    for (int i = 0; i < allParticles.size(); i++) {
        if ((allParticles[i]->par()->getCharge() == 0) && (allParticles[i]->par()->getPid() == 2112) && (allParticles[i]->getRegion() == clas12::FD)) {
            bool NeutronPassVeto_Test = NeutronECAL_Cut_Veto(allParticles, electrons, beamE, i, ECAL_veto_cut);

            if (NeutronPassVeto_Test) { ++Multiplicity_AV; }
        }
    }

    hNeutronMulti_BV.hFill(Multiplicity_BV, Weight);
    hNeutronMulti_AV.hFill(Multiplicity_AV, Weight);
}
#pragma endregion

#pragma region /* Neutrons by redefinition */
void ParticleID::FillNeutMultiPlots(std::vector<clas12::region_part_ptr>& allParticles, std::vector<clas12::region_part_ptr>& electrons, const double& Weight, const double& beamE,
                                    const double& ECAL_veto_cut, hPlot1D& hNeutronMulti_BPID_BV, hPlot1D& hNeutronMulti_BPID_AV, std::vector<int>& Neutrons_BPID,
                                    hPlot1D& hNeutronMulti_APID_BV, hPlot1D& hNeutronMulti_APID_AV, std::vector<int>& Neutrons_APID) {
    FillMultiPlots(allParticles, electrons, Weight, beamE, ECAL_veto_cut, hNeutronMulti_BPID_BV, hNeutronMulti_BPID_AV, Neutrons_BPID);
    FillMultiPlots(allParticles, electrons, Weight, beamE, ECAL_veto_cut, hNeutronMulti_APID_BV, hNeutronMulti_APID_AV, Neutrons_APID);
}

void ParticleID::FillMultiPlots(std::vector<clas12::region_part_ptr>& allParticles, std::vector<clas12::region_part_ptr>& electrons, const double& Weight, const double& beamE,
                                const double& ECAL_veto_cut, hPlot1D& hNeutronMulti_BV, hPlot1D& hNeutronMulti_AV, std::vector<int>& Neutrons_indices) {
    int Multiplicity_BV = Neutrons_indices.size();
    int Multiplicity_AV = 0;

    for (int& i : Neutrons_indices) {
        bool NeutronPassVeto_Test = this->NeutronECAL_Cut_Veto(allParticles, electrons, beamE, i, ECAL_veto_cut);

        if (NeutronPassVeto_Test) { ++Multiplicity_AV; }
    }

    hNeutronMulti_BV.hFill(Multiplicity_BV, Weight);
    hNeutronMulti_AV.hFill(Multiplicity_AV, Weight);
}
#pragma endregion
