#include "Debugger.h"

// SafetyCheck_clas12ana_particles function ------------------------------------------------------------------------------------------------------------------------------------------------

void Debugger::SafetyCheck_clas12ana_particles(const char* FILE, const int LINE, const bool& clas12ana_particles, std::vector<clas12::region_part_ptr>& allParticles, const int Nf) {
    if ((clas12ana_particles) && (allParticles.size() != Nf)) { this->PrintErrorMessage(FILE, LINE, "allParticles.size() is different than Nf!", ""); }
}

// SafetyCheck_FD_protons function ------------------------------------------------------------------------------------------------------------------------------------------------

void Debugger::SafetyCheck_FD_protons(const char* FILE, const int LINE, const std::vector<int>& Protons_ind, std::vector<clas12::region_part_ptr>& protons, const DSCuts& pFD_mom_th) {
    for (int i = 0; i < Protons_ind.size(); i++) {
        if (protons[Protons_ind.at(i)]->getRegion() == clas12::FD) {
            double Reco_Proton_Momentum = protons[Protons_ind.at(i)]->getP();

            if (!((Reco_Proton_Momentum <= pFD_mom_th.GetUpperCutConst()) && (Reco_Proton_Momentum >= pFD_mom_th.GetLowerCutConst()))) {
                this->PrintErrorMessage(FILE, LINE,
                                        "FD proton check: an FD proton is outside momentum th. (" + basic_tools::ToStringWithPrecision(pFD_mom_th.GetUpperCutConst(), 2) + ") range!", "");
            }

            for (int j = i + 1; j < Protons_ind.size(); j++) {
                if (Protons_ind.at(i) == Protons_ind.at(j)) { this->PrintErrorMessage(FILE, LINE, "FD proton check: duplicated FD protons!", ""); }
            }
        }
    }
}

// SafetyCheck_CD_protons function ------------------------------------------------------------------------------------------------------------------------------------------------

void Debugger::SafetyCheck_CD_protons(const char* FILE, const int LINE, const std::vector<int>& Protons_ind, std::vector<clas12::region_part_ptr>& protons, const DSCuts& pCD_mom_th) {
    for (int i = 0; i < Protons_ind.size(); i++) {
        if (protons[Protons_ind.at(i)]->getRegion() == clas12::CD) {
            double Reco_Proton_Momentum = protons[Protons_ind.at(i)]->getP();

            if (!((Reco_Proton_Momentum <= pCD_mom_th.GetUpperCutConst()) && (Reco_Proton_Momentum >= pCD_mom_th.GetLowerCutConst()))) {
                this->PrintErrorMessage(FILE, LINE,
                                        "CD proton check: a CD proton is outside momentum th. (" + basic_tools::ToStringWithPrecision(pCD_mom_th.GetUpperCutConst(), 2) + ") range!", "");
            }

            for (int j = i + 1; j < Protons_ind.size(); j++) {
                if (Protons_ind.at(i) == Protons_ind.at(j)) { this->PrintErrorMessage(FILE, LINE, "CD proton check: duplicated CD protons!", ""); }
            }
        }
    }
}

// SafetyCheck_leading_FD_neutron function ------------------------------------------------------------------------------------------------------------------------------------------------

