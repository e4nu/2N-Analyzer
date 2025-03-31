//
// Created by alons on 22/06/2023.
//

#ifndef AMAPS_H
#define AMAPS_H

#include <TPaletteAxis.h>
#include <TApplication.h>
#include <TCanvas.h>
#include <TChain.h>
#include <TDatabasePDG.h>
#include <TF1.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TLatex.h>
#include <TLorentzVector.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TTree.h>
#include <math.h>

#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <typeinfo>
#include <vector>

// #include "../../functions/GeneralFunctions.h"
#include "../../namespaces/general_utilities/utilities/utilities.h"
#include "../DSCuts/DSCuts.h"
#include "../hPlots/hPlot2D.cpp"
#include "CustomSliceLimits.h"
// #include "clas12reader.h"

using namespace std;
using namespace utilities;

class AMaps {
   private:
    std::string AMaps_Mode = "AMaps";

    /* Acceptance maps from class and before cuts (to be compared with one generated with the file) */
    hPlot2D reco_theta_e_VS_phi_e_BC, reco_theta_p_VS_phi_p_BC, reco_theta_n_VS_phi_n_BC, reco_theta_nuc_VS_phi_nuc_BC;

    std::vector<std::vector<double>> InvertedElectronMomSliceLimits, ElectronMomSliceLimits;
    std::vector<std::vector<double>> InvertedNucleonMomSliceLimits, NucleonMomSliceLimits;

    double hBinLowerXLim = -180, hBinUpperXLim = 180;
    double hBinLowerYLim = 0, hBinUpperYLim = 50;

    int HistElectronSliceNumOfXBins;  // 100 by Default
    int HistElectronSliceNumOfYBins;  // 100 by Default
    int HistNucSliceNumOfXBins;       // 75 by Default
    int HistNucSliceNumOfYBins;       // 75 by Default
    int NumberNucOfMomSlices;
    int NumberElecOfMomSlices;

    double Nucleon_Momentum_Slice_Th = 0.4;
    double Electron_Momentum_Slice_Th = 0.8;  // Not th! but distance from beamE of last mom bin!

    /* Truth-level theta VS phi */
    std::vector<hPlot2D> truth_theta_e_VS_phi_e_BySlice, truth_theta_p_VS_phi_p_BySlice, truth_theta_n_VS_phi_n_BySlice;
    hPlot2D truth_theta_n_VS_phi_n;
    std::vector<TH1 *> truth_e_BySlice, truth_p_BySlice, truth_n_BySlice;

    /* Reco. Acceptance maps */
    std::vector<hPlot2D> reco_theta_e_VS_phi_e_BySlice, reco_theta_p_VS_phi_p_BySlice, reco_theta_n_VS_phi_n_BySlice;
    hPlot2D reco_theta_n_VS_phi_n;
    std::vector<TH1 *> reco_e_BySlice, reco_p_BySlice, reco_n_BySlice;

    /* Reco./TL ratio maps */
    std::vector<hPlot2D> acceptance_eff_e_BySlice, acceptance_eff_p_BySlice, acceptance_eff_n_BySlice;
    hPlot2D acceptance_eff_n;
    std::vector<TH1 *> acc_eff_e_BySlice, acc_eff_p_BySlice, acc_eff_n_BySlice;

    /* Separated acceptance & weight maps (= reco. above ratio th.) */
    std::vector<hPlot2D> filtered_reco_theta_e_VS_phi_e_BySlice, filtered_reco_theta_p_VS_phi_p_BySlice, filtered_reco_theta_n_VS_phi_n_BySlice;  // separated AMaps for each slice
    std::vector<TH1 *> filtered_reco_e_BySlice, filtered_reco_p_BySlice, filtered_reco_n_BySlice;

