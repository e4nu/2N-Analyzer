//
// Obtained from Ron Wagner on 06/11/2025.
// Edited by Alon Sportes.
//

#include <TCanvas.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TLorentzVector.h>
#include <TTree.h>
#include <TVector3.h>

#include <cmath>
#include <iostream>
#include <vector>

#include "ChargedHit.h"
#include "NeutronHitStruct.cpp"
#include "NeutronSelection.cpp"

const double mass_neutron = 0.939565;
const double clight = 29.9792458;

void tree_maker_mc_isotropic_neutrons_after_veto(int runNum) {
    // Step 1: Create a TChain and load files
    TChain chain("clas");  // Replace "myTree" with your tree name
    // chain.Add("/lustre24/expphy/volatile/hallc/comm2017/rwagner/clas12/root_data/src/D_latest/tree_nop_*"); // Add your ROOT files
    // TString filePath = Form("/Users/ronwagner/Desktop/PhD/clas12/DATA/root_trees/40Ca/mf/before_neutron_veto/tree_nop_*");
    TString filePath = Form("/lustre24/expphy/volatile/hallc/comm2017/rwagner/clas12/root_data/simulations/isotropic_neutrons/before_n_veto_test/tree_nop_%d_vec.root", runNum);
    chain.Add(filePath.Data());
    // chain.Add("/lustre24/expphy/volatile/hallc/comm2017/rwagner/clas12/root_data/src/D_for_eff/tree_nop_%d_vec.root",runNum); // Add your ROOT files
    // chain.Add("file2.root");

    // Step 2: Create a new ROOT file
    // TString outputFileName = Form("/Users/ronwagner/Desktop/PhD/clas12/DATA/root_trees/40Ca/mf/after_neutron_veto/neutron_veto_all.root");
    TString outputFileName = Form("/lustre24/expphy/volatile/hallc/comm2017/rwagner/clas12/root_data/simulations/isotropic_neutrons/after_veto_new/neutron_veto_%d.root", runNum);
    TFile outFile(outputFileName.Data(), "RECREATE");
    // TFile outFile("/lustre24/expphy/volatile/hallc/comm2017/rwagner/clas12/root_data/src/D_latest/output_tree_with_cuts_latest2.root", "RECREATE");
    // TFile outFile("/lustre24/expphy/volatile/hallc/comm2017/rwagner/clas12/root_data/src/D_for_eff_after_veto/tree_after_veto_%d_vec.root", "RECREATE");
    //  Step 3: Clone the TChain to a TTree
    TTree* tree = chain.CloneTree(0);  // Create an empty TTree with the same structure

    // Define variables to hold the values from the branches (adjust with your actual branches)
    TLorentzVector* q;
    std::vector<std::vector<double> >* Pp;

    Int_t p_no;
    Int_t p_no_cd;
    Int_t p_no_fd;

    q = nullptr;
    Pp = nullptr;

    // Define vectors to hold the branch data
    std::vector<int>*charge = nullptr, *particle_ID = nullptr;

    std::vector<bool>*FTOF1A_hit = nullptr, *FTOF1B_hit = nullptr, *FTOF2_hit = nullptr;
    std::vector<int>*FTOF1A_sector = nullptr, *FTOF1B_sector = nullptr, *FTOF2_sector = nullptr;
    std::vector<double>*FTOF1A_tof = nullptr, *FTOF1B_tof = nullptr, *FTOF2_tof = nullptr;
    std::vector<bool>*FTOF1A_path = nullptr, *FTOF1B_path = nullptr, *FTOF2_path = nullptr;
    std::vector<double>*FTOF1A_x = nullptr, *FTOF1B_x = nullptr, *FTOF2_x = nullptr;
    std::vector<double>*FTOF1A_y = nullptr, *FTOF1B_y = nullptr, *FTOF2_y = nullptr;
    std::vector<double>*FTOF1A_z = nullptr, *FTOF1B_z = nullptr, *FTOF2_z = nullptr;

    std::vector<bool>*pcal_hit = nullptr, *ecin_hit = nullptr, *ecout_hit = nullptr;
    std::vector<int>*pcal_sector = nullptr, *ecin_sector = nullptr, *ecout_sector = nullptr;
    std::vector<double>*pcal_tof = nullptr, *ecin_tof = nullptr, *ecout_tof = nullptr;
    std::vector<double>*pcal_path = nullptr, *ecin_path = nullptr, *ecout_path = nullptr;
    std::vector<double>*pcal_dE = nullptr, *ecin_dE = nullptr, *ecout_dE = nullptr;
    std::vector<double>*pcal_x = nullptr, *ecin_x = nullptr, *ecout_x = nullptr;
    std::vector<double>*pcal_y = nullptr, *ecin_y = nullptr, *ecout_y = nullptr;
    std::vector<double>*pcal_z = nullptr, *ecin_z = nullptr, *ecout_z = nullptr;

    std::vector<double>*P_x = nullptr, *P_y = nullptr, *P_z = nullptr;
    TVector3* vtx_e = nullptr;

    //--------------Set branch addresses---------------------------
    chain.SetBranchAddress("charge", &charge);
    chain.SetBranchAddress("particle_ID", &particle_ID);

    // FTOF
    chain.SetBranchAddress("FTOF1A_hit", &FTOF1A_hit);
    chain.SetBranchAddress("FTOF1B_hit", &FTOF1B_hit);
    chain.SetBranchAddress("FTOF2_hit", &FTOF2_hit);
    chain.SetBranchAddress("FTOF1A_sector", &FTOF1A_sector);
    chain.SetBranchAddress("FTOF1B_sector", &FTOF1B_sector);
    chain.SetBranchAddress("FTOF2_sector", &FTOF2_sector);
    chain.SetBranchAddress("FTOF1A_tof", &FTOF1A_tof);
    chain.SetBranchAddress("FTOF1B_tof", &FTOF1B_tof);
    chain.SetBranchAddress("FTOF2_tof", &FTOF2_tof);
    chain.SetBranchAddress("FTOF1A_path", &FTOF1A_path);
    chain.SetBranchAddress("FTOF1B_path", &FTOF1B_path);
    chain.SetBranchAddress("FTOF2_path", &FTOF2_path);
    chain.SetBranchAddress("FTOF1A_x", &FTOF1A_x);
    chain.SetBranchAddress("FTOF1B_x", &FTOF1B_x);
    chain.SetBranchAddress("FTOF2_x", &FTOF2_x);
    chain.SetBranchAddress("FTOF1A_y", &FTOF1A_y);
    chain.SetBranchAddress("FTOF1B_y", &FTOF1B_y);
    chain.SetBranchAddress("FTOF2_y", &FTOF2_y);
    chain.SetBranchAddress("FTOF1A_z", &FTOF1A_z);
    chain.SetBranchAddress("FTOF1B_z", &FTOF1B_z);
    chain.SetBranchAddress("FTOF2_z", &FTOF2_z);

    // ECAL
    chain.SetBranchAddress("pcal_hit", &pcal_hit);
    chain.SetBranchAddress("ecin_hit", &ecin_hit);
    chain.SetBranchAddress("ecout_hit", &ecout_hit);
    chain.SetBranchAddress("pcal_sector", &pcal_sector);
    chain.SetBranchAddress("ecin_sector", &ecin_sector);
    chain.SetBranchAddress("ecout_sector", &ecout_sector);
    chain.SetBranchAddress("pcal_tof", &pcal_tof);
    chain.SetBranchAddress("ecin_tof", &ecin_tof);
    chain.SetBranchAddress("ecout_tof", &ecout_tof);
    chain.SetBranchAddress("pcal_path", &pcal_path);
    chain.SetBranchAddress("ecin_path", &ecin_path);
    chain.SetBranchAddress("ecout_path", &ecout_path);
    chain.SetBranchAddress("pcal_dE", &pcal_dE);
    chain.SetBranchAddress("ecin_dE", &ecin_dE);
    chain.SetBranchAddress("ecout_dE", &ecout_dE);
    chain.SetBranchAddress("pcal_x", &pcal_x);
    chain.SetBranchAddress("ecin_x", &ecin_x);
    chain.SetBranchAddress("ecout_x", &ecout_x);
    chain.SetBranchAddress("pcal_y", &pcal_y);
    chain.SetBranchAddress("ecin_y", &ecin_y);
    chain.SetBranchAddress("ecout_y", &ecout_y);
    chain.SetBranchAddress("pcal_z", &pcal_z);
    chain.SetBranchAddress("ecin_z", &ecin_z);
    chain.SetBranchAddress("ecout_z", &ecout_z);

    chain.SetBranchAddress("P_x", &P_x);
    chain.SetBranchAddress("P_y", &P_y);
    chain.SetBranchAddress("P_z", &P_z);

    chain.SetBranchAddress("q", &q);
    chain.SetBranchAddress("vtx_e", &vtx_e);

    // proton branches
    chain.SetBranchAddress("Pp", &Pp);
    chain.SetBranchAddress("p_no", &p_no);
    chain.SetBranchAddress("p_no_fd", &p_no_fd);
    chain.SetBranchAddress("p_no_cd", &p_no_cd);

    //-----------------------------------------------------------------

    // Veto branches

    // Step 4: Create a new TLorentzVector branch

    // after veto
    int after_veto_num_n;
    std::vector<double> Pn_calc_x, Pn_calc_y, Pn_calc_z, Pn_calc_E, Pn_calc_theta, Pn_calc_phi, Pn_calc_vect_mag;
    std::vector<double> Pn_corrected_x, Pn_corrected_y, Pn_corrected_z, Pn_corrected_E, Pn_corrected_theta, Pn_corrected_phi, Pn_corrected_vect_mag;
    std::vector<double> Pn_clas_x, Pn_clas_y, Pn_clas_z, Pn_clas_E, Pn_clas_theta, Pn_clas_phi, Pn_clas_vect_mag;
    std::vector<double> after_veto_tof_n, after_veto_beta_n, after_veto_path_n;
    std::vector<double> after_veto_beta_corrected_n, after_veto_path_corrected_n;
    std::vector<int> after_veto_pindex;
    std::vector<int> after_veto_detector, after_veto_layer;
    std::vector<int> after_veto_sector;
    std::vector<double> after_veto_dE;
    std::vector<double> after_veto_hit_x, after_veto_hit_y, after_veto_hit_z;

    std::vector<double> nq_clas, nq_calc, theta_nq_clas, theta_nq_calc;
    std::vector<double> nq_corrected, theta_nq_corrected;

    // after Veto
    tree->Branch("after_veto_num_n", &after_veto_num_n);
    tree->Branch("Pn_calc_x", &Pn_calc_x);
    tree->Branch("Pn_calc_y", &Pn_calc_y);
    tree->Branch("Pn_calc_z", &Pn_calc_z);
    tree->Branch("Pn_calc_E", &Pn_calc_E);
    tree->Branch("Pn_calc_theta", &Pn_calc_theta);
    tree->Branch("Pn_calc_phi", &Pn_calc_phi);
    tree->Branch("Pn_calc_vect_mag", &Pn_calc_vect_mag);

    tree->Branch("Pn_clas_x", &Pn_clas_x);
    tree->Branch("Pn_clas_y", &Pn_clas_y);
    tree->Branch("Pn_clas_z", &Pn_clas_z);
    tree->Branch("Pn_clas_E", &Pn_clas_E);
    tree->Branch("Pn_clas_theta", &Pn_clas_theta);
    tree->Branch("Pn_clas_phi", &Pn_clas_phi);
    tree->Branch("Pn_clas_vect_mag", &Pn_clas_vect_mag);

    tree->Branch("Pn_corrected_x", &Pn_corrected_x);
    tree->Branch("Pn_corrected_y", &Pn_corrected_y);
    tree->Branch("Pn_corrected_z", &Pn_corrected_z);
    tree->Branch("Pn_corrected_E", &Pn_corrected_E);
    tree->Branch("Pn_corrected_theta", &Pn_corrected_theta);
    tree->Branch("Pn_corrected_phi", &Pn_corrected_phi);
    tree->Branch("Pn_corrected_vect_mag", &Pn_corrected_vect_mag);

    tree->Branch("after_veto_tof_n", &after_veto_tof_n);
    tree->Branch("after_veto_beta_n", &after_veto_beta_n);
    tree->Branch("after_veto_path_n", &after_veto_path_n);
    tree->Branch("after_veto_pindex", &after_veto_pindex);
    tree->Branch("after_veto_detector", &after_veto_detector);
    tree->Branch("after_veto_layer", &after_veto_layer);
    tree->Branch("after_veto_sector", &after_veto_sector);
    tree->Branch("after_veto_dE", &after_veto_dE);
    tree->Branch("after_veto_hit_x", &after_veto_hit_x);
    tree->Branch("after_veto_hit_y", &after_veto_hit_y);
    tree->Branch("after_veto_hit_z", &after_veto_hit_z);

    tree->Branch("after_veto_beta_corrected_n", &after_veto_beta_corrected_n);
    tree->Branch("after_veto_path_corrected_n", &after_veto_path_corrected_n);

    tree->Branch("nq_corrected", &nq_corrected);
    tree->Branch("theta_nq_corrected", &theta_nq_corrected);

    tree->Branch("nq_calc", &nq_calc);
    tree->Branch("nq_clas", &nq_clas);
    tree->Branch("theta_nq_calc", &theta_nq_calc);
    tree->Branch("theta_nq_clas", &theta_nq_clas);

    // Step 5: Loop over entries and fill the tree
    TLorentzVector Pd;          // deouterium 4-momentum
    double md = 1.87561294257;  // GeV
    Pd.SetPxPyPzE(0, 0, 0, md);
    Long64_t nentries = chain.GetEntries();

    // Loop over each event
    int total_neutral_num = 0;
    // initializing values
    std::vector<NeutronHit*> neutralHits;
    std::vector<ChargedHit> chargeHits;
    cout << "chain num of entries:  " << chain.GetEntries() << endl;
    for (Long64_t i = 0; i < nentries; i++) {
        chain.GetEntry(i);
        if (i % 100000 == 0) cout << "Event  " << i << " / " << nentries << endl;

        after_veto_num_n = 0;

        // after_veto
        Pn_calc_x.clear();
        Pn_calc_y.clear();
        Pn_calc_z.clear();
        Pn_calc_E.clear();
        Pn_calc_theta.clear();
        Pn_calc_phi.clear();
        Pn_calc_vect_mag.clear();

        Pn_corrected_x.clear();
        Pn_corrected_y.clear();
        Pn_corrected_z.clear();
        Pn_corrected_E.clear();
        Pn_corrected_theta.clear();
        Pn_corrected_phi.clear();
        Pn_corrected_vect_mag.clear();

        Pn_clas_x.clear();
        Pn_clas_y.clear();
        Pn_clas_z.clear();
        Pn_clas_E.clear();
        Pn_clas_theta.clear();
        Pn_clas_phi.clear();
        Pn_clas_vect_mag.clear();

        after_veto_tof_n.clear();
        after_veto_beta_n.clear();
        after_veto_path_n.clear();
        after_veto_pindex.clear();

        after_veto_beta_corrected_n.clear();
        after_veto_path_corrected_n.clear();

        after_veto_detector.clear();
        after_veto_layer.clear();
        after_veto_sector.clear();
        after_veto_dE.clear();
        after_veto_hit_x.clear();
        after_veto_hit_y.clear();
        after_veto_hit_z.clear();

        nq_corrected.clear();
        theta_nq_corrected.clear();

        nq_calc.clear();
        nq_clas.clear();
        theta_nq_calc.clear();
        theta_nq_clas.clear();
        //------------------------------------------------

        // std::cout << "Event " << i << ":\n";

        size_t vecSize = charge->size();  // Get the size of any vector (since they are the same length)
        int nHitnum = 0;
        int cHitnum = 0;
        NeutronHit* nHit;
        ChargedHit cHit;

        // init
        //  Delete each allocated NeutronHit object
        for (auto hit : neutralHits) { delete hit; }
        neutralHits.clear();
        chargeHits.clear();
        for (size_t j = 0; j < vecSize; j++) {
            // std::cout << "Particle: " << j << "\n";
            if (FTOF1B_hit->at(j)) {
                // std::cout << "charge: " << charge->at(j) << "\t detector: "<< 12 << "\t layer: " << 2 << "\n";
                if (charge->at(j) == 0) {
                    nHit = new NeutronHit();
                    nHit->reset();

                    nHit->hitIndex = nHitnum;
                    nHit->pindex = j;
                    nHit->detector = 12;
                    nHit->layer = 2;
                    nHit->sector = FTOF1B_sector->at(j);
                    nHit->x = FTOF1B_x->at(j);
                    nHit->y = FTOF1B_y->at(j);
                    nHit->z = FTOF1B_z->at(j);
                    nHit->Px = P_x->at(j);
                    nHit->Py = P_y->at(j);
                    nHit->Pz = P_z->at(j);
                    nHit->path = FTOF1B_path->at(j);
                    nHit->tof = FTOF1B_tof->at(j);

                    neutralHits.push_back(nHit);
                    nHitnum++;
                } else {
                    cHit.reset();

                    cHit.hitIndex = cHitnum;
                    cHit.pindex = j;
                    cHit.charge = charge->at(j);
                    cHit.PID = particle_ID->at(j);
                    cHit.detector = 12;
                    cHit.layer = 2;
                    cHit.sector = FTOF1B_sector->at(j);
                    cHit.x = FTOF1B_x->at(j);
                    cHit.y = FTOF1B_y->at(j);
                    cHit.z = FTOF1B_z->at(j);
                    cHit.path = FTOF1B_path->at(j);
                    cHit.tof = FTOF1B_tof->at(j);

                    chargeHits.push_back(cHit);
                    cHitnum++;
                }
            }
            if (FTOF1A_hit->at(j)) {
                // std::cout << "charge: " << charge->at(j) << "\t detector: "<< 12 << "\t layer: " << 1 << "\n";
                if (charge->at(j) == 0) {
                    nHit = new NeutronHit();
                    nHit->reset();

                    nHit->hitIndex = nHitnum;
                    nHit->pindex = j;
                    nHit->detector = 12;
                    nHit->layer = 1;
                    nHit->sector = FTOF1A_sector->at(j);
                    nHit->x = FTOF1A_x->at(j);
                    nHit->y = FTOF1A_y->at(j);
                    nHit->z = FTOF1A_z->at(j);
                    nHit->Px = P_x->at(j);
                    nHit->Py = P_y->at(j);
                    nHit->Pz = P_z->at(j);
                    nHit->path = FTOF1A_path->at(j);
                    nHit->tof = FTOF1A_tof->at(j);

                    neutralHits.push_back(nHit);
                    nHitnum++;
                } else {
                    cHit.reset();

                    cHit.hitIndex = cHitnum;
                    cHit.pindex = j;
                    cHit.charge = charge->at(j);
                    cHit.PID = particle_ID->at(j);
                    cHit.detector = 12;
                    cHit.layer = 1;
                    cHit.sector = FTOF1A_sector->at(j);
                    cHit.x = FTOF1A_x->at(j);
                    cHit.y = FTOF1A_y->at(j);
                    cHit.z = FTOF1A_z->at(j);
                    cHit.path = FTOF1A_path->at(j);
                    cHit.tof = FTOF1A_tof->at(j);

                    chargeHits.push_back(cHit);
                    cHitnum++;
                }
            }
            if (FTOF2_hit->at(j)) {
                // std::cout << "charge: " << charge->at(j) << "\t detector: "<< 12 << "\t layer: " << 3 << "\n";
                if (charge->at(j) == 0) {
                    nHit = new NeutronHit();
                    nHit->reset();

                    nHit->hitIndex = nHitnum;
                    nHit->pindex = j;
                    nHit->detector = 12;
                    nHit->layer = 3;
                    nHit->sector = FTOF2_sector->at(j);
                    nHit->x = FTOF2_x->at(j);
                    nHit->y = FTOF2_y->at(j);
                    nHit->z = FTOF2_z->at(j);
                    nHit->Px = P_x->at(j);
                    nHit->Py = P_y->at(j);
                    nHit->Pz = P_z->at(j);
                    nHit->path = FTOF2_path->at(j);
                    nHit->tof = FTOF2_tof->at(j);

                    neutralHits.push_back(nHit);
                    nHitnum++;
                } else {
                    cHit.reset();

                    cHit.hitIndex = cHitnum;
                    cHit.pindex = j;
                    cHit.charge = charge->at(j);
                    cHit.PID = particle_ID->at(j);
                    cHit.detector = 12;
                    cHit.layer = 3;
                    cHit.sector = FTOF2_sector->at(j);
                    cHit.x = FTOF2_x->at(j);
                    cHit.y = FTOF2_y->at(j);
                    cHit.z = FTOF2_z->at(j);
                    cHit.path = FTOF2_path->at(j);
                    cHit.tof = FTOF2_tof->at(j);

                    chargeHits.push_back(cHit);
                    cHitnum++;
                }
            }
            if (pcal_hit->at(j)) {
                // std::cout << "charge: " << charge->at(j) << "\t detector: "<< 7 << "\t layer: " << 1 << "\n";
                if (charge->at(j) == 0) {
                    nHit = new NeutronHit();
                    nHit->reset();

                    nHit->hitIndex = nHitnum;
                    nHit->pindex = j;
                    nHit->detector = 7;
                    nHit->layer = 1;
                    nHit->sector = pcal_sector->at(j);
                    nHit->x = pcal_x->at(j);
                    nHit->y = pcal_y->at(j);
                    nHit->z = pcal_z->at(j);
                    nHit->dE = pcal_dE->at(j);
                    nHit->Px = P_x->at(j);
                    nHit->Py = P_y->at(j);
                    nHit->Pz = P_z->at(j);
                    nHit->path = pcal_path->at(j);
                    nHit->tof = pcal_tof->at(j);

                    neutralHits.push_back(nHit);
                    nHitnum++;
                } else {
                    cHit.reset();

                    cHit.hitIndex = cHitnum;
                    cHit.pindex = j;
                    cHit.charge = charge->at(j);
                    cHit.PID = particle_ID->at(j);
                    cHit.detector = 7;
                    cHit.layer = 1;
                    cHit.sector = pcal_sector->at(j);
                    cHit.x = pcal_x->at(j);
                    cHit.y = pcal_y->at(j);
                    cHit.z = pcal_z->at(j);
                    cHit.path = pcal_path->at(j);
                    cHit.tof = pcal_tof->at(j);

                    chargeHits.push_back(cHit);
                    cHitnum++;
                }
            }
            if (ecin_hit->at(j)) {
                // std::cout << "charge: " << charge->at(j) << "\t detector: "<< 7 << "\t layer: " << 2 << "\n";
                if (charge->at(j) == 0) {
                    nHit = new NeutronHit();
                    nHit->reset();

                    nHit->hitIndex = nHitnum;
                    nHit->pindex = j;
                    nHit->detector = 7;
                    nHit->layer = 2;
                    nHit->sector = ecin_sector->at(j);
                    nHit->x = ecin_x->at(j);
                    nHit->y = ecin_y->at(j);
                    nHit->z = ecin_z->at(j);
                    nHit->dE = ecin_dE->at(j);
                    nHit->Px = P_x->at(j);
                    nHit->Py = P_y->at(j);
                    nHit->Pz = P_z->at(j);
                    nHit->path = ecin_path->at(j);
                    nHit->tof = ecin_tof->at(j);

                    neutralHits.push_back(nHit);
                    nHitnum++;
                } else {
                    cHit.reset();

                    cHit.hitIndex = cHitnum;
                    cHit.pindex = j;
                    cHit.charge = charge->at(j);
                    cHit.PID = particle_ID->at(j);
                    cHit.detector = 7;
                    cHit.layer = 2;
                    cHit.sector = ecin_sector->at(j);
                    cHit.x = ecin_x->at(j);
                    cHit.y = ecin_y->at(j);
                    cHit.z = ecin_z->at(j);
                    cHit.path = ecin_path->at(j);
                    cHit.tof = ecin_tof->at(j);

                    chargeHits.push_back(cHit);
                    cHitnum++;
                }
            }
            if (ecout_hit->at(j)) {
                // std::cout << "charge: " << charge->at(j) << "\t detector: "<< 7 << "\t layer: " << 3 << "\n";
                if (charge->at(j) == 0) {
                    nHit = new NeutronHit();
                    nHit->reset();

                    nHit->hitIndex = nHitnum;
                    nHit->pindex = j;
                    nHit->detector = 7;
                    nHit->layer = 3;
                    nHit->sector = ecout_sector->at(j);
                    nHit->x = ecout_x->at(j);
                    nHit->y = ecout_y->at(j);
                    nHit->z = ecout_z->at(j);
                    nHit->dE = ecout_dE->at(j);
                    nHit->Px = P_x->at(j);
                    nHit->Py = P_y->at(j);
                    nHit->Pz = P_z->at(j);
                    nHit->path = ecout_path->at(j);
                    nHit->tof = ecout_tof->at(j);

                    neutralHits.push_back(nHit);
                    nHitnum++;
                } else {
                    cHit.reset();

                    cHit.hitIndex = cHitnum;
                    cHit.pindex = j;
                    cHit.charge = charge->at(j);
                    cHit.PID = particle_ID->at(j);
                    cHit.detector = 7;
                    cHit.layer = 3;
                    cHit.sector = ecout_sector->at(j);
                    cHit.x = ecout_x->at(j);
                    cHit.y = ecout_y->at(j);
                    cHit.z = ecout_z->at(j);
                    cHit.path = ecout_path->at(j);
                    cHit.tof = ecout_tof->at(j);

                    chargeHits.push_back(cHit);
                    cHitnum++;
                }
            }
        }

        std::vector<NeutronHit*> after_veto = neutSelection(neutralHits, chargeHits);
        // cout << after_veto.size() << endl;

        // FINISHED ALL THE SET UP NEED TO FILL ALL THE NEW BRANCHES AFTER THE VETO

        after_veto_num_n = after_veto.size();
        for (int j = 0; j < after_veto.size(); j++) {
            // DONE - std::vector<double> Pn_calc_x, Pn_calc_y, Pn_calc_z, Pn_calc_E, Pn_calc_theta, Pn_calc_phi, Pn_calc_vect_mag;
            // DONE - std::vector<double> Pn_corrected_x, Pn_corrected_y, Pn_corrected_z, Pn_corrected_E, Pn_corrected_theta, Pn_corrected_phi, Pn_corrected_vect_mag;
            // DONE - std::vector<double> Pn_clas_x, Pn_clas_y, Pn_clas_z, Pn_clas_E, Pn_clas_theta, Pn_clas_phi, Pn_clas_vect_mag;
            // DONE - std::vector<double> after_veto_tof_n, after_veto_beta_n, after_veto_path_n;
            // DONE - std::vector<double> after_veto_beta_corrected_n, after_veto_path_corrected_n;
            // DONE - std::vector<int> after_veto_pindex;
            // DONE - std::vector<int> after_veto_detector, after_veto_layer;
            // DONE - std::vector<double> after_veto_hit_x, after_veto_hit_y, after_veto_hit_z;

            // std::vector<double> nq_clas, nq_calc
            // DONE - std::vector<double> theta_nq_clas, theta_nq_calc;

            int k = after_veto.at(j)->hitIndex;
            int d = after_veto.at(j)->detector;
            int l = after_veto.at(j)->layer;

            TLorentzVector Pn_clas;
            TVector3 hit_n(after_veto.at(j)->x, after_veto.at(j)->y, after_veto.at(j)->z);

            // clas_momentum calculation
            Pn_clas.SetXYZM(after_veto.at(j)->Px, after_veto.at(j)->Py, after_veto.at(j)->Pz, mass_neutron);
            Pn_clas_x.push_back(Pn_clas.Vect().X());
            Pn_clas_y.push_back(Pn_clas.Vect().Y());
            Pn_clas_z.push_back(Pn_clas.Vect().Z());
            Pn_clas_E.push_back(Pn_clas.E());
            Pn_clas_theta.push_back(Pn_clas.Vect().Theta() * TMath::RadToDeg());
            Pn_clas_phi.push_back(Pn_clas.Vect().Phi() * TMath::RadToDeg());
            Pn_clas_vect_mag.push_back(TMath::Sqrt(Pn_clas.Vect().Mag2()));

            nq_clas.push_back(Pn_clas.Vect().Mag() / q->Vect().Mag());
            theta_nq_clas.push_back(Pn_clas.Vect().Angle(q->Vect()) * TMath::RadToDeg());

            // neutron parameters
            after_veto_tof_n.push_back(after_veto.at(j)->tof);
            after_veto_path_n.push_back(after_veto.at(j)->path);
            after_veto_beta_n.push_back(after_veto.at(j)->path / after_veto.at(j)->tof / clight);
            after_veto_pindex.push_back(after_veto.at(j)->pindex);
            after_veto_detector.push_back(d);
            after_veto_layer.push_back(l);
            after_veto_sector.push_back(after_veto.at(j)->sector);
            after_veto_dE.push_back(after_veto.at(j)->dE);
            after_veto_hit_x.push_back(after_veto.at(j)->x);
            after_veto_hit_y.push_back(after_veto.at(j)->y);
            after_veto_hit_z.push_back(after_veto.at(j)->z);

            // all momentum calculated using the hit in the ECAL
            double beta_calc = after_veto.at(j)->path / after_veto.at(j)->tof / clight;
            double Pn_mag;
            if (beta_calc >= 1.0 && beta_calc < 0.)
                Pn_mag = 0.;
            else
                Pn_mag = mass_neutron * beta_calc / TMath::Sqrt(1 - beta_calc * beta_calc);

            TLorentzVector Pn_calc;
            double hit_theta = hit_n.Theta(), hit_phi = hit_n.Phi();
            double px = Pn_mag * TMath::Sin(hit_theta) * TMath::Cos(hit_phi);
            double py = Pn_mag * TMath::Sin(hit_theta) * TMath::Sin(hit_phi);
            double pz = Pn_mag * TMath::Cos(hit_theta);
            Pn_calc.SetXYZM(px, py, pz, mass_neutron);
            Pn_calc_x.push_back(Pn_calc.Vect().X());
            Pn_calc_y.push_back(Pn_calc.Vect().Y());
            Pn_calc_z.push_back(Pn_calc.Vect().Z());
            Pn_calc_E.push_back(Pn_calc.E());
            Pn_calc_theta.push_back(Pn_calc.Vect().Theta() * TMath::RadToDeg());
            Pn_calc_phi.push_back(Pn_calc.Vect().Phi() * TMath::RadToDeg());
            Pn_calc_vect_mag.push_back(TMath::Sqrt(Pn_calc.Vect().Mag2()));

            nq_calc.push_back(Pn_calc.Vect().Mag() / q->Vect().Mag());
            theta_nq_calc.push_back(Pn_calc.Vect().Angle(q->Vect()) * TMath::RadToDeg());
            // correcting the momentum and beta using the electron vertex
            //  Compute path vector
            double dx = hit_n.X() - vtx_e->X();
            double dy = hit_n.Y() - vtx_e->Y();
            double dz = hit_n.Z() - vtx_e->Z();

            TVector3 hit_n_corrected(dx, dy, dz);
            // Path length in cm
            double path_length = TMath::Sqrt(dx * dx + dy * dy + dz * dz);

            // Velocity (dimensionless)
            double new_beta = path_length / after_veto.at(j)->tof / clight;

            // push the new beta and path
            after_veto_beta_corrected_n.push_back(new_beta);
            after_veto_path_corrected_n.push_back(path_length);

            double Pn_mag_corrected;
            if (new_beta >= 1.0 && new_beta < 0.)
                Pn_mag_corrected = 0.;
            else
                Pn_mag_corrected = mass_neutron * new_beta / TMath::Sqrt(1 - new_beta * new_beta);

            TLorentzVector Pn_corrected;
            double hit_theta_corrected = hit_n_corrected.Theta(), hit_phi_corrected = hit_n_corrected.Phi();
            double px_corrected = Pn_mag_corrected * TMath::Sin(hit_theta_corrected) * TMath::Cos(hit_phi_corrected);
            double py_corrected = Pn_mag_corrected * TMath::Sin(hit_theta_corrected) * TMath::Sin(hit_phi_corrected);
            double pz_corrected = Pn_mag_corrected * TMath::Cos(hit_theta_corrected);
            Pn_corrected.SetXYZM(px_corrected, py_corrected, pz_corrected, mass_neutron);

            Pn_corrected_x.push_back(Pn_corrected.Vect().X());
            Pn_corrected_y.push_back(Pn_corrected.Vect().Y());
            Pn_corrected_z.push_back(Pn_corrected.Vect().Z());
            Pn_corrected_E.push_back(Pn_corrected.E());
            Pn_corrected_theta.push_back(Pn_corrected.Vect().Theta() * TMath::RadToDeg());
            Pn_corrected_phi.push_back(Pn_corrected.Vect().Phi() * TMath::RadToDeg());
            Pn_corrected_vect_mag.push_back(TMath::Sqrt(Pn_corrected.Vect().Mag2()));

            nq_corrected.push_back(Pn_corrected.Vect().Mag() / q->Vect().Mag());
            theta_nq_corrected.push_back(Pn_corrected.Vect().Angle(q->Vect()) * TMath::RadToDeg());
        }

        tree->Fill();
    }

    cout << "tree num of entries:  " << tree->GetEntries() << endl;
    // Step 5: Write the tree to the file and close it
    tree->Write();
    outFile.Close();
}