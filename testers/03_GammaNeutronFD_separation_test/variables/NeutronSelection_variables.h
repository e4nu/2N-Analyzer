#pragma once

#include <cmath>
#include <string>
#include <vector>

#include "TH1D.h"
#include "TH2D.h"
#include "TObject.h"
#include "TVector3.h"

// Include libraries:
#include "../../../framework/classes/NeutronVetoRon/NeutralHit.h"
#include "../../../framework/namespaces/general_utilities/utilities.h"
#include "../helpers/FDNeutralHelpers.h"

// Include classes:
#include "../../../framework/classes/DSCuts/DSCuts.h"
#include "../../../framework/classes/NeutronVetoRon/nfdveto.h"

// Include CLAS12 libraries:
#include "../../../framework/includes/clas12_include.h"

namespace raf = reco_analysis_functions;

struct NeutronSelection_variables {
    const TVector3& reco_P_e;

    // Per-neutron computed state (filled by RunOnIndex)
    bool ParticleInPCAL = false;
    bool ParticleInECIN = false;
    bool ParticleInECOUT = false;

    bool nFD_pass_dSector_cuts = true;
    int nFD_nSector = -1;
    int e_nSector = -1;
    bool Same_e_nFD_Sector = false;

    double Path_nFD = 0.0;
    double Edep_ECAL_nFD = 0.0;

    int PCAL_hit = 0;
    int ECIN_hit = 0;
    int ECOUT_hit = 0;

    int PCAL_hit_test = 0;
    int ECIN_hit_test = 0;
    int ECOUT_hit_test = 0;
    int ECAL_detlayer_test = -1;

    TVector3 reco_P_nFD;
    double reco_ToF_nFD = 0.0;
    double reco_Beta_nFD = 0.0;
    double reco_Gamma_nFD = 0.0;

    double dTheta = 0.0;
    double dPhi = 0.0;

    bool DoesNotPass_theta_nFD_e_vs_dTheta_cuts = false;

    bool Pass_dTheta_status = true;
    bool Pass_dPhi_status = true;

    int nFD_status = -1;
    int e_status = -1;

    TVector3 v_nhit;
    TVector3 shifted_e_unit_v;
    TVector3 v_dist;

    // Small helper for your existing abort style
    static void Abort(const char* msg) {
        std::cout << "\n\n\033[31mError!\033[0m " << msg << "\n\n";
        std::exit(1);
    }

