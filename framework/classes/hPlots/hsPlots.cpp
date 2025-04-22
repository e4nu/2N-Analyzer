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
                 const int& nbinsX, const double& xlow, const double& xup, const int& nbinsY, const double& ylow, const double& yup, std::string slice_var)
    : SliceLimits(sliceLimits), histoType(type) {
    bool PrintOut = false;

    auto slice_var_pair = basic_tools::splitVarAndUnits(slice_var);  // Slice variable = slice_var_pair.first and [units] = slice_var_pair.second

    int count = 0;

    if (PrintOut) {
        std::cout << "hsPlots constructor: SliceLimits.size() = " << SliceLimits.size() << std::endl;
        std::cout << "hsPlots constructor: histoType = " << histoType << std::endl;
        std::cout << "hsPlots constructor: baseName = " << baseName << std::endl;
        std::cout << "hsPlots constructor: titleTemplate = " << titleTemplate << std::endl;
        std::cout << "hsPlots constructor: slice_var_pair.first = " << slice_var_pair.first << std::endl;
        std::cout << "hsPlots constructor: slice_var_pair.second = " << slice_var_pair.second << std::endl;
    }

    // Create histograms based on the provided slice limits
    for (const auto& range : SliceLimits) {
        if (range.size() != 2) {
            std::cerr << "Invalid slice range at index " << count << std::endl;
            continue;
        }

        std::ostringstream name, title;
        name << baseName << "_slice_from_" << basic_tools::ToStringWithPrecision(range.at(0), 2) << "_to_" << basic_tools::ToStringWithPrecision(range.at(1), 2);
        // name << baseName << "_slice_" << count;
        title << "#splitline{ " << titleTemplate << " }{ " << basic_tools::ToStringWithPrecision(range.at(0), 2) << "#leq" << slice_var_pair.first << "#leq"
              << basic_tools::ToStringWithPrecision(range.at(1), 2) << " " + slice_var_pair.second + " }";
        // title << "#splitline{ " << titleTemplate << " }{ (" << range[0] << " to " << range[1] << ") }";

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
    bool PrintOut = true;

    int index = FindSliceIndex(sliceVar);

    // Check if slice index is valid:
    if ((index < 0) || (index >= static_cast<int>(SlicedHistoList.size()))) {
        std::cerr << "\033[31m\n\nhsPlots::Fill: ERROR: Invalid slice index!\n";
        std::cerr << "index = " << index << "\n";
        std::cerr << "sliceVar = " << sliceVar << "\n";
        std::cerr << "Aborting...\033[0m\n";
        exit(1);
    }

    if (PrintOut) {
        for (size_t i = 0; i < SlicedHistoList.size(); ++i) {
            std::cout << "Index " << i << ": ";
            if (!SlicedHistoList[i]) {
                std::cout << "nullptr\n";
            } else {
                std::cout << SlicedHistoList[i]->ClassName() << "\n";
            }
        }
    }
    
    if (!SlicedHistoList[index]) {
        std::cerr << "\033[31m\n\nhsPlots::Fill: ERROR: Histogram at index = " << index << " is null!\n";
        std::cerr << "Aborting...\033[0m\n";
        exit(1);
    }

    TObject* obj = SlicedHistoList[index];

    if (histoType == TH1D_TYPE) {
        TH1D* h1_cast = dynamic_cast<TH1D*>(obj);

        if (h1_cast) {
            h1_cast->Fill(x, weight);
        } else {
            std::cerr << "\033[31m\n\nhsPlots::Fill: ERROR: Histogram at index " << index << " is not TH1D as expected.\n";

            if (obj) {
                std::cerr << "Actual class name: " << obj->ClassName() << "\n";
            } else {
                std::cerr << "Histogram pointer is null!\n";
            }

            std::cerr << "Aborting...\033[0m\n";
            exit(1);
        }
    } else if (histoType == TH2D_TYPE) {
        TH2D* h2_cast = dynamic_cast<TH2D*>(obj);

        if (h2_cast) {
            h2_cast->Fill(x, y, weight);
        } else {
            std::cerr << "\033[31m\n\nhsPlots::Fill: ERROR: Histogram at index " << index << " is not TH2D as expected.\n";

            if (obj) {
                std::cerr << "Actual class name: " << obj->ClassName() << "\n";
            } else {
                std::cerr << "Histogram pointer is null!\n";
            }

            std::cerr << "Aborting...\033[0m\n";
            exit(1);
        }
    }

    // if (histoType == TH1D_TYPE) {
    //     dynamic_cast<TH1D*>(SlicedHistoList[index])->Fill(x, weight);
    // } else if (histoType == TH2D_TYPE) {
    //     dynamic_cast<TH2D*>(SlicedHistoList[index])->Fill(x, y, weight);
    // }
}

// SaveHistograms function ---------------------------------------------------------------------------------------------------------------------------------------------

// This function saves the histograms to a multipage PDF file and individual PNG files.
// It takes the output directory and base file name as parameters.
void hsPlots::SaveHistograms(const std::string& outputDir, const std::string& baseFileName) const {
    std::string PDF_File = outputDir + "/" + baseFileName + ".pdf";
    std::string PNG_Files_Base_Directory = outputDir + "/PNG_plots_" + baseFileName + "/";
    system(("mkdir -p " + PNG_Files_Base_Directory).c_str());  // Create output directory if it doesn't exist

    TCanvas* canvas = new TCanvas("canvas", "Histogram Canvas", 1000, 750);
    canvas->cd()->SetGrid();
    canvas->cd()->SetTopMargin(0.14);
    canvas->cd()->SetBottomMargin(0.14);
    canvas->cd()->SetLeftMargin(0.18);
    canvas->cd()->SetRightMargin(0.12);

    canvas->Print((PDF_File + "[").c_str());  // Open multipage PDF

    for (size_t i = 0; i < SlicedHistoList.size(); ++i) {
        canvas->cd();
        canvas->Clear();

        TH1* hist = SlicedHistoList[i];

        // if (hist->GetEntries() == 0) {
        //     std::cout << "Skipping empty histogram [" << i << "]" << std::endl;
        //     continue;
        // }

        hist->GetXaxis()->SetTitleSize(0.06);
        hist->GetXaxis()->SetLabelSize(0.0425);
        hist->GetXaxis()->CenterTitle(true);
        hist->GetYaxis()->SetTitleSize(0.06);
        hist->GetYaxis()->SetLabelSize(0.0425);
        hist->GetYaxis()->CenterTitle(true);

        if (hist->InheritsFrom("TH2")) {
            hist->Draw("colz");
        } else if (hist->InheritsFrom("TH1")) {
            hist->Draw("hist");
        }

        // Save to PDF page
        canvas->Print(PDF_File.c_str());

        // Save PNG (single underscore version)
        std::string PNG_File = PNG_Files_Base_Directory + "/" + std::to_string(i) + "_" + hist->GetName() + ".png";
        canvas->SaveAs(PNG_File.c_str());
    }

    canvas->Print((PDF_File + "]").c_str());  // Close multipage PDF

    delete canvas;
}

// GetSlicedHistoList function ------------------------------------------------------------------------------------------------------------------------------------------

std::vector<TH1*> hsPlots::GetSlicedHistoList() const { return SlicedHistoList; }

#endif  // HSPLOTS_H