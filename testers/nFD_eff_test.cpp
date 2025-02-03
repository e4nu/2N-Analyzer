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

double CalcPnFD(region_part_ptr NeutronFD) {
    int ParticlePDG = NeutronFD->par()->getPid();

    double Momentum;

    if (ParticlePDG == 2112) {
        Momentum = NeutronFD->par()->getP();
    } else if (ParticlePDG == 22) {
        double Beta_ph = NeutronFD->par()->getBeta();
        double Gamma_ph = 1 / sqrt(1 - (Beta_ph * Beta_ph));
        Momentum = m_n * Beta_ph * Gamma_ph;
    } else {
        cout << "\n\nError! Particle PDG is not 22 or 2112! Aborting...\n\n", exit(0);
    }

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
        if (allParticles[j]->par()->getCharge() == 0) { continue; } /* looking on charged particles only */
        TVector3 v_chit;                                            /* v_chit = location of charged particle hit */

        if ((detlayer == clas12::ECIN) && (allParticles[j]->cal(clas12::ECIN)->getZ() != 0)) {
            /* if both particles hit the inner calorimeter, use the inner calorimeter to determine v_chit */
            v_chit.SetXYZ(allParticles[j]->cal(clas12::ECIN)->getX(), allParticles[j]->cal(clas12::ECIN)->getY(), allParticles[j]->cal(clas12::ECIN)->getZ());
            TVector3 v_dist = v_nhit - v_chit;

            if (v_dist.Mag() < veto_cut) { Veto = true; }
        } else if ((detlayer == clas12::ECOUT) && (allParticles[j]->cal(clas12::ECOUT)->getZ() != 0)) {
            /* if both particles hit the outer calorimeter, use the outer calorimeter to determine v_chit */
            v_chit.SetXYZ(allParticles[j]->cal(clas12::ECOUT)->getX(), allParticles[j]->cal(clas12::ECOUT)->getY(), allParticles[j]->cal(clas12::ECOUT)->getZ());
            TVector3 v_dist = v_nhit - v_chit;

            if (v_dist.Mag() < veto_cut) { Veto = true; }
        } else {
            /* the neutral has to hit either the ECIN or ECOUT.
               If the charged particle hit the other calorimeter, then look at where the charged particle was expected to be according to the trajectory. */
            int trajlayer = (detlayer == clas12::ECIN) ? 4 : 7;
            v_chit.SetXYZ(allParticles[j]->traj(clas12::ECAL, trajlayer)->getX(), allParticles[j]->traj(clas12::ECAL, trajlayer)->getY(),
                          allParticles[j]->traj(clas12::ECAL, trajlayer)->getZ());
            TVector3 v_dist = v_nhit - v_chit;

            if (v_dist.Mag() < veto_cut) { Veto = true; }
        }
    }

    if (Veto) { return false; } /* if any of the vetoes are true, return false */

    return true; /* we survived up to this point, we do have a neutral particle */
}