void Debugger::SafetyCheck_Reco_leading_FD_neutron(const char* FILE, const int LINE, const bool& apply_nucleon_cuts, const bool& ES_by_leading_FDneutron, const int& NeutronsFD_ind_mom_max,
                                                   std::vector<clas12::region_part_ptr>& allParticles, std::vector<int>& NeutronsFD_ind, ParticleID& pid,
                                                   clas12::region_part_ptr electron, const double& starttime) {
    if (ES_by_leading_FDneutron) {
        // Safety checks that leading nFD is neutron by definition
        if (NeutronsFD_ind_mom_max != -1) {
            bool LeadingnFDPCAL = (allParticles[NeutronsFD_ind_mom_max]->cal(clas12::PCAL)->getDetector() == 7);    // PCAL hit
            bool LeadingnFDECIN = (allParticles[NeutronsFD_ind_mom_max]->cal(clas12::ECIN)->getDetector() == 7);    // ECIN hit
            bool LeadingnFDECOUT = (allParticles[NeutronsFD_ind_mom_max]->cal(clas12::ECOUT)->getDetector() == 7);  // ECOUT hit

            if (allParticles[NeutronsFD_ind_mom_max]->getRegion() != clas12::FD) { this->PrintErrorMessage(FILE, LINE, "Leading reco nFD check: Leading nFD is not in the FD!", ""); }

            if (!((allParticles[NeutronsFD_ind_mom_max]->par()->getPid() == 2112) || (allParticles[NeutronsFD_ind_mom_max]->par()->getPid() == 22))) {
                PrintErrorMessage(FILE, LINE, ("Leading reco nFD check: A neutron PDG is not 2112 or 22 (" + std::to_string(allParticles[NeutronsFD_ind_mom_max]->par()->getPid())), "");
            }

            if (LeadingnFDPCAL) { this->PrintErrorMessage(FILE, LINE, "Leading reco nFD check: neutron hit in PCAL!", ""); }

            if (!(LeadingnFDECIN || LeadingnFDECOUT)) { this->PrintErrorMessage(FILE, LINE, "Leading reco nFD check: no neutron hit in ECIN or ECOUT!", ""); }
        }

        // Safety check for leading nFD assignment
        if ((NeutronsFD_ind.size() > 0) && (NeutronsFD_ind_mom_max == -1)) { this->PrintErrorMessage(FILE, LINE, "Leading reco nFD check: leading was not assigned!", ""); }

            if (NeutronsFD_ind.size() == 1) {
            if (NeutronsFD_ind.at(0) != NeutronsFD_ind_mom_max) { this->PrintErrorMessage(FILE, LINE, "Leading reco nFD check: leading was assigned incorrectly!", ""); }
        } else if (NeutronsFD_ind.size() > 1) {
            for (int& i : NeutronsFD_ind) {
                double Leading_neutron_momentum = pid.GetFDNeutronP(allParticles[NeutronsFD_ind_mom_max], electron, starttime, apply_nucleon_cuts);
                double Temp_neutron_momentum = pid.GetFDNeutronP(allParticles[i], electron, starttime, apply_nucleon_cuts);
                double dMomentum = Leading_neutron_momentum - Temp_neutron_momentum;

                if (dMomentum < 0) { this->PrintErrorMessage(FILE, LINE, "Leading reco nFD check: assigned nFD is not the leading!", ""); }
            }
        }
    }
}

void Debugger::SafetyCheck_Truth_leading_FD_neutron(const char* FILE, const int LINE, const bool& ES_by_leading_FDneutron, const int& TL_IDed_Leading_nFD_ind,
                                                    const double& TL_IDed_Leading_nFD_momentum, const double& Leading_TL_FDNeutron_Momentum, std::vector<int> TL_NeutronsFD_mom_ind,
                                                    mcpar_ptr mcpbank) {
    if (ES_by_leading_FDneutron) {
        if ((TL_IDed_Leading_nFD_ind != -1) && (TL_IDed_Leading_nFD_momentum != Leading_TL_FDNeutron_Momentum)) {
            cout << "\033[33m\nTL_IDed_Leading_nFD_momentum = " << TL_IDed_Leading_nFD_momentum << "\n\033[0m";
            cout << "\033[33mLeading_TL_FDNeutron_Momentum = " << Leading_TL_FDNeutron_Momentum << "\n\033[0m";
            this->PrintErrorMessage(FILE, LINE, "Leading TL nFD check: momentum magnitude inconsistent!", "");
        }

        if ((TL_NeutronsFD_mom_ind.size() > 0) && (TL_IDed_Leading_nFD_ind == -1)) { this->PrintErrorMessage(FILE, LINE, "Leading TL nFD check: leading was not assigned!", ""); }

        if (TL_NeutronsFD_mom_ind.size() == 1) {
            if (TL_NeutronsFD_mom_ind.at(0) != TL_IDed_Leading_nFD_ind) { this->PrintErrorMessage(FILE, LINE, "Leading TL nFD check: leading was assigned incorrectly!", ""); }
        } else if (TL_NeutronsFD_mom_ind.size() > 1) {
            for (int& i : TL_NeutronsFD_mom_ind) {
                mcpbank->setEntry(i);

                double Leading_neutron_momentum = Leading_TL_FDNeutron_Momentum;
                double Temp_neutron_momentum = analysis_math::RadCalc(mcpbank->getPx(), mcpbank->getPy(), mcpbank->getPz());
                double dMomentum = Leading_neutron_momentum - Temp_neutron_momentum;

                if (dMomentum < 0) { this->PrintErrorMessage(FILE, LINE, "Leading TL nFD check: assigned nFD is not the leading!", ""); }
            }
        }
    }
}

