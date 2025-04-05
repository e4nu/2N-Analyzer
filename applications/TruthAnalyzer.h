//
// Created by Alon Sportes on 05/04/2025.
//

// #ifndef TRUTHANALYZER_H
// #define TRUTHANALYZER_H
#ifdef TRUTHANALYZER_H

    #include <TCanvas.h>
    #include <TFile.h>
    #include <TH2.h>
    #include <TMath.h>
    #include <TStyle.h>
    #include <math.h>

    #include <cmath>
    #include <fstream>
    #include <iomanip>
    #include <iostream>
    #include <string>

    #include "../framework/classes/gst/gst.cpp"

// using namespace std;

class TruthAnalyzer : public gst {
   private:
   public:
    // TruthAnalyzer constructor ----------------------------------------------------------------------------------------------------------------------------------------

    TruthAnalyzer();

    ~TruthAnalyzer() = default;
};

#endif  // TRUTHANALYZER_H