    /* Acceptance maps (= reco. above ratio th.) */
    hPlot2D filtered_reco_theta_e_VS_phi_e;  // combined electron AMap
    hPlot2D filtered_reco_theta_p_VS_phi_p;  // combined proton AMap
    hPlot2D filtered_reco_theta_n_VS_phi_n;
    hPlot2D filtered_reco_theta_nuc_VS_phi_nuc;

    /* Acceptance maps */
    std::vector<std::vector<int>> e_AMap;
    std::vector<std::vector<std::vector<int>>> e_AMap_Slices;
    std::vector<std::vector<int>> p_AMap;
    std::vector<std::vector<std::vector<int>>> p_AMap_Slices;
    std::vector<std::vector<int>> n_AMap;
    std::vector<std::vector<std::vector<int>>> n_AMap_Slices;
    std::vector<std::vector<int>> nuc_AMap;
    std::vector<std::vector<std::vector<int>>> nuc_AMap_Slices;
    std::vector<std::vector<std::vector<int>>> nuc_WMap_Slices;  // TODO: figure out if really need these!

    /* Weight maps */
    std::vector<std::vector<std::vector<double>>> e_WMap_Slices;
    std::vector<std::vector<std::vector<double>>> p_WMap_Slices;
    std::vector<std::vector<double>> n_WMap;
    std::vector<std::vector<std::vector<double>>> n_WMap_Slices;

    bool calc_Electron_RecoToTL_Ratio = true, calc_Proton_RecoToTL_Ratio = true, calc_Neutron_RecoToTL_Ratio = true;

    double Charged_particle_min_Ratio = 0.7;
    double Neutral_particle_min_Ratio = 0.2;

    TList *AcceptanceMapsBC = new TList();
    TList *TLAMaps = new TList();
    TList *RecoAMaps = new TList();
    TList *AMapsRatio = new TList();
    TList *Charged_particle_Sep_AMaps = new TList();
    TList *AcceptanceMaps = new TList();

    int pixelx = 1980, pixely = 1530;

    std::string AcceptanceMapsBC_OutFile0;
    std::string TLAMaps_OutFile0;
    std::string RecoAMaps_OutFile0;
    std::string AMapsRatio_OutFile0;
    std::string Charged_particle_Sep_AMaps_OutFile0;
    std::string AcceptanceMaps_OutFile0;

    std::string AMapSavePath, AMapCopySavePath;

    std::string AMapsBC_prefix = "00_AMaps_BC_-_";
    std::string AMap_TL_prefix = "01_AMap_TL_-_";
    std::string AMap_Reco_prefix = "02_AMap_Reco_-_";
    std::string AMap_Ratio_prefix = "03_AMap_Ratio_-_";
    std::string cPart_Sep_AMaps_prefix = "04_cPart_Sep_AMaps_-_";
    std::string AMaps_prefix = "05_AMaps_-_";

    std::vector<std::vector<double>> Loaded_ElectronMomSliceLimits;
    std::vector<std::vector<double>> Loaded_NucleonMomSliceLimits;

    /* Loaded acceptance maps */
    std::vector<std::vector<int>> Loaded_e_AMap;
    std::vector<std::vector<std::vector<int>>> Loaded_e_AMap_Slices;
    std::vector<std::vector<int>> Loaded_p_AMap;
    std::vector<std::vector<std::vector<int>>> Loaded_p_AMap_Slices;
    std::vector<std::vector<int>> Loaded_n_AMap;
    std::vector<std::vector<std::vector<int>>> Loaded_n_AMap_Slices;
    std::vector<std::vector<int>> Loaded_nuc_AMap;
    std::vector<std::vector<std::vector<int>>> Loaded_nuc_AMap_Slices;

    /* Loaded weight maps */
    std::vector<std::vector<std::vector<double>>> Loaded_e_WMap_Slices;
    std::vector<std::vector<std::vector<double>>> Loaded_p_WMap_Slices;
    std::vector<std::vector<int>> Loaded_n_WMap;
    std::vector<std::vector<std::vector<double>>> Loaded_n_WMap_Slices;

