//
// Created by alons on 05/04/2025.
//

// #ifndef TRUTHEVENTANALYSER_H
// #define TRUTHEVENTANALYSER_H
#ifdef TRUTHEVENTANALYSER_H

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

    TruthAnalyser(const std::string &AnalyzeFilePath, const std::string &AnalyzeFileSample, const std::string &AnalyzeFile);

    ~TruthAnalyser() = default;
};

#endif  // TRUTHEVENTANALYSER_H
