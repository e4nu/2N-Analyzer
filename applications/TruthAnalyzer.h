//
// Created by alons on 05/04/2025.
//

// #ifndef TRUTHANALYSER_H
// #define TRUTHANALYSER_H
#ifdef TRUTHANALYSER_H

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

class TruthAnalyser : public gst {
   private:
   public:
    // TruthAnalyser constructor ----------------------------------------------------------------------------------------------------------------------------------------

    TruthAnalyser();

    ~TruthAnalyser() = default;
};

#endif  // TRUTHANALYSER_H
