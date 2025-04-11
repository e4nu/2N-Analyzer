//
// Created by Alon Sportes on 11/04/2025.
//

#ifndef HSPLOTS_H
#define HSPLOTS_H

#include "hsPlots.h"

#include <iostream>
#include <sstream>

// hsPlots constructor --------------------------------------------------------------------------------------------------------------------------------------------------

// Constructor to initialize the hsPlots object
// This constructor takes a vector of slice limits, a histogram type (TH1D or TH2D), a base name for the histograms,
// a title template for the histograms, and the number of bins and limits for the x and y axes.
// It creates the histograms based on the provided slice limits and adds them to the SlicedHistoList.
// The histograms are created using the ROOT library's TH1D and TH2D classes.
// The constructor also takes a vector of TObject pointers (HistoList) to store the created histograms.
// The histograms are named using the base name and the slice index, and the titles are generated using the title template and the slice limits.
hsPlots::hsPlots(const std::vector<std::vector<double>>& sliceLimits, HistoType type, std::vector<TObject*>& HistoList, const std::string& baseName, const std::string& titleTemplate,
                 int nbinsX, double xlow, double xup, int nbinsY, double ylow, double yup)
    : SliceLimits(sliceLimits), histoType(type) {
    int count = 0;
    for (const auto& range : SliceLimits) {
        if (range.size() != 2) {
            std::cerr << "Invalid slice range at index " << count << std::endl;
            continue;
        }

        std::ostringstream name, title;
        name << baseName << "_slice_" << count;
        title << "#splitline{ " << titleTemplate << " }{ (" << range[0] << " to " << range[1] << ") }";

        if (histoType == TH1D_TYPE) {
            SlicedHistoList.push_back(new TH1D(name.str().c_str(), title.str().c_str(), nbinsX, xlow, xup));
        } else if (histoType == TH2D_TYPE) {
            SlicedHistoList.push_back(new TH2D(name.str().c_str(), title.str().c_str(), nbinsX, xlow, xup, nbinsY, ylow, yup));
        }

        ++count;
    }

    // Add the sliced histograms to the provided HistoList
    for (int i = 0; i < SlicedHistoList.size(); i++) { HistoList.push_back(SlicedHistoList[i]); }
}

// hsPlots Destructor ---------------------------------------------------------------------------------------------------------------------------------------------------

// Destructor to clean up the dynamically allocated histograms
// This destructor iterates through the SlicedHistoList and deletes each histogram
// to free up the memory allocated for them. This is important to prevent memory leaks in the program.
hsPlots::~hsPlots() {
    for (auto* h : SlicedHistoList) { delete h; }
}

// FindSliceIndex function ---------------------------------------------------------------------------------------------------------------------------------------------

// This function takes a value and checks which slice it belongs to by iterating through the SliceLimits vector.
// If the value falls within the range defined by a slice, it returns the index of that slice.
// If the value does not fall within any slice, it returns -1 to indicate that the value was not found in any slice.
int hsPlots::FindSliceIndex(double value) {
    for (size_t i = 0; i < SliceLimits.size(); ++i) {
        if (value >= SliceLimits[i][0] && value < SliceLimits[i][1]) { return static_cast<int>(i); }
    }

    return -1;  // Not found
}

// Fill function --------------------------------------------------------------------------------------------------------------------------------------------------------

// Fill the histogram with the given x, y values and weight
void hsPlots::Fill(double sliceVar, double x, double y, double weight) {
    int index = FindSliceIndex(sliceVar);

    if (index < 0 || index >= static_cast<int>(SlicedHistoList.size())) { return; }  // Invalid slice index

    if (histoType == TH1D_TYPE) {
        dynamic_cast<TH1D*>(SlicedHistoList[index])->Fill(x, weight);
    } else if (histoType == TH2D_TYPE) {
        dynamic_cast<TH2D*>(SlicedHistoList[index])->Fill(x, y, weight);
    }
}

// SaveHistograms function ---------------------------------------------------------------------------------------------------------------------------------------------

