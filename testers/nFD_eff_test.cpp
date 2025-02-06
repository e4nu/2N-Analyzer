// git pull && clas12root -b -q testers/nFD_eff_test.cpp

#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

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

// #include "../source/classes/ParticleID/ParticleID.cpp"
#include "../source/constants.h"
#include "HipoChain.h"
#include "clas12reader.h"

using namespace std;
using namespace clas12;

double CalcPnFD(region_part_ptr NeutronFD, double starttime = 9999) {
    int ParticlePDG = NeutronFD->par()->getPid();

    double Momentum;

    bool ParticleInPCAL = (NeutronFD->cal(clas12::PCAL)->getDetector() == 7);                       // PCAL hit
    bool ParticleInECIN = (NeutronFD->cal(clas12::ECIN)->getDetector() == 7);                       // ECIN hit
    bool ParticleInECOUT = (NeutronFD->cal(clas12::ECOUT)->getDetector() == 7);                     // ECOUT hit
    auto detlayer = ParticleInPCAL ? clas12::PCAL : ParticleInECIN ? clas12::ECIN : clas12::ECOUT;  // determine the earliest layer of the neutral hit

    double reco_Path_nFD = NeutronFD->getPath();
    double reco_ToF_nFD = NeutronFD->cal(detlayer)->getTime() - starttime;
    double reco_Beta_nFD = reco_Path_nFD / (reco_ToF_nFD * c);
    double reco_Gamma_nFD = 1 / sqrt(1 - (reco_Beta_nFD * reco_Beta_nFD));
    Momentum = m_n * reco_Beta_nFD * reco_Gamma_nFD;

    // double Beta_ph = NeutronFD->par()->getBeta();
    // double Path_ph = NeutronFD->getPath();
    // double Time_ph_from_Beta_ph = Path_ph / (c * Beta_ph);
    // double Gamma_ph = 1 / sqrt(1 - (Beta_ph * Beta_ph));
    // Momentum = m_n * Beta_ph * Gamma_ph;

    // double Beta_ph = NeutronFD->par()->getBeta();
    // double Gamma_ph = 1 / sqrt(1 - (Beta_ph * Beta_ph));
    // Momentum = m_n * Beta_ph * Gamma_ph;

    // if (ParticlePDG == 2112) {
    //     Momentum = NeutronFD->par()->getP();
    // } else if (ParticlePDG == 22) {
    //     double Beta_ph = NeutronFD->par()->getBeta();
    //     double Gamma_ph = 1 / sqrt(1 - (Beta_ph * Beta_ph));
    //     Momentum = m_n * Beta_ph * Gamma_ph;
    // } else {
    //     cout << "\n\nError! Particle PDG is not 22 or 2112! Aborting...\n\n", exit(0);
    // }

    // Momentum = NeutronFD->par()->getP();

    return Momentum;
}

double CalcdTheta(double dThetaTemp) {
    double dTheta;

    if (dThetaTemp < 0) {
        dTheta = fabs(dThetaTemp);
    } else {
        dTheta = dThetaTemp;
    }

    return dTheta;
}

double CalcdPhi(double dPhiTemp) {
    double dPhi;

    if (dPhiTemp > 180.0) {
        dPhi = dPhiTemp - 360.0;
    } else if (dPhiTemp < -180.0) {
        dPhi = dPhiTemp + 360.0;
    } else {
        dPhi = dPhiTemp;
    }

    return dPhi;
}

bool findSubstring(string string1, string string2) {
    if (string1.find(string2) != string::npos) {
        return true;
    } else {
        return false;
    }
}

bool NeutronECAL_Cut_Veto(vector<region_part_ptr>& allParticles, vector<region_part_ptr>& electrons, const double& beamE, const int& index, const double& veto_cut) {
    TVector3 p_b(0, 0, beamE); /* beam energy */

    TVector3 p_e; /* our electron */
    p_e.SetMagThetaPhi(electrons[0]->getP(), electrons[0]->getTheta(), electrons[0]->getPhi());
    TVector3 p_q = p_b - p_e; /* 3-momentum transfer */

    if (allParticles[index]->par()->getCharge() != 0) { return false; } /* determine if the particle is neutral or not */

    // Check which layers of the ECAL have been hit
    TVector3 p_n_Angles;
    p_n_Angles.SetMagThetaPhi(1, allParticles[index]->getTheta(), allParticles[index]->getPhi()); /* calculate the angles of the neutral particle */

    /* check where did the particle hit.
     * no hit - we'll get 0
     * we have a hit - we'll get a 7 (7 is the ID of the calorimeter).
       Can also be done by checking deposited energy (double comparison), yet this method is better (int comparison) */
    bool PC = (allParticles[index]->cal(clas12::PCAL)->getDetector() == 7);
    bool IC = (allParticles[index]->cal(clas12::ECIN)->getDetector() == 7);
    bool OC = (allParticles[index]->cal(clas12::ECOUT)->getDetector() == 7);
    auto detlayer = PC ? clas12::PCAL : IC ? clas12::ECIN : clas12::ECOUT; /* determine the earliest layer that the neutral hit in */

    /* v_nhit = location of neutral particle hit */
    TVector3 v_nhit(allParticles[index]->cal(detlayer)->getX(), allParticles[index]->cal(detlayer)->getY(), allParticles[index]->cal(detlayer)->getZ());
    double beta = allParticles[index]->par()->getBeta();
    double gamma = 1 / sqrt(1 - (beta * beta));
    double theta_n = p_n_Angles.Theta() * 180 / pi;
    double theta_q = p_q.Theta() * 180 / pi;
    double theta_nq = p_n_Angles.Angle(p_q) * 180 / pi;

    if (beta < 0) { return false; }
    //    if (beta > 1.1) { return false; }
    //    // physics cuts, to be ignored according to Larry.
    //    if (theta_nq > 25) { return false; }
    //    if (theta_q > 40) { return false; }
    if (theta_n < 1) { return false; } /* to avoid events with theta_n = 0 (the "1" is in deg) */
    if (!(IC || OC)) { return false; } /* hit only one of these layers */
    if (PC) { return false; }          /* to veto out the gammas (photons) */

    // Now let's put a charge particle veto
    bool Veto = false;
    for (int j = 0; j < allParticles.size(); j++) {
        if (allParticles[j]->par()->getCharge() != 0) { /* looking on charged particles */
            TVector3 v_charged_hit;                     /* v_charged_hit = location of charged particle hit */

            if ((detlayer == clas12::ECIN) && (allParticles[j]->cal(clas12::ECIN)->getZ() != 0)) {
                /* if both particles hit the inner calorimeter, use the inner calorimeter to determine v_charged_hit */
                v_charged_hit.SetXYZ(allParticles[j]->cal(clas12::ECIN)->getX(), allParticles[j]->cal(clas12::ECIN)->getY(), allParticles[j]->cal(clas12::ECIN)->getZ());
                TVector3 v_dist = v_nhit - v_charged_hit;

                if (v_dist.Mag() < veto_cut) { Veto = true; }
            } else if ((detlayer == clas12::ECOUT) && (allParticles[j]->cal(clas12::ECOUT)->getZ() != 0)) {
                /* if both particles hit the outer calorimeter, use the outer calorimeter to determine v_charged_hit */
                v_charged_hit.SetXYZ(allParticles[j]->cal(clas12::ECOUT)->getX(), allParticles[j]->cal(clas12::ECOUT)->getY(), allParticles[j]->cal(clas12::ECOUT)->getZ());
                TVector3 v_dist = v_nhit - v_charged_hit;

                if (v_dist.Mag() < veto_cut) { Veto = true; }
            } else {
                /* the neutral has to hit either the ECIN or ECOUT.
                   If the charged particle hit the other calorimeter, then look at where the charged particle was expected to be according to the trajectory. */
                int trajlayer = (detlayer == clas12::ECIN) ? 4 : 7;
                v_charged_hit.SetXYZ(allParticles[j]->traj(clas12::ECAL, trajlayer)->getX(), allParticles[j]->traj(clas12::ECAL, trajlayer)->getY(),
                                     allParticles[j]->traj(clas12::ECAL, trajlayer)->getZ());
                TVector3 v_dist = v_nhit - v_charged_hit;

                if (v_dist.Mag() < veto_cut) { Veto = true; }
            }
        } else {
            bool neutral_hit_PCAL = (allParticles[j]->cal(clas12::PCAL)->getDetector() == 7);

            TVector3 v_neutral_hit; /* v_neutral_hit = location of neutral particle hit */

            if (neutral_hit_PCAL && (allParticles[j]->cal(clas12::PCAL)->getZ() != 0)) {
                /* if other neutral hit the PCAL, use use it to determine v_neutral_hit */
                v_neutral_hit.SetXYZ(allParticles[j]->cal(clas12::PCAL)->getX(), allParticles[j]->cal(clas12::PCAL)->getY(), allParticles[j]->cal(clas12::PCAL)->getZ());
                TVector3 v_dist = v_nhit - v_neutral_hit;

                if (v_dist.Mag() < 10000. * veto_cut) { Veto = true; }
            }
        }

        // if (allParticles[j]->par()->getCharge() == 0) { continue; } /* looking on charged particles only */
        // TVector3 v_chit;                                            /* v_chit = location of charged particle hit */

        // if ((detlayer == clas12::ECIN) && (allParticles[j]->cal(clas12::ECIN)->getZ() != 0)) {
        //     /* if both particles hit the inner calorimeter, use the inner calorimeter to determine v_chit */
        //     v_chit.SetXYZ(allParticles[j]->cal(clas12::ECIN)->getX(), allParticles[j]->cal(clas12::ECIN)->getY(), allParticles[j]->cal(clas12::ECIN)->getZ());
        //     TVector3 v_dist = v_nhit - v_chit;

        //     if (v_dist.Mag() < veto_cut) { Veto = true; }
        // } else if ((detlayer == clas12::ECOUT) && (allParticles[j]->cal(clas12::ECOUT)->getZ() != 0)) {
        //     /* if both particles hit the outer calorimeter, use the outer calorimeter to determine v_chit */
        //     v_chit.SetXYZ(allParticles[j]->cal(clas12::ECOUT)->getX(), allParticles[j]->cal(clas12::ECOUT)->getY(), allParticles[j]->cal(clas12::ECOUT)->getZ());
        //     TVector3 v_dist = v_nhit - v_chit;

        //     if (v_dist.Mag() < veto_cut) { Veto = true; }
        // } else {
        //     /* the neutral has to hit either the ECIN or ECOUT.
        //        If the charged particle hit the other calorimeter, then look at where the charged particle was expected to be according to the trajectory. */
        //     int trajlayer = (detlayer == clas12::ECIN) ? 4 : 7;
        //     v_chit.SetXYZ(allParticles[j]->traj(clas12::ECAL, trajlayer)->getX(), allParticles[j]->traj(clas12::ECAL, trajlayer)->getY(),
        //                   allParticles[j]->traj(clas12::ECAL, trajlayer)->getZ());
        //     TVector3 v_dist = v_nhit - v_chit;

        //     if (v_dist.Mag() < veto_cut) { Veto = true; }
        // }
    }

    if (Veto) { return false; } /* if any of the vetoes are true, return false */

    return true; /* we survived up to this point, we do have a neutral particle */
}