// SafetyCheck_FD_neutron function ------------------------------------------------------------------------------------------------------------------------------------------------

void Debugger::SafetyCheck_FD_neutron(const char* FILE, const int LINE, const bool& apply_nucleon_cuts, std::vector<clas12::region_part_ptr>& allParticles, const DSCuts& n_mom_th,
                                      std::vector<int>& NeutronsFD_ind, ParticleID& pid, clas12::region_part_ptr electron, const double& starttime) {
    for (int i = 0; i < NeutronsFD_ind.size(); i++) {
        double Reco_Neutron_Momentum = pid.GetFDNeutronP(allParticles[NeutronsFD_ind.at(i)], electron, starttime, apply_nucleon_cuts);

        if (!((Reco_Neutron_Momentum <= n_mom_th.GetUpperCutConst()) && (Reco_Neutron_Momentum >= n_mom_th.GetLowerCutConst()))) {
            cout << "\033[33m\n\nallParticles[NeutronsFD_ind.at(i)]->par()->getPid() = " << allParticles[NeutronsFD_ind.at(i)]->par()->getPid() << "\n\033[0m";
            cout << "\033[33mReco_Neutron_Momentum = " << Reco_Neutron_Momentum << "\n\033[0m";
            cout << "\033[33mn_mom_th.GetUpperCutConst() = " << n_mom_th.GetUpperCutConst() << "\n\033[0m";
            cout << "\033[33mn_mom_th.GetLowerCutConst() = " << n_mom_th.GetLowerCutConst() << "\n\033[0m";
            this->PrintErrorMessage(FILE, LINE, "FD neutron check: there are FD neutrons outside momentum th. range!", "");
        }

        for (int j = i + 1; j < NeutronsFD_ind.size(); j++) {
            if (NeutronsFD_ind.at(i) == NeutronsFD_ind.at(j)) { this->PrintErrorMessage(FILE, LINE, "FD neutron check: duplicated FD neutrons!", ""); }
        }
    }
}

// SafetyCheck_AMaps function ------------------------------------------------------------------------------------------------------------------------------------------------

void Debugger::SafetyCheck_AMaps_Truth_electrons(const char* FILE, const int LINE, const int& particlePDGtmp, const bool& inFD_AMaps) {
    if (particlePDGtmp != 11) { this->PrintErrorMessage(FILE, LINE, "TL electrons check (AMaps & WMaps): TL electron PGD is invalid (" + std::to_string(particlePDGtmp) + ")!", ""); }

    if (!inFD_AMaps) { this->PrintErrorMessage(FILE, LINE, "TL electrons check (AMaps & WMaps): TL electron is not in FD!", ""); }
}

void Debugger::SafetyCheck_AMaps_Truth_protons(const char* FILE, const int LINE, const int& particlePDGtmp, const bool& inFD_AMaps) {
    if (particlePDGtmp != 2212) { this->PrintErrorMessage(FILE, LINE, "TL protons check (AMaps & WMaps): TL proton PGD is invalid (" + std::to_string(particlePDGtmp) + ")!", ""); }

    if (!inFD_AMaps) { this->PrintErrorMessage(FILE, LINE, "TL protons check (AMaps & WMaps): TL proton is not in FD!", ""); }
}

void Debugger::SafetyCheck_AMaps_Truth_neutrons(const char* FILE, const int LINE, const int& particlePDGtmp, const bool& inFD_AMaps) {
    if (particlePDGtmp != 2112) { this->PrintErrorMessage(FILE, LINE, "TL neutrons check (AMaps & WMaps): TL neutron PGD is invalid (" + std::to_string(particlePDGtmp) + ")!", ""); }

    if (!inFD_AMaps) { this->PrintErrorMessage(FILE, LINE, "TL neutrons check (AMaps & WMaps): TL neutron is not in FD!", ""); }
}

