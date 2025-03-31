//
// Created by alons on 20/06/2023.
//

#ifndef MOMENTUMRESOLUTION_H
#define MOMENTUMRESOLUTION_H
// #ifdef MOMENTUMRESOLUTION_H

#include <TApplication.h>
#include <TCanvas.h>
#include <TChain.h>
#include <TDatabasePDG.h>
#include <TF1.h>
#include <TFile.h>
#include <TFolder.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TLatex.h>
#include <TLorentzVector.h>
#include <TROOT.h>
#include <TRandom1.h>
#include <TRandom2.h>
#include <TRandom3.h>
#include <TStyle.h>
#include <TTree.h>
#include <math.h>

#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <istream>
#include <map>
#include <sstream>
#include <typeinfo>
#include <vector>

#include "clas12reader.h"
// #include "../../includes/clas12_include.h"

#if MomResDebugMacro
    #include "../hData/hData.h"
    #include "../hPlots/hPlot1D.cpp"
#else
    #include "../hPlots/hPlot1D.cpp"
#endif

#include "../../namespaces/general_utilities/utilities/utilities.h"
// #include "../../functions/GeneralFunctions.h"
#include "../DSCuts/DSCuts.h"

// using namespace std;
using namespace utilities;

class MomentumResolution {
   private:
    bool isNeutron = false, isProton = false;
    std::string MomResParticle = "";

    bool momResTestMode, momResS2CalcMode, momResS2RunMode;

    bool ForceSmallProtonResLimits = false;

    bool FitDebuggingMode = false;

    double Ebeam;

    std::string SmearMode = "NONE", CorrMode = "NONE";

    std::string RecoSliceFitAlgorithm = "", TLSliceFitAlgorithm = "";
    std::vector<hPlot1D> ResTLMomSlices, ResRecoMomSlices;
    std::vector<std::vector<double>> ResTLMomSlicesLimits, ResRecoMomSlicesLimits;
    std::vector<DSCuts> ResTLMomSlicesFitVar, ResRecoMomSlicesFitVar;
    std::vector<DSCuts> ResTLMomSlicesHistVar, ResRecoMomSlicesHistVar;
    std::vector<DSCuts> Loaded_Res_Slices_FitVar;
    std::vector<DSCuts> Loaded_Res_Slices_HistVar;
    std::vector<int> FittedTLMomSlices, FittedRecoMomSlices;

    double SliceUpperMomLim;                                    // upper lim for momentum slices - loaded from file
    double SliceUpperMomLimKC_mu, SliceLowerMomLimKC_mu;        // lower lim for momentum slices - set by constructor
    double SliceUpperMomLimKC_sigma, SliceLowerMomLimKC_sigma;  // lower lim for momentum slices - set by constructor

    double hSliceUpperLim = 1.1;
    double hSliceLowerLim = -1.1;
    int hSliceNumOfBin = 65;

    TList *ResSlicePlots = new TList();
    TFolder *FittedTLNeutronResSlices = new TFolder("Fitted TL neutron resolution slices", "Fitted TL neutron resolution slices");
    TFolder *FittedTLNeutronResSlicesMean = new TFolder("Fitted TL neutron resolution slice mean", "Fitted TL neutron resolution slice mean");
    TFolder *FittedTLNeutronResSlicesWidth = new TFolder("Fitted TL neutron resolution slice width", "Fitted TL neutron resolution slice width");
    TFolder *FittedTLProtonResSlices = new TFolder("Fitted TL proton resolution slices", "Fitted TL proton resolution slices");
    TFolder *FittedTLProtonResSlicesMean = new TFolder("Fitted TL proton resolution slice mean", "Fitted TL proton resolution slice mean");
    TFolder *FittedTLProtonResSlicesWidth = new TFolder("Fitted TL proton resolution slice width", "Fitted TL proton resolution slice width");
    TFolder *FittedRecoNeutronResSlices = new TFolder("Fitted reco neutron resolution slices", "Fitted reco neutron resolution slices");
    TFolder *FittedRecoNeutronResSlicesMean = new TFolder("Fitted reco neutron resolution slice mean", "Fitted reco neutron resolution slice mean");
    TFolder *FittedRecoNeutronResSlicesWidth = new TFolder("Fitted reco neutron resolution slice width", "Fitted reco neutron resolution slice width");
    TFolder *FittedRecoProtonResSlices = new TFolder("Fitted reco proton resolution slices", "Fitted reco proton resolution slices");
    TFolder *FittedRecoProtonResSlicesMean = new TFolder("Fitted reco proton resolution slice mean", "Fitted reco proton resolution slice mean");
    TFolder *FittedRecoProtonResSlicesWidth = new TFolder("Fitted reco proton resolution slice width", "Fitted reco proton resolution slice width");