void nFD_eff_test() {
    cout << "\n\nInitiating nFD_eff_test.cpp\n";

    bool Is2GeV = false, Is4GeV = false, Is6GeV = false;
    // double Ebeam = 4.02962;
    //  Is4GeV = true;
    double Ebeam = 5.98636;
    Is6GeV = true;

    bool ConstrainTLmom = false;

    double P_upperLim;
    if (ConstrainTLmom) {
        P_upperLim = Ebeam * 0.5;
    } else {
        P_upperLim = Ebeam * 1.1;
    }

    // int Limiter = 10000000;
    // int Limiter = 1000000;
    int Limiter = 100000;

    const string OutputDir = "/lustre24/expphy/volatile/clas12/asportes/Analysis_output/nFD_eff_test";
    system(("rm -rf " + OutputDir).c_str());
    system(("mkdir -p " + OutputDir).c_str());

    TFile* outFile = new TFile("/lustre24/expphy/volatile/clas12/asportes/Analysis_output/nFD_eff_test/nFD_eff_test.root", "RECREATE");

    clas12root::HipoChain chain;
    string InputFiles;

    if (Is4GeV) {
        InputFiles = "/lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco/Uniform_e-p-n_samples/4029MeV/OutPut_en/reconhipo/*.hipo";
    } else if (Is6GeV) {
        InputFiles = "/lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco/Uniform_e-p-n_samples/5986MeV/OutPut_en/reconhipo/*.hipo";
    }

    chain.Add(InputFiles);
    chain.SetReaderTags({0});
    chain.db()->turnOffQADB();
    auto config_c12 = chain.GetC12Reader();
    const std::unique_ptr<clas12::clas12reader>& c12 = chain.C12ref();

#pragma region /* Prepare histograms */
    /////////////////////////////////////
    // Prepare histograms
    /////////////////////////////////////
    vector<TH1*> HistoList;
    vector<string> HistSubjects;
    vector<string> HistSubjects2;
    vector<bool> FirstPrint;

    gStyle->SetTitleXSize(0.05);
    gStyle->SetTitleYSize(0.05);

    gStyle->SetTitleXOffset(0.8);
    gStyle->SetTitleYOffset(0.8);

    char temp_name[100];
    char temp_title[100];

#pragma region /* RAW */
    TH1D* h_reco_P_e_1e_cut = new TH1D("reco_P_e_1e_cut", "P^{reco}_{e} in 1e cut;P^{reco}_{e} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_reco_P_e_1e_cut);
    TH1D* h_truth_P_e_1e_cut = new TH1D("truth_P_e_1e_cut", "P^{truth}_{e} in 1e cut;P^{truth}_{e} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_truth_P_e_1e_cut);
    TH1D* h_reco_theta_e_1e_cut = new TH1D("reco_theta_e_1e_cut", "#theta^{reco}_{e} in 1e cut;#theta^{reco}_{e} [#circ];Counts", 50, 0, 45.);
    HistoList.push_back(h_reco_theta_e_1e_cut);
    TH1D* h_truth_theta_e_1e_cut = new TH1D("truth_theta_e_1e_cut", "#theta^{truth}_{e} in 1e cut;#theta^{truth}_{e} [#circ];Counts", 50, 0, 45.);
    HistoList.push_back(h_truth_theta_e_1e_cut);
    TH1D* h_reco_phi_e_1e_cut = new TH1D("reco_phi_e_1e_cut", "#phi^{reco}_{e} in 1e cut;#phi^{reco}_{e} [#circ];Counts", 50, -180, 180.);
    HistoList.push_back(h_reco_phi_e_1e_cut);
    TH1D* h_truth_phi_e_1e_cut = new TH1D("truth_phi_e_1e_cut", "#phi^{truth}_{e} in 1e cut;#phi^{truth}_{e} [#circ];Counts", 50, -180, 180.);
    HistoList.push_back(h_truth_phi_e_1e_cut);
    TH2D* h_reco_theta_e_VS_reco_phi_e_1e_cut =
        new TH2D("reco_theta_e_VS_reco_phi_e_1e_cut", "#theta^{reco}_{e} vs. #phi^{reco}_{e} in 1e cut;#phi^{reco}_{e} [#circ];#theta^{reco}_{e} [#circ]", 100, -180., 180., 100, 0, 50.);
    HistoList.push_back(h_reco_theta_e_VS_reco_phi_e_1e_cut);
    TH2D* h_truth_theta_e_VS_truth_phi_e_1e_cut = new TH2D(
        "truth_theta_e_VS_truth_phi_e_1e_cut", "#theta^{truth}_{e} vs. #phi^{truth}_{e} in 1e cut;#phi^{truth}_{e} [#circ];#theta^{truth}_{e} [#circ]", 100, -180., 180., 100, 0, 50.);
    HistoList.push_back(h_truth_theta_e_VS_truth_phi_e_1e_cut);

    TH1D* h_reco_P_n_1e_cut = new TH1D("reco_P_n_1e_cut", "P^{reco}_{n} in 1e cut;P^{reco}_{n} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_reco_P_n_1e_cut);
    TH1D* h_truth_P_n_1e_cut = new TH1D("truth_P_n_1e_cut", "P^{truth}_{n} in 1e cut;P^{truth}_{n} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_truth_P_n_1e_cut);
    TH1D* h_reco_theta_n_1e_cut = new TH1D("reco_theta_n_1e_cut", "#theta^{reco}_{n} in 1e cut;#theta^{reco}_{n} [#circ];Counts", 50, 0, 45.);
    HistoList.push_back(h_reco_theta_n_1e_cut);
    TH1D* h_truth_theta_n_1e_cut = new TH1D("truth_theta_n_1e_cut", "#theta^{truth}_{n} in 1e cut;#theta^{truth}_{n} [#circ];Counts", 50, 0, 45.);
    HistoList.push_back(h_truth_theta_n_1e_cut);
    TH1D* h_reco_phi_n_1e_cut = new TH1D("reco_phi_n_1e_cut", "#phi^{reco}_{n} in 1e cut;#phi^{reco}_{n} [#circ];Counts", 50, -180, 180.);
    HistoList.push_back(h_reco_phi_n_1e_cut);
    TH1D* h_truth_phi_n_1e_cut = new TH1D("truth_phi_n_1e_cut", "#phi^{truth}_{n} in 1e cut;#phi^{truth}_{n} [#circ];Counts", 50, -180, 180.);
    HistoList.push_back(h_truth_phi_n_1e_cut);
    TH2D* h_reco_theta_n_VS_reco_phi_n_1e_cut =
        new TH2D("reco_theta_n_VS_reco_phi_n_1e_cut", "#theta^{reco}_{n} vs. #phi^{reco}_{n} in 1e cut;#phi^{reco}_{n} [#circ];#theta^{reco}_{n} [#circ]", 100, -180., 180., 100, 0, 50.);
    HistoList.push_back(h_reco_theta_n_VS_reco_phi_n_1e_cut);
    TH2D* h_truth_theta_n_VS_truth_phi_n_1e_cut = new TH2D(
        "truth_theta_n_VS_truth_phi_n_1e_cut", "#theta^{truth}_{n} vs. #phi^{truth}_{n} in 1e cut;#phi^{truth}_{n} [#circ];#theta^{truth}_{n} [#circ]", 100, -180., 180., 100, 0, 50.);
    HistoList.push_back(h_truth_theta_n_VS_truth_phi_n_1e_cut);
#pragma endregion

#pragma region /* clas12reco */
    HistSubjects.push_back("clas12reco");
    HistSubjects2.push_back("#splitline{FD neutron from}{clas12reco}");
    FirstPrint.push_back(true);
    TH1D* h_reco_P_nFD_clas12_1e_cut = new TH1D("reco_P_nFD_clas12_1e_cut", "P^{reco}_{nFD} in 1e cut (clas12reco);P^{reco}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_reco_P_nFD_clas12_1e_cut);
    TH1D* h_truth_P_nFD_clas12_1e_cut = new TH1D("truth_P_nFD_clas12_1e_cut", "P^{truth}_{nFD} in 1e cut (clas12reco);P^{truth}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_truth_P_nFD_clas12_1e_cut);
    TH1D* h_reco_theta_nFD_clas12_1e_cut = new TH1D("reco_theta_nFD_clas12_1e_cut", "#theta^{reco}_{nFD} in 1e cut (clas12reco);#theta^{reco}_{nFD} [#circ];Counts", 50, 0, 45.);
    HistoList.push_back(h_reco_theta_nFD_clas12_1e_cut);
    TH1D* h_truth_theta_nFD_clas12_1e_cut = new TH1D("truth_theta_nFD_clas12_1e_cut", "#theta^{truth}_{nFD} in 1e cut (clas12reco);#theta^{truth}_{nFD} [#circ];Counts", 50, 0, 45.);
    HistoList.push_back(h_truth_theta_nFD_clas12_1e_cut);
    TH1D* h_reco_phi_nFD_clas12_1e_cut = new TH1D("reco_phi_nFD_clas12_1e_cut", "#phi^{reco}_{nFD} in 1e cut (clas12reco);#phi^{reco}_{nFD} [#circ];Counts", 50, -180, 180.);
    HistoList.push_back(h_reco_phi_nFD_clas12_1e_cut);
    TH1D* h_truth_phi_nFD_clas12_1e_cut = new TH1D("truth_phi_nFD_clas12_1e_cut", "#phi^{truth}_{nFD} in 1e cut (clas12reco);#phi^{truth}_{nFD} [#circ];Counts", 50, -180, 180.);
    HistoList.push_back(h_truth_phi_nFD_clas12_1e_cut);
    TH2D* h_reco_theta_nFD_clas12_VS_reco_phi_nFD_clas12_1e_cut =
        new TH2D("reco_theta_nFD_clas12_VS_reco_phi_nFD_clas12_1e_cut",
                 "#theta^{reco}_{nFD} vs. #phi^{reco}_{nFD} in 1e cut (clas12reco);#phi^{reco}_{nFD} [#circ];#theta^{reco}_{nFD} [#circ]", 100, -180., 180., 100, 0, 50.);
    HistoList.push_back(h_reco_theta_nFD_clas12_VS_reco_phi_nFD_clas12_1e_cut);
    TH2D* h_truth_theta_nFD_clas12_VS_truth_phi_nFD_clas12_1e_cut =
        new TH2D("truth_theta_nFD_clas12_VS_truth_phi_nFD_clas12_1e_cut",
                 "#theta^{truth}_{nFD} vs. #phi^{truth}_{nFD} in 1e cut (clas12reco);#phi^{truth}_{nFD} [#circ];#theta^{truth}_{nFD} [#circ]", 100, -180., 180., 100, 0, 50.);
    HistoList.push_back(h_truth_theta_nFD_clas12_VS_truth_phi_nFD_clas12_1e_cut);
    TH2D* h_reco_P_e_VS_P_nFD_clas12_1e_cut = new TH2D(
        "reco_P_e_VS_P_nFD_clas12_1e_cut", "P^{reco}_{e} vs. P^{reco}_{nFD} in 1e cut (clas12reco);P^{reco}_{e} [GeV/c];P^{reco}_{nFD} [GeV/c]", 100, 0., Ebeam * 2., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_e_VS_P_nFD_clas12_1e_cut);
    TH2D* h_reco_theta_nFD_clas12_VS_P_nFD_clas12_1e_cut =
        new TH2D("reco_theta_nFD_clas12_VS_P_nFD_clas12_1e_cut", "#theta^{reco}_{nFD} vs. P^{reco}_{nFD} in 1e cut (clas12reco);#theta^{reco}_{nFD} [#circ];P^{reco}_{nFD} [GeV/c]", 100, 0.,
                 50., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_theta_nFD_clas12_VS_P_nFD_clas12_1e_cut);
#pragma endregion

#pragma region /* redef */
    HistSubjects.push_back("redef");
    HistSubjects2.push_back("redef");
    FirstPrint.push_back(true);
    TH1D* h_reco_P_nFD_redef_1e_cut = new TH1D("reco_P_nFD_redef_1e_cut", "P^{reco}_{nFD} in 1e cut (redef);P^{reco}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_reco_P_nFD_redef_1e_cut);
    TH1D* h_truth_P_nFD_redef_1e_cut = new TH1D("truth_P_nFD_redef_1e_cut", "P^{truth}_{nFD} in 1e cut (redef);P^{truth}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_truth_P_nFD_redef_1e_cut);
    TH1D* h_reco_theta_nFD_redef_1e_cut = new TH1D("reco_theta_nFD_redef_1e_cut", "#theta^{reco}_{nFD} in 1e cut (redef);#theta^{reco}_{nFD} [#circ];Counts", 50, 0, 45.);
    HistoList.push_back(h_reco_theta_nFD_redef_1e_cut);
    TH1D* h_truth_theta_nFD_redef_1e_cut = new TH1D("truth_theta_nFD_redef_1e_cut", "#theta^{truth}_{nFD} in 1e cut (redef);#theta^{truth}_{nFD} [#circ];Counts", 50, 0, 45.);
    HistoList.push_back(h_truth_theta_nFD_redef_1e_cut);
    TH1D* h_reco_phi_nFD_redef_1e_cut = new TH1D("reco_phi_nFD_redef_1e_cut", "#phi^{reco}_{nFD} in 1e cut (redef);#phi^{reco}_{nFD} [#circ];Counts", 50, -180, 180.);
    HistoList.push_back(h_reco_phi_nFD_redef_1e_cut);
    TH1D* h_truth_phi_nFD_redef_1e_cut = new TH1D("truth_phi_nFD_redef_1e_cut", "#phi^{truth}_{nFD} in 1e cut (redef);#phi^{truth}_{nFD} [#circ];Counts", 50, -180, 180.);
    HistoList.push_back(h_truth_phi_nFD_redef_1e_cut);
    TH2D* h_reco_theta_nFD_redef_VS_reco_phi_nFD_redef_1e_cut =
        new TH2D("reco_theta_nFD_redef_VS_reco_phi_nFD_redef_1e_cut", "#theta^{reco}_{nFD} vs. #phi^{reco}_{nFD} in 1e cut (redef);#phi^{reco}_{nFD} [#circ];#theta^{reco}_{nFD} [#circ]",
                 100, -180., 180., 100, 0, 50.);
    HistoList.push_back(h_reco_theta_nFD_redef_VS_reco_phi_nFD_redef_1e_cut);
    TH2D* h_truth_theta_nFD_redef_VS_truth_phi_nFD_redef_1e_cut =
        new TH2D("truth_theta_nFD_redef_VS_truth_phi_nFD_redef_1e_cut",
                 "#theta^{truth}_{nFD} vs. #phi^{truth}_{nFD} in 1e cut (redef);#phi^{truth}_{nFD} [#circ];#theta^{truth}_{nFD} [#circ]", 100, -180., 180., 100, 0, 50.);
    HistoList.push_back(h_truth_theta_nFD_redef_VS_truth_phi_nFD_redef_1e_cut);
    TH2D* h_reco_P_e_VS_P_nFD_redef_1e_cut =
        new TH2D("reco_P_e_VS_P_nFD_redef_1e_cut", "P^{reco}_{e} vs. P^{reco}_{nFD} in 1e cut (redef);P^{reco}_{e} [GeV/c];P^{reco}_{nFD} [GeV/c]", 100, 0., Ebeam * 2., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_e_VS_P_nFD_redef_1e_cut);
    TH2D* h_reco_theta_nFD_redef_VS_P_nFD_redef_1e_cut =
        new TH2D("reco_theta_nFD_redef_VS_P_nFD_redef_1e_cut", "#theta^{reco}_{nFD} vs. P^{reco}_{nFD} in 1e cut (redef);#theta^{reco}_{nFD} [#circ];P^{reco}_{nFD} [GeV/c]", 100, 0., 50.,
                 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_theta_nFD_redef_VS_P_nFD_redef_1e_cut);
#pragma endregion

#pragma region /* ECALveto */
    HistSubjects.push_back("ECALveto");
    HistSubjects2.push_back("#splitline{ECALveto}{and P_{nFD} thresholds}");
    FirstPrint.push_back(true);

#pragma region /* Base histograms (ECALveto) */
    TH1D* h_reco_P_nFD_ECALveto_1e_cut = new TH1D("reco_P_nFD_ECALveto_1e_cut", "P^{reco}_{nFD} in 1e cut (ECALveto);P^{reco}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_reco_P_nFD_ECALveto_1e_cut);
    TH1D* h_truth_P_nFD_ECALveto_1e_cut = new TH1D("truth_P_nFD_ECALveto_1e_cut", "P^{truth}_{nFD} in 1e cut (ECALveto);P^{truth}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_truth_P_nFD_ECALveto_1e_cut);
    TH1D* h_reco_theta_nFD_ECALveto_1e_cut = new TH1D("reco_theta_nFD_ECALveto_1e_cut", "#theta^{reco}_{nFD} in 1e cut (ECALveto);#theta^{reco}_{nFD} [#circ];Counts", 50, 0, 45.);
    HistoList.push_back(h_reco_theta_nFD_ECALveto_1e_cut);
    TH1D* h_truth_theta_nFD_ECALveto_1e_cut = new TH1D("truth_theta_nFD_ECALveto_1e_cut", "#theta^{truth}_{nFD} in 1e cut (ECALveto);#theta^{truth}_{nFD} [#circ];Counts", 50, 0, 45.);
    HistoList.push_back(h_truth_theta_nFD_ECALveto_1e_cut);
    TH1D* h_reco_phi_nFD_ECALveto_1e_cut = new TH1D("reco_phi_nFD_ECALveto_1e_cut", "#phi^{reco}_{nFD} in 1e cut (ECALveto);#phi^{reco}_{nFD} [#circ];Counts", 50, -180, 180.);
    HistoList.push_back(h_reco_phi_nFD_ECALveto_1e_cut);
    TH1D* h_truth_phi_nFD_ECALveto_1e_cut = new TH1D("truth_phi_nFD_ECALveto_1e_cut", "#phi^{truth}_{nFD} in 1e cut (ECALveto);#phi^{truth}_{nFD} [#circ];Counts", 50, -180, 180.);
    HistoList.push_back(h_truth_phi_nFD_ECALveto_1e_cut);
    TH2D* h_reco_theta_nFD_ECALveto_VS_reco_phi_nFD_ECALveto_1e_cut =
        new TH2D("reco_theta_nFD_ECALveto_VS_reco_phi_nFD_ECALveto_1e_cut",
                 "#theta^{reco}_{nFD} vs. #phi^{reco}_{nFD} in 1e cut (ECALveto);#phi^{reco}_{nFD} [#circ];#theta^{reco}_{nFD} [#circ]", 100, -180., 180., 100, 0, 50.);
    HistoList.push_back(h_reco_theta_nFD_ECALveto_VS_reco_phi_nFD_ECALveto_1e_cut);
    TH2D* h_truth_theta_nFD_ECALveto_VS_truth_phi_nFD_ECALveto_1e_cut =
        new TH2D("truth_theta_nFD_ECALveto_VS_truth_phi_nFD_ECALveto_1e_cut",
                 "#theta^{truth}_{nFD} vs. #phi^{truth}_{nFD} in 1e cut (ECALveto);#phi^{truth}_{nFD} [#circ];#theta^{truth}_{nFD} [#circ]", 100, -180., 180., 100, 0, 50.);
    HistoList.push_back(h_truth_theta_nFD_ECALveto_VS_truth_phi_nFD_ECALveto_1e_cut);

    TH2D* h_reco_P_e_VS_P_nFD_ECALveto_1e_cut = new TH2D(
        "reco_P_e_VS_P_nFD_ECALveto_1e_cut", "P^{reco}_{e} vs. P^{reco}_{nFD} in 1e cut (ECALveto);P^{reco}_{e} [GeV/c];P^{reco}_{nFD} [GeV/c]", 100, 0., Ebeam * 2., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_e_VS_P_nFD_ECALveto_1e_cut);
    TH2D* h_reco_theta_e_VS_P_nFD_ECALveto_1e_cut =
        new TH2D("reco_theta_e_VS_P_nFD_ECALveto_1e_cut", "#theta^{reco}_{e} vs. P^{reco}_{nFD} in 1e cut (ECALveto);#theta^{reco}_{e} [#circ];P^{reco}_{nFD} [GeV/c]", 100, 0, 45., 100, 0.,
                 Ebeam * 3.);
    HistoList.push_back(h_reco_theta_e_VS_P_nFD_ECALveto_1e_cut);
    TH2D* h_reco_phi_e_VS_P_nFD_ECALveto_1e_cut =
        new TH2D("reco_phi_e_VS_P_nFD_ECALveto_1e_cut", "#phi^{reco}_{e} vs. P^{reco}_{nFD} in 1e cut (ECALveto);#phi^{reco}_{e} [#circ];P^{reco}_{nFD} [GeV/c]", 100, -180., 180., 100, 0.,
                 Ebeam * 3.);
    HistoList.push_back(h_reco_phi_e_VS_P_nFD_ECALveto_1e_cut);
    TH2D* h_reco_P_e_VS_theta_nFD_ECALveto_1e_cut =
        new TH2D("reco_P_e_VS_theta_nFD_ECALveto_1e_cut", "P^{reco}_{e} vs. #theta^{reco}_{nFD} in 1e cut (ECALveto);P^{reco}_{e} [GeV/c];#theta^{reco}_{nFD} [#circ]", 100, 0., P_upperLim,
                 100, 0, 45.);
    HistoList.push_back(h_reco_P_e_VS_theta_nFD_ECALveto_1e_cut);
    TH2D* h_reco_theta_e_VS_theta_nFD_ECALveto_1e_cut =
        new TH2D("reco_theta_e_VS_theta_nFD_ECALveto_1e_cut", "#theta^{reco}_{e} vs. #theta^{reco}_{nFD} in 1e cut (ECALveto);#theta^{reco}_{e} [#circ];#theta^{reco}_{nFD} [#circ]", 100, 0,
                 45., 100, 0, 45.);
    HistoList.push_back(h_reco_theta_e_VS_theta_nFD_ECALveto_1e_cut);
    TH2D* h_reco_phi_e_VS_theta_nFD_ECALveto_1e_cut =
        new TH2D("reco_phi_e_VS_theta_nFD_ECALveto_1e_cut", "#phi^{reco}_{e} vs. #theta^{reco}_{nFD} in 1e cut (ECALveto);#phi^{reco}_{e} [#circ];#theta^{reco}_{nFD} [#circ]", 100, -180.,
                 180., 100, 0, 45.);
    HistoList.push_back(h_reco_phi_e_VS_theta_nFD_ECALveto_1e_cut);
    TH2D* h_reco_P_e_VS_phi_nFD_ECALveto_1e_cut =
        new TH2D("reco_P_e_VS_phi_nFD_ECALveto_1e_cut", "P^{reco}_{e} vs. #phi^{reco}_{nFD} in 1e cut (ECALveto);P^{reco}_{e} [GeV/c];#phi^{reco}_{nFD} [#circ]", 100, 0., P_upperLim, 100,
                 -180., 180.);
    HistoList.push_back(h_reco_P_e_VS_phi_nFD_ECALveto_1e_cut);
    TH2D* h_reco_theta_e_VS_phi_nFD_ECALveto_1e_cut =
        new TH2D("reco_phi_e_VS_phi_nFD_ECALveto_1e_cut", "#theta^{reco}_{e} vs. #phi^{reco}_{nFD} in 1e cut (ECALveto);#theta^{reco}_{e} [#circ];#phi^{reco}_{nFD} [#circ]", 100, 0, 45.,
                 100, -180., 180.);
    HistoList.push_back(h_reco_theta_e_VS_phi_nFD_ECALveto_1e_cut);
    TH2D* h_reco_phi_e_VS_phi_nFD_ECALveto_1e_cut =
        new TH2D("reco_phi_e_VS_phi_nFD_ECALveto_1e_cut", "#phi^{reco}_{e} vs. #phi^{reco}_{nFD} in 1e cut (ECALveto);#phi^{reco}_{e} [#circ];#phi^{reco}_{nFD} [#circ]", 100, -180., 180.,
                 100, -180., 180.);
    HistoList.push_back(h_reco_phi_e_VS_phi_nFD_ECALveto_1e_cut);
    TH2D* h_reco_theta_nFD_ECALveto_VS_P_nFD_ECALveto_1e_cut =
        new TH2D("reco_theta_nFD_ECALveto_VS_P_nFD_ECALveto_1e_cut", "#theta^{reco}_{nFD} vs. P^{reco}_{nFD} in 1e cut (ECALveto);#theta^{reco}_{nFD} [#circ];P^{reco}_{nFD} [GeV/c]", 100,
                 0., 45., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_theta_nFD_ECALveto_VS_P_nFD_ECALveto_1e_cut);
    TH1D* h_reco_nFD_multi_ECALveto_1e_cut = new TH1D("reco_nFD_multi_ECALveto_1e_cut", "reco nFD multiplicity in 1e cut (ECALveto);nFD multiplicity;Counts", 9, 1, 10);
    HistoList.push_back(h_reco_nFD_multi_ECALveto_1e_cut);
    TH2D* h_reco_nFD_multi_VS_reco_P_nFD_ECALveto_1e_cut =
        new TH2D("reco_nFD_multi_VS_reco_P_nFD_ECALveto_1e_cut", "reco nFD multiplicity vs. P^{reco}_{nFD} in 1e cut (ECALveto);P^{reco}_{nFD} [GeV/c];nFD multiplicity", 100, 0., Ebeam * 3.,
                 9, 1, 10);
    HistoList.push_back(h_reco_nFD_multi_VS_reco_P_nFD_ECALveto_1e_cut);
#pragma endregion

    TH2D* h_reco_P_nFD_VS_reco_L_ECAL_ECALveto_1e_cut =
        new TH2D("reco_P_nFD_VS_reco_L_ECAL_ECALveto_1e_cut", "P^{reco}_{nFD} vs. L^{reco}_{nFD,ECAL} in 1e cut (ECALveto);L^{reco}_{nFD,ECAL} [cm];P^{reco}_{nFD} [#circ]", 100, 700., 850.,
                 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_nFD_VS_reco_L_ECAL_ECALveto_1e_cut);
    TH2D* h_reco_theta_nFD_VS_reco_L_ECAL_ECALveto_1e_cut =
        new TH2D("reco_theta_nFD_VS_reco_L_ECAL_ECALveto_1e_cut", "#theta^{reco}_{nFD} vs. L^{reco}_{nFD,ECAL} in 1e cut (ECALveto);L^{reco}_{nFD,ECAL} [cm];#theta^{reco}_{nFD} [#circ]",
                 100, 700., 850., 100, 0., 45.);
    HistoList.push_back(h_reco_theta_nFD_VS_reco_L_ECAL_ECALveto_1e_cut);
    TH2D* h_reco_phi_nFD_VS_reco_L_ECAL_ECALveto_1e_cut =
        new TH2D("reco_phi_nFD_VS_reco_L_ECAL_ECALveto_1e_cut", "#phi^{reco}_{nFD} vs. L^{reco}_{nFD,ECAL} in 1e cut (ECALveto);L^{reco}_{nFD,ECAL} [cm];#phi^{reco}_{nFD} [#circ]", 100,
                 700., 850., 100, -180., 180.);
    HistoList.push_back(h_reco_phi_nFD_VS_reco_L_ECAL_ECALveto_1e_cut);
    TH2D* h_reco_P_nFD_VS_ToF_nFD_fromFile_ECAL_ECALveto_1e_cut =
        new TH2D("reco_P_nFD_VS_ToF_nFD_fromFile_ECAL_ECALveto_1e_cut", "P^{reco}_{nFD} vs. t^{reco}_{ToF,nFD,ECAL} in 1e cut (ECALveto);t^{reco}_{ToF,nFD,ECAL} [ns];P^{reco}_{nFD} [#circ]",
                 100, 15., 75., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_nFD_VS_ToF_nFD_fromFile_ECAL_ECALveto_1e_cut);
    TH2D* h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECAL_ECALveto_1e_cut =
        new TH2D("reco_theta_nFD_VS_ToF_nFD_fromFile_ECAL_ECALveto_1e_cut",
                 "#theta^{reco}_{nFD} vs. t^{reco}_{ToF,nFD,ECAL} in 1e cut (ECALveto);t^{reco}_{ToF,nFD,ECAL} [ns];#theta^{reco}_{nFD} [#circ]", 100, 15., 75., 100, 0., 45.);
    HistoList.push_back(h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECAL_ECALveto_1e_cut);
    TH2D* h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECAL_ECALveto_1e_cut =
        new TH2D("reco_phi_nFD_VS_ToF_nFD_fromFile_ECAL_ECALveto_1e_cut",
                 "#phi^{reco}_{nFD} vs. t^{reco}_{ToF,nFD,ECAL} in 1e cut (ECALveto);t^{reco}_{ToF,nFD,ECAL} [ns];#phi^{reco}_{nFD} [#circ]", 100, 15., 75., 100, -180., 180.);
    HistoList.push_back(h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECAL_ECALveto_1e_cut);
    // TH2D* h_reco_P_nFD_VS_eff_L_ECAL_ECALveto_1e_cut =
    //     new TH2D("reco_P_nFD_VS_eff_L_ECAL_ECALveto_1e_cut",
    //              "P^{reco}_{nFD} vs. L^{eff}_{nFD,ECAL} in 1e cut (ECALveto);L^{eff}_{nFD,ECAL} = c#beta^{truth}t^{#beta^{reco}_{nFD}}_{ToF} [cm];P^{reco}_{nFD} [#circ]", 100, 700., 850.,
    //              100, 0., Ebeam * 3.);
    // HistoList.push_back(h_reco_P_nFD_VS_eff_L_ECAL_ECALveto_1e_cut);
    // TH2D* h_reco_theta_nFD_VS_eff_L_ECAL_ECALveto_1e_cut =
    //     new TH2D("reco_theta_nFD_VS_eff_L_ECAL_ECALveto_1e_cut",
    //              "#theta^{reco}_{nFD} vs. L^{eff}_{nFD,ECAL} in 1e cut (ECALveto);L^{eff}_{nFD,ECAL} = c#beta^{truth}t^{#beta^{reco}_{nFD}}_{ToF} [cm];#theta^{reco}_{nFD} [#circ]", 100,
    //              700., 850., 100, 0., 45.);
    // HistoList.push_back(h_reco_theta_nFD_VS_eff_L_ECAL_ECALveto_1e_cut);
    // TH2D* h_reco_phi_nFD_VS_eff_L_ECAL_ECALveto_1e_cut =
    //     new TH2D("reco_phi_nFD_VS_eff_L_ECAL_ECALveto_1e_cut",
    //              "#phi^{reco}_{nFD} vs. L^{eff}_{nFD,ECAL} in 1e cut (ECALveto);L^{eff}_{nFD,ECAL} = c#beta^{truth}t^{#beta^{reco}_{nFD}}_{ToF} [cm];#phi^{reco}_{nFD} [#circ]", 100, 700.,
    //              850., 100, -180., 180.);
    // HistoList.push_back(h_reco_phi_nFD_VS_eff_L_ECAL_ECALveto_1e_cut);

    TH2D* h_reco_P_nFD_VS_reco_L_ECIN_ECALveto_1e_cut =
        new TH2D("reco_P_nFD_VS_reco_L_ECIN_ECALveto_1e_cut", "P^{reco}_{nFD} vs. L^{reco}_{nFD,ECIN} in 1e cut (ECALveto);L^{reco}_{nFD,ECIN} [cm];P^{reco}_{nFD} [#circ]", 100, 700., 850.,
                 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_nFD_VS_reco_L_ECIN_ECALveto_1e_cut);
    TH2D* h_reco_theta_nFD_VS_reco_L_ECIN_ECALveto_1e_cut =
        new TH2D("reco_theta_nFD_VS_reco_L_ECIN_ECALveto_1e_cut", "#theta^{reco}_{nFD} vs. L^{reco}_{nFD,ECIN} in 1e cut (ECALveto);L^{reco}_{nFD,ECIN} [cm];#theta^{reco}_{nFD} [#circ]",
                 100, 700., 850., 100, 0., 45.);
    HistoList.push_back(h_reco_theta_nFD_VS_reco_L_ECIN_ECALveto_1e_cut);
    TH2D* h_reco_phi_nFD_VS_reco_L_ECIN_ECALveto_1e_cut =
        new TH2D("reco_phi_nFD_VS_reco_L_ECIN_ECALveto_1e_cut", "#phi^{reco}_{nFD} vs. L^{reco}_{nFD,ECIN} in 1e cut (ECALveto);L^{reco}_{nFD,ECIN} [cm];#phi^{reco}_{nFD} [#circ]", 100,
                 700., 850., 100, -180., 180.);
    HistoList.push_back(h_reco_phi_nFD_VS_reco_L_ECIN_ECALveto_1e_cut);
    TH2D* h_reco_P_nFD_VS_ToF_nFD_fromFile_ECIN_ECALveto_1e_cut =
        new TH2D("reco_P_nFD_VS_ToF_nFD_fromFile_ECIN_ECALveto_1e_cut", "P^{reco}_{nFD} vs. t^{reco}_{ToF,nFD,ECIN} in 1e cut (ECALveto);t^{reco}_{ToF,nFD,ECIN} [ns];P^{reco}_{nFD} [#circ]",
                 100, 15., 75., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_nFD_VS_ToF_nFD_fromFile_ECIN_ECALveto_1e_cut);
    TH2D* h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECIN_ECALveto_1e_cut =
        new TH2D("reco_theta_nFD_VS_ToF_nFD_fromFile_ECIN_ECALveto_1e_cut",
                 "#theta^{reco}_{nFD} vs. t^{reco}_{ToF,nFD,ECIN} in 1e cut (ECALveto);t^{reco}_{ToF,nFD,ECIN} [ns];#theta^{reco}_{nFD} [#circ]", 100, 15., 75., 100, 0., 45.);
    HistoList.push_back(h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECIN_ECALveto_1e_cut);
    TH2D* h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECIN_ECALveto_1e_cut =
        new TH2D("reco_phi_nFD_VS_ToF_nFD_fromFile_ECIN_ECALveto_1e_cut",
                 "#phi^{reco}_{nFD} vs. t^{reco}_{ToF,nFD,ECIN} in 1e cut (ECALveto);t^{reco}_{ToF,nFD,ECIN} [ns];#phi^{reco}_{nFD} [#circ]", 100, 15., 75., 100, -180., 180.);
    HistoList.push_back(h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECIN_ECALveto_1e_cut);
    // TH2D* h_reco_P_nFD_VS_eff_L_ECIN_ECALveto_1e_cut =
    //     new TH2D("reco_P_nFD_VS_eff_L_ECIN_ECALveto_1e_cut",
    //              "P^{reco}_{nFD} vs. L^{eff}_{nFD,ECIN} in 1e cut (ECALveto);L^{eff}_{nFD,ECIN} = c#beta^{truth}t^{#beta^{reco}_{nFD}}_{ToF} [cm];P^{reco}_{nFD} [#circ]", 100, 700., 850.,
    //              100, 0., Ebeam * 3.);
    // HistoList.push_back(h_reco_P_nFD_VS_eff_L_ECIN_ECALveto_1e_cut);
    // TH2D* h_reco_theta_nFD_VS_eff_L_ECIN_ECALveto_1e_cut =
    //     new TH2D("reco_theta_nFD_VS_eff_L_ECIN_ECALveto_1e_cut",
    //              "#theta^{reco}_{nFD} vs. L^{eff}_{nFD,ECIN} in 1e cut (ECALveto);L^{eff}_{nFD,ECIN} = c#beta^{truth}t^{#beta^{reco}_{nFD}}_{ToF} [cm];#theta^{reco}_{nFD} [#circ]", 100,
    //              700., 850., 100, 0., 45.);
    // HistoList.push_back(h_reco_theta_nFD_VS_eff_L_ECIN_ECALveto_1e_cut);
    // TH2D* h_reco_phi_nFD_VS_eff_L_ECIN_ECALveto_1e_cut =
    //     new TH2D("reco_phi_nFD_VS_eff_L_ECIN_ECALveto_1e_cut",
    //              "#phi^{reco}_{nFD} vs. L^{eff}_{nFD,ECIN} in 1e cut (ECALveto);L^{eff}_{nFD,ECIN} = c#beta^{truth}t^{#beta^{reco}_{nFD}}_{ToF} [cm];#phi^{reco}_{nFD} [#circ]", 100, 700.,
    //              850., 100, -180., 180.);
    // HistoList.push_back(h_reco_phi_nFD_VS_eff_L_ECIN_ECALveto_1e_cut);

    TH2D* h_reco_P_nFD_VS_reco_L_ECOUT_ECALveto_1e_cut =
        new TH2D("reco_P_nFD_VS_reco_L_ECOUT_ECALveto_1e_cut", "P^{reco}_{nFD} vs. L^{reco}_{nFD,ECOUT} in 1e cut (ECALveto);L^{reco}_{nFD,ECOUT} [cm];P^{reco}_{nFD} [#circ]", 100, 700.,
                 850., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_nFD_VS_reco_L_ECOUT_ECALveto_1e_cut);
    TH2D* h_reco_theta_nFD_VS_reco_L_ECOUT_ECALveto_1e_cut =
        new TH2D("reco_theta_nFD_VS_reco_L_ECOUT_ECALveto_1e_cut", "#theta^{reco}_{nFD} vs. L^{reco}_{nFD,ECOUT} in 1e cut (ECALveto);L^{reco}_{nFD,ECOUT} [cm];#theta^{reco}_{nFD} [#circ]",
                 100, 700., 850., 100, 0., 45.);
    HistoList.push_back(h_reco_theta_nFD_VS_reco_L_ECOUT_ECALveto_1e_cut);
    TH2D* h_reco_phi_nFD_VS_reco_L_ECOUT_ECALveto_1e_cut =
        new TH2D("reco_phi_nFD_VS_reco_L_ECOUT_ECALveto_1e_cut", "#phi^{reco}_{nFD} vs. L^{reco}_{nFD,ECOUT} in 1e cut (ECALveto);L^{reco}_{nFD,ECOUT} [cm];#phi^{reco}_{nFD} [#circ]", 100,
                 700., 850., 100, -180., 180.);
    HistoList.push_back(h_reco_phi_nFD_VS_reco_L_ECOUT_ECALveto_1e_cut);
    TH2D* h_reco_P_nFD_VS_ToF_nFD_fromFile_ECOUT_ECALveto_1e_cut =
        new TH2D("reco_P_nFD_VS_ToF_nFD_fromFile_ECOUT_ECALveto_1e_cut",
                 "P^{reco}_{nFD} vs. t^{reco}_{ToF,nFD,ECOUT} in 1e cut (ECALveto);t^{reco}_{ToF,nFD,ECOUT} [ns];P^{reco}_{nFD} [#circ]", 100, 15., 75., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_nFD_VS_ToF_nFD_fromFile_ECOUT_ECALveto_1e_cut);
    TH2D* h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECOUT_ECALveto_1e_cut =
        new TH2D("reco_theta_nFD_VS_ToF_nFD_fromFile_ECOUT_ECALveto_1e_cut",
                 "#theta^{reco}_{nFD} vs. t^{reco}_{ToF,nFD,ECOUT} in 1e cut (ECALveto);t^{reco}_{ToF,nFD,ECOUT} [ns];#theta^{reco}_{nFD} [#circ]", 100, 15., 75., 100, 0., 45.);
    HistoList.push_back(h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECOUT_ECALveto_1e_cut);
    TH2D* h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECOUT_ECALveto_1e_cut =
        new TH2D("reco_phi_nFD_VS_ToF_nFD_fromFile_ECOUT_ECALveto_1e_cut",
                 "#phi^{reco}_{nFD} vs. t^{reco}_{ToF,nFD,ECOUT} in 1e cut (ECALveto);t^{reco}_{ToF,nFD,ECOUT} [ns];#phi^{reco}_{nFD} [#circ]", 100, 15., 75., 100, -180., 180.);
    HistoList.push_back(h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECOUT_ECALveto_1e_cut);
    // TH2D* h_reco_P_nFD_VS_eff_L_ECOUT_ECALveto_1e_cut =
    //     new TH2D("reco_P_nFD_VS_eff_L_ECOUT_ECALveto_1e_cut",
    //              "P^{reco}_{nFD} vs. L^{eff}_{nFD,ECOUT} in 1e cut (ECALveto);L^{eff}_{nFD,ECOUT} = c#beta^{truth}t^{#beta^{reco}_{nFD}}_{ToF} [cm];P^{reco}_{nFD} [#circ]", 100, 700.,
    //              850., 100, 0., Ebeam * 3.);
    // HistoList.push_back(h_reco_P_nFD_VS_eff_L_ECOUT_ECALveto_1e_cut);
    // TH2D* h_reco_theta_nFD_VS_eff_L_ECOUT_ECALveto_1e_cut =
    //     new TH2D("reco_theta_nFD_VS_eff_L_ECOUT_ECALveto_1e_cut",
    //              "#theta^{reco}_{nFD} vs. L^{eff}_{nFD,ECOUT} in 1e cut (ECALveto);L^{eff}_{nFD,ECOUT} = c#beta^{truth}t^{#beta^{reco}_{nFD}}_{ToF} [cm];#theta^{reco}_{nFD} [#circ]", 100,
    //              700., 850., 100, 0., 45.);
    // HistoList.push_back(h_reco_theta_nFD_VS_eff_L_ECOUT_ECALveto_1e_cut);
    // TH2D* h_reco_phi_nFD_VS_eff_L_ECOUT_ECALveto_1e_cut =
    //     new TH2D("reco_phi_nFD_VS_eff_L_ECOUT_ECALveto_1e_cut",
    //              "#phi^{reco}_{nFD} vs. L^{eff}_{nFD,ECOUT} in 1e cut (ECALveto);L^{eff}_{nFD,ECOUT} = c#beta^{truth}t^{#beta^{reco}_{nFD}}_{ToF} [cm];#phi^{reco}_{nFD} [#circ]", 100,
    //              700., 850., 100, -180., 180.);
    // HistoList.push_back(h_reco_phi_nFD_VS_eff_L_ECOUT_ECALveto_1e_cut);

    TH1D* h_reco_theta_nFD_minus_reco_theta_e_ECALveto_1e_cut =
        new TH1D("reco_theta_nFD_minus_reco_theta_e_ECALveto_1e_cut",
                 "#Delta#theta^{reco}_{nFD,e} in 1e cut (ECALveto);#Delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} [#circ];Counts", 50, -25., 10.);
    HistoList.push_back(h_reco_theta_nFD_minus_reco_theta_e_ECALveto_1e_cut);
    TH1D* h_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut =
        new TH1D("reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut",
                 "#Delta#phi^{reco}_{nFD,e} in 1e cut (ECALveto);#Delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| [#circ];Counts", 50, -180., 180.);
    HistoList.push_back(h_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut);
    TH2D* h_reco_theta_nFD_minus_reco_theta_e_VS_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut =
        new TH2D("reco_theta_nFD_minus_reco_theta_e_VS_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut",
                 "#Delta#theta^{reco}_{nFD,e} vs. #Delta#phi^{reco}_{nFD,e} in 1e cut (ECALveto);#Delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| "
                 "[#circ];#Delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} [#circ]",
                 100, -180., 180., 100, -25., 10.);
    HistoList.push_back(h_reco_theta_nFD_minus_reco_theta_e_VS_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut);
    TH2D* h_reco_P_nFD_VS_reco_theta_nFD_minus_reco_theta_e_ECALveto_1e_cut =
        new TH2D("reco_P_nFD_VS_reco_theta_nFD_minus_reco_theta_e_ECALveto_1e_cut",
                 "P^{reco}_{nFD} vs. #Delta#theta^{reco}_{nFD,e} in 1e cut (ECALveto);#Delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} [#circ];P^{reco}_{nFD} [GeV/c]",
                 100, -25., 10., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_nFD_VS_reco_theta_nFD_minus_reco_theta_e_ECALveto_1e_cut);
    TH2D* h_reco_P_nFD_VS_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut =
        new TH2D("reco_P_nFD_VS_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut",
                 "P^{reco}_{nFD} vs. #Delta#phi^{reco}_{nFD,e} in 1e cut (ECALveto);#Delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| [#circ];P^{reco}_{nFD} [GeV/c]", 100,
                 -180., 180., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_nFD_VS_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut);

#pragma endregion

#pragma region /* matched */
    HistSubjects.push_back("matched");
    HistSubjects2.push_back("matched");
    FirstPrint.push_back(true);

#pragma region /* Base histograms (matched) */
    TH1D* h_reco_P_nFD_matched_1e_cut = new TH1D("reco_P_nFD_matched_1e_cut", "P^{reco}_{nFD} in 1e cut (matched);P^{reco}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_reco_P_nFD_matched_1e_cut);
    TH1D* h_truth_P_nFD_matched_1e_cut = new TH1D("truth_P_nFD_matched_1e_cut", "P^{truth}_{nFD} in 1e cut (matched);P^{truth}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_truth_P_nFD_matched_1e_cut);
    TH1D* h_reco_theta_nFD_matched_1e_cut = new TH1D("reco_theta_nFD_matched_1e_cut", "#theta^{reco}_{nFD} in 1e cut (matched);#theta^{reco}_{nFD} [#circ];Counts", 50, 0, 45.);
    HistoList.push_back(h_reco_theta_nFD_matched_1e_cut);
    TH1D* h_truth_theta_nFD_matched_1e_cut = new TH1D("truth_theta_nFD_matched_1e_cut", "#theta^{truth}_{nFD} in 1e cut (matched);#theta^{truth}_{nFD} [#circ];Counts", 50, 0, 45.);
    HistoList.push_back(h_truth_theta_nFD_matched_1e_cut);
    TH1D* h_reco_phi_nFD_matched_1e_cut = new TH1D("reco_phi_nFD_matched_1e_cut", "#phi^{reco}_{nFD} in 1e cut (matched);#phi^{reco}_{nFD} [#circ];Counts", 50, -180, 180.);
    HistoList.push_back(h_reco_phi_nFD_matched_1e_cut);
    TH1D* h_truth_phi_nFD_matched_1e_cut = new TH1D("truth_phi_nFD_matched_1e_cut", "#phi^{truth}_{nFD} in 1e cut (matched);#phi^{truth}_{nFD} [#circ];Counts", 50, -180, 180.);
    HistoList.push_back(h_truth_phi_nFD_matched_1e_cut);
    TH2D* h_reco_theta_nFD_matched_VS_reco_phi_nFD_matched_1e_cut =
        new TH2D("reco_theta_nFD_matched_VS_reco_phi_nFD_matched_1e_cut",
                 "#theta^{reco}_{nFD} vs. #phi^{reco}_{nFD} in 1e cut (matched);#phi^{reco}_{nFD} [#circ];#theta^{reco}_{nFD} [#circ]", 100, -180., 180., 100, 0, 50.);
    HistoList.push_back(h_reco_theta_nFD_matched_VS_reco_phi_nFD_matched_1e_cut);
    TH2D* h_truth_theta_nFD_matched_VS_truth_phi_nFD_matched_1e_cut =
        new TH2D("truth_theta_nFD_matched_VS_truth_phi_nFD_matched_1e_cut",
                 "#theta^{truth}_{nFD} vs. #phi^{truth}_{nFD} in 1e cut (matched);#phi^{truth}_{nFD} [#circ];#theta^{truth}_{nFD} [#circ]", 100, -180., 180., 100, 0, 50.);
    HistoList.push_back(h_truth_theta_nFD_matched_VS_truth_phi_nFD_matched_1e_cut);

    TH2D* h_reco_P_e_VS_P_nFD_matched_1e_cut = new TH2D("reco_P_e_VS_P_nFD_matched_1e_cut", "P^{reco}_{e} vs. P^{reco}_{nFD} in 1e cut (matched);P^{reco}_{e} [GeV/c];P^{reco}_{nFD} [GeV/c]",
                                                        100, 0., Ebeam * 2., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_e_VS_P_nFD_matched_1e_cut);
    TH2D* h_reco_theta_e_VS_P_nFD_matched_1e_cut =
        new TH2D("reco_theta_e_VS_P_nFD_matched_1e_cut", "#theta^{reco}_{e} vs. P^{reco}_{nFD} in 1e cut (matched);#theta^{reco}_{e} [#circ];P^{reco}_{nFD} [GeV/c]", 100, 0, 45., 100, 0.,
                 Ebeam * 3.);
    HistoList.push_back(h_reco_theta_e_VS_P_nFD_matched_1e_cut);
    TH2D* h_reco_phi_e_VS_P_nFD_matched_1e_cut = new TH2D(
        "reco_phi_e_VS_P_nFD_matched_1e_cut", "#phi^{reco}_{e} vs. P^{reco}_{nFD} in 1e cut (matched);#phi^{reco}_{e} [#circ];P^{reco}_{nFD} [GeV/c]", 100, -180., 180., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_phi_e_VS_P_nFD_matched_1e_cut);
    TH2D* h_reco_P_e_VS_theta_nFD_matched_1e_cut =
        new TH2D("reco_P_e_VS_theta_nFD_matched_1e_cut", "P^{reco}_{e} vs. #theta^{reco}_{nFD} in 1e cut (matched);P^{reco}_{e} [GeV/c];#theta^{reco}_{nFD} [#circ]", 100, 0., P_upperLim,
                 100, 0, 45.);
    HistoList.push_back(h_reco_P_e_VS_theta_nFD_matched_1e_cut);
    TH2D* h_reco_theta_e_VS_theta_nFD_matched_1e_cut =
        new TH2D("reco_theta_e_VS_theta_nFD_matched_1e_cut", "#theta^{reco}_{e} vs. #theta^{reco}_{nFD} in 1e cut (matched);#theta^{reco}_{e} [#circ];#theta^{reco}_{nFD} [#circ]", 100, 0,
                 45., 100, 0, 45.);
    HistoList.push_back(h_reco_theta_e_VS_theta_nFD_matched_1e_cut);
    TH2D* h_reco_phi_e_VS_theta_nFD_matched_1e_cut =
        new TH2D("reco_phi_e_VS_theta_nFD_matched_1e_cut", "#phi^{reco}_{e} vs. #theta^{reco}_{nFD} in 1e cut (matched);#phi^{reco}_{e} [#circ];#theta^{reco}_{nFD} [#circ]", 100, -180.,
                 180., 100, 0, 45.);
    HistoList.push_back(h_reco_phi_e_VS_theta_nFD_matched_1e_cut);
    TH2D* h_reco_P_e_VS_phi_nFD_matched_1e_cut = new TH2D(
        "reco_P_e_VS_phi_nFD_matched_1e_cut", "P^{reco}_{e} vs. #phi^{reco}_{nFD} in 1e cut (matched);P^{reco}_{e} [GeV/c];#phi^{reco}_{nFD} [#circ]", 100, 0., P_upperLim, 100, -180., 180.);
    HistoList.push_back(h_reco_P_e_VS_phi_nFD_matched_1e_cut);
    TH2D* h_reco_theta_e_VS_phi_nFD_matched_1e_cut =
        new TH2D("reco_phi_e_VS_phi_nFD_matched_1e_cut", "#theta^{reco}_{e} vs. #phi^{reco}_{nFD} in 1e cut (matched);#theta^{reco}_{e} [#circ];#phi^{reco}_{nFD} [#circ]", 100, 0, 45., 100,
                 -180., 180.);
    HistoList.push_back(h_reco_theta_e_VS_phi_nFD_matched_1e_cut);
    TH2D* h_reco_phi_e_VS_phi_nFD_matched_1e_cut =
        new TH2D("reco_phi_e_VS_phi_nFD_matched_1e_cut", "#phi^{reco}_{e} vs. #phi^{reco}_{nFD} in 1e cut (matched);#phi^{reco}_{e} [#circ];#phi^{reco}_{nFD} [#circ]", 100, -180., 180., 100,
                 -180., 180.);
    HistoList.push_back(h_reco_phi_e_VS_phi_nFD_matched_1e_cut);
    TH2D* h_reco_theta_nFD_matched_VS_P_nFD_matched_1e_cut =
        new TH2D("reco_theta_nFD_matched_VS_P_nFD_matched_1e_cut", "#theta^{reco}_{nFD} vs. P^{reco}_{nFD} in 1e cut (matched);#theta^{reco}_{nFD} [#circ];P^{reco}_{nFD} [GeV/c]", 100, 0.,
                 45., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_theta_nFD_matched_VS_P_nFD_matched_1e_cut);
    TH1D* h_reco_nFD_multi_matched_1e_cut = new TH1D("reco_nFD_multi_matched_1e_cut", "reco nFD multiplicity in 1e cut (matched);nFD multiplicity;Counts", 9, 1, 10);
    HistoList.push_back(h_reco_nFD_multi_matched_1e_cut);
    TH2D* h_reco_nFD_multi_VS_reco_P_nFD_matched_1e_cut = new TH2D(
        "reco_nFD_multi_VS_reco_P_nFD_matched_1e_cut", "reco nFD multiplicity vs. P^{reco}_{nFD} in 1e cut (matched);P^{reco}_{nFD} [GeV/c];nFD multiplicity", 100, 0., Ebeam * 3., 9, 1, 10);
    HistoList.push_back(h_reco_nFD_multi_VS_reco_P_nFD_matched_1e_cut);
#pragma endregion

    TH2D* h_reco_P_nFD_VS_reco_L_ECAL_matched_1e_cut =
        new TH2D("reco_P_nFD_VS_reco_L_ECAL_matched_1e_cut", "P^{reco}_{nFD} vs. L^{reco}_{nFD,ECAL} in 1e cut (matched);L^{reco}_{nFD,ECAL} [cm];P^{reco}_{nFD} [#circ]", 100, 700., 850.,
                 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_nFD_VS_reco_L_ECAL_matched_1e_cut);
    TH2D* h_reco_theta_nFD_VS_reco_L_ECAL_matched_1e_cut =
        new TH2D("reco_theta_nFD_VS_reco_L_ECAL_matched_1e_cut", "#theta^{reco}_{nFD} vs. L^{reco}_{nFD,ECAL} in 1e cut (matched);L^{reco}_{nFD,ECAL} [cm];#theta^{reco}_{nFD} [#circ]", 100,
                 700., 850., 100, 0., 45.);
    HistoList.push_back(h_reco_theta_nFD_VS_reco_L_ECAL_matched_1e_cut);
    TH2D* h_reco_phi_nFD_VS_reco_L_ECAL_matched_1e_cut =
        new TH2D("reco_phi_nFD_VS_reco_L_ECAL_matched_1e_cut", "#phi^{reco}_{nFD} vs. L^{reco}_{nFD,ECAL} in 1e cut (matched);L^{reco}_{nFD,ECAL} [cm];#phi^{reco}_{nFD} [#circ]", 100, 700.,
                 850., 100, -180., 180.);
    HistoList.push_back(h_reco_phi_nFD_VS_reco_L_ECAL_matched_1e_cut);
    TH2D* h_reco_P_nFD_VS_ToF_nFD_fromFile_ECAL_matched_1e_cut =
        new TH2D("reco_P_nFD_VS_ToF_nFD_fromFile_ECAL_matched_1e_cut", "P^{reco}_{nFD} vs. t^{reco}_{ToF,nFD,ECAL} in 1e cut (matched);t^{reco}_{ToF,nFD,ECAL} [ns];P^{reco}_{nFD} [#circ]",
                 100, 15., 75., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_nFD_VS_ToF_nFD_fromFile_ECAL_matched_1e_cut);
    TH2D* h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECAL_matched_1e_cut =
        new TH2D("reco_theta_nFD_VS_ToF_nFD_fromFile_ECAL_matched_1e_cut",
                 "#theta^{reco}_{nFD} vs. t^{reco}_{ToF,nFD,ECAL} in 1e cut (matched);t^{reco}_{ToF,nFD,ECAL} [ns];#theta^{reco}_{nFD} [#circ]", 100, 15., 75., 100, 0., 45.);
    HistoList.push_back(h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECAL_matched_1e_cut);
    TH2D* h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECAL_matched_1e_cut =
        new TH2D("reco_phi_nFD_VS_ToF_nFD_fromFile_ECAL_matched_1e_cut",
                 "#phi^{reco}_{nFD} vs. t^{reco}_{ToF,nFD,ECAL} in 1e cut (matched);t^{reco}_{ToF,nFD,ECAL} [ns];#phi^{reco}_{nFD} [#circ]", 100, 15., 75., 100, -180., 180.);
    HistoList.push_back(h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECAL_matched_1e_cut);
    // TH2D* h_reco_P_nFD_VS_eff_L_ECAL_matched_1e_cut =
    //     new TH2D("reco_P_nFD_VS_eff_L_ECAL_matched_1e_cut",
    //              "P^{reco}_{nFD} vs. L^{eff}_{nFD,ECAL} in 1e cut (matched);L^{eff}_{nFD,ECAL} = c#beta^{truth}t^{#beta^{reco}_{nFD}}_{ToF} [cm];P^{reco}_{nFD} [#circ]", 100, 700., 850.,
    //              100, 0., Ebeam * 3.);
    // HistoList.push_back(h_reco_P_nFD_VS_eff_L_ECAL_matched_1e_cut);
    // TH2D* h_reco_theta_nFD_VS_eff_L_ECAL_matched_1e_cut =
    //     new TH2D("reco_theta_nFD_VS_eff_L_ECAL_matched_1e_cut",
    //              "#theta^{reco}_{nFD} vs. L^{eff}_{nFD,ECAL} in 1e cut (matched);L^{eff}_{nFD,ECAL} = c#beta^{truth}t^{#beta^{reco}_{nFD}}_{ToF} [cm];#theta^{reco}_{nFD} [#circ]", 100,
    //              700., 850., 100, 0., 45.);
    // HistoList.push_back(h_reco_theta_nFD_VS_eff_L_ECAL_matched_1e_cut);
    // TH2D* h_reco_phi_nFD_VS_eff_L_ECAL_matched_1e_cut =
    //     new TH2D("reco_phi_nFD_VS_eff_L_ECAL_matched_1e_cut",
    //              "#phi^{reco}_{nFD} vs. L^{eff}_{nFD,ECAL} in 1e cut (matched);L^{eff}_{nFD,ECAL} = c#beta^{truth}t^{#beta^{reco}_{nFD}}_{ToF} [cm];#phi^{reco}_{nFD} [#circ]", 100, 700.,
    //              850., 100, -180., 180.);
    // HistoList.push_back(h_reco_phi_nFD_VS_eff_L_ECAL_matched_1e_cut);

    TH2D* h_reco_P_nFD_VS_reco_L_ECIN_matched_1e_cut =
        new TH2D("reco_P_nFD_VS_reco_L_ECIN_matched_1e_cut", "P^{reco}_{nFD} vs. L^{reco}_{nFD,ECIN} in 1e cut (matched);L^{reco}_{nFD,ECIN} [cm];P^{reco}_{nFD} [#circ]", 100, 700., 850.,
                 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_nFD_VS_reco_L_ECIN_matched_1e_cut);
    TH2D* h_reco_theta_nFD_VS_reco_L_ECIN_matched_1e_cut =
        new TH2D("reco_theta_nFD_VS_reco_L_ECIN_matched_1e_cut", "#theta^{reco}_{nFD} vs. L^{reco}_{nFD,ECIN} in 1e cut (matched);L^{reco}_{nFD,ECIN} [cm];#theta^{reco}_{nFD} [#circ]", 100,
                 700., 850., 100, 0., 45.);
    HistoList.push_back(h_reco_theta_nFD_VS_reco_L_ECIN_matched_1e_cut);
    TH2D* h_reco_phi_nFD_VS_reco_L_ECIN_matched_1e_cut =
        new TH2D("reco_phi_nFD_VS_reco_L_ECIN_matched_1e_cut", "#phi^{reco}_{nFD} vs. L^{reco}_{nFD,ECIN} in 1e cut (matched);L^{reco}_{nFD,ECIN} [cm];#phi^{reco}_{nFD} [#circ]", 100, 700.,
                 850., 100, -180., 180.);
    HistoList.push_back(h_reco_phi_nFD_VS_reco_L_ECIN_matched_1e_cut);
    TH2D* h_reco_P_nFD_VS_ToF_nFD_fromFile_ECIN_matched_1e_cut =
        new TH2D("reco_P_nFD_VS_ToF_nFD_fromFile_ECIN_matched_1e_cut", "P^{reco}_{nFD} vs. t^{reco}_{ToF,nFD,ECIN} in 1e cut (matched);t^{reco}_{ToF,nFD,ECIN} [ns];P^{reco}_{nFD} [#circ]",
                 100, 15., 75., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_nFD_VS_ToF_nFD_fromFile_ECIN_matched_1e_cut);
    TH2D* h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECIN_matched_1e_cut =
        new TH2D("reco_theta_nFD_VS_ToF_nFD_fromFile_ECIN_matched_1e_cut",
                 "#theta^{reco}_{nFD} vs. t^{reco}_{ToF,nFD,ECIN} in 1e cut (matched);t^{reco}_{ToF,nFD,ECIN} [ns];#theta^{reco}_{nFD} [#circ]", 100, 15., 75., 100, 0., 45.);
    HistoList.push_back(h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECIN_matched_1e_cut);
    TH2D* h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECIN_matched_1e_cut =
        new TH2D("reco_phi_nFD_VS_ToF_nFD_fromFile_ECIN_matched_1e_cut",
                 "#phi^{reco}_{nFD} vs. t^{reco}_{ToF,nFD,ECIN} in 1e cut (matched);t^{reco}_{ToF,nFD,ECIN} [ns];#phi^{reco}_{nFD} [#circ]", 100, 15., 75., 100, -180., 180.);
    HistoList.push_back(h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECIN_matched_1e_cut);
    // TH2D* h_reco_P_nFD_VS_eff_L_ECIN_matched_1e_cut =
    //     new TH2D("reco_P_nFD_VS_eff_L_ECIN_matched_1e_cut",
    //              "P^{reco}_{nFD} vs. L^{eff}_{nFD,ECIN} in 1e cut (matched);L^{eff}_{nFD,ECIN} = c#beta^{truth}t^{#beta^{reco}_{nFD}}_{ToF} [cm];P^{reco}_{nFD} [#circ]", 100, 700., 850.,
    //              100, 0., Ebeam * 3.);
    // HistoList.push_back(h_reco_P_nFD_VS_eff_L_ECIN_matched_1e_cut);
    // TH2D* h_reco_theta_nFD_VS_eff_L_ECIN_matched_1e_cut =
    //     new TH2D("reco_theta_nFD_VS_eff_L_ECIN_matched_1e_cut",
    //              "#theta^{reco}_{nFD} vs. L^{eff}_{nFD,ECIN} in 1e cut (matched);L^{eff}_{nFD,ECIN} = c#beta^{truth}t^{#beta^{reco}_{nFD}}_{ToF} [cm];#theta^{reco}_{nFD} [#circ]", 100,
    //              700., 850., 100, 0., 45.);
    // HistoList.push_back(h_reco_theta_nFD_VS_eff_L_ECIN_matched_1e_cut);
    // TH2D* h_reco_phi_nFD_VS_eff_L_ECIN_matched_1e_cut =
    //     new TH2D("reco_phi_nFD_VS_eff_L_ECIN_matched_1e_cut",
    //              "#phi^{reco}_{nFD} vs. L^{eff}_{nFD,ECIN} in 1e cut (matched);L^{eff}_{nFD,ECIN} = c#beta^{truth}t^{#beta^{reco}_{nFD}}_{ToF} [cm];#phi^{reco}_{nFD} [#circ]", 100, 700.,
    //              850., 100, -180., 180.);
    // HistoList.push_back(h_reco_phi_nFD_VS_eff_L_ECIN_matched_1e_cut);

    TH2D* h_reco_P_nFD_VS_reco_L_ECOUT_matched_1e_cut =
        new TH2D("reco_P_nFD_VS_reco_L_ECOUT_matched_1e_cut", "P^{reco}_{nFD} vs. L^{reco}_{nFD,ECOUT} in 1e cut (matched);L^{reco}_{nFD,ECOUT} [cm];P^{reco}_{nFD} [#circ]", 100, 700., 850.,
                 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_nFD_VS_reco_L_ECOUT_matched_1e_cut);
    TH2D* h_reco_theta_nFD_VS_reco_L_ECOUT_matched_1e_cut =
        new TH2D("reco_theta_nFD_VS_reco_L_ECOUT_matched_1e_cut", "#theta^{reco}_{nFD} vs. L^{reco}_{nFD,ECOUT} in 1e cut (matched);L^{reco}_{nFD,ECOUT} [cm];#theta^{reco}_{nFD} [#circ]",
                 100, 700., 850., 100, 0., 45.);
    HistoList.push_back(h_reco_theta_nFD_VS_reco_L_ECOUT_matched_1e_cut);
    TH2D* h_reco_phi_nFD_VS_reco_L_ECOUT_matched_1e_cut =
        new TH2D("reco_phi_nFD_VS_reco_L_ECOUT_matched_1e_cut", "#phi^{reco}_{nFD} vs. L^{reco}_{nFD,ECOUT} in 1e cut (matched);L^{reco}_{nFD,ECOUT} [cm];#phi^{reco}_{nFD} [#circ]", 100,
                 700., 850., 100, -180., 180.);
    HistoList.push_back(h_reco_phi_nFD_VS_reco_L_ECOUT_matched_1e_cut);
    TH2D* h_reco_P_nFD_VS_ToF_nFD_fromFile_ECOUT_matched_1e_cut =
        new TH2D("reco_P_nFD_VS_ToF_nFD_fromFile_ECOUT_matched_1e_cut",
                 "P^{reco}_{nFD} vs. t^{reco}_{ToF,nFD,ECOUT} in 1e cut (matched);t^{reco}_{ToF,nFD,ECOUT} [ns];P^{reco}_{nFD} [#circ]", 100, 15., 75., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_nFD_VS_ToF_nFD_fromFile_ECOUT_matched_1e_cut);
    TH2D* h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECOUT_matched_1e_cut =
        new TH2D("reco_theta_nFD_VS_ToF_nFD_fromFile_ECOUT_matched_1e_cut",
                 "#theta^{reco}_{nFD} vs. t^{reco}_{ToF,nFD,ECOUT} in 1e cut (matched);t^{reco}_{ToF,nFD,ECOUT} [ns];#theta^{reco}_{nFD} [#circ]", 100, 15., 75., 100, 0., 45.);
    HistoList.push_back(h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECOUT_matched_1e_cut);
    TH2D* h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECOUT_matched_1e_cut =
        new TH2D("reco_phi_nFD_VS_ToF_nFD_fromFile_ECOUT_matched_1e_cut",
                 "#phi^{reco}_{nFD} vs. t^{reco}_{ToF,nFD,ECOUT} in 1e cut (matched);t^{reco}_{ToF,nFD,ECOUT} [ns];#phi^{reco}_{nFD} [#circ]", 100, 15., 75., 100, -180., 180.);
    HistoList.push_back(h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECOUT_matched_1e_cut);
    // TH2D* h_reco_P_nFD_VS_eff_L_ECOUT_matched_1e_cut =
    //     new TH2D("reco_P_nFD_VS_eff_L_ECOUT_matched_1e_cut",
    //              "P^{reco}_{nFD} vs. L^{eff}_{nFD,ECOUT} in 1e cut (matched);L^{eff}_{nFD,ECOUT} = c#beta^{truth}t^{#beta^{reco}_{nFD}}_{ToF} [cm];P^{reco}_{nFD} [#circ]", 100, 700.,
    //              850., 100, 0., Ebeam * 3.);
    // HistoList.push_back(h_reco_P_nFD_VS_eff_L_ECOUT_matched_1e_cut);
    // TH2D* h_reco_theta_nFD_VS_eff_L_ECOUT_matched_1e_cut =
    //     new TH2D("reco_theta_nFD_VS_eff_L_ECOUT_matched_1e_cut",
    //              "#theta^{reco}_{nFD} vs. L^{eff}_{nFD,ECOUT} in 1e cut (matched);L^{eff}_{nFD,ECOUT} = c#beta^{truth}t^{#beta^{reco}_{nFD}}_{ToF} [cm];#theta^{reco}_{nFD} [#circ]", 100,
    //              700., 850., 100, 0., 45.);
    // HistoList.push_back(h_reco_theta_nFD_VS_eff_L_ECOUT_matched_1e_cut);
    // TH2D* h_reco_phi_nFD_VS_eff_L_ECOUT_matched_1e_cut =
    //     new TH2D("reco_phi_nFD_VS_eff_L_ECOUT_matched_1e_cut",
    //              "#phi^{reco}_{nFD} vs. L^{eff}_{nFD,ECOUT} in 1e cut (matched);L^{eff}_{nFD,ECOUT} = c#beta^{truth}t^{#beta^{reco}_{nFD}}_{ToF} [cm];#phi^{reco}_{nFD} [#circ]", 100,
    //              700., 850., 100, -180., 180.);
    // HistoList.push_back(h_reco_phi_nFD_VS_eff_L_ECOUT_matched_1e_cut);

    TH1D* h_reco_theta_nFD_minus_reco_theta_e_matched_1e_cut =
        new TH1D("reco_theta_nFD_minus_reco_theta_e_matched_1e_cut",
                 "#Delta#theta^{reco}_{nFD,e} in 1e cut (matched);#Delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} [#circ];Counts", 50, -25., 10.);
    HistoList.push_back(h_reco_theta_nFD_minus_reco_theta_e_matched_1e_cut);
    TH1D* h_reco_phi_nFD_minus_reco_phi_e_matched_1e_cut =
        new TH1D("reco_phi_nFD_minus_reco_phi_e_matched_1e_cut",
                 "#Delta#phi^{reco}_{nFD,e} in 1e cut (matched);#Delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| [#circ];Counts", 50, -180., 180.);
    HistoList.push_back(h_reco_phi_nFD_minus_reco_phi_e_matched_1e_cut);
    TH2D* h_reco_theta_nFD_minus_reco_theta_e_VS_reco_phi_nFD_minus_reco_phi_e_matched_1e_cut =
        new TH2D("reco_theta_nFD_minus_reco_theta_e_VS_reco_phi_nFD_minus_reco_phi_e_matched_1e_cut",
                 "#Delta#theta^{reco}_{nFD,e} vs. #Delta#phi^{reco}_{nFD,e} in 1e cut (matched);#Delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| "
                 "[#circ];#Delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} [#circ]",
                 100, -180., 180., 100, -25., 10.);
    HistoList.push_back(h_reco_theta_nFD_minus_reco_theta_e_VS_reco_phi_nFD_minus_reco_phi_e_matched_1e_cut);
    TH2D* h_reco_P_nFD_VS_reco_theta_nFD_minus_reco_theta_e_matched_1e_cut =
        new TH2D("reco_P_nFD_VS_reco_theta_nFD_minus_reco_theta_e_matched_1e_cut",
                 "P^{reco}_{nFD} vs. #Delta#theta^{reco}_{nFD,e} in 1e cut (matched);#Delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} [#circ];P^{reco}_{nFD} [GeV/c]",
                 100, -25., 10., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_nFD_VS_reco_theta_nFD_minus_reco_theta_e_matched_1e_cut);
    TH2D* h_reco_P_nFD_VS_reco_phi_nFD_minus_reco_phi_e_matched_1e_cut =
        new TH2D("reco_P_nFD_VS_reco_phi_nFD_minus_reco_phi_e_matched_1e_cut",
                 "P^{reco}_{nFD} vs. #Delta#phi^{reco}_{nFD,e} in 1e cut (matched);#Delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| [#circ];P^{reco}_{nFD} [GeV/c]", 100,
                 -180., 180., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_nFD_VS_reco_phi_nFD_minus_reco_phi_e_matched_1e_cut);

#pragma endregion

#pragma endregion

    int counter = 0;

    int counter_ECALveto_ECINorECOUT_FDneutrons = 0;
    int counter_ECALveto_ECINorECOUT_FDphotons = 0;
    int counter_matched_ECINorECOUT_FDneutrons = 0;
    int counter_matched_ECINorECOUT_FDphotons = 0;

    // ParticleID PID;

    while (chain.Next() == true) {
        // Display completed
        ++counter;
        if ((counter % 1000000) == 0) { cerr << "\n" << counter / 1000000 << " million completed"; }
        if ((counter % 100000) == 0) { cerr << "."; }

        if (counter > Limiter) { break; }

        // get particles by type
        auto allParticles = c12->getDetParticles();
        auto electrons = c12->getByID(11);

        if (electrons.size() != 1) { continue; }

        double weight = 1;

        double starttime = c12->event()->getStartTime();

        TVector3 P_b(0, 0, Ebeam);

        TVector3 reco_P_e;
        reco_P_e.SetMagThetaPhi(electrons[0]->getP(), electrons[0]->getTheta(), electrons[0]->getPhi());

        double vtz_e = electrons[0]->par()->getVz();

        TVector3 P_q = P_b - reco_P_e;

        double nu = Ebeam - reco_P_e.Mag();
        double QSq = P_q.Mag2() - (nu * nu);
        double xB = QSq / (2 * m_n * nu);
        double WSq = (m_n * m_n) - QSq + (2 * nu * m_n);
        double theta_e = reco_P_e.Theta() * 180 / M_PI;
        double EoP_e = (electrons[0]->cal(clas12::PCAL)->getEnergy() + electrons[0]->cal(ECIN)->getEnergy() + electrons[0]->cal(ECOUT)->getEnergy()) / reco_P_e.Mag();
        double Edep_PCAL = electrons[0]->cal(clas12::PCAL)->getEnergy();

        //  =======================================================================================================================================================================
        //  1e cut (truth)
        //  =======================================================================================================================================================================

#pragma region /* 1e cut (truth) */
        double Truth_beta;

        bool TLpassCuts = true;

        auto c12 = chain.GetC12Reader();
        auto mceve = c12->mcevent();
        auto mcpbank = c12->mcparts();
        const Int_t Ngen = mcpbank->getRows();

        vector<int> truth_NeutronsFD;

        for (Int_t i = 0; i < Ngen; i++) {
            mcpbank->setEntry(i);

            auto pid_temp = mcpbank->getPid();

            auto p = mcpbank->getP();
            auto px = mcpbank->getPx();
            auto py = mcpbank->getPy();
            auto pz = mcpbank->getPz();

            bool PassMomth = (p >= 0.4);

            if (ConstrainTLmom && (pid_temp == 2112 && p > 2.)) {
                TLpassCuts = false;
                continue;
            }

            if (pid_temp == 11) {
                TVector3 truth_P_e;
                truth_P_e.SetXYZ(px, py, pz);

                h_truth_P_e_1e_cut->Fill(truth_P_e.Mag(), weight);
                h_truth_theta_e_1e_cut->Fill(truth_P_e.Theta() * 180 / M_PI, weight);
                h_truth_phi_e_1e_cut->Fill(truth_P_e.Phi() * 180 / M_PI, weight);
                h_truth_theta_e_VS_truth_phi_e_1e_cut->Fill(truth_P_e.Phi() * 180 / M_PI, truth_P_e.Theta() * 180 / M_PI, weight);
            } else if (pid_temp == 2112) {
                TVector3 truth_P_n;
                truth_P_n.SetXYZ(px, py, pz);

                h_truth_P_n_1e_cut->Fill(truth_P_n.Mag(), weight);
                h_truth_theta_n_1e_cut->Fill(truth_P_n.Theta() * 180 / M_PI, weight);
                h_truth_phi_n_1e_cut->Fill(truth_P_n.Phi() * 180 / M_PI, weight);
                h_truth_theta_n_VS_truth_phi_n_1e_cut->Fill(truth_P_n.Phi() * 180 / M_PI, truth_P_n.Theta() * 180 / M_PI, weight);

                if (truth_P_n.Theta() * 180 / M_PI <= 40.) {
                    double truth_E_nFD = sqrt(m_n * m_n + truth_P_n.Mag2());
                    Truth_beta = truth_P_n.Mag() / truth_E_nFD;

                    truth_NeutronsFD.push_back(i);

                    h_truth_P_nFD_clas12_1e_cut->Fill(truth_P_n.Mag(), weight);
                    h_truth_theta_nFD_clas12_1e_cut->Fill(truth_P_n.Theta() * 180 / M_PI, weight);
                    h_truth_phi_nFD_clas12_1e_cut->Fill(truth_P_n.Phi() * 180 / M_PI, weight);
                    h_truth_theta_nFD_clas12_VS_truth_phi_nFD_clas12_1e_cut->Fill(truth_P_n.Phi() * 180 / M_PI, truth_P_n.Theta() * 180 / M_PI, weight);

                    h_truth_P_nFD_redef_1e_cut->Fill(truth_P_n.Mag(), weight);
                    h_truth_theta_nFD_redef_1e_cut->Fill(truth_P_n.Theta() * 180 / M_PI, weight);
                    h_truth_phi_nFD_redef_1e_cut->Fill(truth_P_n.Phi() * 180 / M_PI, weight);
                    h_truth_theta_nFD_redef_VS_truth_phi_nFD_redef_1e_cut->Fill(truth_P_n.Phi() * 180 / M_PI, truth_P_n.Theta() * 180 / M_PI, weight);

                    if (PassMomth) {
                        h_truth_P_nFD_ECALveto_1e_cut->Fill(truth_P_n.Mag(), weight);
                        h_truth_theta_nFD_ECALveto_1e_cut->Fill(truth_P_n.Theta() * 180 / M_PI, weight);
                        h_truth_phi_nFD_ECALveto_1e_cut->Fill(truth_P_n.Phi() * 180 / M_PI, weight);
                        h_truth_theta_nFD_ECALveto_VS_truth_phi_nFD_ECALveto_1e_cut->Fill(truth_P_n.Phi() * 180 / M_PI, truth_P_n.Theta() * 180 / M_PI, weight);
                    }
                }
            }
        }

        if (ConstrainTLmom && !TLpassCuts) { continue; }
#pragma endregion

        //  =======================================================================================================================================================================
        //  1e cut (reco)
        //  =======================================================================================================================================================================

#pragma region /* 1e cut (reco) */

        //  Electron PID cuts -----------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* Electron PID cuts */
        if (electrons[0]->che(clas12::HTCC)->getNphe() <= 2) { continue; }
        if (Edep_PCAL <= 0.06) { continue; }
        if (EoP_e < 0.2 || EoP_e > 0.28) { continue; }
        if (electrons[0]->cal(clas12::PCAL)->getLv() < 14. || electrons[0]->cal(clas12::PCAL)->getLw() < 14.) { continue; }
        if (electrons[0]->par()->getVz() < -6. || electrons[0]->par()->getVz() > 0.) { continue; }

        h_reco_P_e_1e_cut->Fill(reco_P_e.Mag(), weight);
        h_reco_theta_e_1e_cut->Fill(reco_P_e.Theta() * 180 / M_PI, weight);
        h_reco_phi_e_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, weight);
        h_reco_theta_e_VS_reco_phi_e_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, reco_P_e.Theta() * 180 / M_PI, weight);
#pragma endregion

        //  Setting up neutrals ---------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* Setting up neutrals (RAW) */
        vector<region_part_ptr> neutrons;
        vector<region_part_ptr> photons;

        for (int i = 0; i < allParticles.size(); i++) {
            int pid_temp = allParticles[i]->par()->getPid();

            if (pid_temp == 2112) { neutrons.push_back(allParticles[i]); }
            if (pid_temp == 22) { photons.push_back(allParticles[i]); }
        }

        for (int i = 0; i < neutrons.size(); i++) {
            TVector3 reco_P_n;
            reco_P_n.SetMagThetaPhi(neutrons[i]->getP(), neutrons[i]->getTheta(), neutrons[i]->getPhi());

            h_reco_P_n_1e_cut->Fill(reco_P_n.Mag(), weight);
            h_reco_theta_n_1e_cut->Fill(reco_P_n.Theta() * 180 / M_PI, weight);
            h_reco_phi_n_1e_cut->Fill(reco_P_n.Phi() * 180 / M_PI, weight);
            h_reco_theta_n_VS_reco_phi_n_1e_cut->Fill(reco_P_n.Phi() * 180 / M_PI, reco_P_n.Theta() * 180 / M_PI, weight);
        }
#pragma endregion

        //  Setting up FD neutrals (clas12reco) -----------------------------------------------------------------------------------------------------------------------------------

#pragma region /* Setting up FD neutrals (clas12reco) */
        vector<region_part_ptr> neutrons_FD_clas12;
        vector<region_part_ptr> photons_FD_clas12;

        for (int i = 0; i < allParticles.size(); i++) {
            int pid_temp = allParticles[i]->par()->getPid();

            if (pid_temp == 2112 && allParticles[i]->getRegion() == FD) { neutrons_FD_clas12.push_back(allParticles[i]); }
            if (pid_temp == 22 && allParticles[i]->getRegion() == FD) { photons_FD_clas12.push_back(allParticles[i]); }
        }

        for (int i = 0; i < neutrons_FD_clas12.size(); i++) {
            TVector3 reco_P_nFD;
            reco_P_nFD.SetMagThetaPhi(CalcPnFD(neutrons_FD_clas12[i], starttime), neutrons_FD_clas12[i]->getTheta(), neutrons_FD_clas12[i]->getPhi());
            // reco_P_nFD.SetMagThetaPhi(neutrons_FD_clas12[i]->getP(), neutrons_FD_clas12[i]->getTheta(), neutrons_FD_clas12[i]->getPhi());

            h_reco_P_nFD_clas12_1e_cut->Fill(reco_P_nFD.Mag(), weight);
            h_reco_theta_nFD_clas12_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI, weight);
            h_reco_phi_nFD_clas12_1e_cut->Fill(reco_P_nFD.Phi() * 180 / M_PI, weight);
            h_reco_theta_nFD_clas12_VS_reco_phi_nFD_clas12_1e_cut->Fill(reco_P_nFD.Phi() * 180 / M_PI, reco_P_nFD.Theta() * 180 / M_PI, weight);
            h_reco_P_e_VS_P_nFD_clas12_1e_cut->Fill(reco_P_e.Mag(), reco_P_nFD.Mag(), weight);
            h_reco_theta_nFD_clas12_VS_P_nFD_clas12_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI, reco_P_nFD.Mag(), weight);
        }
#pragma endregion

        //  Setting up FD neutrals (redef) ----------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* Setting up FD neutrals (redef) */
        vector<region_part_ptr> neutrons_FD_redef;
        vector<region_part_ptr> photons_FD_redef;

        for (int i = 0; i < allParticles.size(); i++) {
            int pid_temp = allParticles[i]->par()->getPid();
            double starttime = c12->event()->getStartTime();
            double ToF_temp = allParticles[i]->getTime() - starttime;

            if ((allParticles[i]->par()->getCharge() == 0) && (allParticles[i]->getRegion() == FD) && (pid_temp != 0)
                //  && !(ToF_temp < 0 || ToF_temp > 40.)
            ) {  // If particle is neutral and in the FD

                bool ParticleInPCAL = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7);    // PCAL hit
                bool ParticleInECIN = (allParticles[i]->cal(clas12::ECIN)->getDetector() == 7);    // ECIN hit
                bool ParticleInECOUT = (allParticles[i]->cal(clas12::ECOUT)->getDetector() == 7);  // ECOUT hit
                auto Neutron_ECAL_detlayer = ParticleInECIN ? clas12::ECIN : clas12::ECOUT;        // find first layer of hit

                if ((pid_temp == 2112) || (pid_temp == 22)) {
                    if (ParticleInPCAL) {
                        if (pid_temp == 22) { photons_FD_redef.push_back(allParticles[i]); }
                    } else if (!ParticleInPCAL) {  // if there is a neutron or a 'photon' without a PCAL hit
                        if (ParticleInECIN || ParticleInECOUT) { neutrons_FD_redef.push_back(allParticles[i]); }
                    }
                }  // end of clas12root neutron or 'photon' if
            }  // end of neutral and in the FD if
        }

        for (int i = 0; i < neutrons_FD_redef.size(); i++) {
            bool ParticleInPCAL = (neutrons_FD_redef[i]->cal(clas12::PCAL)->getDetector() == 7);    // PCAL hit
            bool ParticleInECIN = (neutrons_FD_redef[i]->cal(clas12::ECIN)->getDetector() == 7);    // ECIN hit
            bool ParticleInECOUT = (neutrons_FD_redef[i]->cal(clas12::ECOUT)->getDetector() == 7);  // ECOUT hit
            if (ParticleInPCAL) { cout << "\n\nError! neutrons_FD_redef is in the PCAL! Aborting...\n\n", exit(0); }

            double Beta_ph = neutrons_FD_redef[i]->par()->getBeta();
            double Gamma_ph = 1 / sqrt(1 - (Beta_ph * Beta_ph));
            double Momentum = m_n * Beta_ph * Gamma_ph;

            TVector3 reco_P_nFD;
            reco_P_nFD.SetMagThetaPhi(CalcPnFD(neutrons_FD_redef[i], starttime), neutrons_FD_redef[i]->getTheta(), neutrons_FD_redef[i]->getPhi());

            h_reco_P_nFD_redef_1e_cut->Fill(reco_P_nFD.Mag(), weight);
            h_reco_theta_nFD_redef_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI, weight);
            h_reco_phi_nFD_redef_1e_cut->Fill(reco_P_nFD.Phi() * 180 / M_PI, weight);
            h_reco_theta_nFD_redef_VS_reco_phi_nFD_redef_1e_cut->Fill(reco_P_nFD.Phi() * 180 / M_PI, reco_P_nFD.Theta() * 180 / M_PI, weight);
            h_reco_P_e_VS_P_nFD_redef_1e_cut->Fill(reco_P_e.Mag(), reco_P_nFD.Mag(), weight);
            h_reco_theta_nFD_redef_VS_P_nFD_redef_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI, reco_P_nFD.Mag(), weight);
        }
#pragma endregion

        //  Setting up FD neutrals (ECALveto) ----------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* Setting up FD neutrals (ECALveto) */
        vector<region_part_ptr> neutrons_FD_ECALveto;

        for (int i = 0; i < allParticles.size(); i++) {
            int pid_temp = allParticles[i]->par()->getPid();

            if ((allParticles[i]->par()->getCharge() == 0) && (allParticles[i]->getRegion() == FD) && (pid_temp != 0)) {  // If particle is neutral and in the FD
                bool ParticleInPCAL = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7);                           // PCAL hit
                bool ParticleInECIN = (allParticles[i]->cal(clas12::ECIN)->getDetector() == 7);                           // ECIN hit
                bool ParticleInECOUT = (allParticles[i]->cal(clas12::ECOUT)->getDetector() == 7);                         // ECOUT hit
                auto Neutron_ECAL_detlayer = ParticleInECIN ? clas12::ECIN : clas12::ECOUT;                               // find first layer of hit

                if ((pid_temp == 2112) || (pid_temp == 22)) {
                    if (ParticleInPCAL) {
                        if (pid_temp == 22) { photons_FD_redef.push_back(allParticles[i]); }
                    } else if (!ParticleInPCAL) {  // if there is a neutron or a 'photon' without a PCAL hit
                        if (ParticleInECIN || ParticleInECOUT) {
                            double Momentum = CalcPnFD(allParticles[i], starttime);

                            bool PassMomth = (Momentum >= 0.4);
                            bool passECALeadgeCuts = (allParticles[i]->cal(Neutron_ECAL_detlayer)->getLv() > 14. && allParticles[i]->cal(Neutron_ECAL_detlayer)->getLw() > 14.);
                            bool passVeto = NeutronECAL_Cut_Veto(allParticles, electrons, Ebeam, i, 100.);

                            if (PassMomth && passECALeadgeCuts && passVeto) { neutrons_FD_ECALveto.push_back(allParticles[i]); }  // end of clas12root neutron or 'photon' if
                        }
                    }
                }  // end of clas12root neutron or 'photon' if
            }  // end of neutral and in the FD if
        }

        for (int i = 0; i < neutrons_FD_ECALveto.size(); i++) {
            bool ParticleInPCAL = (neutrons_FD_ECALveto[i]->cal(clas12::PCAL)->getDetector() == 7);         // PCAL hit
            bool ParticleInECIN = (neutrons_FD_ECALveto[i]->cal(clas12::ECIN)->getDetector() == 7);         // ECIN hit
            bool ParticleInECOUT = (neutrons_FD_ECALveto[i]->cal(clas12::ECOUT)->getDetector() == 7);       // ECOUT hit
            auto detlayer = ParticleInPCAL ? clas12::PCAL : ParticleInECIN ? clas12::ECIN : clas12::ECOUT;  // determine the earliest layer of the neutral hit
            if (ParticleInPCAL) { cout << "\n\nError! neutrons_FD_ECALveto is in the PCAL! Aborting...\n\n", exit(0); }

            double Path_nFD = neutrons_FD_ECALveto[i]->getPath();
            double eff_path_nFD = c * Truth_beta;
            double reco_ToF_nFD = neutrons_FD_ECALveto[i]->cal(detlayer)->getTime() - starttime;

            TVector3 reco_P_nFD;
            reco_P_nFD.SetMagThetaPhi(CalcPnFD(neutrons_FD_ECALveto[i], starttime), neutrons_FD_ECALveto[i]->getTheta(), neutrons_FD_ECALveto[i]->getPhi());

            h_reco_P_nFD_ECALveto_1e_cut->Fill(reco_P_nFD.Mag(), weight);
            h_reco_theta_nFD_ECALveto_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI, weight);
            h_reco_phi_nFD_ECALveto_1e_cut->Fill(reco_P_nFD.Phi() * 180 / M_PI, weight);
            h_reco_theta_nFD_ECALveto_VS_reco_phi_nFD_ECALveto_1e_cut->Fill(reco_P_nFD.Phi() * 180 / M_PI, reco_P_nFD.Theta() * 180 / M_PI, weight);
            h_reco_P_e_VS_P_nFD_ECALveto_1e_cut->Fill(reco_P_e.Mag(), reco_P_nFD.Mag(), weight);
            h_reco_theta_e_VS_P_nFD_ECALveto_1e_cut->Fill(reco_P_e.Theta() * 180 / M_PI, reco_P_nFD.Mag(), weight);
            h_reco_phi_e_VS_P_nFD_ECALveto_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, reco_P_nFD.Mag(), weight);
            h_reco_P_e_VS_theta_nFD_ECALveto_1e_cut->Fill(reco_P_e.Mag(), reco_P_nFD.Theta() * 180 / M_PI, weight);
            h_reco_theta_e_VS_theta_nFD_ECALveto_1e_cut->Fill(reco_P_e.Theta() * 180 / M_PI, reco_P_nFD.Theta() * 180 / M_PI, weight);
            h_reco_phi_e_VS_theta_nFD_ECALveto_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, reco_P_nFD.Theta() * 180 / M_PI, weight);
            h_reco_P_e_VS_phi_nFD_ECALveto_1e_cut->Fill(reco_P_e.Mag(), reco_P_nFD.Phi() * 180 / M_PI, weight);
            h_reco_theta_e_VS_phi_nFD_ECALveto_1e_cut->Fill(reco_P_e.Theta() * 180 / M_PI, reco_P_nFD.Phi() * 180 / M_PI, weight);
            h_reco_phi_e_VS_phi_nFD_ECALveto_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, reco_P_nFD.Phi() * 180 / M_PI, weight);
            h_reco_theta_nFD_ECALveto_VS_P_nFD_ECALveto_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI, reco_P_nFD.Mag(), weight);
            h_reco_nFD_multi_VS_reco_P_nFD_ECALveto_1e_cut->Fill(reco_P_nFD.Mag(), neutrons_FD_ECALveto.size(), weight);

            if (ParticleInECIN || ParticleInECOUT) {
                h_reco_P_nFD_VS_reco_L_ECAL_ECALveto_1e_cut->Fill(Path_nFD, reco_P_nFD.Mag(), weight);
                h_reco_theta_nFD_VS_reco_L_ECAL_ECALveto_1e_cut->Fill(Path_nFD, reco_P_nFD.Theta() * 180 / M_PI, weight);
                h_reco_phi_nFD_VS_reco_L_ECAL_ECALveto_1e_cut->Fill(Path_nFD, reco_P_nFD.Phi() * 180 / M_PI, weight);
                h_reco_P_nFD_VS_ToF_nFD_fromFile_ECAL_ECALveto_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Mag(), weight);
                h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECAL_ECALveto_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Theta() * 180 / M_PI, weight);
                h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECAL_ECALveto_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Phi() * 180 / M_PI, weight);

                if (ParticleInECIN) {
                    h_reco_P_nFD_VS_reco_L_ECIN_ECALveto_1e_cut->Fill(Path_nFD, reco_P_nFD.Mag(), weight);
                    h_reco_theta_nFD_VS_reco_L_ECIN_ECALveto_1e_cut->Fill(Path_nFD, reco_P_nFD.Theta() * 180 / M_PI, weight);
                    h_reco_phi_nFD_VS_reco_L_ECIN_ECALveto_1e_cut->Fill(Path_nFD, reco_P_nFD.Phi() * 180 / M_PI, weight);
                    h_reco_P_nFD_VS_ToF_nFD_fromFile_ECIN_ECALveto_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Mag(), weight);
                    h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECIN_ECALveto_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Theta() * 180 / M_PI, weight);
                    h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECIN_ECALveto_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Phi() * 180 / M_PI, weight);
                }

                if (ParticleInECOUT) {
                    h_reco_P_nFD_VS_reco_L_ECOUT_ECALveto_1e_cut->Fill(Path_nFD, reco_P_nFD.Mag(), weight);
                    h_reco_theta_nFD_VS_reco_L_ECOUT_ECALveto_1e_cut->Fill(Path_nFD, reco_P_nFD.Theta() * 180 / M_PI, weight);
                    h_reco_phi_nFD_VS_reco_L_ECOUT_ECALveto_1e_cut->Fill(Path_nFD, reco_P_nFD.Phi() * 180 / M_PI, weight);
                    h_reco_P_nFD_VS_ToF_nFD_fromFile_ECOUT_ECALveto_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Mag(), weight);
                    h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECOUT_ECALveto_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Theta() * 180 / M_PI, weight);
                    h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECOUT_ECALveto_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Phi() * 180 / M_PI, weight);
                }
            }

            h_reco_theta_nFD_minus_reco_theta_e_ECALveto_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI, weight);
            h_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut->Fill(CalcdPhi(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), weight);
            h_reco_theta_nFD_minus_reco_theta_e_VS_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut->Fill(CalcdPhi(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI),
                                                                                                       reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI, weight);
            h_reco_P_nFD_VS_reco_theta_nFD_minus_reco_theta_e_ECALveto_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI, reco_P_nFD.Mag(), weight);
            h_reco_P_nFD_VS_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut->Fill(CalcdPhi(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), reco_P_nFD.Mag(), weight);
        }

        if (neutrons_FD_ECALveto.size() != 0) { h_reco_nFD_multi_ECALveto_1e_cut->Fill(neutrons_FD_ECALveto.size(), weight); }
