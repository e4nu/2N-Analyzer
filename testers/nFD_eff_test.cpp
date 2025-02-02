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
    chain.Add("/lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco/Uniform_e-p-n_samples/4029MeV/OutPut_en/reconhipo/*.hipo");
    chain.SetReaderTags({0});
    chain.db()->turnOffQADB();
    auto config_c12 = chain.GetC12Reader();
    const std::unique_ptr<clas12::clas12reader>& c12 = chain.C12ref();

    /////////////////////////////////////
    // Prepare histograms
    /////////////////////////////////////
    vector<TH1*> HistoList;
    vector<string> HistSubjects;
    vector<bool> FirstPrint;

    gStyle->SetTitleXSize(0.05);
    gStyle->SetTitleYSize(0.05);

    gStyle->SetTitleXOffset(0.8);
    gStyle->SetTitleYOffset(0.8);

    char temp_name[100];
    char temp_title[100];

    TH1D* h_reco_P_e_1e_cut = new TH1D("reco_P_e_1e_cut", "reco P_{e} in 1e cut;P_{e} [GeV/c];Counts", 50, 0, Ebeam * 1.1);
    HistoList.push_back(h_reco_P_e_1e_cut);
    TH1D* h_truth_P_e_1e_cut = new TH1D("truth_P_e_1e_cut", "truth P_{e} in 1e cut;P_{e} [GeV/c];Counts", 50, 0, Ebeam * 1.1);
    HistoList.push_back(h_truth_P_e_1e_cut);
    TH1D* h_reco_theta_e_1e_cut = new TH1D("reco_theta_e_1e_cut", "reco #theta_{e} in 1e cut;#theta_{e} [#circ];Counts", 50, 0, 50.);
    HistoList.push_back(h_reco_theta_e_1e_cut);
    TH1D* h_truth_theta_e_1e_cut = new TH1D("truth_theta_e_1e_cut", "truth #theta_{e} in 1e cut;#theta_{e} [#circ];Counts", 50, 0, 50.);
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
    TH1D* h_reco_theta_n_1e_cut = new TH1D("reco_theta_n_1e_cut", "reco #theta_{n} in 1e cut;#theta_{n} [#circ];Counts", 50, 0, 50.);
    HistoList.push_back(h_reco_theta_n_1e_cut);
    TH1D* h_truth_theta_n_1e_cut = new TH1D("truth_theta_n_1e_cut", "truth #theta_{n} in 1e cut;#theta_{n} [#circ];Counts", 50, 0, 50.);
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

    HistSubjects.push_back("clas12reco");
    FirstPrint.push_back(true);
    TH1D* h_reco_P_nFD_clas12_1e_cut = new TH1D("reco_P_nFD_clas12_1e_cut", "reco P_{nFD} in 1e cut (clas12reco);P_{nFD} [GeV/c];Counts", 50, 0, Ebeam * 1.1);
    HistoList.push_back(h_reco_P_nFD_clas12_1e_cut);
    TH1D* h_truth_P_nFD_clas12_1e_cut = new TH1D("truth_P_nFD_clas12_1e_cut", "truth P_{nFD} in 1e cut (clas12reco);P_{nFD} [GeV/c];Counts", 50, 0, Ebeam * 1.1);
    HistoList.push_back(h_truth_P_nFD_clas12_1e_cut);
    TH1D* h_reco_theta_nFD_clas12_1e_cut = new TH1D("reco_theta_nFD_clas12_1e_cut", "reco #theta_{nFD} in 1e cut (clas12reco);#theta_{nFD} [#circ];Counts", 50, 0, 50.);
    HistoList.push_back(h_reco_theta_nFD_clas12_1e_cut);
    TH1D* h_truth_theta_nFD_clas12_1e_cut = new TH1D("truth_theta_nFD_clas12_1e_cut", "truth #theta_{nFD} in 1e cut (clas12reco);#theta_{nFD} [#circ];Counts", 50, 0, 50.);
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
        "reco_theta_nFD_clas12_VS_P_nFD_clas12_1e_cut", "reco #theta_{nFD} vs. reco P_{nFD} in 1e cut (clas12reco);#theta_{nFD} [circ];P_{nFD} [GeV/c]", 100, 0., 50., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_theta_nFD_clas12_VS_P_nFD_clas12_1e_cut);

    HistSubjects.push_back("redef");
    FirstPrint.push_back(true);
    TH1D* h_reco_P_nFD_redef_1e_cut = new TH1D("reco_P_nFD_redef_1e_cut", "reco P_{nFD} in 1e cut (redef);P_{nFD} [GeV/c];Counts", 50, 0, Ebeam * 1.1);
    HistoList.push_back(h_reco_P_nFD_redef_1e_cut);
    TH1D* h_truth_P_nFD_redef_1e_cut = new TH1D("truth_P_nFD_redef_1e_cut", "truth P_{nFD} in 1e cut (redef);P_{nFD} [GeV/c];Counts", 50, 0, Ebeam * 1.1);
    HistoList.push_back(h_truth_P_nFD_redef_1e_cut);
    TH1D* h_reco_theta_nFD_redef_1e_cut = new TH1D("reco_theta_nFD_redef_1e_cut", "reco #theta_{nFD} in 1e cut (redef);#theta_{nFD} [#circ];Counts", 50, 0, 50.);
    HistoList.push_back(h_reco_theta_nFD_redef_1e_cut);
    TH1D* h_truth_theta_nFD_redef_1e_cut = new TH1D("truth_theta_nFD_redef_1e_cut", "truth #theta_{nFD} in 1e cut (redef);#theta_{nFD} [#circ];Counts", 50, 0, 50.);
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
    TH2D* h_reco_theta_nFD_redef_VS_P_nFD_redef_1e_cut =
        new TH2D("reco_theta_nFD_redef_VS_P_nFD_redef_1e_cut", "reco #theta_{nFD} vs. reco P_{nFD} in 1e cut (redef);#theta_{nFD} [circ];P_{nFD} [GeV/c]", 100, 0., 50., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_theta_nFD_redef_VS_P_nFD_redef_1e_cut);

    HistSubjects.push_back("ECALveto");
    FirstPrint.push_back(true);
    TH1D* h_reco_P_nFD_ECALveto_1e_cut = new TH1D("reco_P_nFD_ECALveto_1e_cut", "reco P_{nFD} in 1e cut (ECALveto);P_{nFD} [GeV/c];Counts", 50, 0, Ebeam * 1.1);
    HistoList.push_back(h_reco_P_nFD_ECALveto_1e_cut);
    TH1D* h_truth_P_nFD_ECALveto_1e_cut = new TH1D("truth_P_nFD_ECALveto_1e_cut", "truth P_{nFD} in 1e cut (ECALveto);P_{nFD} [GeV/c];Counts", 50, 0, Ebeam * 1.1);
    HistoList.push_back(h_truth_P_nFD_ECALveto_1e_cut);
    TH1D* h_reco_theta_nFD_ECALveto_1e_cut = new TH1D("reco_theta_nFD_ECALveto_1e_cut", "reco #theta_{nFD} in 1e cut (ECALveto);#theta_{nFD} [#circ];Counts", 50, 0, 50.);
    HistoList.push_back(h_reco_theta_nFD_ECALveto_1e_cut);
    TH1D* h_truth_theta_nFD_ECALveto_1e_cut = new TH1D("truth_theta_nFD_ECALveto_1e_cut", "truth #theta_{nFD} in 1e cut (ECALveto);#theta_{nFD} [#circ];Counts", 50, 0, 50.);
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
    TH2D* h_reco_theta_nFD_ECALveto_VS_P_nFD_ECALveto_1e_cut = new TH2D(
        "reco_theta_nFD_ECALveto_VS_P_nFD_ECALveto_1e_cut", "reco #theta_{nFD} vs. reco P_{nFD} in 1e cut (ECALveto);#theta_{nFD} [circ];P_{nFD} [GeV/c]", 100, 0., 50., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_theta_nFD_ECALveto_VS_P_nFD_ECALveto_1e_cut);

    HistSubjects.push_back("matched");
    FirstPrint.push_back(true);
    TH1D* h_reco_P_nFD_matched_1e_cut = new TH1D("reco_P_nFD_matched_1e_cut", "reco P_{nFD} in 1e cut (matched);P_{nFD} [GeV/c];Counts", 50, 0, Ebeam * 1.1);
    HistoList.push_back(h_reco_P_nFD_matched_1e_cut);
    TH1D* h_truth_P_nFD_matched_1e_cut = new TH1D("truth_P_nFD_matched_1e_cut", "truth P_{nFD} in 1e cut (matched);P_{nFD} [GeV/c];Counts", 50, 0, Ebeam * 1.1);
    HistoList.push_back(h_truth_P_nFD_matched_1e_cut);
    TH1D* h_reco_theta_nFD_matched_1e_cut = new TH1D("reco_theta_nFD_matched_1e_cut", "reco #theta_{nFD} in 1e cut (matched);#theta_{nFD} [#circ];Counts", 50, 0, 50.);
    HistoList.push_back(h_reco_theta_nFD_matched_1e_cut);
    TH1D* h_truth_theta_nFD_matched_1e_cut = new TH1D("truth_theta_nFD_matched_1e_cut", "truth #theta_{nFD} in 1e cut (matched);#theta_{nFD} [#circ];Counts", 50, 0, 50.);
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
    TH2D* h_reco_theta_nFD_matched_VS_P_nFD_matched_1e_cut = new TH2D(
        "reco_theta_nFD_matched_VS_P_nFD_matched_1e_cut", "reco #theta_{nFD} vs. reco P_{nFD} in 1e cut (matched);#theta_{nFD} [circ];P_{nFD} [GeV/c]", 100, 0., 50., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_theta_nFD_matched_VS_P_nFD_matched_1e_cut);

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

        auto c12 = chain.GetC12Reader();
        auto mceve = c12->mcevent();
        auto mcpbank = c12->mcparts();
        const Int_t Ngen = mcpbank->getRows();

        vector<int> truth_NeutronsFD;

        for (Int_t i = 0; i < Ngen; i++) {
            mcpbank->setEntry(i);

            auto pid_temp = mcpbank->getPid();

            auto px = mcpbank->getPx();
            auto py = mcpbank->getPy();
            auto pz = mcpbank->getPz();

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

                    h_truth_P_nFD_ECALveto_1e_cut->Fill(truth_P_n.Mag(), weight);
                    h_truth_theta_nFD_ECALveto_1e_cut->Fill(truth_P_n.Theta() * 180 / M_PI, weight);
                    h_truth_phi_nFD_ECALveto_1e_cut->Fill(truth_P_n.Phi() * 180 / M_PI, weight);
                    h_truth_theta_nFD_ECALveto_VS_truth_phi_nFD_ECALveto_1e_cut->Fill(truth_P_n.Phi() * 180 / M_PI, truth_P_n.Theta() * 180 / M_PI, weight);
                }
            }
        }

        //  =======================================================================================================================================================================
        //  1e cut (reco)
        //  =======================================================================================================================================================================

        //  Electron PID cuts -----------------------------------------------------------------------------------------------------------------------------------------------------

        if (electrons[0]->che(clas12::HTCC)->getNphe() <= 2) { continue; }
        if (EoP_e < 0.2 || EoP_e > 0.28) { continue; }
        if (electrons[0]->cal(clas12::PCAL)->getLv() < 14. || electrons[0]->cal(clas12::PCAL)->getLw() < 14.) { continue; }
        if (electrons[0]->par()->getVz() < -6. || electrons[0]->par()->getVz() > 0.) { continue; }

        h_reco_P_e_1e_cut->Fill(reco_P_e.Mag(), weight);
        h_reco_theta_e_1e_cut->Fill(reco_P_e.Theta() * 180 / M_PI, weight);
        h_reco_phi_e_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, weight);
        h_reco_theta_e_VS_reco_phi_e_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, reco_P_e.Theta() * 180 / M_PI, weight);

        //  Setting up neutrals ---------------------------------------------------------------------------------------------------------------------------------------------------

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

        //  Setting up FD neutrals (clas12reco) -----------------------------------------------------------------------------------------------------------------------------------

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

        //  Setting up FD neutrals (redef) ----------------------------------------------------------------------------------------------------------------------------------------

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

        //  Setting up FD neutrals (ECALveto) ----------------------------------------------------------------------------------------------------------------------------------------

        vector<region_part_ptr> neutrons_FD_ECALveto;

        for (int i = 0; i < allParticles.size(); i++) {
            int pid_temp = allParticles[i]->par()->getPid();
            double starttime = c12->event()->getStartTime();
            double ToF_temp = allParticles[i]->getTime() - starttime;

            if ((allParticles[i]->par()->getCharge() == 0) && (allParticles[i]->getRegion() == FD) && (pid_temp != 0)
                //  && !(ToF_temp < 0 || ToF_temp > 40.)
            ) {                                                                                    // If particle is neutral and in the FD
                bool ParticleInPCAL = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7);    // PCAL hit
                bool ParticleInECIN = (allParticles[i]->cal(clas12::ECIN)->getDetector() == 7);    // ECIN hit
                bool ParticleInECOUT = (allParticles[i]->cal(clas12::ECOUT)->getDetector() == 7);  // ECOUT hit

                if ((pid_temp == 2112) || (pid_temp == 22)) {
                    if (ParticleInPCAL) {
                        if (pid_temp == 22) { photons_FD_redef.push_back(allParticles[i]); }
                    } else if (!ParticleInPCAL) {  // if there is a neutron or a 'photon' without a PCAL hit
                        if (ParticleInECIN || ParticleInECOUT) {
                            bool passVeto = NeutronECAL_Cut_Veto(allParticles, electrons, Ebeam, i, 100);

                            if (passVeto) { neutrons_FD_ECALveto.push_back(allParticles[i]); }  // end of clas12root neutron or 'photon' if
                        }
                    }
                }  // end of clas12root neutron or 'photon' if
            }  // end of neutral and in the FD if
        }

        for (int i = 0; i < neutrons_FD_ECALveto.size(); i++) {
            double Beta_ph = neutrons_FD_ECALveto[i]->par()->getBeta();
            double Gamma_ph = 1 / sqrt(1 - (Beta_ph * Beta_ph));
            double Momentum = m_n * Beta_ph * Gamma_ph;

            TVector3 reco_P_nFD;
            reco_P_nFD.SetMagThetaPhi(Momentum, neutrons_FD_ECALveto[i]->getTheta(), neutrons_FD_ECALveto[i]->getPhi());

            h_reco_P_nFD_ECALveto_1e_cut->Fill(reco_P_nFD.Mag(), weight);
            h_reco_theta_nFD_ECALveto_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI, weight);
            h_reco_phi_nFD_ECALveto_1e_cut->Fill(reco_P_nFD.Phi() * 180 / M_PI, weight);
            h_reco_theta_nFD_ECALveto_VS_reco_phi_nFD_ECALveto_1e_cut->Fill(reco_P_nFD.Phi() * 180 / M_PI, reco_P_nFD.Theta() * 180 / M_PI, weight);
            h_reco_P_e_VS_P_nFD_ECALveto_1e_cut->Fill(reco_P_e.Mag(), reco_P_nFD.Mag(), weight);
            h_reco_theta_nFD_ECALveto_VS_P_nFD_ECALveto_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI, reco_P_nFD.Mag(), weight);
        }

        //  Setting up FD neutrals (matched) -----------------------------------------------------------------------------------------------------------------------------------------

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
                            bool passVeto = NeutronECAL_Cut_Veto(allParticles, electrons, Ebeam, i, 100);

                            if (passVeto) {
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
            // bool ParticleInPCAL = (neutrons_FD_matched[i]->cal(clas12::PCAL)->getDetector() == 7);    // PCAL hit
            // bool ParticleInECIN = (neutrons_FD_matched[i]->cal(clas12::ECIN)->getDetector() == 7);    // ECIN hit
            // bool ParticleInECOUT = (neutrons_FD_matched[i]->cal(clas12::ECOUT)->getDetector() == 7);  // ECOUT hit

            // // double Beta_ph = tl_Beta;
            // double Path_ph = neutrons_FD_matched[i]->getPath();
            // double starttime = c12->event()->getStartTime();
            // double ToF;

            // if (ParticleInECIN) {
            //     ToF = neutrons_FD_matched[i]->cal(clas12::ECIN)->getTime() - starttime;
            // } else if (ParticleInECOUT) {
            //     ToF = neutrons_FD_matched[i]->cal(clas12::ECOUT)->getTime() - starttime;
            // }

            // double Beta_ph = Path_ph/(ToF*c);
            double Beta_ph = neutrons_FD_matched[i]->par()->getBeta();
            double Gamma_ph = 1 / sqrt(1 - (Beta_ph * Beta_ph));
            double Momentum = m_n * Beta_ph * Gamma_ph;

            // cout << "\n\n\n";
            // cout << "Path_ph = " << Path_ph << "\n";
            // cout << "ToF = " << ToF << "\n";
            // cout << "neutrons_FD_matched[i]->cal(clas12::ECIN)->getTime() = " << neutrons_FD_matched[i]->cal(clas12::ECIN)->getTime() << "\n";
            // cout << "neutrons_FD_matched[i]->cal(clas12::ECOUT)->getTime() = " << neutrons_FD_matched[i]->cal(clas12::ECOUT)->getTime() << "\n";
            // cout << "\n\n\n";

            TVector3 reco_P_nFD;
            reco_P_nFD.SetMagThetaPhi(Momentum, neutrons_FD_matched[i]->getTheta(), neutrons_FD_matched[i]->getPhi());

            h_reco_P_nFD_matched_1e_cut->Fill(reco_P_nFD.Mag(), weight);
            h_reco_theta_nFD_matched_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI, weight);
            h_reco_phi_nFD_matched_1e_cut->Fill(reco_P_nFD.Phi() * 180 / M_PI, weight);
            h_reco_theta_nFD_matched_VS_reco_phi_nFD_matched_1e_cut->Fill(reco_P_nFD.Phi() * 180 / M_PI, reco_P_nFD.Theta() * 180 / M_PI, weight);
            h_reco_P_e_VS_P_nFD_matched_1e_cut->Fill(reco_P_e.Mag(), reco_P_nFD.Mag(), weight);
            h_reco_theta_nFD_matched_VS_P_nFD_matched_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI, reco_P_nFD.Mag(), weight);
        }
    }

    cout << counter << endl;

    for (int i = 0; i < HistoList.size(); i++) {
        if (HistoList[i]->InheritsFrom("TH1D")) { HistoList[i]->Sumw2(); }

        HistoList[i]->Sumw2();
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
    text.DrawLatex(0.2, 0.9, "(e,e'p) Cuts:");
    text.DrawLatex(0.2, 0.8, "(e,e') Cuts");
    text.DrawLatex(0.2, 0.7, "Neutrons in CND");
    myText->Print(fileName, "pdf");
    myText->Clear();

    for (int i = 0; i < HistoList.size(); i++) {
        for (int j = 0; j < HistSubjects.size(); j++) {
            if (FirstPrint.at(j) && findSubstring(HistoList[i]->GetTitle(), HistSubjects.at(j))) {
                myText->cd();
                titles.DrawLatex(0.3, 0.5, HistSubjects.at(j).c_str());
                myText->Print(fileName, "pdf");
                myText->Clear();

                // Set the PDF title and header for the bookmark
                gStyle->SetTitlePS(HistSubjects.at(j).c_str());                                         // This sets the title in metadata
                gStyle->SetHeaderPS(("[ /Title " + HistSubjects.at(j) + " /DOCVIEW pdfmark").c_str());  // Adds a PDF title

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

        // Set the PDF title and header for the bookmark
        string Title = HistoList[i]->GetTitle();
        gStyle->SetTitlePS(Title.c_str());                                         // This sets the title in metadata
        gStyle->SetHeaderPS(("[ /Title " + Title + " /DOCVIEW pdfmark").c_str());  // Adds a PDF title

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