    /* Loaded Acceptance maps */
    // TODO: delete these histograms if the .par loading works
    std::vector<TH2 *> LoadedElectronAMaps, LoadedProtonAMaps;  // separated AMaps for each bin
    TH2D *LoadedElectronAMaps0;
    TH2D *LoadedProtonAMap;
    TH2D *LoadedNeutronAMap;
    TH2D *LoadedNucleonAMap;

    std::string SName;

    //    bool e_single_slice_test, nuc_single_slice_test;
    bool e_single_slice_test = false;    // keep as false for normal runs! (false by default)
    bool nuc_single_slice_test = false;  // keep as false for normal runs! (false by default)
    std::vector<int> Slices2Test;

   public:
    // default constructor --------------------------------------------------------------------------------------------------------------------------------------------------

    AMaps() = default;

    // constructor ----------------------------------------------------------------------------------------------------------------------------------------------------------

    // AMaps generation constructor:
    AMaps(const std::string &SampleName, const std::string &P_e_bin_profile, const std::string &P_nuc_bin_profile, double beamE, const std::string &AMapsMode = "", const std::string &SavePath = "./",
          int nOfNucMomBins = 4, int nOfElecMomBins = 4, int hnsNumOfXBins = 75, int hnsNumOfYBins = 75, int hesNumOfXBins = 100, int hesNumOfYBins = 100);

    // AMaps loading constructor:
    AMaps(const std::string &AcceptanceMapsDirectory, const std::string &SampleName, const bool &Electron_single_slice_test, const bool &Nucleon_single_slice_test, const std::vector<int> &TestSlices);

    // SetBins functions ----------------------------------------------------------------------------------------------------------------------------------------------------

    void SetBins(const std::string &P_nuc_bin_profile, double beamE);

    void SetElectronBins(const std::string &P_e_bin_profile, double beamE);

    void SetBins(double beamE, double NumberNucOfMomSlices);  // old

    // isElectron function --------------------------------------------------------------------------------------------------------------------------------------------------

    bool isElectron(const std::string &SampleType);

    // isProton function ----------------------------------------------------------------------------------------------------------------------------------------------------

    bool isProton(const std::string &SampleType);

    // isNeutron function ---------------------------------------------------------------------------------------------------------------------------------------------------

    bool isNeutron(const std::string &SampleType);

    // isTL function --------------------------------------------------------------------------------------------------------------------------------------------------------

    bool isTL(const std::string &SampleType);

    // isReco function --------------------------------------------------------------------------------------------------------------------------------------------------------

    bool isReco(const std::string &SampleType);

    // hFillHitMaps function ------------------------------------------------------------------------------------------------------------------------------------------------

    void hFillHitMaps(const std::string &SampleType, const std::string &particle, double Momentum, double Theta, double Phi, double Weight);

    // CalcAMapsRatio function --------------------------------------------------------------------------------------------------------------------------------------------

    void CalcAMapsRatio();

    // GenerateSeparateCPartAMaps function ----------------------------------------------------------------------------------------------------------------------------------

    void GenerateSeparateCPartAMaps(double cP_minR);

    // GenerateCPartAMaps function ------------------------------------------------------------------------------------------------------------------------------------------

    void GenerateCPartAMaps(double cP_minR);

    // GenerateNPartAMaps function ------------------------------------------------------------------------------------------------------------------------------------------

    void GenerateNPartAMaps(double nP_minR);

    // GenerateNucleonAMap function
    // -----------------------------------------------------------------------------------------------------------------------------------------

    void GenerateNucleonAMap();

    // SaveHitMaps function -------------------------------------------------------------------------------------------------------------------------------------------------

    void SaveHitMaps(const std::string &SampleName, const std::string &AcceptanceMapsDirectory);

    // DrawAndSaveHitMaps function ------------------------------------------------------------------------------------------------------------------------------------------

    void DrawAndSaveHitMapsPDFs(std::vector<TH1 *> HistoList, const std::string &PDFfileName);