    std::string ResSlicePlots_OutFile0;

    //<editor-fold desc="Random number generators">
    std::string RandomNumGenerator = "TRandom3";  // TRandom3 by default

    TRandom1 *Rand1 = new TRandom1();
    TRandom2 *Rand2 = new TRandom2();
    TRandom3 *Rand3 = new TRandom3();
    //</editor-fold>

    double delta, deltaFactor = 1.;
    int TL_NumberOfSlices = 0, Reco_NumberOfSlices = 0;

    //<editor-fold desc="Correction and smear fit variables">
    /* General vector structure:   {{A, B, ...}, {A_Err, B_Err, ...}, {ChiSquare_Smear_pol2_wKC, NDF_Smear_pol2_wKC},} */

    //<editor-fold desc="Correction and smear fit variables">
    std::vector<std::vector<double>> TL_FitParam_Smear_pol1, Reco_FitParam_Smear_pol1;
    std::vector<std::vector<double>> TL_FitParam_Smear_pol1_wKC, Reco_FitParam_Smear_pol1_wKC;

    std::vector<std::vector<double>> TL_FitParam_Smear_pol2, Reco_FitParam_Smear_pol2;
    std::vector<std::vector<double>> TL_FitParam_Smear_pol2_wKC, Reco_FitParam_Smear_pol2_wKC;

    std::vector<std::vector<double>> TL_FitParam_Smear_pol3, Reco_FitParam_Smear_pol3;
    std::vector<std::vector<double>> TL_FitParam_Smear_pol3_wKC, Reco_FitParam_Smear_pol3_wKC;

    std::vector<std::vector<double>> TL_FitParam_Corr_pol1, Reco_FitParam_Corr_pol1;
    std::vector<std::vector<double>> TL_FitParam_Corr_pol1_wKC, Reco_FitParam_Corr_pol1_wKC;

    std::vector<std::vector<double>> TL_FitParam_Corr_pol2, Reco_FitParam_Corr_pol2;
    std::vector<std::vector<double>> TL_FitParam_Corr_pol2_wKC, Reco_FitParam_Corr_pol2_wKC;

    std::vector<std::vector<double>> TL_FitParam_Corr_pol3, Reco_FitParam_Corr_pol3;
    std::vector<std::vector<double>> TL_FitParam_Corr_pol3_wKC, Reco_FitParam_Corr_pol3_wKC;
    //</editor-fold>

    //<editor-fold desc="Loaded correction and smear fit variables">
    std::string Loaded_Std_coefficients_path;
    std::vector<double> Loaded_Smear_coefficients_values;
    std::vector<string> Loaded_Smear_coefficients_names;

    std::vector<std::vector<double>> Loaded_TL_FitParam_Smear_pol1, Loaded_Reco_FitParam_Smear_pol1;
    std::vector<std::vector<double>> Loaded_TL_FitParam_Smear_pol1_wKC, Loaded_Reco_FitParam_Smear_pol1_wKC;
    std::vector<std::vector<double>> Loaded_TL_FitParam_Smear_pol2, Loaded_Reco_FitParam_Smear_pol2;
    std::vector<std::vector<double>> Loaded_TL_FitParam_Smear_pol2_wKC, Loaded_Reco_FitParam_Smear_pol2_wKC;
    std::vector<std::vector<double>> Loaded_TL_FitParam_Smear_pol3, Loaded_Reco_FitParam_Smear_pol3;
    std::vector<std::vector<double>> Loaded_TL_FitParam_Smear_pol3_wKC, Loaded_Reco_FitParam_Smear_pol3_wKC;

    std::string Loaded_Corr_coefficients_path;
    std::vector<double> Loaded_Corr_coefficients_values;
    std::vector<string> Loaded_Corr_coefficients_names;

