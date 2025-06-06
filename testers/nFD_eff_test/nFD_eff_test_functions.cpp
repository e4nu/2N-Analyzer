// framework nFD_eff_tester_run.sh
// git pull && clas12root -b -q testers/nFD_eff_test.cpp

#ifndef NFD_EFF_TEST_FUNCTIONS
#define NFD_EFF_TEST_FUNCTIONS

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
#include <cstdlib>
#include <iostream>
#include <typeinfo>
#include <vector>

// Include libraries:
#include "../../framework/namespaces/general_utilities/utilities.h"

// Include classes:
#include "../../framework/classes/AMaps/AMaps.cpp"

// Include CLAS12 libraries:
#include "../../framework/includes/clas12_include.h"

using namespace utilities;

// #pragma region /* CheckForECALHits function */
// void CheckForECALHits(bool& ParticleInPCAL, bool& ParticleInECIN, bool& ParticleInECOUT, short& Neutron_ECAL_detlayer, vector<region_part_ptr>& allParticles, const int& i) {
//     ParticleInPCAL = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7);    // PCAL hit
//     ParticleInECIN = (allParticles[i]->cal(clas12::ECIN)->getDetector() == 7);    // ECIN hit
//     ParticleInECOUT = (allParticles[i]->cal(clas12::ECOUT)->getDetector() == 7);  // ECOUT hit
//     Neutron_ECAL_detlayer = ParticleInECIN ? clas12::ECIN : clas12::ECOUT;        // find first layer of hit
// }
// #pragma endregion

// #pragma region /* CalcToFnFD function */

// double CalcToFnFD(region_part_ptr NeutronFD, double starttime = 9999) {
//     bool ParticleInPCAL = (NeutronFD->cal(clas12::PCAL)->getDetector() == 7);                       // PCAL hit
//     bool ParticleInECIN = (NeutronFD->cal(clas12::ECIN)->getDetector() == 7);                       // ECIN hit
//     bool ParticleInECOUT = (NeutronFD->cal(clas12::ECOUT)->getDetector() == 7);                     // ECOUT hit
//     auto detlayer = ParticleInPCAL ? clas12::PCAL : ParticleInECIN ? clas12::ECIN : clas12::ECOUT;  // determine the earliest layer of the neutral hit

//     // double reco_ToF_nFD = NeutronFD->getTime() - starttime;
//     double reco_ToF_nFD = NeutronFD->cal(detlayer)->getTime() - starttime;

//     return reco_ToF_nFD;
// }

// #pragma endregion

// #pragma region /* CalcPathnFD function */

// double CalcPathnFD(region_part_ptr NeutronFD, region_part_ptr electron) {
//     bool ParticleInPCAL = (NeutronFD->cal(clas12::PCAL)->getDetector() == 7);                       // PCAL hit
//     bool ParticleInECIN = (NeutronFD->cal(clas12::ECIN)->getDetector() == 7);                       // ECIN hit
//     bool ParticleInECOUT = (NeutronFD->cal(clas12::ECOUT)->getDetector() == 7);                     // ECOUT hit
//     auto detlayer = ParticleInPCAL ? clas12::PCAL : ParticleInECIN ? clas12::ECIN : clas12::ECOUT;  // determine the earliest layer of the neutral hit

//     TVector3 v_nvtx_3v;  // Neutron's vertex location
//     v_nvtx_3v.SetXYZ(electron->par()->getVx(), electron->par()->getVy(), electron->par()->getVz());
//     // v_nvtx_3v.SetXYZ(NeutronFD->par()->getVx(), NeutronFD->par()->getVy(), NeutronFD->par()->getVz());

//     TVector3 v_hit_3v;  // Neutron's hit location in CND
//     v_hit_3v.SetXYZ(NeutronFD->cal(detlayer)->getX(), NeutronFD->cal(detlayer)->getY(), NeutronFD->cal(detlayer)->getZ());

//     TVector3 v_path_3v = v_hit_3v - v_nvtx_3v;  // Direct calculation of neutron's path (in vector form)

//     double reco_Path_nFD = v_path_3v.Mag();

//     return reco_Path_nFD;
// }

// #pragma endregion

// #pragma region /* CalcPnFD function */

// double CalcPnFD(region_part_ptr NeutronFD, region_part_ptr electron, double starttime = 9999) {
//     double reco_Path_nFD = CalcPathnFD(NeutronFD, electron);
//     // double reco_Path_nFD = NeutronFD->getPath();
//     double reco_ToF_nFD = CalcToFnFD(NeutronFD, starttime);
//     double reco_Beta_nFD = reco_Path_nFD / (reco_ToF_nFD * c);
//     double reco_Gamma_nFD = 1 / sqrt(1 - (reco_Beta_nFD * reco_Beta_nFD));

