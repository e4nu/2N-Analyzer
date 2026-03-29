#include <TCanvas.h>
#include <TF1.h>
#include <TFile.h>
#include <TH2.h>

#include <iostream>

void ReplotBeta() {
    constexpr const char* kInputFile = "/Users/alon/Desktop/C12_sim_G18_Q2_0_02_2070MeV_S03ACNC_momResS1_plots.root";
    constexpr const char* kHistName = "Reco neutron #font[12]{#beta^{reco}_{nFD}} vs. #font[12]{P^{truth}_{nFD}} (1n, FD)";
    constexpr double kNeutronMassGeV = 0.939565;

    TFile* f = TFile::Open(kInputFile, "READ");
    if (!f || f->IsZombie()) {
        std::cerr << "Failed to open input file: " << kInputFile << '\n';
        return;
    }

    TH2D* Beta1Plot = dynamic_cast<TH2D*>(f->Get(kHistName));
    if (!Beta1Plot) {
        std::cerr << "Histogram not found: " << kHistName << '\n';
        f->Close();
        return;
    }

    TCanvas* Canvas = new TCanvas("Canvas", "Canvas", 1000, 750);

    Canvas->cd();
    Canvas->cd()->SetGrid();
    Canvas->cd()->SetBottomMargin(0.14);
    Canvas->cd()->SetLeftMargin(0.17);
    Canvas->cd()->SetRightMargin(0.12);
    Beta1Plot->SetDirectory(nullptr);
    Beta1Plot->Draw("colz");

    TF1* beta_kaon = new TF1("beta_kaon", "x/sqrt(x*x + [0]*[0])", 0, 2.);
    beta_kaon->SetParameter(0, 0.493677);
    // beta_kaon->SetParameter(0, kNeutronMassGeV);
    beta_kaon->SetLineColor(kBlue + 1);
    beta_kaon->SetLineWidth(2);
    beta_kaon->Draw("same");

    TF1* beta_deuteron = new TF1("beta_deuteron", "x/sqrt(x*x + [0]*[0])", 0, 2.);
    beta_deuteron->SetParameter(0, 1.8756);
    // beta_deuteron->SetParameter(0, kNeutronMassGeV);
    beta_deuteron->SetLineColor(kRed + 1);
    beta_deuteron->SetLineWidth(2);
    beta_deuteron->Draw("same");

    TF1* beta_neutron = new TF1("beta_neutron", "x/sqrt(x*x + [0]*[0])", 0, 2.);
    // beta_neutron->SetParameter(0, 1.8756);
    beta_neutron->SetParameter(0, kNeutronMassGeV);
    beta_neutron->SetLineColor(kGreen + 1);
    beta_neutron->SetLineWidth(2);
    beta_neutron->Draw("same");

    double x_1_Cut_legend = gStyle->GetStatX(), y_1_Cut_legend = gStyle->GetStatY() - 0.2 + 0.1;
    double x_2_Cut_legend = gStyle->GetStatX() - 0.2, y_2_Cut_legend = gStyle->GetStatY() - 0.3 + 0.05;

    auto HistogramStackLegend = new TLegend(x_1_Cut_legend, y_1_Cut_legend, x_2_Cut_legend, y_2_Cut_legend);

    TLegendEntry* Histogram1_Entry = HistogramStackLegend->AddEntry(beta_kaon, "beta_kaon", "l");
    TLegendEntry* Histogram3_Entry = HistogramStackLegend->AddEntry(beta_neutron, "beta_neutron", "l");
    TLegendEntry* Histogram2_Entry = HistogramStackLegend->AddEntry(beta_deuteron, "beta_deuteron", "l");
    HistogramStackLegend->SetTextSize(0.017);
    HistogramStackLegend->Draw();

    Canvas->SaveAs("Beta1Plot.pdf");
    f->Close();
}