    std::vector<std::vector<double>> Loaded_TL_FitParam_Corr_pol1, Loaded_Reco_FitParam_Corr_pol1;
    std::vector<std::vector<double>> Loaded_TL_FitParam_Corr_pol1_wKC, Loaded_Reco_FitParam_Corr_pol1_wKC;
    std::vector<std::vector<double>> Loaded_TL_FitParam_Corr_pol2, Loaded_Reco_FitParam_Corr_pol2;
    std::vector<std::vector<double>> Loaded_TL_FitParam_Corr_pol2_wKC, Loaded_Reco_FitParam_Corr_pol2_wKC;
    std::vector<std::vector<double>> Loaded_TL_FitParam_Corr_pol3, Loaded_Reco_FitParam_Corr_pol3;
    std::vector<std::vector<double>> Loaded_TL_FitParam_Corr_pol3_wKC, Loaded_Reco_FitParam_Corr_pol3_wKC;
    //</editor-fold>

    //</editor-fold>

    double Neutron_Momentum_cut = 9999.;  // from clas12ana
    std::string SName;

    std::string SlicesSavePath;
    std::string SlicesSaveNamePrefix;

   public:
    // Default constructor --------------------------------------------------------------------------------------------------------------------------------------------------

    MomentumResolution() = default;

    // Other constructors ---------------------------------------------------------------------------------------------------------------------------------------------------

    MomentumResolution(const std::string &Particle);

    // MomResInit function --------------------------------------------------------------------------------------------------------------------------------------------------

    void MomResInit(const bool &plot_and_fit_MomRes, const bool &Calculate_momResS2, const bool &Run_with_momResS2, const std::string &SampleName, const std::string &NucleonCutsDirectory,
                    const double &beamE, const DSCuts &MomRes_mu_cuts, const DSCuts &MomRes_sigma_cuts, const double &ParticleMomTh, const std::string &MomentumResolutionDirectory,
                    const std::string &SavePath = "./", const double &DeltaSlices = 0.2, const bool &VaryingDelta = false, const std::string &SmearM = "pol1",
                    const std::string &CorrM = "pol1", const bool &momRes_test = false, const bool &ForceSmallpResLimits = false, const bool &FitDebugging = false);

    void SetMomResCalculations(const std::string &SampleName, const std::string &NucleonCutsDirectory, const double &beamE, const DSCuts &MomRes_mu_cuts, const DSCuts &MomRes_sigma_cuts,
                               const double &ParticleMomTh, bool const &Calculate_momResS2, bool const &Run_in_momResS2, const std::string &MomentumResolutionDirectory,
                               const std::string &SavePath = "./", const double &DeltaSlices = 0.2, const bool &VaryingDelta = false, const std::string &SmearM = "pol1",
                               const std::string &CorrM = "pol1", const bool momRes_test = false, const bool ForceSmallpResLimits = false, const bool &FitDebugging = false);

    void SetMomResSlicesByType(const std::string &SampleName, const std::string &NucleonCutsDirectory, const double &beamE, const double &ParticleMomTh, const std::string &MomentumType,
                               const std::string &SavePath = "./", const bool &VaryingDelta = false, const bool &momRes_test = false, const bool &ForceSmallpResLimits = false,
                               const bool &FitDebugging = false);

    void SetMomResSlices(const std::string &SampleName, const std::string &NucleonCutsDirectory, const double &beamE, const double &ParticleMomTh, const std::string &MomentumType,
                         const std::string &SavePath, const bool &VaryingDelta, const bool &momRes_test, const bool &ForceSmallpResLimits, std::vector<hPlot1D> &ResSlices0,
                         std::vector<std::vector<double>> &ResSlicesLimits0, std::vector<DSCuts> &ResSlicesFitVar0, std::vector<DSCuts> &ResSlicesHistVar0, int &NumberOfSlices0,
                         const bool &FitDebugging = false);

    void SetUpperMomCut(const std::string &SampleName, const std::string &NucleonCutsDirectory, const bool &FitDebugging = false);

    void LoadFitParam(const std::string &SampleName, const std::string &NucleonCutsDirectory, bool const &Calculate_momResS2, const std::string &MomentumResolutionDirectory);