void nFD_eff_test() {
    cout << "\n\nInitiating nFD_eff_test.cpp\n";

    double Ebeam = 4.02962;

    int Limiter = 100000;

    const string OutputDir = "/lustre24/expphy/volatile/clas12/asportes/Analysis_output/nFD_eff_test";
    system(("rm -rf " + OutputDir).c_str());
    system(("mkdir -p " + OutputDir).c_str());

    TFile* outFile = new TFile("/lustre24/expphy/volatile/clas12/asportes/Analysis_output/nFD_eff_test/nFD_eff_test.root", "RECREATE");

    clas12root::HipoChain chain;
    string InputFiles = "/lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco/Uniform_e-p-n_samples/4029MeV/OutPut_en/reconhipo/*.hipo";
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
    TH1D* h_reco_P_e_1e_cut = new TH1D("reco_P_e_1e_cut", "reco P_{e} in 1e cut;P_{e} [GeV/c];Counts", 50, 0, Ebeam * 1.1);
    HistoList.push_back(h_reco_P_e_1e_cut);
    TH1D* h_truth_P_e_1e_cut = new TH1D("truth_P_e_1e_cut", "truth P_{e} in 1e cut;P_{e} [GeV/c];Counts", 50, 0, Ebeam * 1.1);
    HistoList.push_back(h_truth_P_e_1e_cut);
    TH1D* h_reco_theta_e_1e_cut = new TH1D("reco_theta_e_1e_cut", "reco #theta_{e} in 1e cut;#theta_{e} [#circ];Counts", 50, 0, 45.);
    HistoList.push_back(h_reco_theta_e_1e_cut);
    TH1D* h_truth_theta_e_1e_cut = new TH1D("truth_theta_e_1e_cut", "truth #theta_{e} in 1e cut;#theta_{e} [#circ];Counts", 50, 0, 45.);
    HistoList.push_back(h_truth_theta_e_1e_cut);
    TH1D* h_reco_phi_e_1e_cut = new TH1D("reco_phi_e_1e_cut", "reco #phi_{e} in 1e cut;#phi_{e} [#circ];Counts", 50, -180, 180.);
    HistoList.push_back(h_reco_phi_e_1e_cut);
    TH1D* h_truth_phi_e_1e_cut = new TH1D("truth_phi_e_1e_cut", "truth #phi_{e} in 1e cut;#phi_{e} [#circ];Counts", 50, -180, 180.);
    HistoList.push_back(h_truth_phi_e_1e_cut);
    TH2D* h_reco_theta_e_VS_reco_phi_e_1e_cut =
        new TH2D("reco_theta_e_VS_reco_phi_e_1e_cut", "reco #theta_{e} vs. reco #phi_{e} in 1e cut;#phi_{e} [#circ];#theta_{e} [#circ]", 100, -180., 180., 100, 0, 50.);
    HistoList.push_back(h_reco_theta_e_VS_reco_phi_e_1e_cut);
    TH2D* h_truth_theta_e_VS_truth_phi_e_1e_cut =
        new TH2D("truth_theta_e_VS_truth_phi_e_1e_cut", "truth #theta_{e} vs. truth #phi_{e} in 1e cut;#phi_{e} [#circ];#theta_{e} [#circ]", 100, -180., 180., 100, 0, 50.);
    HistoList.push_back(h_truth_theta_e_VS_truth_phi_e_1e_cut);

    TH1D* h_reco_P_n_1e_cut = new TH1D("reco_P_n_1e_cut", "reco P_{n} in 1e cut;P_{n} [GeV/c];Counts", 50, 0, Ebeam * 1.1);
    HistoList.push_back(h_reco_P_n_1e_cut);
    TH1D* h_truth_P_n_1e_cut = new TH1D("truth_P_n_1e_cut", "truth P_{n} in 1e cut;P_{n} [GeV/c];Counts", 50, 0, Ebeam * 1.1);
    HistoList.push_back(h_truth_P_n_1e_cut);
    TH1D* h_reco_theta_n_1e_cut = new TH1D("reco_theta_n_1e_cut", "reco #theta_{n} in 1e cut;#theta_{n} [#circ];Counts", 50, 0, 45.);
    HistoList.push_back(h_reco_theta_n_1e_cut);
    TH1D* h_truth_theta_n_1e_cut = new TH1D("truth_theta_n_1e_cut", "truth #theta_{n} in 1e cut;#theta_{n} [#circ];Counts", 50, 0, 45.);
    HistoList.push_back(h_truth_theta_n_1e_cut);
    TH1D* h_reco_phi_n_1e_cut = new TH1D("reco_phi_n_1e_cut", "reco #phi_{n} in 1e cut;#phi_{n} [#circ];Counts", 50, -180, 180.);
    HistoList.push_back(h_reco_phi_n_1e_cut);
    TH1D* h_truth_phi_n_1e_cut = new TH1D("truth_phi_n_1e_cut", "truth #phi_{n} in 1e cut;#phi_{n} [#circ];Counts", 50, -180, 180.);
    HistoList.push_back(h_truth_phi_n_1e_cut);
    TH2D* h_reco_theta_n_VS_reco_phi_n_1e_cut =
        new TH2D("reco_theta_n_VS_reco_phi_n_1e_cut", "reco #theta_{n} vs. reco #phi_{n} in 1e cut;#phi_{n} [#circ];#theta_{n} [#circ]", 100, -180., 180., 100, 0, 50.);
    HistoList.push_back(h_reco_theta_n_VS_reco_phi_n_1e_cut);
    TH2D* h_truth_theta_n_VS_truth_phi_n_1e_cut =
        new TH2D("truth_theta_n_VS_truth_phi_n_1e_cut", "truth #theta_{n} vs. truth #phi_{n} in 1e cut;#phi_{n} [#circ];#theta_{n} [#circ]", 100, -180., 180., 100, 0, 50.);
    HistoList.push_back(h_truth_theta_n_VS_truth_phi_n_1e_cut);
#pragma endregion

#pragma region /* clas12reco */
    HistSubjects.push_back("clas12reco");
    HistSubjects2.push_back("#splitline{FD neutron from}{clas12reco}");
    FirstPrint.push_back(true);
    TH1D* h_reco_P_nFD_clas12_1e_cut = new TH1D("reco_P_nFD_clas12_1e_cut", "reco P_{nFD} in 1e cut (clas12reco);P_{nFD} [GeV/c];Counts", 50, 0, Ebeam * 1.1);
    HistoList.push_back(h_reco_P_nFD_clas12_1e_cut);
    TH1D* h_truth_P_nFD_clas12_1e_cut = new TH1D("truth_P_nFD_clas12_1e_cut", "truth P_{nFD} in 1e cut (clas12reco);P_{nFD} [GeV/c];Counts", 50, 0, Ebeam * 1.1);
    HistoList.push_back(h_truth_P_nFD_clas12_1e_cut);
    TH1D* h_reco_theta_nFD_clas12_1e_cut = new TH1D("reco_theta_nFD_clas12_1e_cut", "reco #theta_{nFD} in 1e cut (clas12reco);#theta_{nFD} [#circ];Counts", 50, 0, 45.);
    HistoList.push_back(h_reco_theta_nFD_clas12_1e_cut);
    TH1D* h_truth_theta_nFD_clas12_1e_cut = new TH1D("truth_theta_nFD_clas12_1e_cut", "truth #theta_{nFD} in 1e cut (clas12reco);#theta_{nFD} [#circ];Counts", 50, 0, 45.);
    HistoList.push_back(h_truth_theta_nFD_clas12_1e_cut);
    TH1D* h_reco_phi_nFD_clas12_1e_cut = new TH1D("reco_phi_nFD_clas12_1e_cut", "reco #phi_{nFD} in 1e cut (clas12reco);#phi_{nFD} [#circ];Counts", 50, -180, 180.);
    HistoList.push_back(h_reco_phi_nFD_clas12_1e_cut);
    TH1D* h_truth_phi_nFD_clas12_1e_cut = new TH1D("truth_phi_nFD_clas12_1e_cut", "truth #phi_{nFD} in 1e cut (clas12reco);#phi_{nFD} [#circ];Counts", 50, -180, 180.);
    HistoList.push_back(h_truth_phi_nFD_clas12_1e_cut);
    TH2D* h_reco_theta_nFD_clas12_VS_reco_phi_nFD_clas12_1e_cut =
        new TH2D("reco_theta_nFD_clas12_VS_reco_phi_nFD_clas12_1e_cut", "reco #theta_{nFD} vs. reco #phi_{nFD} in 1e cut (clas12reco);#phi_{nFD} [#circ];#theta_{nFD} [#circ]", 100, -180.,
                 180., 100, 0, 50.);
    HistoList.push_back(h_reco_theta_nFD_clas12_VS_reco_phi_nFD_clas12_1e_cut);
    TH2D* h_truth_theta_nFD_clas12_VS_truth_phi_nFD_clas12_1e_cut =
        new TH2D("truth_theta_nFD_clas12_VS_truth_phi_nFD_clas12_1e_cut", "truth #theta_{nFD} vs. truth #phi_{nFD} in 1e cut (clas12reco);#phi_{nFD} [#circ];#theta_{nFD} [#circ]", 100,
                 -180., 180., 100, 0, 50.);
    HistoList.push_back(h_truth_theta_nFD_clas12_VS_truth_phi_nFD_clas12_1e_cut);
    TH2D* h_reco_P_e_VS_P_nFD_clas12_1e_cut =
        new TH2D("reco_P_e_VS_P_nFD_clas12_1e_cut", "reco P_{e} vs. reco P_{nFD} in 1e cut (clas12reco);P_{e} [GeV/c];P_{nFD} [GeV/c]", 100, 0., Ebeam * 2., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_e_VS_P_nFD_clas12_1e_cut);
    TH2D* h_reco_theta_nFD_clas12_VS_P_nFD_clas12_1e_cut = new TH2D(
        "reco_theta_nFD_clas12_VS_P_nFD_clas12_1e_cut", "reco #theta_{nFD} vs. reco P_{nFD} in 1e cut (clas12reco);#theta_{nFD} [#circ];P_{nFD} [GeV/c]", 100, 0., 50., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_theta_nFD_clas12_VS_P_nFD_clas12_1e_cut);
#pragma endregion

#pragma region /* redef */
    HistSubjects.push_back("redef");
    HistSubjects2.push_back("redef");
    FirstPrint.push_back(true);
    TH1D* h_reco_P_nFD_redef_1e_cut = new TH1D("reco_P_nFD_redef_1e_cut", "reco P_{nFD} in 1e cut (redef);P_{nFD} [GeV/c];Counts", 50, 0, Ebeam * 1.1);
    HistoList.push_back(h_reco_P_nFD_redef_1e_cut);
    TH1D* h_truth_P_nFD_redef_1e_cut = new TH1D("truth_P_nFD_redef_1e_cut", "truth P_{nFD} in 1e cut (redef);P_{nFD} [GeV/c];Counts", 50, 0, Ebeam * 1.1);
    HistoList.push_back(h_truth_P_nFD_redef_1e_cut);
    TH1D* h_reco_theta_nFD_redef_1e_cut = new TH1D("reco_theta_nFD_redef_1e_cut", "reco #theta_{nFD} in 1e cut (redef);#theta_{nFD} [#circ];Counts", 50, 0, 45.);
    HistoList.push_back(h_reco_theta_nFD_redef_1e_cut);
    TH1D* h_truth_theta_nFD_redef_1e_cut = new TH1D("truth_theta_nFD_redef_1e_cut", "truth #theta_{nFD} in 1e cut (redef);#theta_{nFD} [#circ];Counts", 50, 0, 45.);
    HistoList.push_back(h_truth_theta_nFD_redef_1e_cut);
    TH1D* h_reco_phi_nFD_redef_1e_cut = new TH1D("reco_phi_nFD_redef_1e_cut", "reco #phi_{nFD} in 1e cut (redef);#phi_{nFD} [#circ];Counts", 50, -180, 180.);
    HistoList.push_back(h_reco_phi_nFD_redef_1e_cut);
    TH1D* h_truth_phi_nFD_redef_1e_cut = new TH1D("truth_phi_nFD_redef_1e_cut", "truth #phi_{nFD} in 1e cut (redef);#phi_{nFD} [#circ];Counts", 50, -180, 180.);
    HistoList.push_back(h_truth_phi_nFD_redef_1e_cut);
    TH2D* h_reco_theta_nFD_redef_VS_reco_phi_nFD_redef_1e_cut =
        new TH2D("reco_theta_nFD_redef_VS_reco_phi_nFD_redef_1e_cut", "reco #theta_{nFD} vs. reco #phi_{nFD} in 1e cut (redef);#phi_{nFD} [#circ];#theta_{nFD} [#circ]", 100, -180., 180.,
                 100, 0, 50.);
    HistoList.push_back(h_reco_theta_nFD_redef_VS_reco_phi_nFD_redef_1e_cut);
    TH2D* h_truth_theta_nFD_redef_VS_truth_phi_nFD_redef_1e_cut =
        new TH2D("truth_theta_nFD_redef_VS_truth_phi_nFD_redef_1e_cut", "truth #theta_{nFD} vs. truth #phi_{nFD} in 1e cut (redef);#phi_{nFD} [#circ];#theta_{nFD} [#circ]", 100, -180., 180.,
                 100, 0, 50.);
    HistoList.push_back(h_truth_theta_nFD_redef_VS_truth_phi_nFD_redef_1e_cut);
    TH2D* h_reco_P_e_VS_P_nFD_redef_1e_cut =
        new TH2D("reco_P_e_VS_P_nFD_redef_1e_cut", "reco P_{e} vs. reco P_{nFD} in 1e cut (redef);P_{e} [GeV/c];P_{nFD} [GeV/c]", 100, 0., Ebeam * 2., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_e_VS_P_nFD_redef_1e_cut);
    TH2D* h_reco_theta_nFD_redef_VS_P_nFD_redef_1e_cut = new TH2D(
        "reco_theta_nFD_redef_VS_P_nFD_redef_1e_cut", "reco #theta_{nFD} vs. reco P_{nFD} in 1e cut (redef);#theta_{nFD} [#circ];P_{nFD} [GeV/c]", 100, 0., 50., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_theta_nFD_redef_VS_P_nFD_redef_1e_cut);
#pragma endregion

#pragma region /* ECALveto */
    HistSubjects.push_back("ECALveto");
    HistSubjects2.push_back("#splitline{ECALveto}{and P_{nFD} thresholds}");
    FirstPrint.push_back(true);
    TH1D* h_reco_P_nFD_ECALveto_1e_cut = new TH1D("reco_P_nFD_ECALveto_1e_cut", "reco P_{nFD} in 1e cut (ECALveto);P_{nFD} [GeV/c];Counts", 50, 0, Ebeam * 1.1);
    HistoList.push_back(h_reco_P_nFD_ECALveto_1e_cut);
    TH1D* h_truth_P_nFD_ECALveto_1e_cut = new TH1D("truth_P_nFD_ECALveto_1e_cut", "truth P_{nFD} in 1e cut (ECALveto);P_{nFD} [GeV/c];Counts", 50, 0, Ebeam * 1.1);
    HistoList.push_back(h_truth_P_nFD_ECALveto_1e_cut);
    TH1D* h_reco_theta_nFD_ECALveto_1e_cut = new TH1D("reco_theta_nFD_ECALveto_1e_cut", "reco #theta_{nFD} in 1e cut (ECALveto);#theta_{nFD} [#circ];Counts", 50, 0, 45.);
    HistoList.push_back(h_reco_theta_nFD_ECALveto_1e_cut);
    TH1D* h_truth_theta_nFD_ECALveto_1e_cut = new TH1D("truth_theta_nFD_ECALveto_1e_cut", "truth #theta_{nFD} in 1e cut (ECALveto);#theta_{nFD} [#circ];Counts", 50, 0, 45.);
    HistoList.push_back(h_truth_theta_nFD_ECALveto_1e_cut);
    TH1D* h_reco_phi_nFD_ECALveto_1e_cut = new TH1D("reco_phi_nFD_ECALveto_1e_cut", "reco #phi_{nFD} in 1e cut (ECALveto);#phi_{nFD} [#circ];Counts", 50, -180, 180.);
    HistoList.push_back(h_reco_phi_nFD_ECALveto_1e_cut);
    TH1D* h_truth_phi_nFD_ECALveto_1e_cut = new TH1D("truth_phi_nFD_ECALveto_1e_cut", "truth #phi_{nFD} in 1e cut (ECALveto);#phi_{nFD} [#circ];Counts", 50, -180, 180.);
    HistoList.push_back(h_truth_phi_nFD_ECALveto_1e_cut);
    TH2D* h_reco_theta_nFD_ECALveto_VS_reco_phi_nFD_ECALveto_1e_cut =
        new TH2D("reco_theta_nFD_ECALveto_VS_reco_phi_nFD_ECALveto_1e_cut", "reco #theta_{nFD} vs. reco #phi_{nFD} in 1e cut (ECALveto);#phi_{nFD} [#circ];#theta_{nFD} [#circ]", 100, -180.,
                 180., 100, 0, 50.);
    HistoList.push_back(h_reco_theta_nFD_ECALveto_VS_reco_phi_nFD_ECALveto_1e_cut);
    TH2D* h_truth_theta_nFD_ECALveto_VS_truth_phi_nFD_ECALveto_1e_cut =
        new TH2D("truth_theta_nFD_ECALveto_VS_truth_phi_nFD_ECALveto_1e_cut", "truth #theta_{nFD} vs. truth #phi_{nFD} in 1e cut (ECALveto);#phi_{nFD} [#circ];#theta_{nFD} [#circ]", 100,
                 -180., 180., 100, 0, 50.);
    HistoList.push_back(h_truth_theta_nFD_ECALveto_VS_truth_phi_nFD_ECALveto_1e_cut);

    TH2D* h_reco_P_e_VS_P_nFD_ECALveto_1e_cut =
        new TH2D("reco_P_e_VS_P_nFD_ECALveto_1e_cut", "reco P_{e} vs. reco P_{nFD} in 1e cut (ECALveto);P_{e} [GeV/c];P_{nFD} [GeV/c]", 100, 0., Ebeam * 2., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_e_VS_P_nFD_ECALveto_1e_cut);
    TH2D* h_reco_theta_e_VS_P_nFD_ECALveto_1e_cut =
        new TH2D("reco_theta_e_VS_P_nFD_ECALveto_1e_cut", "reco #theta_{e} vs. reco P_{nFD} in 1e cut (ECALveto);#theta_{e} [#circ];P_{nFD} [GeV/c]", 100, 0, 45., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_theta_e_VS_P_nFD_ECALveto_1e_cut);
    TH2D* h_reco_phi_e_VS_P_nFD_ECALveto_1e_cut =
        new TH2D("reco_phi_e_VS_P_nFD_ECALveto_1e_cut", "reco #phi_{e} vs. reco P_{nFD} in 1e cut (ECALveto);#phi_{e} [#circ];P_{nFD} [GeV/c]", 100, -180., 180., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_phi_e_VS_P_nFD_ECALveto_1e_cut);
    TH2D* h_reco_P_e_VS_theta_nFD_ECALveto_1e_cut =
        new TH2D("reco_P_e_VS_theta_nFD_ECALveto_1e_cut", "reco P_{e} vs. reco #theta_{nFD} in 1e cut (ECALveto);P_{e} [GeV/c];#theta_{nFD} [#circ]", 100, 0., Ebeam * 1.1, 100, 0, 45.);
    HistoList.push_back(h_reco_P_e_VS_theta_nFD_ECALveto_1e_cut);
    TH2D* h_reco_theta_e_VS_theta_nFD_ECALveto_1e_cut =
        new TH2D("reco_theta_e_VS_theta_nFD_ECALveto_1e_cut", "reco #theta_{e} vs. reco #theta_{nFD} in 1e cut (ECALveto);#theta_{e} [#circ];#theta_{nFD} [#circ]", 100, 0, 45., 100, 0, 45.);
    HistoList.push_back(h_reco_theta_e_VS_theta_nFD_ECALveto_1e_cut);
    TH2D* h_reco_phi_e_VS_theta_nFD_ECALveto_1e_cut =
        new TH2D("reco_phi_e_VS_theta_nFD_ECALveto_1e_cut", "reco #phi_{e} vs. reco #theta_{nFD} in 1e cut (ECALveto);#phi_{e} [#circ];#theta_{nFD} [#circ]", 100, -180., 180., 100, 0, 45.);
    HistoList.push_back(h_reco_phi_e_VS_theta_nFD_ECALveto_1e_cut);
    TH2D* h_reco_P_e_VS_phi_nFD_ECALveto_1e_cut =
        new TH2D("reco_P_e_VS_phi_nFD_ECALveto_1e_cut", "reco P_{e} vs. reco #phi_{nFD} in 1e cut (ECALveto);P_{e} [GeV/c];#phi_{nFD} [#circ]", 100, 0., Ebeam * 1.1, 100, -180., 180.);
    HistoList.push_back(h_reco_P_e_VS_phi_nFD_ECALveto_1e_cut);
    TH2D* h_reco_theta_e_VS_phi_nFD_ECALveto_1e_cut =
        new TH2D("reco_phi_e_VS_phi_nFD_ECALveto_1e_cut", "reco #theta_{e} vs. reco #phi_{nFD} in 1e cut (ECALveto);#theta_{e} [#circ];#phi_{nFD} [#circ]", 100, 0, 45., 100, -180., 180.);
    HistoList.push_back(h_reco_theta_e_VS_phi_nFD_ECALveto_1e_cut);
    TH2D* h_reco_phi_e_VS_phi_nFD_ECALveto_1e_cut =
        new TH2D("reco_phi_e_VS_phi_nFD_ECALveto_1e_cut", "reco #phi_{e} vs. reco #phi_{nFD} in 1e cut (ECALveto);#phi_{e} [#circ];#phi_{nFD} [#circ]", 100, -180., 180., 100, -180., 180.);
    HistoList.push_back(h_reco_phi_e_VS_phi_nFD_ECALveto_1e_cut);
    TH2D* h_reco_theta_nFD_ECALveto_VS_P_nFD_ECALveto_1e_cut =
        new TH2D("reco_theta_nFD_ECALveto_VS_P_nFD_ECALveto_1e_cut", "reco #theta_{nFD} vs. reco P_{nFD} in 1e cut (ECALveto);#theta_{nFD} [#circ];P_{nFD} [GeV/c]", 100, 0., 100., 100, 0.,
                 Ebeam * 3.);
    HistoList.push_back(h_reco_theta_nFD_ECALveto_VS_P_nFD_ECALveto_1e_cut);
    TH1D* h_reco_nFD_multi_ECALveto_1e_cut = new TH1D("reco_nFD_multi_ECALveto_1e_cut", "reco nFD multiplicity in 1e cut (ECALveto);nFD multiplicity;Counts", 9, 1, 10);
    HistoList.push_back(h_reco_nFD_multi_ECALveto_1e_cut);
    TH2D* h_reco_nFD_multi_VS_recp_P_nFD_ECALveto_1e_cut =
        new TH2D("reco_nFD_multi_VS_recp_P_nFD_ECALveto_1e_cut", "reco nFD multiplicity vs. P_{nFD} in 1e cut (ECALveto);P_{nFD} [GeV/c];nFD multiplicity", 100, 0., Ebeam * 3., 9, 1, 10);
    HistoList.push_back(h_reco_nFD_multi_VS_recp_P_nFD_ECALveto_1e_cut);

    TH1D* h_reco_theta_e_minus_reco_theta_nFD_ECALveto_1e_cut =
        new TH1D("reco_theta_e_minus_reco_theta_nFD_ECALveto_1e_cut", "reco #theta_{e} vs. reco #theta_{nFD} in 1e cut (ECALveto);#theta_{e}-#theta_{nFD} [#circ];Counts", 50, -40., 40.);
    HistoList.push_back(h_reco_theta_e_minus_reco_theta_nFD_ECALveto_1e_cut);
    TH1D* h_reco_phi_e_minus_reco_phi_nFD_ECALveto_1e_cut =
        new TH1D("reco_phi_e_minus_reco_phi_nFD_ECALveto_1e_cut", "reco #phi_{e} vs. reco #phi_{nFD} in 1e cut (ECALveto);|#phi_{e}-#phi_{nFD}| [#circ];Counts", 50, -180., 180.);
    HistoList.push_back(h_reco_phi_e_minus_reco_phi_nFD_ECALveto_1e_cut);
    TH2D* h_reco_theta_e_minus_reco_theta_nFD_VS_reco_phi_e_minus_reco_phi_nFD_ECALveto_1e_cut =
        new TH2D("reco_theta_e_minus_reco_theta_nFD_VS_reco_phi_e_minus_reco_phi_nFD_ECALveto_1e_cut",
                 "#theta_{e}-#theta_{nFD} vs. |#phi_{e}-#phi_{nFD}| in 1e cut (ECALveto);|#phi_{e}-#phi_{nFD}| [#circ];#theta_{e}-#theta_{nFD} [#circ]", 100, 0., Ebeam * 3., 9, 1, 10);
    HistoList.push_back(h_reco_theta_e_minus_reco_theta_nFD_VS_reco_phi_e_minus_reco_phi_nFD_ECALveto_1e_cut);

#pragma endregion

#pragma region /* matched */
    HistSubjects.push_back("matched");
    HistSubjects2.push_back("matched");
    FirstPrint.push_back(true);
    TH1D* h_reco_P_nFD_matched_1e_cut = new TH1D("reco_P_nFD_matched_1e_cut", "reco P_{nFD} in 1e cut (matched);P_{nFD} [GeV/c];Counts", 50, 0, Ebeam * 1.1);
    HistoList.push_back(h_reco_P_nFD_matched_1e_cut);
    TH1D* h_truth_P_nFD_matched_1e_cut = new TH1D("truth_P_nFD_matched_1e_cut", "truth P_{nFD} in 1e cut (matched);P_{nFD} [GeV/c];Counts", 50, 0, Ebeam * 1.1);
    HistoList.push_back(h_truth_P_nFD_matched_1e_cut);
    TH1D* h_reco_theta_nFD_matched_1e_cut = new TH1D("reco_theta_nFD_matched_1e_cut", "reco #theta_{nFD} in 1e cut (matched);#theta_{nFD} [#circ];Counts", 50, 0, 45.);
    HistoList.push_back(h_reco_theta_nFD_matched_1e_cut);
    TH1D* h_truth_theta_nFD_matched_1e_cut = new TH1D("truth_theta_nFD_matched_1e_cut", "truth #theta_{nFD} in 1e cut (matched);#theta_{nFD} [#circ];Counts", 50, 0, 45.);
    HistoList.push_back(h_truth_theta_nFD_matched_1e_cut);
    TH1D* h_reco_phi_nFD_matched_1e_cut = new TH1D("reco_phi_nFD_matched_1e_cut", "reco #phi_{nFD} in 1e cut (matched);#phi_{nFD} [#circ];Counts", 50, -180, 180.);
    HistoList.push_back(h_reco_phi_nFD_matched_1e_cut);
    TH1D* h_truth_phi_nFD_matched_1e_cut = new TH1D("truth_phi_nFD_matched_1e_cut", "truth #phi_{nFD} in 1e cut (matched);#phi_{nFD} [#circ];Counts", 50, -180, 180.);
    HistoList.push_back(h_truth_phi_nFD_matched_1e_cut);
    TH2D* h_reco_theta_nFD_matched_VS_reco_phi_nFD_matched_1e_cut =
        new TH2D("reco_theta_nFD_matched_VS_reco_phi_nFD_matched_1e_cut", "reco #theta_{nFD} vs. reco #phi_{nFD} in 1e cut (matched);#phi_{nFD} [#circ];#theta_{nFD} [#circ]", 100, -180.,
                 180., 100, 0, 50.);
    HistoList.push_back(h_reco_theta_nFD_matched_VS_reco_phi_nFD_matched_1e_cut);
    TH2D* h_truth_theta_nFD_matched_VS_truth_phi_nFD_matched_1e_cut =
        new TH2D("truth_theta_nFD_matched_VS_truth_phi_nFD_matched_1e_cut", "truth #theta_{nFD} vs. truth #phi_{nFD} in 1e cut (matched);#phi_{nFD} [#circ];#theta_{nFD} [#circ]", 100, -180.,
                 180., 100, 0, 50.);
    HistoList.push_back(h_truth_theta_nFD_matched_VS_truth_phi_nFD_matched_1e_cut);

    TH2D* h_reco_P_e_VS_P_nFD_matched_1e_cut =
        new TH2D("reco_P_e_VS_P_nFD_matched_1e_cut", "reco P_{e} vs. reco P_{nFD} in 1e cut (matched);P_{e} [GeV/c];P_{nFD} [GeV/c]", 100, 0., Ebeam * 2., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_e_VS_P_nFD_matched_1e_cut);
    TH2D* h_reco_theta_e_VS_P_nFD_matched_1e_cut =
        new TH2D("reco_theta_e_VS_P_nFD_matched_1e_cut", "reco #theta_{e} vs. reco P_{nFD} in 1e cut (matched);#theta_{e} [#circ];P_{nFD} [GeV/c]", 100, 0, 45., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_theta_e_VS_P_nFD_matched_1e_cut);
    TH2D* h_reco_phi_e_VS_P_nFD_matched_1e_cut =
        new TH2D("reco_phi_e_VS_P_nFD_matched_1e_cut", "reco #phi_{e} vs. reco P_{nFD} in 1e cut (matched);#phi_{e} [#circ];P_{nFD} [GeV/c]", 100, -180., 180., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_phi_e_VS_P_nFD_matched_1e_cut);
    TH2D* h_reco_P_e_VS_theta_nFD_matched_1e_cut =
        new TH2D("reco_P_e_VS_theta_nFD_matched_1e_cut", "reco P_{e} vs. reco #theta_{nFD} in 1e cut (matched);P_{e} [GeV/c];#theta_{nFD} [#circ]", 100, 0., Ebeam * 1.1, 100, 0, 45.);
    HistoList.push_back(h_reco_P_e_VS_theta_nFD_matched_1e_cut);
    TH2D* h_reco_theta_e_VS_theta_nFD_matched_1e_cut =
        new TH2D("reco_theta_e_VS_theta_nFD_matched_1e_cut", "reco #theta_{e} vs. reco #theta_{nFD} in 1e cut (matched);#theta_{e} [#circ];#theta_{nFD} [#circ]", 100, 0, 45., 100, 0, 45.);
    HistoList.push_back(h_reco_theta_e_VS_theta_nFD_matched_1e_cut);
    TH2D* h_reco_phi_e_VS_theta_nFD_matched_1e_cut =
        new TH2D("reco_phi_e_VS_theta_nFD_matched_1e_cut", "reco #phi_{e} vs. reco #theta_{nFD} in 1e cut (matched);#phi_{e} [#circ];#theta_{nFD} [#circ]", 100, -180., 180., 100, 0, 45.);
    HistoList.push_back(h_reco_phi_e_VS_theta_nFD_matched_1e_cut);
    TH2D* h_reco_P_e_VS_phi_nFD_matched_1e_cut =
        new TH2D("reco_P_e_VS_phi_nFD_matched_1e_cut", "reco P_{e} vs. reco #phi_{nFD} in 1e cut (matched);P_{e} [GeV/c];#phi_{nFD} [#circ]", 100, 0., Ebeam * 1.1, 100, -180., 180.);
    HistoList.push_back(h_reco_P_e_VS_phi_nFD_matched_1e_cut);
    TH2D* h_reco_theta_e_VS_phi_nFD_matched_1e_cut =
        new TH2D("reco_phi_e_VS_phi_nFD_matched_1e_cut", "reco #theta_{e} vs. reco #phi_{nFD} in 1e cut (matched);#theta_{e} [#circ];#phi_{nFD} [#circ]", 100, 0, 45., 100, -180., 180.);
    HistoList.push_back(h_reco_theta_e_VS_phi_nFD_matched_1e_cut);
    TH2D* h_reco_phi_e_VS_phi_nFD_matched_1e_cut =
        new TH2D("reco_phi_e_VS_phi_nFD_matched_1e_cut", "reco #phi_{e} vs. reco #phi_{nFD} in 1e cut (matched);#phi_{e} [#circ];#phi_{nFD} [#circ]", 100, -180., 180., 100, -180., 180.);
    HistoList.push_back(h_reco_phi_e_VS_phi_nFD_matched_1e_cut);
    TH2D* h_reco_theta_nFD_matched_VS_P_nFD_matched_1e_cut = new TH2D(
        "reco_theta_nFD_matched_VS_P_nFD_matched_1e_cut", "reco #theta_{nFD} vs. reco P_{nFD} in 1e cut (matched);#theta_{nFD} [#circ];P_{nFD} [GeV/c]", 100, 0., 100., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_theta_nFD_matched_VS_P_nFD_matched_1e_cut);
    TH1D* h_reco_nFD_multi_matched_1e_cut = new TH1D("reco_nFD_multi_matched_1e_cut", "reco nFD multiplicity in 1e cut (matched);nFD multiplicity;Counts", 9, 1, 10);
    HistoList.push_back(h_reco_nFD_multi_matched_1e_cut);
    TH2D* h_reco_nFD_multi_VS_recp_P_nFD_matched_1e_cut =
        new TH2D("reco_nFD_multi_VS_recp_P_nFD_matched_1e_cut", "reco nFD multiplicity vs. P_{nFD} in 1e cut (matched);P_{nFD} [GeV/c];nFD multiplicity", 100, 0., Ebeam * 3., 9, 1, 10);
    HistoList.push_back(h_reco_nFD_multi_VS_recp_P_nFD_matched_1e_cut);

    TH1D* h_reco_theta_e_minus_reco_theta_nFD_matched_1e_cut =
        new TH1D("reco_theta_e_minus_reco_theta_nFD_matched_1e_cut", "reco #theta_{e} vs. reco #theta_{nFD} in 1e cut (matched);#theta_{e}-#theta_{nFD} [#circ];Counts", 50, -40., 40.);
    HistoList.push_back(h_reco_theta_e_minus_reco_theta_nFD_matched_1e_cut);
    TH1D* h_reco_phi_e_minus_reco_phi_nFD_matched_1e_cut =
        new TH1D("reco_phi_e_minus_reco_phi_nFD_matched_1e_cut", "reco #phi_{e} vs. reco #phi_{nFD} in 1e cut (matched);|#phi_{e}-#phi_{nFD}| [#circ];Counts", 50, -180., 180.);
    HistoList.push_back(h_reco_phi_e_minus_reco_phi_nFD_matched_1e_cut);
    TH2D* h_reco_theta_e_minus_reco_theta_nFD_VS_reco_phi_e_minus_reco_phi_nFD_matched_1e_cut =
        new TH2D("reco_theta_e_minus_reco_theta_nFD_VS_reco_phi_e_minus_reco_phi_nFD_matched_1e_cut",
                 "#theta_{e}-#theta_{nFD} vs. |#phi_{e}-#phi_{nFD}| in 1e cut (matched);|#phi_{e}-#phi_{nFD}| [#circ];#theta_{e}-#theta_{nFD} [#circ]", 100, 0., Ebeam * 3., 9, 1, 10);
    HistoList.push_back(h_reco_theta_e_minus_reco_theta_nFD_VS_reco_phi_e_minus_reco_phi_nFD_matched_1e_cut);

#pragma endregion

#pragma endregion

    int counter = 0;

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

        //  =======================================================================================================================================================================
        //  1e cut (truth)
        //  =======================================================================================================================================================================

#pragma region /* 1e cut (truth) */
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
#pragma endregion

        //  =======================================================================================================================================================================
        //  1e cut (reco)
        //  =======================================================================================================================================================================

#pragma region /* 1e cut (reco) */

        //  Electron PID cuts -----------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* Electron PID cuts */
        if (electrons[0]->che(clas12::HTCC)->getNphe() <= 2) { continue; }
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
            reco_P_nFD.SetMagThetaPhi(neutrons_FD_clas12[i]->getP(), neutrons_FD_clas12[i]->getTheta(), neutrons_FD_clas12[i]->getPhi());

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
            double Beta_ph = neutrons_FD_redef[i]->par()->getBeta();
            double Gamma_ph = 1 / sqrt(1 - (Beta_ph * Beta_ph));
            double Momentum = m_n * Beta_ph * Gamma_ph;

            TVector3 reco_P_nFD;
            reco_P_nFD.SetMagThetaPhi(neutrons_FD_redef[i]->getP(), neutrons_FD_redef[i]->getTheta(), neutrons_FD_redef[i]->getPhi());

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
            double starttime = c12->event()->getStartTime();
            double ToF_temp = allParticles[i]->getTime() - starttime;

            if ((allParticles[i]->par()->getCharge() == 0) && (allParticles[i]->getRegion() == FD) && (pid_temp != 0)
                //  && !(ToF_temp < 0 || ToF_temp > 40.)
            ) {
                double Momentum = CalcPnFD(allParticles[i]);
                bool PassMomth = (Momentum >= 0.4);
                // If particle is neutral and in the FD
                bool ParticleInPCAL = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7);    // PCAL hit
                bool ParticleInECIN = (allParticles[i]->cal(clas12::ECIN)->getDetector() == 7);    // ECIN hit
                bool ParticleInECOUT = (allParticles[i]->cal(clas12::ECOUT)->getDetector() == 7);  // ECOUT hit

                if ((pid_temp == 2112) || (pid_temp == 22)) {
                    if (ParticleInPCAL) {
                        if (pid_temp == 22) { photons_FD_redef.push_back(allParticles[i]); }
                    } else if (!ParticleInPCAL) {  // if there is a neutron or a 'photon' without a PCAL hit
                        if (ParticleInECIN || ParticleInECOUT) {
                            bool passVeto = NeutronECAL_Cut_Veto(allParticles, electrons, Ebeam, i, 100);

                            if (passVeto && PassMomth) { neutrons_FD_ECALveto.push_back(allParticles[i]); }  // end of clas12root neutron or 'photon' if
                        }
                    }
                }  // end of clas12root neutron or 'photon' if
            }  // end of neutral and in the FD if
        }

        for (int i = 0; i < neutrons_FD_ECALveto.size(); i++) {
            TVector3 reco_P_nFD;
            reco_P_nFD.SetMagThetaPhi(CalcPnFD(neutrons_FD_ECALveto[i]), neutrons_FD_ECALveto[i]->getTheta(), neutrons_FD_ECALveto[i]->getPhi());

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
            h_reco_nFD_multi_ECALveto_1e_cut->Fill(neutrons_FD_ECALveto.size(), weight);
            h_reco_nFD_multi_VS_recp_P_nFD_ECALveto_1e_cut->Fill(reco_P_nFD.Mag(), neutrons_FD_ECALveto.size(), weight);

            h_reco_theta_e_minus_reco_theta_nFD_ECALveto_1e_cut->Fill(reco_P_e.Theta() * 180 / M_PI - reco_P_nFD.Theta() * 180 / M_PI, weight);
            h_reco_phi_e_minus_reco_phi_nFD_ECALveto_1e_cut->Fill(CalcdPhi(reco_P_e.Phi() * 180 / M_PI - reco_P_nFD.Phi() * 180 / M_PI), weight);
            h_reco_theta_e_minus_reco_theta_nFD_VS_reco_phi_e_minus_reco_phi_nFD_ECALveto_1e_cut->Fill(CalcdPhi(reco_P_e.Phi() * 180 / M_PI - reco_P_nFD.Phi() * 180 / M_PI),
                                                                                                       reco_P_e.Theta() * 180 / M_PI - reco_P_nFD.Theta() * 180 / M_PI, weight);
        }

        h_reco_nFD_multi_ECALveto_1e_cut->Fill(neutrons_FD_ECALveto.size(), weight);