//     double Momentum = m_n * reco_Beta_nFD * reco_Gamma_nFD;

//     /*
//      if (ParticlePDG == 2112) {
//         Momentum = NeutronFD->par()->getP();
//     } else if (ParticlePDG == 22) {
//         double Beta_ph = NeutronFD->par()->getBeta();
//         double Gamma_ph = 1 / sqrt(1 - (Beta_ph * Beta_ph));
//         Momentum = m_n * Beta_ph * Gamma_ph;
//     } else {
//         cout << "\n\nError! Particle PDG is not 22 or 2112! Aborting...\n\n", exit(1);
//     }
//     */

//     // Momentum = NeutronFD->par()->getP();

//     return Momentum;
// }

// #pragma endregion

// #pragma region /* checkEcalDiagCuts function */

// bool checkEcalDiagCuts(region_part_ptr electrons) {
//     double ecal_diag_cut = 0.2;  // diagonal cut on SF

//     double mom = electrons->par()->getP();
//     // true if inside cut
//     if (electrons->par()->getPid() == 11) {
//         if ((electrons->cal(clas12::PCAL)->getEnergy() + electrons->cal(clas12::ECIN)->getEnergy()) / mom > ecal_diag_cut && mom > 4.5) {
//             return true;
//         } else if (mom <= 4.5) {
//             return true;
//         } else {
//             return false;
//         }
//     } else {
//         return true;
//     }
// }

// #pragma endregion

// #pragma region /* DCEdgeCuts function */

// bool DCEdgeCuts(region_part_ptr p) {
//     std::vector<double> dc_edge_cut_el = {4.5, 3.5, 7.5};  // units cm; {region1, region2, region3} cuts for electrons INBENDING
//     std::vector<double> dc_edge_cut_ptr = {2.5, 3, 10.5};  // units cm; {region1, region2, region3} cuts for protons  OUTBENDING

//     // true if inside cut
//     // cut all charged particles
//     if (p->par()->getCharge() != 0) {
//         auto traj_index_1 = p->traj(DC, 6)->getIndex();   // layer 1
//         auto traj_index_2 = p->traj(DC, 18)->getIndex();  // layer 2
//         auto traj_index_3 = p->traj(DC, 36)->getIndex();  // layer 3

//         auto traj_edge_1 = p->traj(DC, 6)->getFloat("edge", traj_index_1);
//         auto traj_edge_2 = p->traj(DC, 18)->getFloat("edge", traj_index_2);
//         auto traj_edge_3 = p->traj(DC, 36)->getFloat("edge", traj_index_3);

//         // PUT DC EDGE CUTS IN PARAMETER FILE

//         // electron DC cuts
//         if (p->par()->getCharge() < 0 && (dc_edge_cut_el.size() == 3 && traj_edge_1 > dc_edge_cut_el[0] && traj_edge_2 > dc_edge_cut_el[1] && traj_edge_3 > dc_edge_cut_el[2])) {
//             return true;
//         }
//         // proton DC cuts
//         else if (p->par()->getCharge() > 0 && (dc_edge_cut_ptr.size() == 3 && traj_edge_1 > dc_edge_cut_ptr[0] && traj_edge_2 > dc_edge_cut_ptr[1] && traj_edge_3 > dc_edge_cut_ptr[2])) {
//             return true;
//         } else {
//             return false;
//         }
//     } else {
//         return true;
//     }
// }

// #pragma endregion

// #pragma region /* fillDCdebug function */

// void fillDCdebug(region_part_ptr p, TH2D** h, double weight) {
//     //  if(p->par()->getPid() == 11)
//     //    {
//     h[1]->Fill(p->traj(DC, 6)->getX(), p->traj(DC, 6)->getY(), weight);
//     h[2]->Fill(p->traj(DC, 18)->getX(), p->traj(DC, 18)->getY(), weight);
//     h[3]->Fill(p->traj(DC, 36)->getX(), p->traj(DC, 36)->getY(), weight);
//     //    }
// }

// #pragma endregion

// #pragma region /* CalcdTheta function */

// double CalcdTheta(double dThetaTemp) {
//     double dTheta;

//     if (dThetaTemp < 0) {
//         dTheta = fabs(dThetaTemp);
//     } else {
//         dTheta = dThetaTemp;
//     }

//     return dTheta;
// }

// #pragma endregion

// #pragma region /* CalcdPhi1 function */