    // ReadInputParam function ----------------------------------------------------------------------------------------------------------------------------------------------

    void ReadInputParam(const char *filename);

    // hFillResPlotsByType function -----------------------------------------------------------------------------------------------------------------------------------------

    void hFillResPlotsByType(const double &MomentumTL, const double &MomentumReco, const double &Resolution, const double &Weight);

    void hFillResPlots(const double &Momentum, const double &Resolution, const double &Weight, std::vector<hPlot1D> &ResSlices0, std::vector<std::vector<double>> &ResSlicesLimits0,
                       int &NumberOfSlices0);

    // SliceFitDrawAndSaveByType function -----------------------------------------------------------------------------------------------------------------------------------

    void SliceFitDrawAndSaveByType(const std::string &SampleName, const double &beamE);

    void SliceFitDrawAndSave(const std::string &SampleName, const double &beamE, const std::string &MomentumType, std::vector<hPlot1D> &ResSlices0,
                             std::vector<std::vector<double>> &ResSlicesLimits0, std::vector<DSCuts> &ResSlicesFitVar0, std::vector<DSCuts> &ResSlicesHistVar0,
                             std::vector<int> &FittedSlices0, int &NumberOfSlices0);

    void PolyFitter(const std::string &MomentumType, const int &PolynomialDegree, const std::string &FitType, const std::string &MomentumFitRange,
                    std::vector<std::vector<double>> &FitParam);

    // DrawAndSaveResSlices function ----------------------------------------------------------------------------------------------------------------------------------------

    void DrawAndSaveResSlices(const std::string &SampleName, TCanvas *h1DCanvas, const std::string &plots_path, const std::string &MomentumResolutionDirectory);

    // LogResDataToFile function --------------------------------------------------------------------------------------------------------------------------------------------

    void LogResDataToFile(const std::string &SampleName, const std::string &plots_path, const std::string &MomentumResolutionDirectory);

    // LogFitDataToFile function --------------------------------------------------------------------------------------------------------------------------------------------

    void LogFitDataToFile(const std::string &SampleName, const std::string &plots_path, const std::string &MomentumResolutionDirectory);

    void AutoLogger(const std::string &SampleName, const std::string &plots_path, const std::string &MomentumResolutionDirectory, const std::string &LogHeader,
                    const std::vector<std::vector<double>> &Vector2Log, std::ofstream &Neutron_res_fit_param);

    // LogHistDataToFile function -------------------------------------------------------------------------------------------------------------------------------------------

    void LogHistDataToFile(const std::string &SampleName, const std::string &plots_path, const std::string &MomentumResolutionDirectory);

    // ReadResDataParam function --------------------------------------------------------------------------------------------------------------------------------------------

    void ReadResDataParam(const char *filename, const bool &Calculate_momResS2, const std::string &SampleName, const std::string &NucleonCutsDirectory, const std::string &MomentumType,
                          const bool &Load_correction = false, const bool &Load_smearing = false);

    void AutoReader(const std::string &MomentumType, const int &PolynomialDegree, const std::string &parameter, std::basic_istream<char> &ss2, std::vector<std::vector<double>> &Loading_Dest,
                    std::vector<double> &Loaded_coefficients_values, std::vector<string> &Loaded_coefficients_names);

    void AutoReader(const std::string &MomentumType, const int &PolynomialDegree, const std::string &parameter, std::basic_istream<char> &ss2,
                    std::vector<std::vector<double>> &Loading_Dest);

    // PSmear function ------------------------------------------------------------------------------------------------------------------------------------------------------

    double PSmear(const bool &apply_nucleon_SmearAndCorr, const double &Momentum);

    // NCorr function -------------------------------------------------------------------------------------------------------------------------------------------------------

    double NCorr(const bool &apply_nucleon_SmearAndCorr, const double &Momentum);

    // Other functions ------------------------------------------------------------------------------------------------------------------------------------------------------

    // Set functions
    void SetHistSliceUpperLim(double hsUpperLim) { hSliceUpperLim = hsUpperLim; };

    void SetHistSliceLowerLim(double hsLowerLim) { hSliceLowerLim = hsLowerLim; };