void Debugger::SafetyCheck_AMaps_Reco_leading_neutrons(const char* FILE, const int LINE, std::vector<clas12::region_part_ptr>& allParticles, const int& NeutronsFD_ind_mom_max,
                                                       const bool& hitPCAL_1e_cut, const bool& hitECIN_1e_cut, const bool& hitECOUT_1e_cut) {
    if (allParticles[NeutronsFD_ind_mom_max]->getRegion() != clas12::FD) { this->PrintErrorMessage(FILE, LINE, "Leading reco nFD check (AMaps & WMaps): Leading nFD is not in the FD!", ""); }

    if (!((allParticles[NeutronsFD_ind_mom_max]->par()->getPid() == 2112) || (allParticles[NeutronsFD_ind_mom_max]->par()->getPid() == 22))) {
        this->PrintErrorMessage(FILE, LINE,
                                ("Leading reco nFD check (AMaps & WMaps): A neutron PDG is not 2112 or 22 (" + std::to_string(allParticles[NeutronsFD_ind_mom_max]->par()->getPid()) + ")!"), "");
    }

    if (hitPCAL_1e_cut) { this->PrintErrorMessage(FILE, LINE, "Leading reco nFD check (AMaps & WMaps): neutron hit in PCAL!", ""); }

    if (!(hitECIN_1e_cut || hitECOUT_1e_cut)) { this->PrintErrorMessage(FILE, LINE, "Leading reco nFD check (AMaps & WMaps): no neutron hit in ECIN or ECOUT!", ""); }
}

// SafetyCheck_one_good_electron function ------------------------------------------------------------------------------------------------------------------------------------------------

void Debugger::SafetyCheck_one_good_electron(const char* FILE, const int LINE, std::vector<clas12::region_part_ptr>& electrons) {
    if (electrons.size() != 1) { this->PrintErrorMessage(FILE, LINE, "1e cut: single electron cut is not implemented!", ""); }
}

// SafetyCheck_one_good_electron function ------------------------------------------------------------------------------------------------------------------------------------------------

void Debugger::SafetyCheck_1e_cut_electron(const char* FILE, const int LINE, std::vector<clas12::region_part_ptr>& electrons, const std::vector<int>& Electron_ind) {
    if (electrons.size() != 1) { this->PrintErrorMessage(FILE, LINE, "1e cut: electrons.size() is different than 1!", ""); }

    if (Electron_ind.at(0) != 0) { this->PrintErrorMessage(FILE, LINE, "1e cut: Electron_ind.at(0) is different than 0!", ""); }

    if (electrons[0]->getRegion() != clas12::FD) { this->PrintErrorMessage(FILE, LINE, "1e cut: electrons is not in the FD!", ""); }
}

// SafetyCheck_basic_event_selection function ------------------------------------------------------------------------------------------------------------------------------------------------

void Debugger::SafetyCheck_basic_event_selection(const char* FILE, const int LINE, const std::string FinaleState, std::vector<clas12::region_part_ptr>& Kplus, std::vector<clas12::region_part_ptr>& Kminus,
                                                 const std::vector<int>& Piplus_ind, const std::vector<int>& Piminus_ind, const std::vector<int>& Electron_ind, std::vector<clas12::region_part_ptr>& deuterons) {
    if (Kplus.size() != 0) { this->PrintErrorMessage(FILE, LINE, (FinaleState + ": Kplus.size() is different than 0!"), ""); }

    if (Kminus.size() != 0) { this->PrintErrorMessage(FILE, LINE, (FinaleState + ": Kminus.size() is different than 0!"), ""); }

    if (Piplus_ind.size() != 0) { this->PrintErrorMessage(FILE, LINE, (FinaleState + ": Piplus_ind.size() is different than 0!"), ""); }

    if (Piminus_ind.size() != 0) { this->PrintErrorMessage(FILE, LINE, (FinaleState + ": Piminus_ind.size() is different than 0!"), ""); }

    if (Electron_ind.size() != 1) { this->PrintErrorMessage(FILE, LINE, (FinaleState + ": Electron_ind.size() is different than 1!"), ""); }

    if (deuterons.size() != 0) { this->PrintErrorMessage(FILE, LINE, (FinaleState + ": deuterons.size() is different than 0!"), ""); }
}

