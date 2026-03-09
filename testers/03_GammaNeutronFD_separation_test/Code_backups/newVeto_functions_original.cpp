// source gammaNeutronFD_separation_test_run.sh

#ifndef NEWVETO_FUNCTIONS
#define NEWVETO_FUNCTIONS

#include <TCanvas.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TLatex.h>
#include <TLorentzVector.h>
#include <TStyle.h>
#include <TTree.h>

#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <tuple>
#include <typeinfo>
#include <vector>

// Include libraries:
#include "../../framework/namespaces/general_utilities/utilities.h"
#include "../../framework/namespaces/setup/path_definitions.h"

// Include classes:
#include "../../framework/classes/AMaps/AMaps.cpp"
#include "../../framework/classes/NeutronVetoRon/ChargedHit.h"
#include "../../framework/classes/NeutronVetoRon/NeutralHit.h"
#include "../../framework/classes/ParticleID/ParticleID.cpp"
#include "../../framework/classes/clas12ana/clas12ana.h"

// Include CLAS12 libraries:
#include "../../framework/includes/clas12_include.h"

using namespace utilities;
using std::string;
using std::vector;

#pragma region /* New veto patrameters */
double dTime_cut = 5.;
double dHitTime_cut = 5.;
double Opening_ang_cut_deg = 5.;
// double Dif_Z = 1045.;
int dSector_cut = 0;
#pragma endregion

#pragma region /* GenerateHits function */