    void SetSmearMode(const std::string &SmearM) { SmearMode = SmearM; };

    void SetCorrMode(const std::string &ShiftM) { CorrMode = ShiftM; };

    void SetSmearAndCorrModes(const std::string &SmearM, const std::string &ShiftM) { SmearMode = SmearM, CorrMode = ShiftM; };

    void SetForceSmallpResLimits(const bool &fsprl) { ForceSmallProtonResLimits = fsprl; };

    void OverwriteResRecoMomSlices(std::vector<TH1D *> ResSlices0) {
        for (int i = 0; i < ResRecoMomSlices.size(); i++) { ResRecoMomSlices.at(i).SetHistogram1D(ResSlices0.at(i)); }
    }

    void OverwriteResTLMomSlices(std::vector<TH1D *> ResSlices0) {
        for (int i = 0; i < ResTLMomSlices.size(); i++) { ResTLMomSlices.at(i).SetHistogram1D(ResSlices0.at(i)); }
    }

    void SetResRecoMomSlicesLimits(std::vector<std::vector<double>> ResSlicesLimits0) { ResRecoMomSlicesLimits = ResSlicesLimits0; };

    void SetResTLMomSlicesLimits(std::vector<std::vector<double>> ResSlicesLimits0) { ResTLMomSlicesLimits = ResSlicesLimits0; };

    void SetResRecoMomSlicesFitVar(std::vector<DSCuts> ResSlicesFitVar0) { ResRecoMomSlicesFitVar = ResSlicesFitVar0; };

    void SetResTLMomSlicesFitVar(std::vector<DSCuts> ResSlicesFitVar0) { ResTLMomSlicesFitVar = ResSlicesFitVar0; };

    void SetResRecoMomSlicesHistVar(std::vector<DSCuts> ResSlicesHistVar0) { ResRecoMomSlicesHistVar = ResSlicesHistVar0; };

    void SetResTLMomSlicesHistVar(std::vector<DSCuts> ResSlicesHistVar0) { ResTLMomSlicesHistVar = ResSlicesHistVar0; };

    void SetFittedRecoMomSlices(std::vector<int> FittedSlices0) { FittedRecoMomSlices = FittedSlices0; };

    void SetFittedTLMomSlices(std::vector<int> FittedSlices0) { FittedTLMomSlices = FittedSlices0; };

    // Get functions
    double GetSliceUpperMomLim() { return SliceUpperMomLim; };

    std::string Get_SmearMode() { return SmearMode; };

    std::string Get_Loaded_Std_coefficients_path() { return Loaded_Std_coefficients_path; };

    std::vector<double> Get_Loaded_Smear_coefficients_values() { return Loaded_Smear_coefficients_values; };

    std::vector<string> Get_Loaded_Smear_coefficients_names() { return Loaded_Smear_coefficients_names; };

    std::string Get_CorrMode() { return CorrMode; };

    std::string Get_Loaded_Corr_coefficients_path() { return Loaded_Corr_coefficients_path; };

    std::vector<double> Get_Loaded_Corr_coefficients_values() { return Loaded_Corr_coefficients_values; };

    std::vector<string> Get_Loaded_Corr_coefficients_names() { return Loaded_Corr_coefficients_names; };

    std::vector<std::vector<double>> GetResRecoMomSlicesLimits() { return ResRecoMomSlicesLimits; };

    std::vector<std::vector<double>> GetResTLMomSlicesLimits() { return ResTLMomSlicesLimits; };

    std::vector<DSCuts> GetResRecoMomSlicesFitVar() { return ResRecoMomSlicesFitVar; };

    std::vector<DSCuts> GetResTLMomSlicesFitVar() { return ResTLMomSlicesFitVar; };

    std::vector<DSCuts> GetResRecoMomSlicesHistVar() { return ResRecoMomSlicesHistVar; };

    std::vector<DSCuts> GetResTLMomSlicesHistVar() { return ResTLMomSlicesHistVar; };

    std::vector<int> GetFittedRecoMomSlices() { return FittedRecoMomSlices; };

    std::vector<int> GetFittedTLMomSlices() { return FittedTLMomSlices; };

    std::string GetRandNumGenerator() { return RandomNumGenerator; };
};

#endif  // MOMENTUMRESOLUTION_H