// This function saves the histograms to a multipage PDF file and individual PNG files.
// It takes the output directory and base file name as parameters.
void hsPlots::SaveHistograms(const std::string& outputDir, const std::string& baseFileName) const {
    std::string pdfFile = outputDir + "/" + baseFileName + ".pdf";
    TCanvas* canvas = new TCanvas("canvas", "Histogram Canvas", 800, 600);
    canvas->cd()->SetGrid();
    canvas->cd()->SetBottomMargin(0.14);
    canvas->cd()->SetLeftMargin(0.18);
    canvas->cd()->SetRightMargin(0.12);

    canvas->Print((pdfFile + "[").c_str());  // Open multipage PDF

    for (size_t i = 0; i < SlicedHistoList.size(); ++i) {
        canvas->cd();
        canvas->Clear();

        TH1* hist = SlicedHistoList[i];
        if (hist->InheritsFrom(TH2D::Class())) {
            TH2D* h2 = (TH2D*)hist;
            h2->GetXaxis()->SetTitleSize(0.06);
            h2->GetXaxis()->SetLabelSize(0.0425);
            h2->GetXaxis()->CenterTitle(true);
            h2->GetYaxis()->SetTitleSize(0.06);
            h2->GetYaxis()->SetLabelSize(0.0425);
            h2->GetYaxis()->CenterTitle(true);
            h2->Draw("colz");
        } else if (hist->InheritsFrom(TH1D::Class())) {
            TH1D* h1 = (TH1D*)hist;
            h1->GetXaxis()->SetTitleSize(0.06);
            h1->GetXaxis()->SetLabelSize(0.0425);
            h1->GetXaxis()->CenterTitle(true);
            h1->GetYaxis()->SetTitleSize(0.06);
            h1->GetYaxis()->SetLabelSize(0.0425);
            h1->GetYaxis()->CenterTitle(true);
            h1->Draw();
        }

        canvas->Print(pdfFile.c_str());  // Save histogram to PDF
        std::string pngName = outputDir + "/" + baseFileName + "_" + std::to_string(i) + ".png";
        canvas->Print(pngName.c_str());  // Save histogram to PNG
    }

    canvas->Print((pdfFile + "]").c_str());  // Close multipage PDF

    std::cout << "\n\nHistograms saved to pdf file." << std::endl;

    for (size_t i = 0; i < SlicedHistoList.size(); ++i) {
        canvas->cd();
        canvas->Clear();

        TH1* hist = SlicedHistoList[i];
        if (SlicedHistoList[i]->InheritsFrom("TH2")) {
            // TH2D* h2 = (TH2D*)SlicedHistoList[i];
            SlicedHistoList[i]->GetXaxis()->SetTitleSize(0.06);
            SlicedHistoList[i]->GetXaxis()->SetLabelSize(0.0425);
            SlicedHistoList[i]->GetXaxis()->CenterTitle(true);
            SlicedHistoList[i]->GetYaxis()->SetTitleSize(0.06);
            SlicedHistoList[i]->GetYaxis()->SetLabelSize(0.0425);
            SlicedHistoList[i]->GetYaxis()->CenterTitle(true);
            SlicedHistoList[i]->Draw("colz");
            
            std::cout << "SlicedHistoList[i]->GetEntries() (TH2D) == " << SlicedHistoList[i]->GetEntries() << std::endl;
            
            canvas->SaveAs((outputDir + "/" + baseFileName + "__" + std::to_string(i) + ".png").c_str());
        } else if (SlicedHistoList[i]->InheritsFrom("TH1")) {
            // TH1D* h1 = (TH1D*)SlicedHistoList[i];
            SlicedHistoList[i]->GetXaxis()->SetTitleSize(0.06);
            SlicedHistoList[i]->GetXaxis()->SetLabelSize(0.0425);
            SlicedHistoList[i]->GetXaxis()->CenterTitle(true);
            SlicedHistoList[i]->GetYaxis()->SetTitleSize(0.06);
            SlicedHistoList[i]->GetYaxis()->SetLabelSize(0.0425);
            SlicedHistoList[i]->GetYaxis()->CenterTitle(true);
            SlicedHistoList[i]->Draw("hist");

            std::cout << "SlicedHistoList[i]->GetEntries() (TH1D) == " << SlicedHistoList[i]->GetEntries() << std::endl;

            canvas->SaveAs((outputDir + "/" + baseFileName + "__" + std::to_string(i) + ".png").c_str());
        }
    }

    std::cout << "Histograms saved to png files." << std::endl;

    delete canvas;
}

// GetSlicedHistoList function ------------------------------------------------------------------------------------------------------------------------------------------

std::vector<TH1*> hsPlots::GetSlicedHistoList() const { return SlicedHistoList; }

#endif  // HSPLOTS_H