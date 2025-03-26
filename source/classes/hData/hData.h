//
// Created by alons on 09/07/2023.
//

// #ifndef HDATA_H
// #define HDATA_H
#ifdef HDATA_H

#include <TApplication.h>
#include <TCanvas.h>
#include <TChain.h>
#include <TDatabasePDG.h>
#include <TF1.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TLatex.h>
#include <TLorentzVector.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TTree.h>
#include <math.h>

#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <typeinfo>
#include <vector>

#include "../../namespaces/general_utilities/data_processor/data_processor.h"
// #include "../../functions/GeneralFunctions.h"

using namespace std;
using namespace data_processor;

class hData {
   private:
    int StandardCanvasWidth = 1000, StandardCanvasHeight = 750;
    double BottomMargin = 0.14, LeftMargin = 0.16, RightMargin = 0.16, TopMargin = 0.12;

   public:
    // constructors ---------------------------------------------------------------------------------------------------------------------------------------------------------

    // Default constructor:
    hData() = default;

    // GetParticleName function ---------------------------------------------------------------------------------------------------------------------------------------------

    std::string GetParticleName(const std::string &Source, const bool &PluralParticles = false);

    std::string GetParticleName1(const std::string &Source, const bool &PluralParticles = false);

    // GetParticleNameFromSubscript function --------------------------------------------------------------------------------------------------------------------------------

    std::string GetParticleNameFromSubscript(const std::string &Source, const bool &PluralParticles = false);

    // GetParticleNameLC function -------------------------------------------------------------------------------------------------------------------------------------------

    std::string GetParticleNameLC(const std::string &Source, const bool &PluralParticles = false);

    // GetParticleNameLCFromSubscript function ------------------------------------------------------------------------------------------------------------------------------

    std::string GetParticleNameLCFromSubscript(const std::string &Source, const bool &PluralParticles = false);

    // GetParticleNameShort function ----------------------------------------------------------------------------------------------------------------------------------------

    std::string GetParticleNameShort(const std::string &Source);

    // GetParticleNameShortFromSubscript function ---------------------------------------------------------------------------------------------------------------------------

    std::string GetParticleNameShortFromSubscript(const std::string &Source);

    // GetType function -----------------------------------------------------------------------------------------------------------------------------------------------------

    std::string GetType(const std::string &Source);

    // GetDRegion function --------------------------------------------------------------------------------------------------------------------------------------------------

    std::string GetDRegionExplicit(const std::string &Source, const bool &ReturnGoingRegion = false);

    // GetDRegion function --------------------------------------------------------------------------------------------------------------------------------------------------

    std::string GetDRegion(const std::string &Source, const bool &ReturnGoingRegion = false);

    // GetFSRTitle function -------------------------------------------------------------------------------------------------------------------------------------------------

    std::string GetFSRTitle(const std::string &Source, const std::string &PlotsT);

    // GetTitle function ----------------------------------------------------------------------------------------------------------------------------------------------------

    std::string GetTitle(const std::string &Source);

    // GetStatsTitle function -----------------------------------------------------------------------------------------------------------------------------------------------

    std::string GetStatsTitle(const std::string &Source);

    // GetFS function -------------------------------------------------------------------------------------------------------------------------------------------------------

    std::string GetFS(const std::string &Source);

    // GetTopology function -------------------------------------------------------------------------------------------------------------------------------------------------

    std::string GetTopology(const std::string &Source);

    // Other get function ---------------------------------------------------------------------------------------------------------------------------------------------------

    int GetStandardCanvasWidth() { return StandardCanvasWidth; }

    int GetStandardCanvasHeight() { return StandardCanvasHeight; }

    int GetBottomMargin() { return BottomMargin; }

    int GetLeftMargin() { return LeftMargin; }

    int GetRightMargin() { return RightMargin; }

    int GetTopMargin() { return TopMargin; }

    // SetXLabel function ---------------------------------------------------------------------------------------------------------------------------------------------------

    std::string SetXLabel(const std::string &Source);

    // SetSaveDir function --------------------------------------------------------------------------------------------------------------------------------------------------

    std::string SetSaveDir(const std::string &Source, const std::string &BaseSaveDir, const std::string &Mod = "");

    // ReplaceSubStr function -----------------------------------------------------------------------------------------------------------------------------------------------

    void ReplaceSubStr(string &str, const std::string &subStr, const std::string &replacement);
};

#endif  // HDATA_H
