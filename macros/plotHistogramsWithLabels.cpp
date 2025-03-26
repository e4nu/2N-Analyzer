//
// Created by Alon Sportes on 14/03/2025.
//

#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TText.h>
#include <iostream>

void plotHistogramsWithLabels(const std::vector<std::string> &fileNames, const std::string &histName) {
    if (fileNames.size() != 6) {
        std::std::cerr << "Error: Function requires exactly 6 ROOT files.\n";
        return;
    }

    TFile *files[6];
    TH1 *histograms[6];

    // Open the files and retrieve the histograms
    for (int i = 0; i < 6; ++i) {
        files[i] = TFile::Open(fileNames[i].c_str(), "READ");
        if (!files[i] || files[i]->IsZombie()) {
            std::std::cerr << "Error: Could not open file " << fileNames[i] << "\n";
            return;
        }

        histograms[i] = dynamic_cast<TH1 *>(files[i]->Get(histName.c_str()));
        if (!histograms[i]) {
            std::std::cerr << "Error: Histogram " << histName << " not found in " << fileNames[i] << "\n";
            return;
        }
    }

    // Create a canvas with a 2x3 grid
    TCanvas *canvas = new TCanvas("canvas", "Comparison of Histograms", 1200, 800);
    canvas->Divide(3, 2); // 2 rows, 3 columns

    for (int i = 0; i < 6; ++i) {
        canvas->cd(i + 1);
        histograms[i]->SetLineColor(i + 1); // Set different colors
        histograms[i]->Draw();

        // Add a label (text box) with the filename
        TText *text = new TText(0.15, 0.85, fileNames[i].c_str());
        text->SetNDC(); // Set text position in normalized device coordinates
        text->SetTextSize(0.04);
        text->SetTextColor(kBlack);
        text->Draw();
    }

    canvas->SaveAs("comparison.png");

    // Close the files
    for (int i = 0; i < 6; ++i) {
        files[i]->Close();
        delete files[i];
    }
}

void RunMacroPlots(const std::string &histName) {
    std::vector<std::string> fileNames = {
        "file1.root", "file2.root", "file3.root",
        "file4.root", "file5.root", "file6.root"
    };

    plotHistogramsWithLabels(fileNames, "hist_name");
}