#pragma endregion

        //  Setting up FD neutrals (matched) -----------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* Setting up FD neutrals (matched) */
        vector<region_part_ptr> neutrons_FD_matched;

        double tl_Beta;

        for (int i = 0; i < allParticles.size(); i++) {
            int pid_temp = allParticles[i]->par()->getPid();
            double starttime = c12->event()->getStartTime();
            double ToF_temp = allParticles[i]->getTime() - starttime;

            if ((allParticles[i]->par()->getCharge() == 0) && (allParticles[i]->getRegion() == FD) && (pid_temp != 0)
                // && !(ToF_temp < 0 || ToF_temp > 40.)
            ) {                                                                                    // If particle is neutral and in the FD
                bool ParticleInPCAL = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7);    // PCAL hit
                bool ParticleInECIN = (allParticles[i]->cal(clas12::ECIN)->getDetector() == 7);    // ECIN hit
                bool ParticleInECOUT = (allParticles[i]->cal(clas12::ECOUT)->getDetector() == 7);  // ECOUT hit

                if ((pid_temp == 2112) || (pid_temp == 22)) {
                    if (ParticleInPCAL) {
                        if (pid_temp == 22) { photons_FD_redef.push_back(allParticles[i]); }
                    } else if (!ParticleInPCAL) {  // if there is a neutron or a 'photon' without a PCAL hit
                        if (ParticleInECIN || ParticleInECOUT) {
                            double Momentum = CalcPnFD(allParticles[i]);
                            bool PassMomth = (Momentum >= 0.4);

                            bool passVeto = NeutronECAL_Cut_Veto(allParticles, electrons, Ebeam, i, 100);

                            if (passVeto && PassMomth) {
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
            TVector3 reco_P_nFD;
            reco_P_nFD.SetMagThetaPhi(CalcPnFD(neutrons_FD_matched[i]), neutrons_FD_matched[i]->getTheta(), neutrons_FD_matched[i]->getPhi());

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
            h_reco_nFD_multi_matched_1e_cut->Fill(neutrons_FD_matched.size(), weight);
            h_reco_nFD_multi_VS_recp_P_nFD_matched_1e_cut->Fill(reco_P_nFD.Mag(), neutrons_FD_matched.size(), weight);

            h_reco_theta_e_minus_reco_theta_nFD_matched_1e_cut->Fill(reco_P_e.Theta() * 180 / M_PI - reco_P_nFD.Theta() * 180 / M_PI, weight);
            h_reco_phi_e_minus_reco_phi_nFD_matched_1e_cut->Fill(CalcdPhi(reco_P_e.Phi() * 180 / M_PI - reco_P_nFD.Phi() * 180 / M_PI), weight);
            h_reco_theta_e_minus_reco_theta_nFD_VS_reco_phi_e_minus_reco_phi_nFD_matched_1e_cut->Fill(CalcdPhi(reco_P_e.Phi() * 180 / M_PI - reco_P_nFD.Phi() * 180 / M_PI),
                                                                                                      reco_P_e.Theta() * 180 / M_PI - reco_P_nFD.Theta() * 180 / M_PI, weight);
        }

        h_reco_nFD_multi_matched_1e_cut->Fill(neutrons_FD_matched.size(), weight);
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

        // // Set the PDF title and header for the bookmark
        // string Title = HistoList[i]->GetTitle();
        // gStyle->SetTitlePS(Title.c_str());  // This sets the title in metadata
        //                                     // gStyle->SetHeaderPS(("[ /Title " + Title + " /DOCVIEW pdfmark").c_str());  // Adds a PDF title
        // gStyle->SetHeaderPS(("[ /Page " + to_string(i + 1) + " /View [/Fit] /Title (myTitle) ] /OUT pdfmark").c_str());

        if (HistoList[i]->InheritsFrom("TH1D")) {
            HistoList[i]->Draw();
        } else if (HistoList[i]->InheritsFrom("TH2D")) {
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