// double CalcdPhi1(double dPhiTemp) {
//     double dPhi;

//     if (dPhiTemp > 180.0) {
//         dPhi = dPhiTemp - 360.0;
//     } else if (dPhiTemp < -180.0) {
//         dPhi = dPhiTemp + 360.0;
//     } else {
//         dPhi = dPhiTemp;
//     }

//     return dPhi;
// }

// #pragma endregion

// #pragma region /* GetPhi_e function */

// // Function to find the angle closest to 180 degrees away from phi_N
// double GetPhi_e(TString OutPutFolder, double phi_N) {
//     double phi_e_offset = 0.;  // Electron phi_e offset due to the solenoid field

//     std::string OutPutFolder0(OutPutFolder.Data());

//     if (FindSubstring(OutPutFolder0, "2070MeV")) {
//         phi_e_offset = 16.;
//     } else if (FindSubstring(OutPutFolder0, "4029MeV")) {
//         phi_e_offset = 7.;
//     } else if (FindSubstring(OutPutFolder0, "5986MeV")) {
//         phi_e_offset = 5.;
//     }

//     std::vector<double> possible_angles = {-120, -60, 0, 60, 120, 180};

//     // Calculate the target angle (180 degrees away from phi_N)
//     double target_angle = CalcdPhi1(phi_N + 180);

//     // Find the closest possible angle
//     double closest_angle = possible_angles[0];
//     double min_diff = std::abs(CalcdPhi1(target_angle - closest_angle));

//     for (double angle : possible_angles) {
//         double diff = std::abs(CalcdPhi1(target_angle - angle));
//         if (diff < min_diff) {
//             min_diff = diff;
//             closest_angle = angle;
//         }
//     }

//     return closest_angle + phi_e_offset;
//     // return closest_angle;
// }

// #pragma endregion

// #pragma region /* NeutronECAL_Cut_Veto function */

// bool NeutronECAL_Cut_Veto(vector<region_part_ptr>& allParticles, vector<region_part_ptr>& electrons, const double& beamE, const int& index, bool apply_PCAL_neutral_veto,
//                           const double& cPart_veto_radius, const double& nPart_veto_radius) {
//     TVector3 p_b(0, 0, beamE); /* beam energy */

//     TVector3 p_e; /* our electron */
//     p_e.SetMagThetaPhi(electrons[0]->getP(), electrons[0]->getTheta(), electrons[0]->getPhi());
//     TVector3 p_q = p_b - p_e; /* 3-momentum transfer */

//     if (allParticles[index]->par()->getCharge() != 0) { return false; } /* determine if the particle is neutral or not */

//     // Check which layers of the ECAL have been hit
//     TVector3 p_n_Angles;
//     p_n_Angles.SetMagThetaPhi(1, allParticles[index]->getTheta(), allParticles[index]->getPhi()); /* calculate the angles of the neutral particle */

//     /* check where did the particle hit.
//      * no hit - we'll get 0
//      * we have a hit - we'll get a 7 (7 is the ID of the calorimeter).
//        Can also be done by checking deposited energy (double comparison), yet this method is better (int comparison) */
//     bool PC = (allParticles[index]->cal(clas12::PCAL)->getDetector() == 7);
//     bool IC = (allParticles[index]->cal(clas12::ECIN)->getDetector() == 7);
//     bool OC = (allParticles[index]->cal(clas12::ECOUT)->getDetector() == 7);
//     auto detlayer = PC ? clas12::PCAL : IC ? clas12::ECIN : clas12::ECOUT; /* determine the earliest layer that the neutral hit in */

//     /* v_nhit = location of neutral particle hit */
//     TVector3 v_nhit(allParticles[index]->cal(detlayer)->getX(), allParticles[index]->cal(detlayer)->getY(), allParticles[index]->cal(detlayer)->getZ());
//     double beta = allParticles[index]->par()->getBeta();
//     double gamma = 1 / sqrt(1 - (beta * beta));
//     double theta_n = p_n_Angles.Theta() * 180 / pi;
//     double theta_q = p_q.Theta() * 180 / pi;
//     double theta_nq = p_n_Angles.Angle(p_q) * 180 / pi;

//     if (beta < 0) { return false; }
//     //    if (beta > 1.1) { return false; }
//     //    // physics cuts, to be ignored according to Larry.
//     //    if (theta_nq > 25) { return false; }
//     //    if (theta_q > 40) { return false; }
//     if (theta_n < 1) { return false; } /* to avoid events with theta_n = 0 (the "1" is in deg) */
//     if (!(IC || OC)) { return false; } /* hit only one of these layers */
//     if (PC) { return false; }          /* to veto out the gammas (photons) */