#pragma endregion

        //  Setting up FD neutrals (matched) -----------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* Setting up FD neutrals (matched) */
        vector<region_part_ptr> neutrons_FD_matched;

        double tl_Beta;

        for (int i = 0; i < allParticles.size(); i++) {
            int pid_temp = allParticles[i]->par()->getPid();

            if ((allParticles[i]->par()->getCharge() == 0) && (allParticles[i]->getRegion() == FD) && (pid_temp != 0)) {  // If particle is neutral and in the FD
                bool ParticleInPCAL = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7);                           // PCAL hit
                bool ParticleInECIN = (allParticles[i]->cal(clas12::ECIN)->getDetector() == 7);                           // ECIN hit
                bool ParticleInECOUT = (allParticles[i]->cal(clas12::ECOUT)->getDetector() == 7);                         // ECOUT hit
                auto Neutron_ECAL_detlayer = ParticleInECIN ? clas12::ECIN : clas12::ECOUT;                               // find first layer of hit

                if ((pid_temp == 2112) || (pid_temp == 22)) {
                    if (ParticleInPCAL) {
                        if (pid_temp == 22) { photons_FD_redef.push_back(allParticles[i]); }
                    } else if (!ParticleInPCAL) {  // if there is a neutron or a 'photon' without a PCAL hit
                        if (ParticleInECIN || ParticleInECOUT) {
                            double Momentum = CalcPnFD(allParticles[i], starttime);

                            bool PassMomth = (Momentum >= 0.4);
                            bool passECALeadgeCuts = (allParticles[i]->cal(Neutron_ECAL_detlayer)->getLv() > 14. && allParticles[i]->cal(Neutron_ECAL_detlayer)->getLw() > 14.);
                            bool passVeto = NeutronECAL_Cut_Veto(allParticles, electrons, Ebeam, i, 100);

                            if (PassMomth && passECALeadgeCuts && passVeto) {
                                for (int j = 0; j < truth_NeutronsFD.size(); j++) {
                                    mcpbank->setEntry(truth_NeutronsFD.at(j));

                                    double tl_P = mcpbank->getP();
                                    double tl_Theta = mcpbank->getTheta() * 180 / M_PI;
                                    double tl_Phi = mcpbank->getPhi() * 180 / M_PI;

                                    double reco_Theta = allParticles[i]->getTheta() * 180 / M_PI;
                                    double reco_Phi = allParticles[i]->getPhi() * 180 / M_PI;

                                    bool thetaCut = (fabs(tl_Theta - reco_Theta) <= 2.);
                                    bool phiCut = (fabs(tl_Phi - reco_Phi) <= 5.);

                                    double tl_E_nFD = sqrt(m_n * m_n + tl_P * tl_P);

                                    if (thetaCut && phiCut) {
                                        neutrons_FD_matched.push_back(allParticles[i]);

                                        tl_Beta = tl_P / tl_E_nFD;

                                        h_truth_P_nFD_matched_1e_cut->Fill(tl_P, weight);
                                        h_truth_theta_nFD_matched_1e_cut->Fill(tl_Theta, weight);
                                        h_truth_phi_nFD_matched_1e_cut->Fill(tl_Phi, weight);
                                        h_truth_theta_nFD_matched_VS_truth_phi_nFD_matched_1e_cut->Fill(tl_Phi, tl_Theta, weight);
                                    }
                                }
                            }  // end of clas12root neutron or 'photon' if
                        }
                    }
                }  // end of clas12root neutron or 'photon' if
            }  // end of neutral and in the FD if
        }

        for (int i = 0; i < neutrons_FD_matched.size(); i++) {
            bool ParticleInPCAL = (neutrons_FD_matched[i]->cal(clas12::PCAL)->getDetector() == 7);    // PCAL hit
            bool ParticleInECIN = (neutrons_FD_matched[i]->cal(clas12::ECIN)->getDetector() == 7);    // ECIN hit
            bool ParticleInECOUT = (neutrons_FD_matched[i]->cal(clas12::ECOUT)->getDetector() == 7);  // ECOUT hit
            if (ParticleInPCAL) { cout << "\n\nError! neutrons_FD_matched is in the PCAL! Aborting...\n\n", exit(0); }
            auto detlayer = ParticleInPCAL ? clas12::PCAL : ParticleInECIN ? clas12::ECIN : clas12::ECOUT;  // determine the earliest layer of the neutral hit

            double Path_nFD = neutrons_FD_matched[i]->getPath();
            double eff_path_nFD = c * Truth_beta;
            double reco_ToF_nFD = neutrons_FD_matched[i]->cal(detlayer)->getTime() - starttime;  // NOTE: neutrons_FD_matched[i]->cal(detlayer)->getTime() = neutrons_FD_matched[i]->getTime()
            // double reco_ToF_nFD = neutrons_FD_matched[i]->getTime();  // NOTE: neutrons_FD_matched[i]->cal(detlayer)->getTime() = neutrons_FD_matched[i]->getTime()

            TVector3 reco_P_nFD;
            reco_P_nFD.SetMagThetaPhi(CalcPnFD(neutrons_FD_matched[i], starttime), neutrons_FD_matched[i]->getTheta(), neutrons_FD_matched[i]->getPhi());

            h_reco_P_nFD_matched_1e_cut->Fill(reco_P_nFD.Mag(), weight);
            h_reco_theta_nFD_matched_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI, weight);
            h_reco_phi_nFD_matched_1e_cut->Fill(reco_P_nFD.Phi() * 180 / M_PI, weight);
            h_reco_theta_nFD_matched_VS_reco_phi_nFD_matched_1e_cut->Fill(reco_P_nFD.Phi() * 180 / M_PI, reco_P_nFD.Theta() * 180 / M_PI, weight);
            h_reco_P_e_VS_P_nFD_matched_1e_cut->Fill(reco_P_e.Mag(), reco_P_nFD.Mag(), weight);
            h_reco_theta_e_VS_P_nFD_matched_1e_cut->Fill(reco_P_e.Theta() * 180 / M_PI, reco_P_nFD.Mag(), weight);
            h_reco_phi_e_VS_P_nFD_matched_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, reco_P_nFD.Mag(), weight);
            h_reco_P_e_VS_theta_nFD_matched_1e_cut->Fill(reco_P_e.Mag(), reco_P_nFD.Theta() * 180 / M_PI, weight);
            h_reco_theta_e_VS_theta_nFD_matched_1e_cut->Fill(reco_P_e.Theta() * 180 / M_PI, reco_P_nFD.Theta() * 180 / M_PI, weight);
            h_reco_phi_e_VS_theta_nFD_matched_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, reco_P_nFD.Theta() * 180 / M_PI, weight);
            h_reco_P_e_VS_phi_nFD_matched_1e_cut->Fill(reco_P_e.Mag(), reco_P_nFD.Phi() * 180 / M_PI, weight);
            h_reco_theta_e_VS_phi_nFD_matched_1e_cut->Fill(reco_P_e.Theta() * 180 / M_PI, reco_P_nFD.Phi() * 180 / M_PI, weight);
            h_reco_phi_e_VS_phi_nFD_matched_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, reco_P_nFD.Phi() * 180 / M_PI, weight);
            h_reco_theta_nFD_matched_VS_P_nFD_matched_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI, reco_P_nFD.Mag(), weight);
            h_reco_nFD_multi_VS_reco_P_nFD_matched_1e_cut->Fill(reco_P_nFD.Mag(), neutrons_FD_matched.size(), weight);

            if (ParticleInECIN || ParticleInECOUT) {
                h_reco_P_nFD_VS_reco_L_ECAL_matched_1e_cut->Fill(Path_nFD, reco_P_nFD.Mag(), weight);
                h_reco_theta_nFD_VS_reco_L_ECAL_matched_1e_cut->Fill(Path_nFD, reco_P_nFD.Theta() * 180 / M_PI, weight);
                h_reco_phi_nFD_VS_reco_L_ECAL_matched_1e_cut->Fill(Path_nFD, reco_P_nFD.Phi() * 180 / M_PI, weight);
                h_reco_P_nFD_VS_ToF_nFD_fromFile_ECAL_matched_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Mag(), weight);
                h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECAL_matched_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Theta() * 180 / M_PI, weight);
                h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECAL_matched_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Phi() * 180 / M_PI, weight);

                if (ParticleInECIN) {
                    h_reco_P_nFD_VS_reco_L_ECIN_matched_1e_cut->Fill(Path_nFD, reco_P_nFD.Mag(), weight);
                    h_reco_theta_nFD_VS_reco_L_ECIN_matched_1e_cut->Fill(Path_nFD, reco_P_nFD.Theta() * 180 / M_PI, weight);
                    h_reco_phi_nFD_VS_reco_L_ECIN_matched_1e_cut->Fill(Path_nFD, reco_P_nFD.Phi() * 180 / M_PI, weight);
                    h_reco_P_nFD_VS_ToF_nFD_fromFile_ECIN_matched_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Mag(), weight);
                    h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECIN_matched_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Theta() * 180 / M_PI, weight);
                    h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECIN_matched_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Phi() * 180 / M_PI, weight);
                }

                if (ParticleInECOUT) {
                    h_reco_P_nFD_VS_reco_L_ECOUT_matched_1e_cut->Fill(Path_nFD, reco_P_nFD.Mag(), weight);
                    h_reco_theta_nFD_VS_reco_L_ECOUT_matched_1e_cut->Fill(Path_nFD, reco_P_nFD.Theta() * 180 / M_PI, weight);
                    h_reco_phi_nFD_VS_reco_L_ECOUT_matched_1e_cut->Fill(Path_nFD, reco_P_nFD.Phi() * 180 / M_PI, weight);
                    h_reco_P_nFD_VS_ToF_nFD_fromFile_ECOUT_matched_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Mag(), weight);
                    h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECOUT_matched_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Theta() * 180 / M_PI, weight);
                    h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECOUT_matched_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Phi() * 180 / M_PI, weight);
                }
            }

            h_reco_theta_nFD_minus_reco_theta_e_matched_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI, weight);
            h_reco_phi_nFD_minus_reco_phi_e_matched_1e_cut->Fill(CalcdPhi(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), weight);
            h_reco_theta_nFD_minus_reco_theta_e_VS_reco_phi_nFD_minus_reco_phi_e_matched_1e_cut->Fill(CalcdPhi(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI),
                                                                                                      reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI, weight);
            h_reco_P_nFD_VS_reco_theta_nFD_minus_reco_theta_e_matched_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI, reco_P_nFD.Mag(), weight);
            h_reco_P_nFD_VS_reco_phi_nFD_minus_reco_phi_e_matched_1e_cut->Fill(CalcdPhi(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), reco_P_nFD.Mag(), weight);
        }

        if (neutrons_FD_matched.size() != 0) { h_reco_nFD_multi_matched_1e_cut->Fill(neutrons_FD_matched.size(), weight); }