std::tuple<std::vector<NeutralHit>, std::vector<ChargedHit>> GenerateHits(std::vector<region_part_ptr>& allParticles) {
    //////////////////////////////////////////////////////////////////////////
    // Collect all neutral related hits in the FTOF, PCAL, and EC
    //////////////////////////////////////////////////////////////////////////

    std::vector<NeutralHit> NeutralsFD;  // Neutral particles in the FD
    std::vector<ChargedHit> ChargedFD;   // Charged particles in the FD

    int nHitnum = 0;
    int cHitnum = 0;

    for (int i = 0; i < allParticles.size(); i++) {
        bool IsNeutral = allParticles[i]->par()->getCharge() == 0;
        bool IsCharged = !IsNeutral;

        bool IsInFD = allParticles[i]->getRegion() == FD;         // Particle is in FD
        bool NonZeroPDG = allParticles[i]->par()->getPid() != 0;  // Neutral in FD with non-zero PDG

        auto [FTOF1A_hit, FTOF1B_hit, FTOF2_hit] = reco_analysis_functions::CheckForFTOFHits(allParticles[i]);
        bool FTOFhit = FTOF1A_hit || FTOF1B_hit || FTOF2_hit;

        auto [PCAL_hit, ECIN_hit, ECOUT_hit, ECAL_detlayer] = reco_analysis_functions::CheckForECALHits(allParticles[i]);
        bool EChit = ECIN_hit || ECOUT_hit;

        bool IsNeutralFD = (IsNeutral && IsInFD && NonZeroPDG) && (FTOFhit || PCAL_hit || EChit);
        bool IsChargedFD = (IsCharged && IsInFD && NonZeroPDG) && (FTOFhit || PCAL_hit || EChit);

        if (IsNeutralFD) {  // Neutron in FD
            int Temp_num_of_ECAL_hits = 0;
            if (PCAL_hit) { Temp_num_of_ECAL_hits++; }
            if (ECIN_hit) { Temp_num_of_ECAL_hits++; }
            if (ECOUT_hit) { Temp_num_of_ECAL_hits++; }

            if (FTOF1A_hit) {
                NeutralHit TempNeutralHit = NeutralHit();
                TempNeutralHit.reset();

                auto Temp_Neutron_ECAL_detlayer = PCAL_hit ? clas12::PCAL : ECIN_hit ? clas12::ECIN : ECOUT_hit ? clas12::ECOUT : -999.;

                TempNeutralHit.hitIndex = nHitnum, TempNeutralHit.pindex = i;

                TempNeutralHit.detector = 12, TempNeutralHit.layer = 1, TempNeutralHit.sector = allParticles[i]->sci(clas12::FTOF1A)->getSector();

                TempNeutralHit.clas12particle_hit_in_PCAL = PCAL_hit, TempNeutralHit.clas12particle_hit_in_ECIN = ECIN_hit, TempNeutralHit.clas12particle_hit_in_ECOUT = ECOUT_hit;
                TempNeutralHit.Neutron_ECAL_detlayer = Temp_Neutron_ECAL_detlayer;

                TempNeutralHit.Lv = allParticles[i]->cal(Temp_Neutron_ECAL_detlayer)->getLv(), TempNeutralHit.Lw = allParticles[i]->cal(Temp_Neutron_ECAL_detlayer)->getLw();

                TempNeutralHit.xHit = allParticles[i]->sci(clas12::FTOF1A)->getX(), TempNeutralHit.yHit = allParticles[i]->sci(clas12::FTOF1A)->getY(),
                TempNeutralHit.zHit = allParticles[i]->sci(clas12::FTOF1A)->getZ();

                TempNeutralHit.Px = allParticles[i]->par()->getPx(), TempNeutralHit.Py = allParticles[i]->par()->getPy(), TempNeutralHit.Pz = allParticles[i]->par()->getPz();

                TempNeutralHit.HitTime = allParticles[i]->sci(clas12::FTOF1A)->getTime();

                TempNeutralHit.path = allParticles[i]->sci(clas12::FTOF1A)->getPath();

                NeutralsFD.push_back(TempNeutralHit);
                nHitnum++;
            }

            if (FTOF1B_hit) {
                NeutralHit TempNeutralHit = NeutralHit();
                TempNeutralHit.reset();

                auto Temp_Neutron_ECAL_detlayer = PCAL_hit ? clas12::PCAL : ECIN_hit ? clas12::ECIN : ECOUT_hit ? clas12::ECOUT : -999.;

                TempNeutralHit.hitIndex = nHitnum, TempNeutralHit.pindex = i;

                TempNeutralHit.detector = 12, TempNeutralHit.layer = 2, TempNeutralHit.sector = allParticles[i]->sci(clas12::FTOF1B)->getSector();

                TempNeutralHit.clas12particle_hit_in_PCAL = PCAL_hit, TempNeutralHit.clas12particle_hit_in_ECIN = ECIN_hit, TempNeutralHit.clas12particle_hit_in_ECOUT = ECOUT_hit;
                TempNeutralHit.Neutron_ECAL_detlayer = Temp_Neutron_ECAL_detlayer;

                TempNeutralHit.Lv = allParticles[i]->cal(Temp_Neutron_ECAL_detlayer)->getLv(), TempNeutralHit.Lw = allParticles[i]->cal(Temp_Neutron_ECAL_detlayer)->getLw();

                TempNeutralHit.xHit = allParticles[i]->sci(clas12::FTOF1B)->getX(), TempNeutralHit.yHit = allParticles[i]->sci(clas12::FTOF1B)->getY(),
                TempNeutralHit.zHit = allParticles[i]->sci(clas12::FTOF1B)->getZ();

                TempNeutralHit.Px = allParticles[i]->par()->getPx(), TempNeutralHit.Py = allParticles[i]->par()->getPy(), TempNeutralHit.Pz = allParticles[i]->par()->getPz();

                TempNeutralHit.HitTime = allParticles[i]->sci(clas12::FTOF1B)->getTime();

                TempNeutralHit.path = allParticles[i]->sci(clas12::FTOF1B)->getPath();

                NeutralsFD.push_back(TempNeutralHit);
                nHitnum++;
            }

            if (FTOF2_hit) {
                NeutralHit TempNeutralHit = NeutralHit();
                TempNeutralHit.reset();

                auto Temp_Neutron_ECAL_detlayer = PCAL_hit ? clas12::PCAL : ECIN_hit ? clas12::ECIN : ECOUT_hit ? clas12::ECOUT : -999.;

                TempNeutralHit.hitIndex = nHitnum, TempNeutralHit.pindex = i;

                TempNeutralHit.detector = 12, TempNeutralHit.layer = 3, TempNeutralHit.sector = allParticles[i]->sci(clas12::FTOF2)->getSector();

                TempNeutralHit.clas12particle_hit_in_PCAL = PCAL_hit, TempNeutralHit.clas12particle_hit_in_ECIN = ECIN_hit, TempNeutralHit.clas12particle_hit_in_ECOUT = ECOUT_hit;
                TempNeutralHit.Neutron_ECAL_detlayer = Temp_Neutron_ECAL_detlayer;

                TempNeutralHit.Lv = allParticles[i]->cal(Temp_Neutron_ECAL_detlayer)->getLv(), TempNeutralHit.Lw = allParticles[i]->cal(Temp_Neutron_ECAL_detlayer)->getLw();

                TempNeutralHit.xHit = allParticles[i]->sci(clas12::FTOF2)->getX(), TempNeutralHit.yHit = allParticles[i]->sci(clas12::FTOF2)->getY(),
                TempNeutralHit.zHit = allParticles[i]->sci(clas12::FTOF2)->getZ();

                TempNeutralHit.Px = allParticles[i]->par()->getPx(), TempNeutralHit.Py = allParticles[i]->par()->getPy(), TempNeutralHit.Pz = allParticles[i]->par()->getPz();

                TempNeutralHit.HitTime = allParticles[i]->sci(clas12::FTOF2)->getTime();

                TempNeutralHit.path = allParticles[i]->sci(clas12::FTOF2)->getPath();

                NeutralsFD.push_back(TempNeutralHit);
                nHitnum++;
            }

            if (PCAL_hit) {
                NeutralHit TempNeutralHit = NeutralHit();
                TempNeutralHit.reset();

                auto Temp_Neutron_ECAL_detlayer = PCAL_hit ? clas12::PCAL : ECIN_hit ? clas12::ECIN : ECOUT_hit ? clas12::ECOUT : -999.;

                TempNeutralHit.hitIndex = nHitnum, TempNeutralHit.pindex = i;

                TempNeutralHit.detector = 7, TempNeutralHit.layer = 1, TempNeutralHit.sector = allParticles[i]->sci(clas12::PCAL)->getSector();  // Detector layer in veto, not CLAS12!

                TempNeutralHit.clas12particle_hit_in_PCAL = PCAL_hit, TempNeutralHit.clas12particle_hit_in_ECIN = ECIN_hit, TempNeutralHit.clas12particle_hit_in_ECOUT = ECOUT_hit;
                TempNeutralHit.Neutron_ECAL_detlayer = Temp_Neutron_ECAL_detlayer;

                TempNeutralHit.Lv = allParticles[i]->cal(Temp_Neutron_ECAL_detlayer)->getLv(), TempNeutralHit.Lw = allParticles[i]->cal(Temp_Neutron_ECAL_detlayer)->getLw();

                TempNeutralHit.xHit = allParticles[i]->sci(clas12::PCAL)->getX(), TempNeutralHit.yHit = allParticles[i]->sci(clas12::PCAL)->getY(),
                TempNeutralHit.zHit = allParticles[i]->sci(clas12::PCAL)->getZ();

                TempNeutralHit.Px = allParticles[i]->par()->getPx(), TempNeutralHit.Py = allParticles[i]->par()->getPy(), TempNeutralHit.Pz = allParticles[i]->par()->getPz();

                TempNeutralHit.HitTime = allParticles[i]->sci(clas12::PCAL)->getTime();

                TempNeutralHit.path = allParticles[i]->sci(clas12::PCAL)->getPath();

                TempNeutralHit.num_of_ECAL_hits = Temp_num_of_ECAL_hits;

                NeutralsFD.push_back(TempNeutralHit);
                nHitnum++;
            }

            if (ECIN_hit) {
                NeutralHit TempNeutralHit = NeutralHit();
                TempNeutralHit.reset();

                auto Temp_Neutron_ECAL_detlayer = PCAL_hit ? clas12::PCAL : ECIN_hit ? clas12::ECIN : ECOUT_hit ? clas12::ECOUT : -999.;

                TempNeutralHit.hitIndex = nHitnum, TempNeutralHit.pindex = i;

                TempNeutralHit.detector = 7, TempNeutralHit.layer = 2, TempNeutralHit.sector = allParticles[i]->sci(clas12::ECIN)->getSector();  // Detector layer in veto, not CLAS12!

                TempNeutralHit.clas12particle_hit_in_PCAL = PCAL_hit, TempNeutralHit.clas12particle_hit_in_ECIN = ECIN_hit, TempNeutralHit.clas12particle_hit_in_ECOUT = ECOUT_hit;
                TempNeutralHit.Neutron_ECAL_detlayer = Temp_Neutron_ECAL_detlayer;

                TempNeutralHit.Lv = allParticles[i]->cal(Temp_Neutron_ECAL_detlayer)->getLv(), TempNeutralHit.Lw = allParticles[i]->cal(Temp_Neutron_ECAL_detlayer)->getLw();

                TempNeutralHit.xHit = allParticles[i]->sci(clas12::ECIN)->getX(), TempNeutralHit.yHit = allParticles[i]->sci(clas12::ECIN)->getY(),
                TempNeutralHit.zHit = allParticles[i]->sci(clas12::ECIN)->getZ();

                TempNeutralHit.Px = allParticles[i]->par()->getPx(), TempNeutralHit.Py = allParticles[i]->par()->getPy(), TempNeutralHit.Pz = allParticles[i]->par()->getPz();

                TempNeutralHit.HitTime = allParticles[i]->sci(clas12::ECIN)->getTime();

                TempNeutralHit.path = allParticles[i]->sci(clas12::ECIN)->getPath();

                TempNeutralHit.num_of_ECAL_hits = Temp_num_of_ECAL_hits;

                NeutralsFD.push_back(TempNeutralHit);
                nHitnum++;
            }

            if (ECOUT_hit) {
                NeutralHit TempNeutralHit = NeutralHit();
                TempNeutralHit.reset();

                auto Temp_Neutron_ECAL_detlayer = PCAL_hit ? clas12::PCAL : ECIN_hit ? clas12::ECIN : ECOUT_hit ? clas12::ECOUT : -999.;

                TempNeutralHit.hitIndex = nHitnum, TempNeutralHit.pindex = i;

                TempNeutralHit.detector = 7, TempNeutralHit.layer = 3, TempNeutralHit.sector = allParticles[i]->sci(clas12::ECOUT)->getSector();  // Detector layer in veto, not CLAS12!

                TempNeutralHit.clas12particle_hit_in_PCAL = PCAL_hit, TempNeutralHit.clas12particle_hit_in_ECIN = ECIN_hit, TempNeutralHit.clas12particle_hit_in_ECOUT = ECOUT_hit;
                TempNeutralHit.Neutron_ECAL_detlayer = Temp_Neutron_ECAL_detlayer;

                TempNeutralHit.Lv = allParticles[i]->cal(Temp_Neutron_ECAL_detlayer)->getLv(), TempNeutralHit.Lw = allParticles[i]->cal(Temp_Neutron_ECAL_detlayer)->getLw();

                TempNeutralHit.xHit = allParticles[i]->sci(clas12::ECOUT)->getX(), TempNeutralHit.yHit = allParticles[i]->sci(clas12::ECOUT)->getY(),
                TempNeutralHit.zHit = allParticles[i]->sci(clas12::ECOUT)->getZ();

                TempNeutralHit.Px = allParticles[i]->par()->getPx(), TempNeutralHit.Py = allParticles[i]->par()->getPy(), TempNeutralHit.Pz = allParticles[i]->par()->getPz();

                TempNeutralHit.HitTime = allParticles[i]->sci(clas12::ECOUT)->getTime();

                TempNeutralHit.path = allParticles[i]->sci(clas12::ECOUT)->getPath();

                TempNeutralHit.num_of_ECAL_hits = Temp_num_of_ECAL_hits;

                NeutralsFD.push_back(TempNeutralHit);
                nHitnum++;
            }
        } else if (IsChargedFD) {  // Charged in FD
            if (FTOF1A_hit) {
                ChargedHit TempChargedHit = ChargedHit();
                TempChargedHit.reset();

                TempChargedHit.hitIndex = cHitnum;
                TempChargedHit.pindex = i;

                TempChargedHit.charge = allParticles[i]->par()->getCharge();
                TempChargedHit.PID = allParticles[i]->par()->getPid();
                TempChargedHit.detector = 12;
                TempChargedHit.layer = 1;
                TempChargedHit.sector = allParticles[i]->sci(clas12::FTOF1A)->getSector();
                TempChargedHit.xHit = allParticles[i]->sci(clas12::FTOF1A)->getX();
                TempChargedHit.yHit = allParticles[i]->sci(clas12::FTOF1A)->getY();
                TempChargedHit.zHit = allParticles[i]->sci(clas12::FTOF1A)->getZ();
                TempChargedHit.path = allParticles[i]->sci(clas12::FTOF1A)->getPath();
                TempChargedHit.HitTime = allParticles[i]->sci(clas12::FTOF1A)->getTime();

                ChargedFD.push_back(TempChargedHit);
                cHitnum++;
            }

            if (FTOF1B_hit) {
                ChargedHit TempChargedHit = ChargedHit();
                TempChargedHit.reset();

                TempChargedHit.hitIndex = cHitnum;
                TempChargedHit.pindex = i;
                TempChargedHit.charge = allParticles[i]->par()->getCharge();
                TempChargedHit.PID = allParticles[i]->par()->getPid();
                TempChargedHit.detector = 12;
                TempChargedHit.layer = 2;
                TempChargedHit.sector = allParticles[i]->sci(clas12::FTOF1B)->getSector();
                TempChargedHit.xHit = allParticles[i]->sci(clas12::FTOF1B)->getX();
                TempChargedHit.yHit = allParticles[i]->sci(clas12::FTOF1B)->getY();
                TempChargedHit.zHit = allParticles[i]->sci(clas12::FTOF1B)->getZ();
                TempChargedHit.path = allParticles[i]->sci(clas12::FTOF1B)->getPath();
                TempChargedHit.HitTime = allParticles[i]->sci(clas12::FTOF1B)->getTime();

                ChargedFD.push_back(TempChargedHit);
                cHitnum++;
            }

            if (FTOF2_hit) {
                ChargedHit TempChargedHit = ChargedHit();
                TempChargedHit.reset();

                TempChargedHit.hitIndex = cHitnum;
                TempChargedHit.pindex = i;
                TempChargedHit.charge = allParticles[i]->par()->getCharge();
                TempChargedHit.PID = allParticles[i]->par()->getPid();
                TempChargedHit.detector = 12;
                TempChargedHit.layer = 3;
                TempChargedHit.sector = allParticles[i]->sci(clas12::FTOF2)->getSector();
                TempChargedHit.xHit = allParticles[i]->sci(clas12::FTOF2)->getX();
                TempChargedHit.yHit = allParticles[i]->sci(clas12::FTOF2)->getY();
                TempChargedHit.zHit = allParticles[i]->sci(clas12::FTOF2)->getZ();
                TempChargedHit.path = allParticles[i]->sci(clas12::FTOF2)->getPath();
                TempChargedHit.HitTime = allParticles[i]->sci(clas12::FTOF2)->getTime();

                ChargedFD.push_back(TempChargedHit);
                cHitnum++;
            }

            if (PCAL_hit) {
                ChargedHit TempChargedHit = ChargedHit();
                TempChargedHit.reset();

                TempChargedHit.hitIndex = cHitnum;
                TempChargedHit.pindex = i;
                TempChargedHit.charge = allParticles[i]->par()->getCharge();
                TempChargedHit.PID = allParticles[i]->par()->getPid();
                TempChargedHit.detector = 7;
                TempChargedHit.layer = 1;  // Detector layer in veto, not CLAS12!
                TempChargedHit.sector = allParticles[i]->sci(clas12::PCAL)->getSector();
                TempChargedHit.xHit = allParticles[i]->sci(clas12::PCAL)->getX();
                TempChargedHit.yHit = allParticles[i]->sci(clas12::PCAL)->getY();
                TempChargedHit.zHit = allParticles[i]->sci(clas12::PCAL)->getZ();
                TempChargedHit.path = allParticles[i]->sci(clas12::PCAL)->getPath();
                TempChargedHit.HitTime = allParticles[i]->sci(clas12::PCAL)->getTime();

                ChargedFD.push_back(TempChargedHit);
                cHitnum++;
            }

            if (ECIN_hit) {
                ChargedHit TempChargedHit = ChargedHit();
                TempChargedHit.reset();

                TempChargedHit.hitIndex = cHitnum;
                TempChargedHit.pindex = i;
                TempChargedHit.charge = allParticles[i]->par()->getCharge();
                TempChargedHit.PID = allParticles[i]->par()->getPid();
                TempChargedHit.detector = 7;
                TempChargedHit.layer = 2;  // Detector layer in veto, not CLAS12!
                TempChargedHit.sector = allParticles[i]->sci(clas12::ECIN)->getSector();
                TempChargedHit.xHit = allParticles[i]->sci(clas12::ECIN)->getX();
                TempChargedHit.yHit = allParticles[i]->sci(clas12::ECIN)->getY();
                TempChargedHit.zHit = allParticles[i]->sci(clas12::ECIN)->getZ();
                TempChargedHit.path = allParticles[i]->sci(clas12::ECIN)->getPath();
                TempChargedHit.HitTime = allParticles[i]->sci(clas12::ECIN)->getTime();

                ChargedFD.push_back(TempChargedHit);
                cHitnum++;
            }

            if (ECOUT_hit) {
                ChargedHit TempChargedHit = ChargedHit();
                TempChargedHit.reset();

                TempChargedHit.hitIndex = cHitnum;
                TempChargedHit.pindex = i;
                TempChargedHit.charge = allParticles[i]->par()->getCharge();
                TempChargedHit.PID = allParticles[i]->par()->getPid();
                TempChargedHit.detector = 7;
                TempChargedHit.layer = 3;  // Detector layer in veto, not CLAS12!
                TempChargedHit.sector = allParticles[i]->sci(clas12::ECOUT)->getSector();
                TempChargedHit.xHit = allParticles[i]->sci(clas12::ECOUT)->getX();
                TempChargedHit.yHit = allParticles[i]->sci(clas12::ECOUT)->getY();
                TempChargedHit.zHit = allParticles[i]->sci(clas12::ECOUT)->getZ();
                TempChargedHit.path = allParticles[i]->sci(clas12::ECOUT)->getPath();
                TempChargedHit.HitTime = allParticles[i]->sci(clas12::ECOUT)->getTime();

                ChargedFD.push_back(TempChargedHit);
                cHitnum++;
            }
        }
    }

    return {NeutralsFD, ChargedFD};  // If we reached this point, then the neutral passes all veto conditions
}