// SafetyCheck_1p function ------------------------------------------------------------------------------------------------------------------------------------------------

void Debugger::SafetyCheck_1p(const char* FILE, const int LINE, const std::vector<int>& Protons_ind, clas12::region_part_ptr& e_1p, clas12::region_part_ptr& p_1p, const bool& Enable_FD_photons,
                              std::vector<int>& PhotonsFD_ind) {
    if (Protons_ind.size() != 1) { this->PrintErrorMessage(FILE, LINE, "1p: Protons_ind.size() is different than 1!", ""); }

    if (e_1p->getRegion() != clas12::FD) { this->PrintErrorMessage(FILE, LINE, "1p: 1p proton is not in the FD!", ""); }

    if (p_1p->getRegion() != clas12::FD) { this->PrintErrorMessage(FILE, LINE, "1p: 1p electron is not in the FD!", ""); }

    if (!(Enable_FD_photons || (PhotonsFD_ind.size() == 0))) { this->PrintErrorMessage(FILE, LINE, ("1p: PhotonsFD_ind.size() is non-zero (" + std::to_string(PhotonsFD_ind.size()) + ")!"), ""); }
}

// SafetyCheck_1n function ------------------------------------------------------------------------------------------------------------------------------------------------

void Debugger::SafetyCheck_1n(const char* FILE, const int LINE, std::vector<int>& NeutronsFD_ind, clas12::region_part_ptr& e_1n, clas12::region_part_ptr& n_1n, const bool& Enable_FD_photons,
                              std::vector<int>& PhotonsFD_ind, const bool& ES_by_leading_FDneutron, ParticleID& pid, std::vector<clas12::region_part_ptr>& allParticles, const int& NeutronsFD_ind_mom_max,
                              const bool& apply_nucleon_cuts, const bool& NeutronInPCAL_1n, const bool& NeutronInECIN_1n, const bool& NeutronInECOUT_1n, const int& n_detlayer_1n,
                              const double& starttime) {
    if (e_1n->getRegion() != clas12::FD) { this->PrintErrorMessage(FILE, LINE, "1n: Electron is not in the FD!", ""); }

    if (n_1n->getRegion() != clas12::FD) { this->PrintErrorMessage(FILE, LINE, "1n: nFD is not in the FD!", ""); }

    if (!(Enable_FD_photons || (PhotonsFD_ind.size() == 0))) { this->PrintErrorMessage(FILE, LINE, ("1n: PhotonsFD_ind.size() is non-zero (" + std::to_string(PhotonsFD_ind.size()) + ")!"), ""); }

    if (ES_by_leading_FDneutron) {  // TODO: add this check to nFDpCD
        double P_max_test = pid.GetFDNeutronP(allParticles[NeutronsFD_ind_mom_max], e_1n, starttime, apply_nucleon_cuts);

        for (int& i : NeutronsFD_ind) {
            if (i != NeutronsFD_ind_mom_max) {
                double P_temp = pid.GetFDNeutronP(allParticles[i], e_1n, starttime, apply_nucleon_cuts);

                if (P_max_test < P_temp) { this->PrintErrorMessage(FILE, LINE, "1n: NeutronsFD_ind_mom_max is not leading FD neutron!", ""); }
            }
        }
    }

    for (int& i : NeutronsFD_ind) {
        bool NeutronInPCAL_1n = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7);  // PCAL hit
        if (NeutronInPCAL_1n) { this->PrintErrorMessage(FILE, LINE, "1n: a neutron have been found with a PCAL hit!", ""); }

        if (!((allParticles[i]->par()->getPid() == 2112) || (allParticles[i]->par()->getPid() == 22))) {
            this->PrintErrorMessage(FILE, LINE, ("1n: A neutron PDG is not 2112 or 22 (" + std::to_string(allParticles[i]->par()->getPid()) + ")!"), "");
        }
    }

    for (int& i : PhotonsFD_ind) {
        bool PhotonInPCAL_1n = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7);  // PCAL hit
        if (!PhotonInPCAL_1n) { this->PrintErrorMessage(FILE, LINE, "1n: a photon have been found without a PCAL hit!", ""); }

        if (allParticles[i]->par()->getPid() != 22) {
            this->PrintErrorMessage(FILE, LINE, ("1n: A photon PDG is not 2112 or 22 (" + std::to_string(allParticles[i]->par()->getPid()) + ")!"), "");
        }
    }

    /* Safety check that we are looking at good neutron (BEFORE VETO!!!) */
    int NeutronPDG = n_1n->par()->getPid();

    if (n_1n->getRegion() != clas12::FD) { this->PrintErrorMessage(FILE, LINE, "1n: neutron is not in FD!", ""); }

    if (!((NeutronPDG == 22) || (NeutronPDG == 2112))) { this->PrintErrorMessage(FILE, LINE, ("1n: neutral PDG is not 2112 or 22 (" + std::to_string(NeutronPDG) + ")!"), ""); }

    if (NeutronInPCAL_1n) { this->PrintErrorMessage(FILE, LINE, "1n: neutron hit in PCAL!", ""); }

    if (!(NeutronInECIN_1n || NeutronInECOUT_1n)) { this->PrintErrorMessage(FILE, LINE, "1n: no neutron hit in ECIN or ECOUT!", ""); }

    if (!(!NeutronInPCAL_1n && (NeutronInECIN_1n || NeutronInECOUT_1n))) { this->PrintErrorMessage(FILE, LINE, "1n: not neutron by definition!", ""); }
}