#pragma endregion

#pragma endregion
    }

    /////////////////////////////////////////////////////
    // Organize histograms
    /////////////////////////////////////////////////////
    cout << counter << endl;

    for (int i = 0; i < HistoList.size(); i++) {
        if (HistoList[i]->InheritsFrom("TH1D")) {
            HistoList[i]->Sumw2();
            HistoList[i]->SetMinimum(0);
            HistoList[i]->SetLineWidth(2);
            HistoList[i]->SetLineColor(kRed);
        }

        HistoList[i]->GetXaxis()->CenterTitle();
        HistoList[i]->GetYaxis()->CenterTitle();
    }

    /////////////////////////////////////////////////////
    // Now create the output PDFs
    /////////////////////////////////////////////////////
    int pixelx = 1980;
    int pixely = 1530;
    TCanvas* myCanvas = new TCanvas("myPage", "myPage", pixelx, pixely);
    TCanvas* myText = new TCanvas("myText", "myText", pixelx, pixely);
    TLatex titles;
    TLatex text;
    titles.SetTextSize(0.1);
    text.SetTextSize(0.05);

    gStyle->SetOptStat("ourmen");

    char fileName[100];
    sprintf(fileName, "%s[", "/lustre24/expphy/volatile/clas12/asportes/Analysis_output/nFD_eff_test/nFD_eff_test.pdf");
    myText->SaveAs(fileName);
    sprintf(fileName, "%s", "/lustre24/expphy/volatile/clas12/asportes/Analysis_output/nFD_eff_test/nFD_eff_test.pdf");

    /////////////////////////////////////
    // CND Neutron Information
    /////////////////////////////////////
    myText->cd();
    text.DrawLatex(0.2, 0.9, "Uniform sample of (e,e'n) events (truth-level)");
    if (findSubstring(InputFiles, "2070MeV")) {
        text.DrawLatex(0.2, 0.7, "Beam eneergy: 2070MeV");
    } else if (findSubstring(InputFiles, "4029MeV")) {
        text.DrawLatex(0.2, 0.7, "Beam eneergy: 4029MeV");
    } else if (findSubstring(InputFiles, "5986MeV")) {
        text.DrawLatex(0.2, 0.7, "Beam eneergy: 5986MeV");
    }
    myText->Print(fileName, "pdf");
    myText->Clear();

    for (int i = 0; i < HistoList.size(); i++) {
        for (int j = 0; j < HistSubjects.size(); j++) {
            if (FirstPrint.at(j) && findSubstring(HistoList[i]->GetTitle(), HistSubjects.at(j))) {
                myText->cd();
                titles.DrawLatex(0.3, 0.5, HistSubjects2.at(j).c_str());
                myText->Print(fileName, "pdf");
                myText->Clear();

                myCanvas->cd(1);
                FirstPrint.at(j) = false;
                break;
            }
        }

        myCanvas->cd();

        myCanvas->cd()->SetGrid();
        myCanvas->cd()->SetBottomMargin(0.14), myCanvas->cd()->SetLeftMargin(0.16), myCanvas->cd()->SetRightMargin(0.12);

        HistoList[i]->GetYaxis()->SetTitleOffset(1.5);
        HistoList[i]->GetXaxis()->SetTitleOffset(1.1);

        gPad->SetRightMargin(0.23);

        // // Set the PDF title and header for the bookmark
        // string Title = HistoList[i]->GetTitle();
        // gStyle->SetTitlePS(Title.c_str());  // This sets the title in metadata
        //                                     // gStyle->SetHeaderPS(("[ /Title " + Title + " /DOCVIEW pdfmark").c_str());  // Adds a PDF title
        // gStyle->SetHeaderPS(("[ /Page " + to_string(i + 1) + " /View [/Fit] /Title (myTitle) ] /OUT pdfmark").c_str());

        if (HistoList[i]->InheritsFrom("TH1D")) {
            HistoList[i]->Draw();
        } else if (HistoList[i]->InheritsFrom("TH2D")) {
            // if (findSubstring(HistoList[i]->GetTitle(), "#Delta#theta_{nFD,e} vs. #Delta#phi_{nFD,e} in 1e cut")) {
            //     gPad->SetRightMargin(0.225);
            // } else {
            //     gPad->SetRightMargin(0.05);
            // }

            HistoList[i]->Draw("colz");

            if (HistoList[i]->GetEntries() != 0) {
                gPad->Update();
                TPaletteAxis* palette = (TPaletteAxis*)HistoList[i]->GetListOfFunctions()->FindObject("palette");
                palette->SetY2NDC(0.55);
                gPad->Modified();
                gPad->Update();
            }
        }

        myCanvas->Print(fileName, "pdf");
        myCanvas->Clear();
    }

    sprintf(fileName, "%s]", "/lustre24/expphy/volatile/clas12/asportes/Analysis_output/nFD_eff_test/nFD_eff_test.pdf");
    myCanvas->Print(fileName, "pdf");

    outFile->cd();
    for (int i = 0; i < HistoList.size(); i++) { HistoList[i]->Write(); }
    outFile->Close();
}