#pragma endregion

#pragma region /* AddNeutrons function */

void AddNeutrons(std::vector<NeutralHit> FTOF_neut_hits, std::vector<NeutralHit> Mid_neut_hits, std::vector<ChargedHit> Ch_hits, std::vector<NeutralHit>& Test_Layer_neut_hits,
                          std::vector<NeutralHit>& Neut_hits_after_veto) {
    /*
    This function add neutral hits from the relevant test layer that pass the veto

    FTOF_neut_hits - neutral hits in the FTOF
    Mid_neut_hits - neutral hits in the layers before the test layer and not the FTOF layer (first layer)
    Ch_hits - all the charged hits from FTOF to the test layer included
    Test_Layer_neut_hits - the test layer of neutron hits
    neut - neutral hits that passed the veto.
    */

    bool Bad_neutral_hit;

    for (int i = 0; i < Test_Layer_neut_hits.size(); i++) {
        TVector3 Test_vector(Test_Layer_neut_hits.at(i).xHit, Test_Layer_neut_hits.at(i).yHit, Test_Layer_neut_hits.at(i).zHit);
        double Test_Layer_ToF = Test_Layer_neut_hits.at(i).HitTime;
        int Test_Layer_sector = Test_Layer_neut_hits.at(i).sector;

        Bad_neutral_hit = false;

#pragma region /* Step 1 */

        // Step 1 - Check that there was no neutral hit in the FTOF that fits test layer hit
        for (int j = 0; j < FTOF_neut_hits.size(); j++) {
            TVector3 FTOF_vector(FTOF_neut_hits.at(j).xHit, FTOF_neut_hits.at(j).yHit, FTOF_neut_hits.at(j).zHit);
            double FTOF_ToF = FTOF_neut_hits.at(j).HitTime;
            double FTOF_opening_ang = FTOF_vector.Angle(Test_vector) * TMath::RadToDeg();
            int FTOF_sector = FTOF_neut_hits.at(j).sector;

            if ((Test_Layer_sector - FTOF_sector == dSector_cut) && (FTOF_opening_ang < Opening_ang_cut_deg) && (TMath::Abs(Test_Layer_ToF - FTOF_ToF) < dHitTime_cut)) {
                Bad_neutral_hit = true;
            }
        }

#pragma endregion

#pragma region /* Step 2 */

        // TODO: find a way to extrapolate the track of the charged particle to the neutral hit on the tested layer

        // Step 2 - Check that there was no charged hit near neutral hit
        for (int j = 0; j < Ch_hits.size(); j++) {
            TVector3 Charged_vector(Ch_hits.at(j).xHit, Ch_hits.at(j).yHit, Ch_hits.at(j).zHit);
            double Charged_ToF = Ch_hits.at(j).HitTime;
            double Charged_opening_ang = Charged_vector.Angle(Test_vector) * TMath::RadToDeg();
            int Charged_sector = Ch_hits.at(j).sector;

            if ((Test_Layer_sector - Charged_sector == dSector_cut) && (Charged_opening_ang < Opening_ang_cut_deg) && (TMath::Abs(Test_Layer_ToF - Charged_ToF) < dHitTime_cut)) {
                Bad_neutral_hit = true;
            }
        }

#pragma endregion

#pragma region /* Step 3 */

        // Step 3 - Check that there was no neutral hit near the tested hit in layers before it
        for (int j = 0; j < Mid_neut_hits.size(); j++) {
            // if (Test_Layer_neut_hits.at(i).pindex == Mid_neut_hits.at(j).pindex) { continue; }

            TVector3 Neutral_vector(Mid_neut_hits.at(j).xHit, Mid_neut_hits.at(j).yHit, Mid_neut_hits.at(j).zHit);
            double Neutral_ToF = Mid_neut_hits.at(j).HitTime;
            double Neutral_opening_ang = Neutral_vector.Angle(Test_vector) * TMath::RadToDeg();
            int Neutral_sector = Mid_neut_hits.at(j).sector;

            if ((Test_Layer_sector - Neutral_sector == dSector_cut) && (Neutral_opening_ang < Opening_ang_cut_deg) && (TMath::Abs(Test_Layer_ToF - Neutral_ToF) < dHitTime_cut)) {
                Bad_neutral_hit = true;
                Mid_neut_hits.at(j).num_of_hits_ahead++;
            }
        }

#pragma endregion

#pragma region /* Step 4 */

        double xAvg = Test_Layer_neut_hits.at(i).xHit;
        double yAvg = Test_Layer_neut_hits.at(i).yHit;
        double zAvg = Test_Layer_neut_hits.at(i).zHit;
        double Num_of_near_neighbors = 1;

        // Step 4 - Check if there are neutral hits in the same sector near each other
        if (!Test_Layer_neut_hits.at(i).blocking) {
            for (int j = i + 1; j < Test_Layer_neut_hits.size(); j++) {
                TVector3 Neutral_vector(Test_Layer_neut_hits.at(j).xHit, Test_Layer_neut_hits.at(j).yHit, Test_Layer_neut_hits.at(j).zHit);
                double Neutral_ToF = Test_Layer_neut_hits.at(j).HitTime;
                double Neutral_opening_ang = Neutral_vector.Angle(Test_vector) * TMath::RadToDeg();
                int Neutral_sector = Test_Layer_neut_hits.at(j).sector;

                if ((Test_Layer_sector - Neutral_sector == dSector_cut) && (Neutral_opening_ang < Opening_ang_cut_deg) && (TMath::Abs(Test_Layer_ToF - Neutral_ToF) < dHitTime_cut) &&
                    (Test_Layer_neut_hits.at(j).blocking == false)) {
                    Num_of_near_neighbors++;
                    xAvg += Test_Layer_neut_hits.at(j).xHit;
                    yAvg += Test_Layer_neut_hits.at(j).yHit;
                    zAvg += Test_Layer_neut_hits.at(j).zHit;
                    Test_Layer_neut_hits.at(j).blocking = true;
                }
            }
        }

        // If there was not found a particle behind it and there is more than one near it
        if ((Bad_neutral_hit == false) && (Num_of_near_neighbors > 0) && !Test_Layer_neut_hits.at(i).blocking) {
            Test_Layer_neut_hits.at(i).xHit = xAvg / Num_of_near_neighbors;
            Test_Layer_neut_hits.at(i).yHit = yAvg / Num_of_near_neighbors;
            Test_Layer_neut_hits.at(i).zHit = zAvg / Num_of_near_neighbors;
        }

#pragma endregion

        // #pragma region /* Step 5 */

        //         // Step 5 - Check if there the neutral has three ECAL hits. If it does, it should not be tagged as a neutral
        //         if (Test_Layer_neut_hits.at(i).num_of_ECAL_hits == 3) { Bad_neutral_hit = true; }

        // #pragma endregion

        if (!Bad_neutral_hit && !Test_Layer_neut_hits.at(i).blocking && (Test_Layer_neut_hits.at(i).num_of_hits_ahead < 2)) { Neut_hits_after_veto.push_back(Test_Layer_neut_hits.at(i)); }
    }
}