    // DrawAndSaveHitMaps function ------------------------------------------------------------------------------------------------------------------------------------------

    void DrawAndSaveHitMaps(const std::string &SampleName, TCanvas *h1DCanvas, const std::string &AcceptanceMapsDirectory);

    // HistCounter function -------------------------------------------------------------------------------------------------------------------------------------------------

    int HistCounter(const char *fname);

    // SetHistBinsFromHistTitle function ------------------------------------------------------------------------------------------------------------------------------------

    void SetHistBinsFromHistTitle(TH2D *Histogram2D);

    // SetSlicesFromHistTitle function --------------------------------------------------------------------------------------------------------------------------------------

    void SetSlicesFromHistTitle(TH2D *Histogram2D, const std::string &Particle);

    void SetSlicesFromHistTitle(TH2D *Histogram2D, std::vector<std::vector<double>> MomBinsLimits);

    // ReadHitMaps function -------------------------------------------------------------------------------------------------------------------------------------------------

    void ReadHitMaps(const std::string &AcceptanceMapsDirectory, const std::string &SampleName);

    // ReadAMapLimits function ----------------------------------------------------------------------------------------------------------------------------------------------

    void ReadAMapLimits(const char *filename, std::vector<std::vector<double>> &Loaded_particle_limits);

    // ReadAMapSlices function ----------------------------------------------------------------------------------------------------------------------------------------------------

    void ReadAMapSlices(const std::string &SampleName, const std::string &AcceptanceMapsDirectory, const std::string &Particle, const std::vector<std::vector<double>> &Loaded_particle_limits,
                        std::vector<std::vector<std::vector<int>>> &Loaded_Particle_AMap_Slices);

    // ReadAMapSlices function ----------------------------------------------------------------------------------------------------------------------------------------------------

    void ReadWMapSlices(const std::string &SampleName, const std::string &AcceptanceMapsDirectory, const std::string &Particle, const std::vector<std::vector<double>> &Loaded_particle_limits,
                        std::vector<std::vector<std::vector<double>>> &Loaded_Particle_WMap_Slices);

    // ReadAMap function ----------------------------------------------------------------------------------------------------------------------------------------------------

    void ReadAMap(const char *filename, std::vector<std::vector<int>> &Loaded_particle_AMap);

    // ReadWMap function ----------------------------------------------------------------------------------------------------------------------------------------------------

    void ReadWMap(const char *filename, std::vector<std::vector<double>> &Loaded_particle_WMap);

    // MatchAngToHitMap function --------------------------------------------------------------------------------------------------------------------------------------------

    bool MatchAngToHitMap(const std::string &Particle, double Momentum, double Theta, double Phi, bool NucleonOverlappingFC = true);

    // GetWeight function ---------------------------------------------------------------------------------------------------------------------------------------------------

    double GetWeight(bool apply_kinematical_weights, const std::string &Particle, double Momentum, double Theta, double Phi);

    // IsInFDQuery function -------------------------------------------------------------------------------------------------------------------------------------------------

    bool IsInFDQuery(bool Generate_AMaps, const DSCuts &ThetaFD, const std::string &Particle, double Momentum, double Theta, double Phi, bool NucleonOverlappingFC = true);

    // Other methods --------------------------------------------------------------------------------------------------------------------------------------------------------

    // Set methods
    void SetNPartMinRatio(double npmr) { Neutral_particle_min_Ratio = npmr; }

    void SetCPartMinRatio(double cpmr) { Charged_particle_min_Ratio = cpmr; }

    // Get methods
    double GetNPartMinRatio() { return Neutral_particle_min_Ratio; }

    double GetCPartMinRatio() { return Charged_particle_min_Ratio; }

    double GetNucleonMomSliceLimitsSize() { return NucleonMomSliceLimits.size(); }

    double GetElectronNucleonMomSliceLimitsSize() { return ElectronMomSliceLimits.size(); }
};

#endif  // AMAPS_H