// PrintRunConfigurationWarnings function ----------------------------------------------------------------------------------------------------------------------------------

void Debugger::PrintRunConfigurationWarnings(const char* FunctionName, const char* FILE, const int LINE, const bool& TestRun, const bool& ApplyLimiter) {
    if (TestRun || ApplyLimiter) {
        if (TestRun) { basic_tools::PrintWarning(FunctionName, FILE, LINE, "running code in testing mode!"); }

        if (ApplyLimiter) { basic_tools::PrintWarning(FunctionName, FILE, LINE, "running code with a limiter on number of events!"); }
    }
}

void Debugger::PrintMissingNeutralFDECALHitWarning(const char* FunctionName, const char* FILE, const int LINE, const clas12::region_part_ptr& NeutralFD, const bool& ParticleInPCAL,
                                                   const bool& ParticleInECIN, const bool& ParticleInECOUT) {
    if ((NeutralFD->getRegion() == clas12::FD) && (!(ParticleInPCAL || ParticleInECIN || ParticleInECOUT))) {
        if (NeutralFD->par()->getCharge() != 0) {
            basic_tools::PrintWarning(FunctionName, FILE, LINE,
                                      "FD neutral particle is charged and has no ECAL hit (PCAL == ECIN == ECOUT == false); defaulting ECAL detlayer to ECOUT.");
        } else {
            basic_tools::PrintWarning(FunctionName, FILE, LINE, "FD neutral has no ECAL hit (PCAL == ECIN == ECOUT == false); defaulting ECAL detlayer to ECOUT.");
        }
    }
}

void Debugger::PrintMissingFTOFHitWarning(const char* FunctionName, const char* FILE, const int LINE, const clas12::region_part_ptr& ParticleFD, const bool& ParticleInFTOF1A,
                                          const bool& ParticleInFTOF1B, const bool& ParticleInFTOF2) {
    if ((ParticleFD->getRegion() == clas12::FD) && (!(ParticleInFTOF1A || ParticleInFTOF1B || ParticleInFTOF2))) {
        if (ParticleFD->par()->getCharge() != 0) {
            basic_tools::PrintWarning(FunctionName, FILE, LINE, "FD charged has no FTOF hit (FTOF1A == FTOF1B == FTOF2 == false).");
        } else {
            basic_tools::PrintWarning(FunctionName, FILE, LINE, "FD neutral has no FTOF hit (FTOF1A == FTOF1B == FTOF2 == false).");
        }
    }
}

// PrintWeightMapsCommentedOutWarning function -----------------------------------------------------------------------------------------------------------------------------

void Debugger::PrintWeightMapsCommentedOutWarning(const char* FunctionName, const char* FILE, const int LINE) {
    // TODO: Remove this temporary warning once weight maps are enabled in the analysis flow.
    basic_tools::PrintWarning(FunctionName, FILE, LINE, "NOTE: weight maps are commented out!");
}