//     // Now let's put a charge particle veto
//     bool Veto = false;
//     for (int j = 0; j < allParticles.size(); j++) {
//         if (apply_PCAL_neutral_veto) {
//             if (allParticles[j]->par()->getCharge() != 0) { /* looking on charged particles */
//                 TVector3 v_charged_hit;                     /* v_charged_hit = location of charged particle hit */

//                 if ((detlayer == clas12::ECIN) && (allParticles[j]->cal(clas12::ECIN)->getZ() != 0)) {
//                     /* if both particles hit the inner calorimeter, use the inner calorimeter to determine v_charged_hit */
//                     v_charged_hit.SetXYZ(allParticles[j]->cal(clas12::ECIN)->getX(), allParticles[j]->cal(clas12::ECIN)->getY(), allParticles[j]->cal(clas12::ECIN)->getZ());
//                     TVector3 v_dist = v_nhit - v_charged_hit;

//                     if (v_dist.Mag() < cPart_veto_radius) { Veto = true; }
//                 } else if ((detlayer == clas12::ECOUT) && (allParticles[j]->cal(clas12::ECOUT)->getZ() != 0)) {
//                     /* if both particles hit the outer calorimeter, use the outer calorimeter to determine v_charged_hit */
//                     v_charged_hit.SetXYZ(allParticles[j]->cal(clas12::ECOUT)->getX(), allParticles[j]->cal(clas12::ECOUT)->getY(), allParticles[j]->cal(clas12::ECOUT)->getZ());
//                     TVector3 v_dist = v_nhit - v_charged_hit;

//                     if (v_dist.Mag() < cPart_veto_radius) { Veto = true; }
//                 } else {
//                     /* the neutral has to hit either the ECIN or ECOUT.
//                        If the charged particle hit the other calorimeter, then look at where the charged particle was expected to be according to the trajectory. */
//                     int trajlayer = (detlayer == clas12::ECIN) ? 4 : 7;
//                     v_charged_hit.SetXYZ(allParticles[j]->traj(clas12::ECAL, trajlayer)->getX(), allParticles[j]->traj(clas12::ECAL, trajlayer)->getY(),
//                                          allParticles[j]->traj(clas12::ECAL, trajlayer)->getZ());
//                     TVector3 v_dist = v_nhit - v_charged_hit;

//                     if (v_dist.Mag() < cPart_veto_radius) { Veto = true; }
//                 }
//             } else {
//                 bool neutral_hit_PCAL = (                                        //
//                     (allParticles[j]->par()->getCharge() == 0)                   // Neutral particle
//                     && (allParticles[j]->cal(clas12::PCAL)->getDetector() == 7)  // PCAL hit
//                 );
//                 bool same_sector = (allParticles[j]->cal(clas12::PCAL)->getSector() == allParticles[index]->cal(detlayer)->getSector());

//                 /*
//                 if (neutral_hit_PCAL && same_sector) { Veto = true; }
//                 */

//                 /*
//                 TVector3 v_neutral_hit;  // v_neutral_hit = location of neutral particle hit

//                 if (neutral_hit_PCAL && (allParticles[j]->cal(clas12::PCAL)->getZ() != 0)) {
//                     v_neutral_hit.SetXYZ(allParticles[j]->cal(clas12::PCAL)->getX(), allParticles[j]->cal(clas12::PCAL)->getY(), allParticles[j]->cal(clas12::PCAL)->getZ());
//                     TVector3 v_dist = v_nhit - v_neutral_hit;

//                     if (v_dist.Mag() < nPart_veto_radius) { Veto = true; }
//                 }
//                 */

//                 /*
//                 bool PCALneutral = neutral_hit_PCAL;

//                 if (PCALneutral) {
//                     TVector3 v_neutral_hit;

//                     int trajlayer = (detlayer == clas12::ECIN) ? 4 : 7;
//                     v_neutral_hit.SetXYZ(allParticles[j]->traj(clas12::ECAL, trajlayer)->getX(), allParticles[j]->traj(clas12::ECAL, trajlayer)->getY(),
//                                          allParticles[j]->traj(clas12::ECAL, trajlayer)->getZ());
//                     TVector3 v_dist = v_nhit - v_neutral_hit;

//                     if (v_dist.Mag() < nPart_veto_radius) { Veto = true; }
//                 }
//                 */

//                 bool PCALneutral = neutral_hit_PCAL;

//                 if (PCALneutral) {
//                     TVector3 v_PCALn_hit; /* v_PCALn_hit = location of PCAL neutral hit */

