#include <iostream>
#include <fstream>
#include <string>

#include <TFile.h>
#include <TTree.h>
#include <TLorentzVector.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TLatex.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TDatabasePDG.h>
#include <TApplication.h>
#include <TROOT.h>

#include "../../source/functions/GeneralFunctions.h"
#include "../../source/constants_include.h"

using namespace std;

TH1D *Histofinder1D(TFile *file, const char *Histogram1DNameSubstring, const bool TLmom = false) {
    bool PrintOut = false;
    bool PrintOut1 = false;

    TH1D *Histogram1D;

    bool HistogramFound = false;
    int Counter = 0, HistogramCounter = 0;
    TString classname("TH1D");
    TString classnameTH2D("TH2D");
    TString classnameTFolder("TFolder");
    TString classnameTHStack("THStack");
    std::string FoundHistName;

    TKey *Key;
    TIter Next((TList *) file->GetListOfKeys());

    while ((Key = (TKey *) Next())) {
        std::string Histogram1DTempName = ((TH1D *) Key->ReadObj())->GetName();

        if (PrintOut1) { std::cout << Histogram1DTempName << "\n\n"; }

        if (findSubstring(Histogram1DTempName, Histogram1DNameSubstring) &&
            (Key->GetClassName() != classnameTH2D("TH2D")) &&
            (Key->GetClassName() != classnameTFolder("TFolder")) &&
            (Key->GetClassName() != classnameTHStack("THStack"))) {

            if (PrintOut) { std::cout << "\n\nKey name: " << ((TH1D *) Key->ReadObj())->GetName() << "; Type: " << Key->GetClassName() << "\n\n"; }

            std::string Histogram1DxLable = ((TH1D *) Key->ReadObj())->GetXaxis()->GetTitle();
            std::string Histogram1DTitle = ((TH1D *) Key->ReadObj())->GetTitle();

            if (PrintOut) {
                std::cout << "\nHistogram1DxLable = " << Histogram1DxLable << "\n";
                std::cout << "Histogram1DTitle = " << Histogram1DTitle << "\n";
                std::cout << "TLmom = " << TLmom << "\n";
            }

            if ((TLmom || !findSubstring(Histogram1DxLable, "Momentum"))) {
                HistogramFound = true;

                Histogram1D = ((TH1D *) Key->ReadObj());
                FoundHistName = Key->GetClassName();
                break;
            }
        }
    }

    if (!HistogramFound) {
        std::cout << "\n\nHistofinder1D: could not find histogram!\n";
        std::cout << "TLmom = " << TLmom << "\n";
        std::cout << "Histogram1DNameSubstring = " << Histogram1DNameSubstring << "\n";
        exit(0);

        return Histogram1D;
    } else {
        if (PrintOut) {
            std::cout << "\n\nHistofinder1D: histogram found!\n";
            std::cout << "FoundHistName = " << FoundHistName << "\n";
        }

        return Histogram1D;
    }
}