    // =======================
    // Constructor
    // =======================
    NeutronSelection_variables(const bool& OnlyGood_nFD, const bool& OnlyBad_nFD, const bool& apply_neutFD_redef, const bool& apply_theta_nFD_e_vs_dTheta_cuts, DSCuts dTheta_cuts,
                               std::vector<region_part_ptr>& allParticles, region_part_ptr& electron, const double& starttime, const NeutralHit& NeutronSelection_NeutralHit,
                               const FDNeutralHelpers& fdNeutralHelpers, const TVector3& reco_P_e)
        : reco_P_e(reco_P_e) {
        // ------------------------------------------------------------
        // Hit flags from your neutron object
        // ------------------------------------------------------------
        ParticleInPCAL = NeutronSelection_NeutralHit.clas12particle_hit_in_PCAL;
        ParticleInECIN = NeutronSelection_NeutralHit.clas12particle_hit_in_ECIN;
        ParticleInECOUT = NeutronSelection_NeutralHit.clas12particle_hit_in_ECOUT;

        // ------------------------------------------------------------
        // Sanity checks against clas12 particle cal(det)->getDetector()
        // ------------------------------------------------------------
        if (ParticleInPCAL && !(allParticles[NeutronSelection_NeutralHit.pindex]->cal(clas12::PCAL)->getDetector() == 7)) {
            Abort("FD neutron is not really in the PCAL (ECALveto)! Aborting...");
        }

        if (ParticleInECIN && !(allParticles[NeutronSelection_NeutralHit.pindex]->cal(clas12::ECIN)->getDetector() == 7)) {
            Abort("FD neutron is not really in the ECIN (ECALveto)! Aborting...");
        }

        if (ParticleInECOUT && !(allParticles[NeutronSelection_NeutralHit.pindex]->cal(clas12::ECOUT)->getDetector() == 7)) {
            Abort("FD neutron is not really in the ECOUT (ECALveto)! Aborting...");
        }

        // ------------------------------------------------------------
        // dSector logic
        // ------------------------------------------------------------

        nFD_nSector = NeutronSelection_NeutralHit.sector;
        e_nSector = electron->getSector();

        if (OnlyGood_nFD) { nFD_pass_dSector_cuts = (std::abs(nFD_nSector - e_nSector) == 3); }
        if (OnlyBad_nFD) { nFD_pass_dSector_cuts = !(std::abs(nFD_nSector - e_nSector) == 3); }

        Same_e_nFD_Sector = (nFD_nSector == e_nSector);

        // ------------------------------------------------------------
        // Kinematics variables
        // ------------------------------------------------------------
        // IMPORTANT: Avoid variable shadowing when unpacking structured bindings.
        //
        // Previously, we wrote:
        //
        //     auto [reco_ToF_nFD, reco_Beta_nFD, reco_Gamma_nFD, reco_P_nFD] = ...
        //
        // However, this silently creates *new local variables* with the same names as
        // the struct members. Those local variables shadow the members, meaning the
        // struct’s actual member variables remain unchanged (e.g. reco_P_nFD stays
        // equal to its default-constructed zero vector).
        //
        // To fix this, we:
        //   1) Unpack into temporary variables (e.g. *_tmp)
        //   2) Explicitly assign them into the struct members using `this->`
        //
        // The `this->` syntax explicitly refers to the current object’s member,
        // ensuring we are modifying the struct’s stored state and not a local copy.
        // ---------------------------------------------------------------------------------
        auto [tof_tmp, beta_tmp, gamma_tmp, p_tmp] = nfdveto::Calc_nFD_vars(NeutronSelection_NeutralHit, starttime);
        // auto [tof_tmp, beta_tmp, gamma_tmp, p_tmp] = nfdveto::Calc_nFD_vars(NeutronSelection_NeutralHit, starttime, apply_neutFD_redef);
        this->reco_ToF_nFD = tof_tmp;
        this->reco_Beta_nFD = beta_tmp;
        this->reco_Gamma_nFD = gamma_tmp;
        this->reco_P_nFD = p_tmp;
        this->Path_nFD = NeutronSelection_NeutralHit.HitPath.Mag();
        this->Edep_ECAL_nFD = NeutronSelection_NeutralHit.Edep_ECAL;

        // ------------------------------------------------------------
        // Hit integer flags and cross-check using raf::CheckForECALHits
        // ------------------------------------------------------------
        PCAL_hit = ParticleInPCAL ? 1 : 0;
        ECIN_hit = ParticleInECIN ? 1 : 0;
        ECOUT_hit = ParticleInECOUT ? 1 : 0;

        auto [PCAL_hit_test, ECIN_hit_test, ECOUT_hit_test, ECAL_detlayer_test] = raf::CheckForECALHits(allParticles[NeutronSelection_NeutralHit.pindex]);
        this->PCAL_hit_test = PCAL_hit_test;
        this->ECIN_hit_test = ECIN_hit_test;
        this->ECOUT_hit_test = ECOUT_hit_test;
        this->ECAL_detlayer_test = ECAL_detlayer_test;
        if (PCAL_hit_test != PCAL_hit) { Abort("Error: PCAL_hit_test != PCAL_hit, aborting..."); }
        if (ECIN_hit_test != ECIN_hit) { Abort("Error: ECIN_hit_test != ECIN_hit, aborting..."); }
        if (ECOUT_hit_test != ECOUT_hit) { Abort("Error: ECOUT_hit_test != ECOUT_hit, aborting..."); }

        // ------------------------------------------------------------
        // Angles
        // ------------------------------------------------------------
        dTheta = (reco_P_nFD.Theta() - reco_P_e.Theta()) * 180 / M_PI;
        dPhi = am::CalcdPhi1((reco_P_nFD.Phi() - reco_P_e.Phi()) * 180 / M_PI);

        DoesNotPass_theta_nFD_e_vs_dTheta_cuts =
            (apply_theta_nFD_e_vs_dTheta_cuts &&
             (Same_e_nFD_Sector && (((dTheta - dTheta_cuts.GetMean()) > dTheta_cuts.GetLowerCut()) && ((dTheta - dTheta_cuts.GetMean()) < dTheta_cuts.GetUpperCut()))));

        Pass_dTheta_status = true;
        Pass_dPhi_status = true;

        e_status = electron->par()->getStatus();
    }
};