//                     if ((detlayer == clas12::ECIN) && (allParticles[j]->cal(clas12::ECIN)->getZ() != 0)) {
//                         /* if both particles hit the inner calorimeter, use the inner calorimeter to determine v_PCALn_hit */
//                         v_PCALn_hit.SetXYZ(allParticles[j]->cal(clas12::ECIN)->getX(), allParticles[j]->cal(clas12::ECIN)->getY(), allParticles[j]->cal(clas12::ECIN)->getZ());
//                         TVector3 v_dist = v_nhit - v_PCALn_hit;

//                         if (v_dist.Mag() < nPart_veto_radius) { Veto = true; }
//                     } else if ((detlayer == clas12::ECOUT) && (allParticles[j]->cal(clas12::ECOUT)->getZ() != 0)) {
//                         /* if both particles hit the outer calorimeter, use the outer calorimeter to determine v_PCALn_hit */
//                         v_PCALn_hit.SetXYZ(allParticles[j]->cal(clas12::ECOUT)->getX(), allParticles[j]->cal(clas12::ECOUT)->getY(), allParticles[j]->cal(clas12::ECOUT)->getZ());
//                         TVector3 v_dist = v_nhit - v_PCALn_hit;

//                         if (v_dist.Mag() < nPart_veto_radius) { Veto = true; }
//                     } else {
//                         /* the neutral has to hit either the ECIN or ECOUT.
//                            If the PCAL neutral hit the other calorimeter, then look at where the PCAL neutral was expected to be according to the trajectory. */
//                         int trajlayer = (detlayer == clas12::ECIN) ? 4 : 7;
//                         v_PCALn_hit.SetXYZ(allParticles[j]->traj(clas12::ECAL, trajlayer)->getX(), allParticles[j]->traj(clas12::ECAL, trajlayer)->getY(),
//                                            allParticles[j]->traj(clas12::ECAL, trajlayer)->getZ());
//                         TVector3 v_dist = v_nhit - v_PCALn_hit;

//                         if (v_dist.Mag() < nPart_veto_radius) { Veto = true; }
//                     }
//                 }
//             }
//         } else {
//             if (allParticles[j]->par()->getCharge() == 0) { continue; } /* looking on charged particles only */
//             TVector3 v_chit;                                            /* v_chit = location of charged particle hit */

//             if ((detlayer == clas12::ECIN) && (allParticles[j]->cal(clas12::ECIN)->getZ() != 0)) {
//                 /* if both particles hit the inner calorimeter, use the inner calorimeter to determine v_chit */
//                 v_chit.SetXYZ(allParticles[j]->cal(clas12::ECIN)->getX(), allParticles[j]->cal(clas12::ECIN)->getY(), allParticles[j]->cal(clas12::ECIN)->getZ());
//                 TVector3 v_dist = v_nhit - v_chit;

//                 if (v_dist.Mag() < cPart_veto_radius) { Veto = true; }
//             } else if ((detlayer == clas12::ECOUT) && (allParticles[j]->cal(clas12::ECOUT)->getZ() != 0)) {
//                 /* if both particles hit the outer calorimeter, use the outer calorimeter to determine v_chit */
//                 v_chit.SetXYZ(allParticles[j]->cal(clas12::ECOUT)->getX(), allParticles[j]->cal(clas12::ECOUT)->getY(), allParticles[j]->cal(clas12::ECOUT)->getZ());
//                 TVector3 v_dist = v_nhit - v_chit;

//                 if (v_dist.Mag() < cPart_veto_radius) { Veto = true; }
//             } else {
//                 /* the neutral has to hit either the ECIN or ECOUT.
//                    If the charged particle hit the other calorimeter, then look at where the charged particle was expected to be according to the trajectory. */
//                 int trajlayer = (detlayer == clas12::ECIN) ? 4 : 7;
//                 v_chit.SetXYZ(allParticles[j]->traj(clas12::ECAL, trajlayer)->getX(), allParticles[j]->traj(clas12::ECAL, trajlayer)->getY(),
//                               allParticles[j]->traj(clas12::ECAL, trajlayer)->getZ());
//                 TVector3 v_dist = v_nhit - v_chit;

//                 if (v_dist.Mag() < cPart_veto_radius) { Veto = true; }
//             }
//         }
//     }

//     if (Veto) { return false; } /* if any of the vetoes are true, return false */

//     return true; /* we survived up to this point, we do have a neutral particle */
// }

// #pragma endregion

#endif  // NFD_EFF_TEST_FUNCTIONS