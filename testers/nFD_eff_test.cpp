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

#include "../source/constants.h"
#include "HipoChain.h"
#include "clas12reader.h"

using namespace std;
using namespace clas12;

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

    TH1D* h_reco_P_nFD_clas12_1e_cut = new TH1D("reco_P_nFD_clas12_1e_cut", "reco P_{nFD} in 1e cut;P_{nFD} [GeV/c];Counts", 50, 0, Ebeam * 1.1);
    HistoList.push_back(h_reco_P_nFD_clas12_1e_cut);
    TH1D* h_truth_P_nFD_clas12_1e_cut = new TH1D("truth_P_nFD_clas12_1e_cut", "truth P_{nFD} in 1e cut;P_{nFD} [GeV/c];Counts", 50, 0, Ebeam * 1.1);
    HistoList.push_back(h_truth_P_nFD_clas12_1e_cut);
    TH1D* h_reco_theta_nFD_clas12_1e_cut = new TH1D("reco_theta_nFD_clas12_1e_cut", "reco #theta_{nFD} in 1e cut;#theta_{nFD} [#circ];Counts", 50, 0, 50.);
    HistoList.push_back(h_reco_theta_nFD_clas12_1e_cut);
    TH1D* h_truth_theta_nFD_clas12_1e_cut = new TH1D("truth_theta_nFD_clas12_1e_cut", "truth #theta_{nFD} in 1e cut;#theta_{nFD} [#circ];Counts", 50, 0, 50.);
    HistoList.push_back(h_truth_theta_nFD_clas12_1e_cut);
    TH1D* h_reco_phi_nFD_clas12_1e_cut = new TH1D("reco_phi_nFD_clas12_1e_cut", "reco #phi_{nFD} in 1e cut;#phi_{nFD} [#circ];Counts", 50, -180, 180.);
    HistoList.push_back(h_reco_phi_nFD_clas12_1e_cut);
    TH1D* h_truth_phi_nFD_clas12_1e_cut = new TH1D("truth_phi_nFD_clas12_1e_cut", "truth #phi_{nFD} in 1e cut;#phi_{nFD} [#circ];Counts", 50, -180, 180.);
    HistoList.push_back(h_truth_phi_nFD_clas12_1e_cut);
    TH2D* h_reco_theta_nFD_clas12_VS_reco_phi_nFD_clas12_1e_cut = new TH2D(
        "reco_theta_nFD_clas12_VS_reco_phi_nFD_clas12_1e_cut", "reco #theta_{nFD} vs. reco #phi_{nFD} in 1e cut;#phi_{nFD} [#circ];#theta_{nFD} [#circ]", 100, -180., 180., 100, 0, 50.);
    HistoList.push_back(h_reco_theta_nFD_clas12_VS_reco_phi_nFD_clas12_1e_cut);
    TH2D* h_truth_theta_nFD_clas12_VS_truth_phi_nFD_clas12_1e_cut = new TH2D(
        "truth_theta_nFD_clas12_VS_truth_phi_nFD_clas12_1e_cut", "truth #theta_{nFD} vs. truth #phi_{nFD} in 1e cut;#phi_{nFD} [#circ];#theta_{nFD} [#circ]", 100, -180., 180., 100, 0, 50.);
    HistoList.push_back(h_truth_theta_nFD_clas12_VS_truth_phi_nFD_clas12_1e_cut);

    int counter = 0;

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
        //  1e cut (reco)
        //  =======================================================================================================================================================================

        //  Electron PID cuts -----------------------------------------------------------------------------------------------------------------------------------------------------

        if (electrons[0]->che(clas12::HTCC)->getNphe() <= 2) { continue; }
        if (EoP_e >= 0.2 || EoP_e <= 0.28) { continue; }
        if (electrons[0]->cal(clas12::PCAL)->getLv() < 14. || electrons[0]->cal(clas12::PCAL)->getLw() < 14.) { continue; }
        if (electrons[0]->par()->getVz() < -6. || electrons[0]->par()->getVz() > 0.) { continue; }

        //  Setting up neutrals ---------------------------------------------------------------------------------------------------------------------------------------------------

        vector<region_part_ptr> neutrons;
        vector<region_part_ptr> photons;

        for (int i = 0; i < allParticles.size(); i++) {
            int pid_temp = allParticles[i]->par()->getPid();

            if (pid_temp == 2112) { neutrons.push_back(allParticles[i]); }
            if (pid_temp == 22) { photons.push_back(allParticles[i]); }
        }

        //  Setting up FD neutrals (clas12reco) -----------------------------------------------------------------------------------------------------------------------------------

        vector<region_part_ptr> neutrons_FD_clas12;
        vector<region_part_ptr> photons_FD_clas12;

        for (int i = 0; i < allParticles.size(); i++) {
            int pid_temp = allParticles[i]->par()->getPid();

            if (pid_temp == 2112 && allParticles[i]->getRegion() == FD) { neutrons_FD_clas12.push_back(allParticles[i]); }
            if (pid_temp == 22 && allParticles[i]->getRegion() == FD) { photons_FD_clas12.push_back(allParticles[i]); }
        }

        h_reco_P_e_1e_cut->Fill(reco_P_e.Mag(), weight);
        h_reco_theta_e_1e_cut->Fill(reco_P_e.Theta() * 180 / M_PI, weight);
        h_reco_phi_e_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, weight);
        h_reco_theta_e_VS_reco_phi_e_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, reco_P_e.Theta() * 180 / M_PI, weight);

        for (int i = 0; i < neutrons.size(); i++) {
            TVector3 reco_P_n;
            reco_P_n.SetMagThetaPhi(neutrons[i]->getP(), neutrons[i]->getTheta(), neutrons[i]->getPhi());

            h_reco_P_n_1e_cut->Fill(reco_P_n.Mag(), weight);
            h_reco_theta_n_1e_cut->Fill(reco_P_n.Theta() * 180 / M_PI, weight);
            h_reco_phi_n_1e_cut->Fill(reco_P_n.Phi() * 180 / M_PI, weight);
            h_reco_theta_n_VS_reco_phi_n_1e_cut->Fill(reco_P_n.Phi() * 180 / M_PI, reco_P_n.Theta() * 180 / M_PI, weight);
        }

        for (int i = 0; i < neutrons_FD_clas12.size(); i++) {
            TVector3 reco_P_nFD;
            reco_P_nFD.SetMagThetaPhi(neutrons_FD_clas12[i]->getP(), neutrons_FD_clas12[i]->getTheta(), neutrons_FD_clas12[i]->getPhi());

            h_reco_P_nFD_clas12_1e_cut->Fill(reco_P_nFD.Mag(), weight);
            h_reco_theta_nFD_clas12_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI, weight);
            h_reco_phi_nFD_clas12_1e_cut->Fill(reco_P_nFD.Phi() * 180 / M_PI, weight);
            h_reco_theta_nFD_clas12_VS_reco_phi_nFD_clas12_1e_cut->Fill(reco_P_nFD.Phi() * 180 / M_PI, reco_P_nFD.Theta() * 180 / M_PI, weight);
        }

        //  =======================================================================================================================================================================
        //  1e cut (truth)
        //  =======================================================================================================================================================================

        auto c12 = chain.GetC12Reader();
        auto mceve = c12->mcevent();
        auto mcpbank = c12->mcparts();
        const Int_t Ngen = mcpbank->getRows();

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
                    h_truth_P_nFD_clas12_1e_cut->Fill(truth_P_n.Mag(), weight);
                    h_truth_theta_nFD_clas12_1e_cut->Fill(truth_P_n.Theta() * 180 / M_PI, weight);
                    h_truth_phi_nFD_clas12_1e_cut->Fill(truth_P_n.Phi() * 180 / M_PI, weight);
                    h_truth_theta_nFD_clas12_VS_truth_phi_nFD_clas12_1e_cut->Fill(truth_P_n.Phi() * 180 / M_PI, truth_P_n.Theta() * 180 / M_PI, weight);
                }
            }
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
    TLatex text;
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

    myCanvas->Divide(1, 1);
    for (int i = 0; i < HistoList.size(); i++) {
        myCanvas->cd(1);

        myCanvas->cd(1)->SetGrid();
        myCanvas->cd(1)->SetBottomMargin(0.14), myCanvas->cd(1)->SetLeftMargin(0.16), myCanvas->cd(1)->SetRightMargin(0.12);

        HistoList[i]->GetYaxis()->SetLabelOffset(0.1);

        if (HistoList[i]->InheritsFrom("TH1D")) {
            HistoList[i]->Draw();
        } else if (HistoList[i]->InheritsFrom("TH2D")) {
            HistoList[i]->Draw("colz");

            // gPad->Update();
            // TPaletteAxis* palette = (TPaletteAxis*)HistoList[i]->GetListOfFunctions()->FindObject("palette");
            // palette->SetY2NDC(0.55);
            // gPad->Modified();
            // gPad->Update();
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