#pragma endregion

#pragma region /* neutSelection function */

std::vector<NeutralHit> neutSelection(std::vector<NeutralHit> nHit, std::vector<ChargedHit> cHit) {
    std::vector<int> output;
    output.clear();

    std::vector<NeutralHit> vLay0n;  // FTOF all parts (we want to throw any neutral related to those hits)
    std::vector<NeutralHit> vLay1n;  // PCAL
    std::vector<NeutralHit> vLay2n;  // ECIN
    std::vector<NeutralHit> vLay3n;  // ECOUT

    std::vector<ChargedHit> vLay0c;  // FTOF all
    std::vector<ChargedHit> vLay1c;  // PCAL
    std::vector<ChargedHit> vLay2c;  // ECIN
    std::vector<ChargedHit> vLay3c;  // ECOUT

    // Introduce temporary vector that combine ALL preciding planes to be used as blocking layer.
    // This is done to avoid cases when we have hit in layer 1 and 3, without hit in layer 2.
    std::vector<NeutralHit> vTempLayersNeutral;
    std::vector<ChargedHit> vTempLayersCharged;

    std::vector<NeutralHit> vNeut;
    vNeut.clear();

    bool found = false;

    struct NeutralHit allHits;

    // Neutral hits:
    for (int i = 0; i < nHit.size(); i++) {
        int d = nHit.at(i).detector;
        int l = nHit.at(i).layer;

        // FTOF detector num in clas12 is 12 and ECAL && PCAL num is 7
        if (d == 12) { vLay0n.push_back(nHit.at(i)); }
        if (d == 7 && l == 1) { vLay1n.push_back(nHit.at(i)); }
        if (d == 7 && l == 2) { vLay2n.push_back(nHit.at(i)); }
        if (d == 7 && l == 3) { vLay3n.push_back(nHit.at(i)); }
    }

    // Charged hits:
    for (int i = 0; i < cHit.size(); i++) {
        int d = cHit.at(i).detector;
        int l = cHit.at(i).layer;

        // FTOF detector num in clas12 is 12 and ECAL && PCAL num is 7
        if (d == 12) { vLay0c.push_back(cHit.at(i)); }
        if (d == 7 && l == 1) { vLay1c.push_back(cHit.at(i)); }
        if (d == 7 && l == 2) { vLay2c.push_back(cHit.at(i)); }
        if (d == 7 && l == 3) { vLay3c.push_back(cHit.at(i)); }
    }

    vNeut.clear();

    // Whe start from neutral hits in ECOUT and go until PCAL

    // ECOUT hits:
    // Charged hits:
    // vTempLayersCharged is now all the charged hits from the FTOF to the ECOUT:
    vTempLayersCharged.clear();
    vTempLayersCharged.insert(vTempLayersCharged.end(), vLay0c.begin(), vLay0c.end());  // Append all elements from vLay0c to the end of vTempLayersCharged
    vTempLayersCharged.insert(vTempLayersCharged.end(), vLay1c.begin(), vLay1c.end());  // Append all elements from vLay1c to the end of vTempLayersCharged
    vTempLayersCharged.insert(vTempLayersCharged.end(), vLay2c.begin(), vLay2c.end());  // Append all elements from vLay2c to the end of vTempLayersCharged
    vTempLayersCharged.insert(vTempLayersCharged.end(), vLay3c.begin(), vLay3c.end());  // Append all elements from vLay3c to the end of vTempLayersCharged

    // Neutral hits:
    vTempLayersNeutral.clear();
    vTempLayersNeutral.insert(vTempLayersNeutral.end(), vLay1n.begin(), vLay1n.end());  // Append all elements from vLay1n to the end of vTempLayersNeutral
    vTempLayersNeutral.insert(vTempLayersNeutral.end(), vLay2n.begin(), vLay2n.end());  // Append all elements from vLay2n to the end of vTempLayersNeutral

    AddNeutrons(vLay0n,              // Neutral hits in the FTOF
                vTempLayersNeutral,  // Neutral hits in the layers before the test layer and not the FTOF layer (first layer)
                vTempLayersCharged,  // All the charged hits from the FTOF to the test layer included
                vLay3n,              // The test layer of neutron hits (here - ECOUT)
                vNeut                // Neutral hits that passed the veto
    );

    // ECIN hits:
    // Charged hits:
    // vTempLayersCharged is now all the charged hits from the FTOF to the ECIN:
    vTempLayersCharged.clear();
    vTempLayersCharged.insert(vTempLayersCharged.end(), vLay0c.begin(), vLay0c.end());
    vTempLayersCharged.insert(vTempLayersCharged.end(), vLay1c.begin(), vLay1c.end());
    vTempLayersCharged.insert(vTempLayersCharged.end(), vLay2c.begin(), vLay2c.end());

    // Neutral hits:
    vTempLayersNeutral.clear();
    vTempLayersNeutral.insert(vTempLayersNeutral.end(), vLay1n.begin(), vLay1n.end());  // Append all elements from vLay1n to the end of vTempLayersNeutral

    AddNeutrons(vLay0n,              // Neutral hits in the FTOF
                vTempLayersNeutral,  // Neutral hits in the layers before the test layer and not the FTOF layer (first layer)
                vTempLayersCharged,  // All the charged hits from the FTOF to the test layer included
                vLay2n,              // The test layer of neutron hits (here - ECIN)
                vNeut                // Neutral hits that passed the veto
    );

    // PCAL hits:
    // Charged hits:
    // vTempLayersCharged is now all the charged hits from the FTOF to the PCAL:
    vTempLayersCharged.clear();
    vTempLayersCharged.insert(vTempLayersCharged.end(), vLay0c.begin(), vLay0c.end());
    vTempLayersCharged.insert(vTempLayersCharged.end(), vLay1c.begin(), vLay1c.end());

    // Neutral hits:
    vTempLayersNeutral.clear();

    AddNeutrons(vLay0n,              // Neutral hits in the FTOF
                vTempLayersNeutral,  // Neutral hits in the layers before the test layer and not the FTOF layer (first layer)
                vTempLayersCharged,  // All the charged hits from the FTOF to the test layer included
                vLay1n,              // The test layer of neutron hits (here - PCAL)
                vNeut                // Neutral hits that passed the veto
    );

    return vNeut;
}

#pragma endregion

#pragma region /* NewNeutronFDVeto function */

// Get FD neutron as std::vector<region_part_ptr>:
std::vector<region_part_ptr> NewNeutronFDVeto(std::vector<region_part_ptr>& allParticles, std::vector<int>& outputParticles_ind) {
    /*
    This function applies the new veto to neutrons in the FD

    allParticles - all particles in the event

    returns - allParticles with neutrons that passed the veto
    */

    auto [NeutralsFD, ChargedFD] = GenerateHits(allParticles);

    std::vector<NeutralHit> NeutronsAfterVeto = neutSelection(NeutralsFD, ChargedFD);

    std::vector<region_part_ptr> outputParticles;
    outputParticles.clear();

    for (int i = 0; i < NeutronsAfterVeto.size(); i++) {
        int pindex = NeutronsAfterVeto.at(i).pindex;

        outputParticles.push_back(allParticles[pindex]);
        outputParticles_ind.push_back(pindex);
    }

    return outputParticles;
}

// Get FD neutron as std::vector<NeutralHit>:
std::vector<NeutralHit> GetNewNeutronFDVetoHits(std::vector<region_part_ptr>& allParticles, std::vector<int>& outputParticles_ind) {
    auto [NeutralsFD, ChargedFD] = GenerateHits(allParticles);

    std::vector<NeutralHit> NeutronsAfterVeto = neutSelection(NeutralsFD, ChargedFD);

    return NeutronsAfterVeto;
}

#pragma endregion

#endif  // NEWVETO_FUNCTIONS