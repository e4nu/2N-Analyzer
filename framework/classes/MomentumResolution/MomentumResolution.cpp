//
// Created by Alon Sportes on 20/06/2023.
//

#ifndef MOMENTUMRESOLUTION_H
#define MOMENTUMRESOLUTION_H

#include "MomentumResolution.h"

// MomentumResolution constructor -----------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* MomentumResolution constructor */
/* A constructor that sets the particle (neutron or proton) */
MomentumResolution::MomentumResolution(const std::string &Particle) {
    bool PrintOut = false;

    if (Particle == "Neutron") {
        isNeutron = true, isProton = false;
        MomResParticle = Particle;

        if (PrintOut) {
            std::cout << "\nParticle = " << Particle << "\n";
            std::cout << "MomResParticle = " << MomResParticle << "\n";
        }
    } else if (Particle == "Proton") {
        isNeutron = false, isProton = true;
        MomResParticle = Particle;

        if (PrintOut) {
            std::cout << "\nParticle = " << Particle << "\n";
            std::cout << "MomResParticle = " << MomResParticle << "\n";
        }
    } else {
        std::cout << "MomentumResolution::MomentumResolution: entered particle is illegal! Aborting...\n\n", exit(1);
    }

#pragma region /* Safety checks */
    if (isNeutron == isProton) { std::cout << "MomentumResolution::MomentumResolution: unclear nucleon selection! Aborting...\n\n", exit(1); }

    if (MomResParticle != "Neutron" && MomResParticle != "Proton") {
        std::cout << "\nMomentumResolution::MomentumResolution: nucleon have not been set!\n";
        std::cout << "Particle = " << Particle << "\n";
        std::cout << "MomResParticle = " << MomResParticle << "\n";
        std::cout << "Aborting...\n\n", exit(1);
    }
#pragma endregion
}
#pragma endregion

// MomResInit function ---------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* MomResInit function */
void MomentumResolution::MomResInit(const bool &plot_and_fit_MomRes, const bool &Calculate_momResS2, const bool &Run_with_momResS2, const std::string &SampleName,
                                    const std::string &NucleonCutsDirectory, const double &beamE, const DSCuts &MomRes_mu_cuts, const DSCuts &MomRes_sigma_cuts, const double &ParticleMomTh,
                                    const std::string &MomentumResolutionDirectory, const std::string &SavePath, const double &DeltaSlices, const bool &VaryingDelta,
                                    const std::string &SmearM, const std::string &CorrM, const bool &momRes_test, const bool &ForceSmallpResLimits, const bool &FitDebugging) {
    if (isNeutron) {
        if (plot_and_fit_MomRes) {
            MomResHistoListPDFFileName = SavePath + "/" + "Neutron_ResSlicePlots.pdf";

            SetMomResCalculations(SampleName, NucleonCutsDirectory, beamE, MomRes_mu_cuts, MomRes_sigma_cuts, ParticleMomTh, Calculate_momResS2, Run_with_momResS2,
                                  MomentumResolutionDirectory, SavePath, DeltaSlices, VaryingDelta, SmearM, CorrM, momRes_test, false, FitDebugging);

            if (momRes_test) {
                if (Calculate_momResS2) {  // if Calculate_momResS2=true => load everything from momResS1 files
                    /* Load neutron correction fit parameters (correction factor from momResS1 reco fits!) */
                    ReadResDataParam((MomentumResolutionDirectory + "Res_data_-_" + SampleName + "/Neutron_momResS1_fit_param_-_" + SampleName + ".par").c_str(), Calculate_momResS2,
                                     SampleName, NucleonCutsDirectory, "reco", true, false);

                    /* Load proton smearing fit parameters (smearing from momResS1 reco fits!) */
                    // TODO: figure out if I really need to load these at this stage
                    ReadResDataParam((MomentumResolutionDirectory + "Res_data_-_" + SampleName + "/Neutron_momResS1_fit_param_-_" + SampleName + ".par").c_str(), Calculate_momResS2,
                                     SampleName, NucleonCutsDirectory, "reco", false, true);
                } else {                      // if Calculate_momResS2=false => load everything from either momResS1 or momResS2
                    if (Run_with_momResS2) {  // if Calculate_momResS2=false && Run_with_momResS2=true => load correction from momResS1 and smearing from momResS2
                        /* Load neutron correction fit parameters (correction factor from momResS1 reco fits!) */
                        ReadResDataParam((MomentumResolutionDirectory + "Res_data_-_" + SampleName + "/Neutron_momResS1_fit_param_-_" + SampleName + ".par").c_str(), Calculate_momResS2,
                                         SampleName, NucleonCutsDirectory, "reco", true, false);

                        /* Load proton smearing fit parameters (smearing from momResS2 reco fits!) */
                        ReadResDataParam((MomentumResolutionDirectory + "Res_data_-_" + SampleName + "/Neutron_momResS2_fit_param_-_" + SampleName + ".par").c_str(), Calculate_momResS2,
                                         SampleName, NucleonCutsDirectory, "reco", false, true);
                    } else {  // if Calculate_momResS2=false and Run_with_momResS2=false => load both correction and smearing from momResS1
                        /* Load neutron correction fit parameters (correction factor from momResS1 reco fits!) */
                        ReadResDataParam((MomentumResolutionDirectory + "Res_data_-_" + SampleName + "/Neutron_momResS1_fit_param_-_" + SampleName + ".par").c_str(), Calculate_momResS2,
                                         SampleName, NucleonCutsDirectory, "reco", true, false);

                        /* Load proton smearing fit parameters (smearing from momResS1 reco fits!) */
                        ReadResDataParam((MomentumResolutionDirectory + "Res_data_-_" + SampleName + "/Neutron_momResS1_fit_param_-_" + SampleName + ".par").c_str(), Calculate_momResS2,
                                         SampleName, NucleonCutsDirectory, "reco", false, true);
                    }
                }
            }
        } else {  // if plot_and_fit_MomRes=false => Calculate_momResS2=false !!!
            SetSmearAndCorrModes(SmearM, CorrM);

            if (Run_with_momResS2) {  // if Run_with_momResS2=true => load correction from momResS1 and smearing from momResS2
                std::string NeutronCorrectionDataFile = MomentumResolutionDirectory + "Res_data_-_" + SampleName + "/Neutron_momResS1_fit_param_-_" + SampleName + ".par";
                std::string ProtonSmearingDataFile = MomentumResolutionDirectory + "Res_data_-_" + SampleName + "/Neutron_momResS2_fit_param_-_" + SampleName + ".par";

                /* Load neutron correction fit parameters (correction factor from momResS1 reco fits!) */
                ReadResDataParam(NeutronCorrectionDataFile.c_str(), Calculate_momResS2, SampleName, NucleonCutsDirectory, "reco", true, false);

                /* Load proton smearing fit parameters (smearing from momResS2 reco fits!) */
                ReadResDataParam(ProtonSmearingDataFile.c_str(), Calculate_momResS2, SampleName, NucleonCutsDirectory, "reco", false, true);

#pragma region /* Safety checks for data files */
                if (!FindSubstring(NeutronCorrectionDataFile, "Neutron") || FindSubstring(NeutronCorrectionDataFile, "Proton")) {
                    std::cout << "\n\nMomentumResolution::MomentumResolution: neutron correction variables are not being loaded from neutron data! "
                                 "Aborting...\n\n",
                        exit(1);
                }

                if (!FindSubstring(ProtonSmearingDataFile, "Neutron") || FindSubstring(ProtonSmearingDataFile, "Proton")) {
                    std::cout << "\n\nMomentumResolution::MomentumResolution: proton smearing variables are not being loaded from neutron data! "
                                 "Aborting...\n\n",
                        exit(1);
                }
#pragma endregion
            } else {  // if Calculate_momResS2 = false and Run_with_momResS2 = false => load both correction and smearing from momResS1
                std::string NeutronCorrectionDataFile = MomentumResolutionDirectory + "Res_data_-_" + SampleName + "/Neutron_momResS1_fit_param_-_" + SampleName + ".par";
                std::string ProtonSmearingDataFile = MomentumResolutionDirectory + "Res_data_-_" + SampleName + "/Neutron_momResS1_fit_param_-_" + SampleName + ".par";

                /* Load neutron correction fit parameters (correction factor from momResS1 reco fits!) */
                ReadResDataParam(NeutronCorrectionDataFile.c_str(), Calculate_momResS2, SampleName, NucleonCutsDirectory, "reco", true, false);

                /* Load proton smearing fit parameters (smearing from momResS2 reco fits!) */
                ReadResDataParam(ProtonSmearingDataFile.c_str(), Calculate_momResS2, SampleName, NucleonCutsDirectory, "reco", false, true);

#pragma region /* Safety checks for data files */
                if (!FindSubstring(NeutronCorrectionDataFile, "Neutron") || FindSubstring(NeutronCorrectionDataFile, "Proton")) {
                    std::cout << "\n\nMomentumResolution::MomentumResolution: neutron correction variables are not being loaded from neutron data! Aborting...\n\n", exit(1);
                }

                if (!FindSubstring(ProtonSmearingDataFile, "Neutron") || FindSubstring(ProtonSmearingDataFile, "Proton")) {
                    std::cout << "\n\nMomentumResolution::MomentumResolution: proton smearing variables are not being loaded from neutron data! Aborting...\n\n", exit(1);
                }
#pragma endregion
            }
        }
    } else if (isProton) {
        if (plot_and_fit_MomRes) {
            MomResHistoListPDFFileName = SavePath + "/" + "Proton_ResSlicePlots.pdf";

            SetMomResCalculations(SampleName, NucleonCutsDirectory, beamE, MomRes_mu_cuts, MomRes_sigma_cuts, ParticleMomTh, Calculate_momResS2, Run_with_momResS2,
                                  MomentumResolutionDirectory, SavePath, DeltaSlices, VaryingDelta, SmearM, CorrM, momRes_test, ForceSmallpResLimits, FitDebugging);
        }
    }
}
#pragma endregion

#pragma region /* SetMomResCalculations function */
void MomentumResolution::SetMomResCalculations(const std::string &SampleName, const std::string &NucleonCutsDirectory, const double &beamE, const DSCuts &MomRes_mu_cuts,
                                               const DSCuts &MomRes_sigma_cuts, const double &ParticleMomTh, bool const &Calculate_momResS2, bool const &Run_in_momResS2,
                                               const std::string &MomentumResolutionDirectory, const std::string &SavePath, const double &DeltaSlices, const bool &VaryingDelta,
                                               const std::string &SmearM, const std::string &CorrM, const bool momRes_test, const bool ForceSmallpResLimits, const bool &FitDebugging) {
    SliceUpperMomLimKC_mu = MomRes_mu_cuts.GetUpperCutConst(), SliceLowerMomLimKC_mu = MomRes_mu_cuts.GetLowerCutConst();
    SliceUpperMomLimKC_sigma = MomRes_sigma_cuts.GetUpperCutConst(), SliceLowerMomLimKC_sigma = MomRes_sigma_cuts.GetLowerCutConst();
    momResS2CalcMode = Calculate_momResS2, momResS2RunMode = Run_in_momResS2;
    SlicesSavePath = SavePath;
    delta = DeltaSlices;
    SmearMode = SmearM, CorrMode = CorrM;
    momResTestMode = momRes_test;
    ForceSmallProtonResLimits = ForceSmallpResLimits;
    FitDebuggingMode = FitDebugging;
    Ebeam = beamE;

#pragma region /* Safety check */
    if (momResS2CalcMode && momResS2RunMode) { std::cout << "MomentumResolution::MomentumResolution: calculating and running on momResS2 is illegal! Aborting...\n\n", exit(1); }
#pragma endregion

    SetMomResSlicesByType(SampleName, NucleonCutsDirectory, beamE, ParticleMomTh, "truth", SavePath, VaryingDelta, momRes_test, ForceSmallpResLimits, FitDebugging);
    SetMomResSlicesByType(SampleName, NucleonCutsDirectory, beamE, ParticleMomTh, "reco", SavePath, VaryingDelta, momRes_test, ForceSmallpResLimits, FitDebugging);

    if (!FitDebugging) { LoadFitParam(SampleName, NucleonCutsDirectory, Calculate_momResS2, MomentumResolutionDirectory); }
}
#pragma endregion

#pragma region /* SetMomResSlicesByType function */
void MomentumResolution::SetMomResSlicesByType(const std::string &SampleName, const std::string &NucleonCutsDirectory, const double &beamE, const double &ParticleMomTh,
                                               const std::string &MomentumType, const std::string &SavePath, const bool &VaryingDelta, const bool &momRes_test,
                                               const bool &ForceSmallpResLimits, const bool &FitDebugging) {
    if (MomentumType == "truth") {
        SetMomResSlices(SampleName, NucleonCutsDirectory, beamE, ParticleMomTh, MomentumType, SavePath, VaryingDelta, momRes_test, ForceSmallpResLimits, ResTLMomSlices, ResTLMomSlicesLimits,
                        ResTLMomSlicesFitVar, ResTLMomSlicesHistVar, TL_NumberOfSlices, FitDebugging);
    } else if (MomentumType == "reco") {
        SetMomResSlices(SampleName, NucleonCutsDirectory, beamE, ParticleMomTh, MomentumType, SavePath, VaryingDelta, momRes_test, ForceSmallpResLimits, ResRecoMomSlices,
                        ResRecoMomSlicesLimits, ResRecoMomSlicesFitVar, ResRecoMomSlicesHistVar, Reco_NumberOfSlices, FitDebugging);
    }
}
#pragma endregion

#pragma region /* SetMomResSlices function */
void MomentumResolution::SetMomResSlices(const std::string &SampleName, const std::string &NucleonCutsDirectory, const double &beamE, const double &ParticleMomTh,
                                         const std::string &MomentumType, const std::string &SavePath, const bool &VaryingDelta, const bool &momRes_test, const bool &ForceSmallpResLimits,
                                         vector<hPlot1D> &ResSlices0, vector<vector<double>> &ResSlicesLimits0, vector<DSCuts> &ResSlicesFitVar0, vector<DSCuts> &ResSlicesHistVar0,
                                         int &NumberOfSlices0, const bool &FitDebugging) {
    double Delta = delta, SliceLowerLim = ParticleMomTh, SliceUpperLim;
    SetUpperMomCut(SampleName, NucleonCutsDirectory, FitDebugging);

    if (!VaryingDelta) {
        SliceUpperLim = SliceLowerLim + delta;
    } else {
        if (MomentumType == "truth") {
            if (beamE == 5.98636) {
                if ((SampleName == "C12_simulation_G18_Q204_6GeV") || (SampleName == "C12x4_simulation_G18_Q204_6GeV")) {
                    SliceUpperLim = SliceLowerLim + 0.25;
                } else {
                    SliceUpperLim = SliceLowerLim + 0.3;
                }
            } else if (beamE == 2.07052) {
                SliceUpperLim = SliceLowerLim + 0.15;
            }
        } else if (MomentumType == "reco") {
            if (beamE == 5.98636) {
                if (SampleName == "C12x4_simulation_G18_Q204_6GeV") { SliceUpperLim = SliceLowerLim + 0.2; }
            }
        }
    }

    bool SliceAndDice = true;
    int SliceNumber = 0;

    /* Variables for debugging purposes: */
    bool TLLimitsPrintOut = false;
    bool TLLimitsPrintOutAndExit = false;
    bool RecoLimitsPrintOut = false;
    bool RecoLimitsPrintOutAndExit = false;

    while (SliceAndDice) {
        ++SliceNumber;

        if (TLLimitsPrintOut && (MomentumType == "truth")) {
            std::cout << "\n\nMomResParticle = " << MomResParticle << " (MomentumType == " << MomentumType << ")\n";
            std::cout << "SliceLowerLim = " << SliceLowerLim << " (MomentumType == " << MomentumType << ")\n";
            std::cout << "SliceUpperLim = " << SliceUpperLim << " (MomentumType == " << MomentumType << ")\n";
        }

        if (RecoLimitsPrintOut && (MomentumType == "reco")) {
            std::cout << "\n\nMomResParticle = " << MomResParticle << " (MomentumType == " << MomentumType << ")\n";
            std::cout << "SliceLowerLim = " << SliceLowerLim << " (MomentumType == " << MomentumType << ")\n";
            std::cout << "SliceUpperLim = " << SliceUpperLim << " (MomentumType == " << MomentumType << ")\n";
        }

#pragma region /* Safety check */
        if ((abs(SliceUpperLim) > beamE * 1.5) || (abs(SliceLowerLim) > beamE * 1.5)) {
            std::cout << "\n\nMomentumResolution::SetMomResSlices: slice limits are not defined properly! Aborting...\n\n", exit(1);
        }
#pragma endregion

        int SliceUpperLimPrecision;
        if (SliceUpperLim == beamE) {
            SliceUpperLimPrecision = 3;
        } else {
            SliceUpperLimPrecision = 2;
        }

        std::string hStatsTitle, hTitle, hSaveName, hCutName;

        hPlot1D hResolutionSlice;

        DSCuts ResSliceFitCuts;

        if (isNeutron) {
            hStatsTitle =
                "n res. - " + ToStringWithPrecision(SliceLowerLim, 2) + "#leqP^{" + MomentumType + "}_{nFD}#leq" + ToStringWithPrecision(SliceUpperLim, SliceUpperLimPrecision) + " [GeV/c]";
            hTitle = "Neutron resolution for " + ToStringWithPrecision(SliceLowerLim, 2) + "#leqP^{" + MomentumType + "}_{nFD}#leq" +
                     ToStringWithPrecision(SliceUpperLim, SliceUpperLimPrecision) + " [GeV/c]";
            hSaveName = to_string(SliceNumber) + "_res_plot_for_" + MomentumType + "_P_n_from_" + ToStringWithPrecision(SliceLowerLim, 2) + "_to_" +
                        ToStringWithPrecision(SliceUpperLim, SliceUpperLimPrecision);
            hCutName = "Slice_#" + to_string(SliceNumber) + "_from_" + ToStringWithPrecision(SliceLowerLim, 2) + "_to_" + ToStringWithPrecision(SliceUpperLim, SliceUpperLimPrecision);

            hResolutionSlice = hPlot1D("1n", "FD", hStatsTitle, hTitle, "R_{nFD} = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}", SlicesSavePath, hSaveName, hSliceLowerLim,
                                       hSliceUpperLim, hSliceNumOfBin);
            ResSliceFitCuts = DSCuts(("fit_" + MomentumType + "_" + hCutName), "FD", "Neutron", "1n", 0, -9999, 9999);
        } else if (isProton) {
            hStatsTitle =
                "p res. - " + ToStringWithPrecision(SliceLowerLim, 2) + "#leqP^{" + MomentumType + "}_{pFD}#leq" + ToStringWithPrecision(SliceUpperLim, SliceUpperLimPrecision) + " [GeV/c]";
            hTitle = "Proton resolution for " + ToStringWithPrecision(SliceLowerLim, 2) + "#leqP^{" + MomentumType + "}_{pFD}#leq" +
                     ToStringWithPrecision(SliceUpperLim, SliceUpperLimPrecision) + " [GeV/c]";
            hSaveName = to_string(SliceNumber) + "_res_plot_for_" + MomentumType + "_P_p_from_" + ToStringWithPrecision(SliceLowerLim, 2) + "_to_" +
                        ToStringWithPrecision(SliceUpperLim, SliceUpperLimPrecision);
            hCutName = "Slice_#" + to_string(SliceNumber) + "_from_" + ToStringWithPrecision(SliceLowerLim, 2) + "_to_" + ToStringWithPrecision(SliceUpperLim, SliceUpperLimPrecision);

            if (!momResS2CalcMode || !momResS2RunMode) {
                if (ForceSmallpResLimits) {
                    hResolutionSlice =
                        hPlot1D("1p", "FD", hStatsTitle, hTitle, "Resolution = (P^{truth}_{pFD} - P^{reco}_{pFD})/P^{truth}_{pFD}", SlicesSavePath, hSaveName, -0.2, 0.2, hSliceNumOfBin);
                } else {
                    hResolutionSlice = hPlot1D("1p", "FD", hStatsTitle, hTitle, "Resolution = (P^{truth}_{pFD} - P^{reco}_{pFD})/P^{truth}_{pFD}", SlicesSavePath, hSaveName, hSliceLowerLim,
                                               hSliceUpperLim, hSliceNumOfBin);
                }
            }

            ResSliceFitCuts = DSCuts(("fit_" + MomentumType + "_" + hCutName), "FD", "Proton", "1n", 0, -9999, 9999);
        }

        ResSliceFitCuts.SetSliceUpperb(SliceUpperLim);
        ResSliceFitCuts.SetSliceLowerb(SliceLowerLim);
        ResSliceFitCuts.SetSliceNumber(SliceNumber);

        ResSlices0.push_back(hResolutionSlice);
        ResSlicesLimits0.push_back({SliceLowerLim, SliceUpperLim});
        ResSlicesFitVar0.push_back(ResSliceFitCuts);

        ResSliceFitCuts.SetCutVariable(("hist_" + MomentumType + "_" + hCutName));
        ResSlicesHistVar0.push_back(ResSliceFitCuts);

        if (SliceUpperLim == SliceUpperMomLim) {
            SliceAndDice = false;
        } else {
            SliceLowerLim = SliceUpperLim;

            if (VaryingDelta) {
                if (MomentumType == "truth") {
                    if (beamE == 5.98636) {
                        if (SampleName == "C12_simulation_G18_Q204_6GeV") {
                            if ((SliceLowerLim >= 0.40) && (SliceLowerLim < 0.60)) {  // 0.4-0.65
                                Delta = delta * 5;
                            } else if ((SliceLowerLim >= 0.60) && (SliceLowerLim < 1.00)) {  // 0.65-1.05
                                Delta = delta * 2;
                            } else if ((SliceLowerLim >= 1.00) && (SliceLowerLim < 1.70)) {  // 1.05-1.70
                                Delta = delta * 1;
                            } else if ((SliceLowerLim >= 1.70) && (SliceLowerLim < 2.25)) {  // 1.70-2.30
                                Delta = delta * 2;
                            } else if ((SliceLowerLim >= 2.25) && (SliceLowerLim < 2.55)) {  // 2.30-2.60
                                Delta = delta * 3;
                            } else if ((SliceLowerLim >= 2.55) && (SliceLowerLim < 2.95)) {  // 2.60-3.00
                                Delta = delta * 4;
                            } else if ((SliceLowerLim >= 2.95) && (SliceLowerLim < 3.25)) {  // 3.00-3.30
                                Delta = delta * 6;
                            } else if (SliceLowerLim >= 3.25) {  // 3.00-SliceUpperMomLim
                                Delta = SliceUpperMomLim - SliceLowerLim;
                            }
                        } else if (SampleName == "C12x4_simulation_G18_Q204_6GeV") {  // Slices by option 2
                            if ((SliceLowerLim >= 0.40) && (SliceLowerLim < 0.60)) {  // 0.4-0.65
                                Delta = delta * 5;
                            } else if ((SliceLowerLim >= 0.60) && (SliceLowerLim < 1.10)) {  // 0.65-1.15
                                Delta = delta * 2;
                            } else if ((SliceLowerLim >= 1.10) && (SliceLowerLim < 1.55)) {  // 1.15-1.55
                                Delta = delta * 1;
                            } else if ((SliceLowerLim >= 1.55) && (SliceLowerLim < 2.25)) {  // 1.55-2.25
                                Delta = delta * 2;
                            } else if ((SliceLowerLim >= 2.25) && (SliceLowerLim < 2.55)) {  // 2.25-2.55
                                Delta = delta * 3;
                            } else if ((SliceLowerLim >= 2.55) && (SliceLowerLim < 2.75)) {  // 2.55-2.75
                                Delta = delta * 4;
                            } else if ((SliceLowerLim >= 2.75) && (SliceLowerLim < 3.00)) {  // 2.75-3.00
                                Delta = delta * 5;
                            } else if ((SliceLowerLim >= 3.00) && (SliceLowerLim < 3.50)) {  // 3.00-3.50
                                Delta = delta * 10;
                            } else if (SliceLowerLim >= 3.50) {  // 3.00-SliceUpperMomLim
                                Delta = SliceUpperMomLim - SliceLowerLim;
                            }
                        } else {
                            std::cout << "MomentumResolution::SetMomResSlices: no set of varying delta is available for '" << SampleName << "'! Aborting...\n\n", exit(1);
                        }
                    } else if (beamE == 2.07052) {
                        if ((SliceLowerLim >= 0.40) && (SliceLowerLim < 0.50)) {  // 0.4-0.55
                            Delta = delta * 3;
                        } else if ((SliceLowerLim >= 0.50) && (SliceLowerLim < 1.10)) {  // 0.55-1.15
                            Delta = delta * 2;
                        } else if ((SliceLowerLim >= 1.10) && (SliceLowerLim < 1.25)) {  // 1.15-1.30
                            Delta = delta * 3;
                        } else if (SliceLowerLim >= 1.25) {  // 1.30-beamE
                            Delta = beamE - SliceLowerLim;
                        }
                    } else {
                        std::cout << "MomentumResolution::SetMomResSlices: no set of varying delta is available for '" << SampleName << "'! Aborting...\n\n", exit(1);
                    }
                } else if (MomentumType == "reco") {
                    if (beamE == 5.98636) {
                        if (SampleName == "C12x4_simulation_G18_Q204_6GeV") {
                            if ((SliceLowerLim >= 0.40) && (SliceLowerLim < 0.60)) {  // 0.40-0.60
                                Delta = delta * 4;
                            } else if ((SliceLowerLim >= 0.60) && (SliceLowerLim < 0.90)) {  // 0.60-0.90
                                Delta = delta * 2;
                            } else if ((SliceLowerLim >= 0.90) && (SliceLowerLim < 1.60)) {  // 0.90-1.60
                                Delta = delta * 1;
                            } else if ((SliceLowerLim >= 1.60) && (SliceLowerLim < 2.10)) {  // 1.60-2.10
                                Delta = delta * 2;
                            } else if ((SliceLowerLim >= 2.10) && (SliceLowerLim < 2.40)) {  // 2.10-2.40
                                Delta = delta * 3;
                            } else if ((SliceLowerLim >= 2.40) && (SliceLowerLim < 2.65)) {  // 2.40-2.65
                                Delta = delta * 5;
                            } else if ((SliceLowerLim >= 2.65) && (SliceLowerLim < 3.05)) {  // 2.65-3.05
                                Delta = delta * 8;
                            } else if (SliceLowerLim >= 3.05) {  // 3.05-SliceUpperMomLim
                                Delta = SliceUpperMomLim - SliceLowerLim;
                            }
                        } else {
                            std::cout << "MomentumResolution::SetMomResSlices: no set of varying delta is available for '" << SampleName << "'! Aborting...\n\n", exit(1);
                        }
                    }
                }
            }

            if ((SliceUpperLim + Delta) > SliceUpperMomLim) {
                SliceUpperLim = SliceUpperMomLim;
            } else {
                SliceUpperLim = SliceLowerLim + Delta;
            }
        }
    }

    if (TLLimitsPrintOut && TLLimitsPrintOutAndExit && (MomentumType == "truth")) { exit(1); }

    if (RecoLimitsPrintOut && RecoLimitsPrintOutAndExit && (MomentumType == "reco")) { exit(1); }

    NumberOfSlices0 = SliceNumber;
}
#pragma endregion

#pragma region /* SetUpperMomCut function */
void MomentumResolution::SetUpperMomCut(const std::string &SampleName, const std::string &NucleonCutsDirectory, const bool &FitDebugging) {
    if (!FitDebugging) {
        // load sample-appropriate cuts file from NucleonCutsDirectory:
        ReadInputParam((NucleonCutsDirectory + "Nucleon_Cuts_-_" + SampleName + ".par").c_str());

        SliceUpperMomLim = Neutron_Momentum_cut;
    } else {
        SliceUpperMomLim = Ebeam;
    }
}
#pragma endregion

#pragma region /* LoadFitParam function */
void MomentumResolution::LoadFitParam(const std::string &SampleName, const std::string &NucleonCutsDirectory, bool const &Calculate_momResS2,
                                      const std::string &MomentumResolutionDirectory) {
    if (isNeutron) {
        if (momResS2CalcMode && !momResS2RunMode) {
            std::cout << "\n\nMomentumResolution::MomentumResolution: running in momResS2 calculation mode. Loading momResS1 variables...\n";

            std::string NeutronCorrectionDataFile = MomentumResolutionDirectory + "Res_data_-_" + SampleName + "/Neutron_momResS1_fit_param_-_" + SampleName + ".par";
            std::string ProtonSmearingDataFile = MomentumResolutionDirectory + "Res_data_-_" + SampleName + "/Neutron_momResS1_fit_param_-_" + SampleName + ".par";

            /* Load neutron correction variables (from momResS1), but not smearing variables */
            ReadResDataParam(NeutronCorrectionDataFile.c_str(), Calculate_momResS2, SampleName, NucleonCutsDirectory, "reco", true, false);

            /* Load proton smearing variables (from momResS1) */
            ReadResDataParam(ProtonSmearingDataFile.c_str(), Calculate_momResS2, SampleName, NucleonCutsDirectory, "reco", false, true);

#pragma region /* Safety checks for data files */
            if (!FindSubstring(NeutronCorrectionDataFile, "Neutron") || FindSubstring(NeutronCorrectionDataFile, "Proton")) {
                std::cout << "\n\nMomentumResolution::MomentumResolution: neutron correction variables are not being loaded from neutron data! Aborting...\n\n", exit(1);
            }

            if (!FindSubstring(ProtonSmearingDataFile, "Neutron") || FindSubstring(ProtonSmearingDataFile, "Proton")) {
                std::cout << "\n\nMomentumResolution::MomentumResolution: proton smearing variables are not being loaded from neutron data! Aborting...\n\n", exit(1);
            }
#pragma endregion

            std::cout << "\nDone.\n";
        } else if (!momResS2CalcMode && momResS2RunMode) {
            std::cout << "\n\nMomentumResolution::MomentumResolution: running in momResS2 run mode.\n";
            std::cout << "Loading correction from momResS1 variables & smearing from momResS2 variables...\n";

            std::string NeutronCorrectionDataFile = MomentumResolutionDirectory + "Res_data_-_" + SampleName + "/Neutron_momResS1_fit_param_-_" + SampleName + ".par";
            std::string ProtonSmearingDataFile = MomentumResolutionDirectory + "Res_data_-_" + SampleName + "/Neutron_momResS2_fit_param_-_" + SampleName + ".par";

            /* Load neutron correction variables (from momResS1) */
            ReadResDataParam(NeutronCorrectionDataFile.c_str(), Calculate_momResS2, SampleName, NucleonCutsDirectory, "reco", true, false);

            /* Load proton smearing variables (from momResS2) */
            ReadResDataParam(ProtonSmearingDataFile.c_str(), Calculate_momResS2, SampleName, NucleonCutsDirectory, "reco", false, true);

#pragma region /* Safety checks for data files */
            if (!FindSubstring(NeutronCorrectionDataFile, "Neutron") || FindSubstring(NeutronCorrectionDataFile, "Proton")) {
                std::cout << "\n\nMomentumResolution::MomentumResolution: neutron correction variables are not being loaded from neutron data! Aborting...\n\n", exit(1);
            }

            if (!FindSubstring(ProtonSmearingDataFile, "Neutron") || FindSubstring(ProtonSmearingDataFile, "Proton")) {
                std::cout << "\n\nMomentumResolution::MomentumResolution: proton smearing variables are not being loaded from neutron data! Aborting...\n\n", exit(1);
            }
#pragma endregion

            std::cout << "\nDone.\n";
        }
    }
}
#pragma endregion

// ReadInputParam function -----------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ReadInputParam function */
/* This function reads nucleon cuts (especially neutron upper th.). It was imported from the clas12ana class */

void MomentumResolution::ReadInputParam(const char *filename) {
    ifstream infile;
    infile.open(filename);

    if (infile.is_open()) {
        std::string tp;

        // remove 3 lines of header
        for (int i = 0; i < 3; i++) { getline(infile, tp); }

        while (getline(infile, tp))  // read data from file object and put it into string.
        {
            stringstream ss(tp);
            std::string parameter, parameter2;
            double value;
            // get cut identifier
            ss >> parameter;

            if (parameter == "nRes_Momentum_cut") {
                ss >> parameter2;
                stringstream ss2(parameter2);
                std::string pid_v;
                int count = 0;
                std::string pid = "";
                vector<double> par;

                while (getline(ss2, pid_v, ':')) {
                    if (count == 0) {
                        pid = pid_v;
                    } else {
                        par.push_back(atof(pid_v.c_str()));
                    }

                    count++;
                }

                if (pid != "") { Neutron_Momentum_cut = par.at(1); }
            }
        }
    }
    // else {
    //     std::cout << "MomentumResolution::ReadInputParam: Parameter file didn't read in:\n" << filename << endl;
    // }

    std::cout << "\033[35m\n\nMomentumResolution::ReadInputParam:\033[36m Warning!\033[0m file:\n" << filename << "\nwas not found!\n\n";
}
#pragma endregion

// hFillResPlotsByType function -----------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* hFillResPlotsByType function */
void MomentumResolution::hFillResPlotsByType(const double &TL_momentum, const double &Reco_momentum, const double &Resolution, const double &Weight) {
    hFillResPlots(TL_momentum, Resolution, Weight, ResTLMomSlices, ResTLMomSlicesLimits, TL_NumberOfSlices);
    hFillResPlots(Reco_momentum, Resolution, Weight, ResRecoMomSlices, ResRecoMomSlicesLimits, Reco_NumberOfSlices);
}
#pragma endregion

#pragma region /* hFillResPlots function */
void MomentumResolution::hFillResPlots(const double &Momentum, const double &Resolution, const double &Weight, vector<hPlot1D> &ResSlices0, vector<vector<double>> &ResSlicesLimits0,
                                       int &NumberOfSlices0) {
    bool Printout = false;

    for (int i = 0; i < NumberOfSlices0; i++) {
        if ((Momentum >= ResSlicesLimits0.at(i).at(0)) && (Momentum < ResSlicesLimits0.at(i).at(1))) {
            ResSlices0.at(i).hFill(Resolution, Weight);

            if (Printout) {
                std::cout << "\n\nResSlicesLimits0.at(" << i << ").at(0) = " << ResSlicesLimits0.at(i).at(0) << "\n";
                std::cout << "Momentum = " << Momentum << "\n";
                std::cout << "ResSlicesLimits0.at(" << i << ").at(1) = " << ResSlicesLimits0.at(i).at(1) << "\n";
            }

            break;  // No need to keep the loop going after filling histogram
        }
    }
}
#pragma endregion

// SliceFitDrawAndSaveByType function -----------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* CFitFunction */
/* CFitFunction function for costume fit */
Double_t CFitFunction(Double_t *v, Double_t *par) {
    Double_t arg = 0;
    if (par[2] != 0) { arg = (v[0] - par[1]) / par[2]; }  // 3 parameters

    Double_t fitval = par[0] * TMath::Exp(-0.5 * arg * arg);
    return fitval;
}
#pragma endregion

#pragma region /* SliceFitDrawAndSaveByType function */
void MomentumResolution::SliceFitDrawAndSaveByType(const std::string &SampleName, const double &beamE) {
    SliceFitDrawAndSave(SampleName, beamE, "truth", ResTLMomSlices, ResTLMomSlicesLimits, ResTLMomSlicesFitVar, ResTLMomSlicesHistVar, FittedTLMomSlices, TL_NumberOfSlices);
    SliceFitDrawAndSave(SampleName, beamE, "reco", ResRecoMomSlices, ResRecoMomSlicesLimits, ResRecoMomSlicesFitVar, ResRecoMomSlicesHistVar, FittedRecoMomSlices, Reco_NumberOfSlices);
}
#pragma endregion

#pragma region /* SliceFitDrawAndSave function */
void MomentumResolution::SliceFitDrawAndSave(const std::string &SampleName, const double &beamE, const std::string &MomentumType, vector<hPlot1D> &ResSlices0,
                                             vector<vector<double>> &ResSlicesLimits0, vector<DSCuts> &ResSlicesFitVar0, vector<DSCuts> &ResSlicesHistVar0, vector<int> &FittedSlices0,
                                             int &NumberOfSlices0) {
    TCanvas *SliceFitCanvas = new TCanvas("SliceFitCanvas", "SliceFitCanvas", 1000, 750);  // normal res
    SliceFitCanvas->SetGrid();
    SliceFitCanvas->SetBottomMargin(0.14), SliceFitCanvas->SetLeftMargin(0.18), SliceFitCanvas->SetRightMargin(0.12);
    SliceFitCanvas->cd();

    for (int i = 0; i < NumberOfSlices0; i++) {
#pragma region /* Setting sNameFlag */
        std::string sNameFlag;

        if (FindSubstring(SampleName, "sim")) {
            sNameFlag = "s";
        } else if (FindSubstring(SampleName, "data")) {
            sNameFlag = "d";
        }
#pragma endregion

        TH1D *hSlice = (TH1D *)ResSlices0.at(i).GetHistogram();
        hSlice->GetXaxis()->SetTitleSize(0.06), hSlice->GetXaxis()->SetLabelSize(0.0425), hSlice->GetXaxis()->CenterTitle(true);
        hSlice->GetYaxis()->SetTitle("Number of events");
        hSlice->GetYaxis()->SetTitleSize(0.06), hSlice->GetYaxis()->SetLabelSize(0.0425), hSlice->GetYaxis()->CenterTitle(true);
        hSlice->Sumw2();

        if (hSlice->Integral() != 0.) {  // Fit only the non-empty histograms
            std::cout << "\n\n";

            double FitUlim, FitLlim;

            if (momResTestMode) {             // In smear & shift test mode
                FitUlim = 1., FitLlim = -1.;  // For both neutrons and protons
            } else {
                if (isNeutron) {
                    FitUlim = 1., FitLlim = -1.;
                } else if (isProton) {
                    if (ForceSmallProtonResLimits) {
                        FitUlim = 0.2, FitLlim = -0.2;
                        //                        FitUlim = 0.5, FitLlim = -0.5;
                    } else {
                        FitUlim = 1., FitLlim = -1.;
                    }
                }
            }

            TF1 *func = new TF1("fit", CFitFunction, FitLlim, FitUlim, 3);  // create a function with 3 parameters in the range [-3,3]
            func->SetLineColor(kRed);

            double SliceMax = hSlice->GetMaximum(), SliceMean = hSlice->GetMean(), SliceStd = hSlice->GetRMS();

            func->SetParameters(SliceMax, SliceMean, 0.5);  // start fit with histogram's max and mean
            func->SetParNames("Constant", "Mean_value", "Sigma");

            if (momResTestMode) {                    // In smear & shift test mode
                func->SetParLimits(1, -1.5, 1.5);    // Mean limits
                func->SetParLimits(2, 0.001, 0.35);  // Sigma limits
            } else {
                if (isNeutron) {
                    func->SetParLimits(1, -1.5, 1.5);    // Mean limits
                    func->SetParLimits(2, 0.001, 0.35);  // Sigma limits
                } else if (isProton) {
                    func->SetParLimits(1, -1.5, 1.5);           // Mean limits
                    func->SetParLimits(2, 0.0000000001, 0.35);  // Sigma limits
                }
            }

            if (MomentumType == "truth") {
                if (TLSliceFitAlgorithm == "") {
                    hSlice->Fit("fit");
                } else if (TLSliceFitAlgorithm == "W") {
                    hSlice->Fit("fit", "W");
                }
            } else if (MomentumType == "reco") {
                if (RecoSliceFitAlgorithm == "") {
                    hSlice->Fit("fit");
                } else if (RecoSliceFitAlgorithm == "W") {
                    hSlice->Fit("fit", "W");
                }
            }

            hSlice->SetLineColor(kBlue);
            hSlice->SetLineWidth(2);

            double FitAmp = func->GetParameter(0);   // get p0
            double FitMean = func->GetParameter(1);  // get p1
            double FitStd = func->GetParameter(2);   // get p2

            ResSlicesFitVar0.at(i).SetMean(FitMean);     // For neutron correction
            ResSlicesFitVar0.at(i).SetUpperCut(FitStd);  // For proton smearing
            ResSlicesHistVar0.at(i).SetMean(SliceMean);
            ResSlicesHistVar0.at(i).SetUpperCut(SliceStd);

            auto FuncList = hSlice->GetListOfFunctions();
            FuncList->Clear();
            func->Draw("same && c");
            func->SetLineColor(kRed);

            double x_1_Cut_legend = gStyle->GetStatX(), y_1_Cut_legend = gStyle->GetStatY() - 0.2;
            double x_2_Cut_legend = gStyle->GetStatX() - 0.2, y_2_Cut_legend = gStyle->GetStatY() - 0.3;
            double x_1_FitParam = x_1_Cut_legend, y_1_FitParam = y_1_Cut_legend;
            double x_2_FitParam = x_2_Cut_legend, y_2_FitParam = y_2_Cut_legend;

#pragma region /* Setting fitter particle */
            std::string SliceFitterParticle;

            if (isNeutron) {
                SliceFitterParticle = "nFD";
            } else if (isProton) {
                SliceFitterParticle = "pFD";
            }
#pragma endregion

            TPaveText *FitParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam - 0.025, "NDC");
            FitParam->SetBorderSize(1), FitParam->SetFillColor(0);
            FitParam->SetTextAlign(12), FitParam->SetTextFont(42), FitParam->SetTextSize(0.03);
            FitParam->AddText(("Fit amp = " + ToStringWithPrecision(FitAmp, 4)).c_str());
            FitParam->AddText(("Fit #mu_{" + SliceFitterParticle + "} = " + ToStringWithPrecision(FitMean, 4)).c_str());
            FitParam->AddText(("Fit #sigma_{" + SliceFitterParticle + "} = " + ToStringWithPrecision(FitStd, 4)).c_str());
            FitParam->Draw("same");

            int SliceUpperLimPrecision;
            if (ResSlicesLimits0.at(i).at(1) == beamE) {
                SliceUpperLimPrecision = 3;
            } else {
                SliceUpperLimPrecision = 2;
            }

            std::string hSlice_CloneSaveDir = ResSlices0.at(i).GetHistogram1DSaveNamePath() + "00_Fitted_" + MomentumType + "_res_slices/";
            std::string hSlice_CloneSaveName = hSlice_CloneSaveDir + sNameFlag + "_" + MomentumType + "_" + ResSlices0.at(i).GetHistogram1DSaveName() + "_fitted.pdf";
            system(("mkdir -p " + hSlice_CloneSaveDir).c_str());

            auto ListOfFunctions = hSlice->GetListOfFunctions();
            ListOfFunctions->Add((TObject *)FitParam);

            std::cout << "\n", SliceFitCanvas->SaveAs(hSlice_CloneSaveName.c_str());

            if (MomentumType == "truth") {
                if (isNeutron) {
                    FittedTLNeutronResSlices->Add(hSlice);
                } else if (isProton) {
                    FittedTLProtonResSlices->Add(hSlice);
                }
            } else if (MomentumType == "reco") {
                if (isNeutron) {
                    FittedRecoNeutronResSlices->Add(hSlice);
                } else if (isProton) {
                    FittedRecoProtonResSlices->Add(hSlice);
                }
            }

            SliceFitCanvas->Clear();

            FittedSlices0.push_back(i);  // Log slices that were fitted
        } else {
            continue;
        }
    }

    if (MomentumType == "truth") {
        PolyFitter(MomentumType, 1, "Corr", "noKC", TL_FitParam_Corr_pol1);
        PolyFitter(MomentumType, 1, "Corr", "wKC", TL_FitParam_Corr_pol1_wKC);
        PolyFitter(MomentumType, 2, "Corr", "noKC", TL_FitParam_Corr_pol2);
        PolyFitter(MomentumType, 2, "Corr", "wKC", TL_FitParam_Corr_pol2_wKC);
        PolyFitter(MomentumType, 3, "Corr", "noKC", TL_FitParam_Corr_pol3);
        PolyFitter(MomentumType, 3, "Corr", "wKC", TL_FitParam_Corr_pol3_wKC);
        PolyFitter(MomentumType, 1, "Smear", "noKC", TL_FitParam_Smear_pol1);
        PolyFitter(MomentumType, 1, "Smear", "wKC", TL_FitParam_Smear_pol1_wKC);
        PolyFitter(MomentumType, 2, "Smear", "noKC", TL_FitParam_Smear_pol2);
        PolyFitter(MomentumType, 2, "Smear", "wKC", TL_FitParam_Smear_pol2_wKC);
        PolyFitter(MomentumType, 3, "Smear", "noKC", TL_FitParam_Smear_pol3);
        PolyFitter(MomentumType, 3, "Smear", "wKC", TL_FitParam_Smear_pol3_wKC);
    } else if (MomentumType == "reco") {
        PolyFitter(MomentumType, 1, "Corr", "noKC", Reco_FitParam_Corr_pol1);
        PolyFitter(MomentumType, 1, "Corr", "wKC", Reco_FitParam_Corr_pol1_wKC);
        PolyFitter(MomentumType, 2, "Corr", "noKC", Reco_FitParam_Corr_pol2);
        PolyFitter(MomentumType, 2, "Corr", "wKC", Reco_FitParam_Corr_pol2_wKC);
        PolyFitter(MomentumType, 3, "Corr", "noKC", Reco_FitParam_Corr_pol3);
        PolyFitter(MomentumType, 3, "Corr", "wKC", Reco_FitParam_Corr_pol3_wKC);
        PolyFitter(MomentumType, 1, "Smear", "noKC", Reco_FitParam_Smear_pol1);
        PolyFitter(MomentumType, 1, "Smear", "wKC", Reco_FitParam_Smear_pol1_wKC);
        PolyFitter(MomentumType, 2, "Smear", "noKC", Reco_FitParam_Smear_pol2);
        PolyFitter(MomentumType, 2, "Smear", "wKC", Reco_FitParam_Smear_pol2_wKC);
        PolyFitter(MomentumType, 3, "Smear", "noKC", Reco_FitParam_Smear_pol3);
        PolyFitter(MomentumType, 3, "Smear", "wKC", Reco_FitParam_Smear_pol3_wKC);
    }
}
#pragma endregion

#pragma region /* PolyFitter function */
void MomentumResolution::PolyFitter(const std::string &MomentumType, const int &PolynomialDegree, const std::string &FitType, const std::string &MomentumFitRange,
                                    vector<vector<double>> &FitParamResults) {
    std::cout << "\n\nPolyFitter variables:\n";
    bool PrintOut = false;
    bool PlotPoints = false;

#pragma region /* Setting fitter particle */
    std::string FitterParticle;

    if (isNeutron) {
        FitterParticle = "nFD";
    } else if (isProton) {
        FitterParticle = "pFD";
    }
#pragma endregion

#pragma region /* Setting polynomial */
    std::string PolynomialFuncName, PolynomialFuncStruct, PolynomialFitType;

    if (MomentumFitRange == "noKC") {
        if (PolynomialDegree == 1) {
            PolynomialFuncName = MomentumType + "_f_" + FitType + "_pol1",
            PolynomialFuncStruct = "[0] * x + [1]";  // A*x + B
            PolynomialFitType = "linear";
        } else if (PolynomialDegree == 2) {
            PolynomialFuncName = MomentumType + "_f_" + FitType + "_pol2",
            PolynomialFuncStruct = "[0] * x * x + [1] * x + [2]";  // A*x*x + B*x + C
            PolynomialFitType = "quadratic";
        } else if (PolynomialDegree == 3) {
            PolynomialFuncName = MomentumType + "_f_" + FitType + "_pol3",
            PolynomialFuncStruct = "[0] * x * x * x + [1] * x * x + [2] * x + [3]";  // A*x*x*x + B*x*x + C*x + D
            PolynomialFitType = "cubic";
        }
    } else if (MomentumFitRange == "wKC") {
        if (PolynomialDegree == 1) {
            PolynomialFuncName = MomentumType + "_f_" + FitType + "_pol1_wKC",
            PolynomialFuncStruct = "[0] * x + [1]";  // A*x + B
            PolynomialFitType = "linear";
        } else if (PolynomialDegree == 2) {
            PolynomialFuncName = MomentumType + "_f_" + FitType + "_pol2_wKC",
            PolynomialFuncStruct = "[0] * x * x + [1] * x + [2]";  // A*x*x + B*x + C
            PolynomialFitType = "quadratic";
        } else if (PolynomialDegree == 3) {
            PolynomialFuncName = MomentumType + "_f_" + FitType + "_pol3_wKC",
            PolynomialFuncStruct = "[0] * x * x * x + [1] * x * x + [2] * x + [3]";  // A*x*x*x + B*x*x + C*x + D
            PolynomialFitType = "cubic";
        }
    } else {
        std::cout << "\n\nMomentumResolution::PolyFitter: MomentumFitRange is illegal! Aborting...\n\n", exit(1);
    }
#pragma endregion

#pragma region /* Setting fitted slices */
    vector<hPlot1D> ResSlices0;
    vector<vector<double>> ResSlicesLimits0;
    vector<DSCuts> ResSlicesFitVar0, ResSlicesHistVar0;
    int NumberOfSlices0;
    vector<int> FittedSlices0;

    if (MomentumType == "truth") {
        ResSlices0 = ResTLMomSlices;
        ResSlicesLimits0 = ResTLMomSlicesLimits, ResSlicesFitVar0 = ResTLMomSlicesFitVar, ResSlicesHistVar0 = ResTLMomSlicesHistVar;
        NumberOfSlices0 = TL_NumberOfSlices, FittedSlices0 = FittedTLMomSlices;
    } else if (MomentumType == "reco") {
        ResSlices0 = ResRecoMomSlices;
        ResSlicesLimits0 = ResRecoMomSlicesLimits, ResSlicesFitVar0 = ResRecoMomSlicesFitVar;
        ResSlicesHistVar0 = ResRecoMomSlicesHistVar;
        NumberOfSlices0 = Reco_NumberOfSlices, FittedSlices0 = FittedRecoMomSlices;
    } else {
        std::cout << "\n\nMomentumResolution::PolyFitter: MomentumType is illegal! Aborting...\n\n", exit(1);
    }
#pragma endregion

#pragma region /* Setting variable to fit */
    std::string FittedVar;

    if (FitType == "Smear") {
        FittedVar = "#sigma_{" + FitterParticle + "}";
    } else if (FitType == "Corr") {
        FittedVar = "#mu_{" + FitterParticle + "}";
    } else {
        std::cout << "\n\nMomentumResolution::PolyFitter: FitType is illegal! Aborting...\n\n", exit(1);
    }
#pragma endregion

#pragma region /* Setting plot x and y data */
    vector<double> MeanPn, Pn_FitVar;

    for (int i = 0; i < NumberOfSlices0; i++) {
        double Mean = (ResSlicesLimits0.at(i).at(1) + ResSlicesLimits0.at(i).at(0)) / 2;

        double Std = ResSlicesFitVar0.at(i).GetUpperCut();
        double Corr = ResSlicesFitVar0.at(i).GetMean();

        if (MomentumFitRange == "wKC") {
            if (FitType == "Smear") {
                if ((Mean >= SliceLowerMomLimKC_sigma) && (Mean <= SliceUpperMomLimKC_sigma)) {
                    Pn_FitVar.push_back(Std);  // TODO: add a mechanism to ignore failed fits
                    MeanPn.push_back(Mean);

                    if (PrintOut) {
                        std::cout << "\nMean = " << Mean << "\n";
                        std::cout << "Std = " << Std << "\n";
                        std::cout << "Corr = " << Corr << "\n";
                    }
                }
            } else if (FitType == "Corr") {
                if ((Mean >= SliceLowerMomLimKC_mu) && (Mean <= SliceUpperMomLimKC_mu)) {
                    Pn_FitVar.push_back(Corr);  // TODO: add a mechanism to ignore failed fits
                    MeanPn.push_back(Mean);

                    if (PrintOut) {
                        std::cout << "\nMean = " << Mean << "\n";
                        std::cout << "Std = " << Std << "\n";
                        std::cout << "Corr = " << Corr << "\n";
                    }
                }
            }
        } else {
            if (FitType == "Smear") {
                Pn_FitVar.push_back(Std);  // TODO: add a mechanism to ignore failed fits
                MeanPn.push_back(Mean);

                if (PrintOut) {
                    std::cout << "\nMean = " << Mean << "\n";
                    std::cout << "Std = " << Std << "\n";
                    std::cout << "Corr = " << Corr << "\n";
                }
            } else if (FitType == "Corr") {
                Pn_FitVar.push_back(Corr);  // TODO: add a mechanism to ignore failed fits
                MeanPn.push_back(Mean);

                if (PrintOut) {
                    std::cout << "\nMean = " << Mean << "\n";
                    std::cout << "Std = " << Std << "\n";
                    std::cout << "Corr = " << Corr << "\n";
                }
            }
        }
    }

    if (PrintOut) {
        std::cout << "\nMeanPn.size() = " << MeanPn.size() << "\n";
        std::cout << "Pn_FitVar.size() = " << Pn_FitVar.size() << "\n";
        std::cout << "FitType = " << FitType << "\n\n";
    }

#pragma region /* Safety check */
    if (MeanPn.size() != Pn_FitVar.size()) { std::cout << "\n\nMomentumResolution::Fitter_Std_pol1: x and y data are of different lengths! Aborting...\n\n", exit(1); }
#pragma endregion

#pragma endregion

#pragma region /* Setting fit canvas */
    TCanvas *Fit_Canvas = new TCanvas("Fit_Canvas", "Fit_Canvas", 1000, 750);
    Fit_Canvas->cd();
    Fit_Canvas->SetGrid();
    Fit_Canvas->SetBottomMargin(0.14), Fit_Canvas->SetLeftMargin(0.16), Fit_Canvas->SetRightMargin(0.12);
#pragma endregion

#pragma region /* Setting graph & preforming the fit */
    TGraph *Graph1D = new TGraph();
    Graph1D->SetName(PolynomialFuncName.c_str());

    for (int i = 0; i < MeanPn.size(); i++) {
        double x = MeanPn.at(i), y = Pn_FitVar.at(i);

        Graph1D->AddPoint(x, y);

        if (PlotPoints) {
            TLatex *latex = new TLatex(x, y, ("(" + to_string(x) + " , " + to_string(y) + ")").c_str());
            latex->SetTextSize(0.02);
            Graph1D->GetListOfFunctions()->Add(latex);
        }
    }

    Graph1D->GetXaxis()->SetTitleSize(0.06), Graph1D->GetXaxis()->SetLabelSize(0.0425), Graph1D->GetXaxis()->CenterTitle(true);
    Graph1D->GetYaxis()->SetTitleSize(0.06), Graph1D->GetYaxis()->SetLabelSize(0.0425), Graph1D->GetYaxis()->CenterTitle(true);
    Graph1D->GetXaxis()->SetTitle(("#LTP^{" + MomentumType + "}_{" + FitterParticle + "}#GT [GeV/c]").c_str());

    if (FitType == "Smear") {
        if (isNeutron) {
            Graph1D->SetTitle(("Neutron resolution fit (" + MomentumFitRange + ", " + PolynomialFitType + " fit)").c_str());
        } else if (isProton) {
            Graph1D->SetTitle(("Proton resolution fit (" + MomentumFitRange + ", " + PolynomialFitType + " fit)").c_str());
        }

        Graph1D->GetYaxis()->SetTitle(("R_{" + FitterParticle + "} width").c_str());
    } else if (FitType == "Corr") {
        if (isNeutron) {
            Graph1D->SetTitle(("Neutron correction fit (" + MomentumFitRange + ", " + PolynomialFitType + " fit)").c_str());
        } else if (isProton) {
            Graph1D->SetTitle(("Proton correction fit (" + MomentumFitRange + ", " + PolynomialFitType + " fit)").c_str());
        }

        Graph1D->GetYaxis()->SetTitle(("R_{" + FitterParticle + "} mean").c_str());
    }

    TF1 *PolynomialFunc = new TF1(PolynomialFuncName.c_str(), PolynomialFuncStruct.c_str());

    Graph1D->Fit(PolynomialFunc);
    Graph1D->Draw("ap");
    Graph1D->SetMarkerStyle(21);
#pragma endregion

#pragma region /* Log fit results to variables */
    vector<double> FitVarResults, FitVarResultsErrors, FitVarResultsGoodness;

    if (PolynomialDegree == 1) {
        FitVarResults.push_back(PolynomialFunc->GetParameter(0));  // get [0]
        FitVarResults.push_back(PolynomialFunc->GetParameter(1));  // get [1]
        FitParamResults.push_back(FitVarResults);

        FitVarResultsErrors.push_back(PolynomialFunc->GetParError(0));  // get [0]
        FitVarResultsErrors.push_back(PolynomialFunc->GetParError(1));  // get [1]
        FitParamResults.push_back(FitVarResultsErrors);

        FitVarResultsGoodness.push_back(PolynomialFunc->GetChisquare());  // ChiSquare
        FitVarResultsGoodness.push_back(PolynomialFunc->GetNDF());        // NDF
        FitParamResults.push_back(FitVarResultsGoodness);
    } else if (PolynomialDegree == 2) {
        FitVarResults.push_back(PolynomialFunc->GetParameter(0));  // get [0]
        FitVarResults.push_back(PolynomialFunc->GetParameter(1));  // get [1]
        FitVarResults.push_back(PolynomialFunc->GetParameter(2));  // get [2]
        FitParamResults.push_back(FitVarResults);

        FitVarResultsErrors.push_back(PolynomialFunc->GetParError(0));  // get [0]
        FitVarResultsErrors.push_back(PolynomialFunc->GetParError(1));  // get [1]
        FitVarResultsErrors.push_back(PolynomialFunc->GetParError(2));  // get [2]
        FitParamResults.push_back(FitVarResultsErrors);

        FitVarResultsGoodness.push_back(PolynomialFunc->GetChisquare());  // ChiSquare
        FitVarResultsGoodness.push_back(PolynomialFunc->GetNDF());        // NDF
        FitParamResults.push_back(FitVarResultsGoodness);
    } else if (PolynomialDegree == 3) {
        FitVarResults.push_back(PolynomialFunc->GetParameter(0));  // get [0]
        FitVarResults.push_back(PolynomialFunc->GetParameter(1));  // get [1]
        FitVarResults.push_back(PolynomialFunc->GetParameter(2));  // get [2]
        FitVarResults.push_back(PolynomialFunc->GetParameter(3));  // get [3]
        FitParamResults.push_back(FitVarResults);

        FitVarResultsErrors.push_back(PolynomialFunc->GetParError(0));  // get [0]
        FitVarResultsErrors.push_back(PolynomialFunc->GetParError(1));  // get [1]
        FitVarResultsErrors.push_back(PolynomialFunc->GetParError(2));  // get [2]
        FitVarResultsErrors.push_back(PolynomialFunc->GetParError(3));  // get [3]
        FitParamResults.push_back(FitVarResultsErrors);

        FitVarResultsGoodness.push_back(PolynomialFunc->GetChisquare());  // ChiSquare
        FitVarResultsGoodness.push_back(PolynomialFunc->GetNDF());        // NDF
        FitParamResults.push_back(FitVarResultsGoodness);
    }
#pragma endregion

#pragma region /* Setting legend & fit variable chart */

#pragma region /* Legend location */
    double x_1, y_1, x_2, y_2;
    double x_1_legend, y_1_legend, x_2_legend, y_2_legend;
    double x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam;
    double x_1_Offset_Legend = 0.075, x_1_Offset_FitParam = 0.0375;

    if (PolynomialDegree == 3) { x_1_Offset_Legend = x_1_Offset_Legend * 2; }

    if (FitType == "Smear") {
        if (MomentumFitRange == "noKC") {
            if (PolynomialDegree == 1) {
                x_1 = gStyle->GetStatX() - 0.6, y_1 = gStyle->GetStatY() - 0.2;
                x_2 = gStyle->GetStatX() - 0.2 - 0.6, y_2 = gStyle->GetStatY() - 0.3;
                x_1_legend = x_1, y_1_legend = y_1 + 0.125;
                x_2_legend = x_2, y_2_legend = y_2 + 0.15;
                x_1_FitParam = x_1, y_1_FitParam = y_1 + 0.025;
                x_2_FitParam = x_2, y_2_FitParam = y_2 + 0.025;
            } else if (PolynomialDegree == 2) {
                x_1 = gStyle->GetStatX() - 0.6, y_1 = gStyle->GetStatY() - 0.2;
                x_2 = gStyle->GetStatX() - 0.2 - 0.6, y_2 = gStyle->GetStatY() - 0.3;
                x_1_legend = x_1 + 0.1, y_1_legend = y_1 + 0.125;
                x_2_legend = x_2, y_2_legend = y_2 + 0.15;
                x_1_FitParam = x_1, y_1_FitParam = y_1 + 0.025;
                x_2_FitParam = x_2, y_2_FitParam = y_2 + 0.025 - 0.05;
            } else if (PolynomialDegree == 3) {
                x_1 = gStyle->GetStatX() - 0.6, y_1 = gStyle->GetStatY() - 0.2;
                x_2 = gStyle->GetStatX() - 0.2 - 0.6, y_2 = gStyle->GetStatY() - 0.3;
                x_1_legend = x_1 + 0.1 + 0.075, y_1_legend = y_1 + 0.125;
                x_2_legend = x_2, y_2_legend = y_2 + 0.15;
                x_1_FitParam = x_1, y_1_FitParam = y_1 + 0.025;
                x_2_FitParam = x_2, y_2_FitParam = y_2 + 0.025 - 0.05 - 0.05;
            }
        } else if (MomentumFitRange == "wKC") {
            if (PolynomialDegree == 1) {
                x_1 = gStyle->GetStatX() - 0.6, y_1 = gStyle->GetStatY() - 0.2;
                x_2 = gStyle->GetStatX() - 0.2 - 0.6, y_2 = gStyle->GetStatY() - 0.3;
                x_1_legend = x_1, y_1_legend = y_1 + 0.125;
                x_2_legend = x_2, y_2_legend = y_2 + 0.15;
                x_1_FitParam = x_1, y_1_FitParam = y_1 + 0.025;
                x_2_FitParam = x_2, y_2_FitParam = y_2 + 0.025;
            } else if (PolynomialDegree == 2) {
                x_1 = gStyle->GetStatX() - 0.6, y_1 = gStyle->GetStatY() - 0.2;
                x_2 = gStyle->GetStatX() - 0.2 - 0.6, y_2 = gStyle->GetStatY() - 0.3;
                x_1_legend = x_1 + 0.1, y_1_legend = y_1 + 0.125;
                x_2_legend = x_2, y_2_legend = y_2 + 0.15;
                x_1_FitParam = x_1, y_1_FitParam = y_1 + 0.025;
                x_2_FitParam = x_2, y_2_FitParam = y_2 + 0.025 - 0.05;
            } else if (PolynomialDegree == 3) {
                x_1 = gStyle->GetStatX() - 0.6, y_1 = gStyle->GetStatY() - 0.2;
                x_2 = gStyle->GetStatX() - 0.2 - 0.6, y_2 = gStyle->GetStatY() - 0.3;
                x_1_legend = x_1 + 0.1 + 0.075, y_1_legend = y_1 + 0.125;
                x_2_legend = x_2, y_2_legend = y_2 + 0.15;
                x_1_FitParam = x_1, y_1_FitParam = y_1 + 0.025;
                x_2_FitParam = x_2, y_2_FitParam = y_2 + 0.025 - 0.05 - 0.05;
            }
        }
    } else if (FitType == "Corr") {
        if (MomentumFitRange == "noKC") {
            if (PolynomialDegree == 1) {
                x_1 = gStyle->GetStatX() - 0.6, y_1 = gStyle->GetStatY() - 0.2;
                x_2 = gStyle->GetStatX() - 0.2 - 0.6, y_2 = gStyle->GetStatY() - 0.3;
                x_1_legend = x_1, y_1_legend = y_1 + 0.125;
                x_2_legend = x_2, y_2_legend = y_2 + 0.15;
                x_1_FitParam = x_1, y_1_FitParam = y_1 + 0.025;
                x_2_FitParam = x_2, y_2_FitParam = y_2 + 0.025;
            } else if (PolynomialDegree == 2) {
                x_1 = gStyle->GetStatX() - 0.6, y_1 = gStyle->GetStatY() - 0.2;
                x_2 = gStyle->GetStatX() - 0.2 - 0.6, y_2 = gStyle->GetStatY() - 0.3;
                x_1_legend = x_1 + 0.1, y_1_legend = y_1 + 0.125;
                x_2_legend = x_2, y_2_legend = y_2 + 0.15;
                x_1_FitParam = x_1, y_1_FitParam = y_1 + 0.025;
                x_2_FitParam = x_2, y_2_FitParam = y_2 + 0.025 - 0.05;
            } else if (PolynomialDegree == 3) {
                x_1 = gStyle->GetStatX() - 0.6, y_1 = gStyle->GetStatY() - 0.2;
                x_2 = gStyle->GetStatX() - 0.2 - 0.6, y_2 = gStyle->GetStatY() - 0.3;
                x_1_legend = x_1 + 0.1 + 0.075, y_1_legend = y_1 + 0.125;
                x_2_legend = x_2, y_2_legend = y_2 + 0.15;
                x_1_FitParam = x_1, y_1_FitParam = y_1 + 0.025;
                x_2_FitParam = x_2, y_2_FitParam = y_2 + 0.025 - 0.05 - 0.05;
            }
        } else if (MomentumFitRange == "wKC") {
            if (PolynomialDegree == 1) {
                x_1 = gStyle->GetStatX() - 0.6, y_1 = gStyle->GetStatY() - 0.2;
                x_2 = gStyle->GetStatX() - 0.2 - 0.6, y_2 = gStyle->GetStatY() - 0.3;
                x_1_legend = x_1, y_1_legend = y_1 + 0.125;
                x_2_legend = x_2, y_2_legend = y_2 + 0.15;
                x_1_FitParam = x_1, y_1_FitParam = y_1 + 0.025;
                x_2_FitParam = x_2, y_2_FitParam = y_2 + 0.025;
            } else if (PolynomialDegree == 2) {
                x_1 = gStyle->GetStatX() - 0.6, y_1 = gStyle->GetStatY() - 0.2;
                x_2 = gStyle->GetStatX() - 0.2 - 0.6, y_2 = gStyle->GetStatY() - 0.3;
                x_1_legend = x_1 + 0.1, y_1_legend = y_1 + 0.125;
                x_2_legend = x_2, y_2_legend = y_2 + 0.15;
                x_1_FitParam = x_1, y_1_FitParam = y_1 + 0.025;
                x_2_FitParam = x_2, y_2_FitParam = y_2 + 0.025 - 0.05;
            } else if (PolynomialDegree == 3) {
                x_1 = gStyle->GetStatX() - 0.6, y_1 = gStyle->GetStatY() - 0.2;
                x_2 = gStyle->GetStatX() - 0.2 - 0.6, y_2 = gStyle->GetStatY() - 0.3;
                x_1_legend = x_1 + 0.1 + 0.075, y_1_legend = y_1 + 0.125;
                x_2_legend = x_2, y_2_legend = y_2 + 0.15;
                x_1_FitParam = x_1, y_1_FitParam = y_1 + 0.025;
                x_2_FitParam = x_2, y_2_FitParam = y_2 + 0.025 - 0.05 - 0.05;
            }
        }
    }
#pragma endregion

#pragma region /* Setting legend */
    auto Graph1D_Legend = new TLegend(x_1_legend + x_1_Offset_Legend, y_1_legend, x_2_legend, y_2_legend);

    TLegendEntry *Graph1D_Legend_fit;

    std::string LegendPolyStruct;

    if (PolynomialDegree == 1) {
        LegendPolyStruct = FittedVar + " = " + "A#LTP^{" + MomentumType + "}_{" + FitterParticle + "}#GT + " + "B";
    } else if (PolynomialDegree == 2) {
        LegendPolyStruct = FittedVar + " = " + "A#LTP^{" + MomentumType + "}_{" + FitterParticle + "}#GT^{2} + " + "B#LTP^{" + MomentumType + "}_{" + FitterParticle + "}#GT + " + "C";
    } else if (PolynomialDegree == 3) {
        LegendPolyStruct = FittedVar + " = " + "A#LTP^{" + MomentumType + "}_{" + FitterParticle + "}#GT^{3} + " + "B#LTP^{" + MomentumType + "}_{" + FitterParticle + "}#GT^{2} + " +
                           "C#LTP^{" + MomentumType + "}_{" + FitterParticle + "}#GT + " + "D";
    }

    Graph1D_Legend_fit = Graph1D_Legend->AddEntry(PolynomialFunc, LegendPolyStruct.c_str(), "l");

    Graph1D_Legend->SetTextFont(42), Graph1D_Legend->SetTextSize(0.03), Graph1D_Legend->Draw("same");
#pragma endregion

#pragma region /* Setting fit variable chart */
    TPaveText *FitParam = new TPaveText(x_1_FitParam + x_1_Offset_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
    FitParam->SetBorderSize(1), FitParam->SetFillColor(0), FitParam->SetTextAlign(12), FitParam->SetTextFont(42), FitParam->SetTextSize(0.03);

    if (PolynomialDegree == 1) {
        FitParam->AddText(("A = " + ToStringWithPrecision(FitParamResults.at(0).at(0), 4) + " #pm " + ToStringWithPrecision(FitParamResults.at(1).at(0), 4)).c_str());
        FitParam->AddText(("B = " + ToStringWithPrecision(FitParamResults.at(0).at(1), 4) + " #pm " + ToStringWithPrecision(FitParamResults.at(1).at(1), 4)).c_str());
    } else if (PolynomialDegree == 2) {
        FitParam->AddText(("A = " + ToStringWithPrecision(FitParamResults.at(0).at(0), 4) + " #pm " + ToStringWithPrecision(FitParamResults.at(1).at(0), 4)).c_str());
        FitParam->AddText(("B = " + ToStringWithPrecision(FitParamResults.at(0).at(1), 4) + " #pm " + ToStringWithPrecision(FitParamResults.at(1).at(1), 4)).c_str());
        FitParam->AddText(("C = " + ToStringWithPrecision(FitParamResults.at(0).at(2), 4) + " #pm " + ToStringWithPrecision(FitParamResults.at(1).at(2), 4)).c_str());
    } else if (PolynomialDegree == 3) {
        FitParam->AddText(("A = " + ToStringWithPrecision(FitParamResults.at(0).at(0), 4) + " #pm " + ToStringWithPrecision(FitParamResults.at(1).at(0), 4)).c_str());
        FitParam->AddText(("B = " + ToStringWithPrecision(FitParamResults.at(0).at(1), 4) + " #pm " + ToStringWithPrecision(FitParamResults.at(1).at(1), 4)).c_str());
        FitParam->AddText(("C = " + ToStringWithPrecision(FitParamResults.at(0).at(2), 4) + " #pm " + ToStringWithPrecision(FitParamResults.at(1).at(2), 4)).c_str());
        FitParam->AddText(("D = " + ToStringWithPrecision(FitParamResults.at(0).at(3), 4) + " #pm " + ToStringWithPrecision(FitParamResults.at(1).at(3), 4)).c_str());
    }

    FitParam->AddText(("Fit #chi^{2} = " + to_string(FitParamResults.at(2).at(0))).c_str());
    FitParam->Draw("same");
#pragma endregion

#pragma endregion

#pragma region /* Save graph and exit */
    std::cout << "\n\n";

    auto ListOfFunctions = Graph1D->GetListOfFunctions();
    ListOfFunctions->Add((TLegend *)Graph1D_Legend);
    ListOfFunctions->Add((TPaveText *)FitParam);

#pragma region /* TFolder sorting */
    if (MomentumType == "truth") {
        if (FitType == "Corr") {
            if (isNeutron) {
                FittedTLNeutronResSlicesMean->Add(Graph1D);
            } else if (isProton) {
                FittedTLProtonResSlicesMean->Add(Graph1D);
            }
        } else if (FitType == "Smear") {
            if (isNeutron) {
                FittedTLNeutronResSlicesWidth->Add(Graph1D);
            } else if (isProton) {
                FittedTLProtonResSlicesWidth->Add(Graph1D);
            }
        }
    } else if (MomentumType == "reco") {
        if (FitType == "Corr") {
            if (isNeutron) {
                FittedRecoNeutronResSlicesMean->Add(Graph1D);
            } else if (isProton) {
                FittedRecoProtonResSlicesMean->Add(Graph1D);
            }
        } else if (FitType == "Smear") {
            if (isNeutron) {
                FittedRecoNeutronResSlicesWidth->Add(Graph1D);
            } else if (isProton) {
                FittedRecoProtonResSlicesWidth->Add(Graph1D);
            }
        }
    }
#pragma endregion

    std::string FitsDir = SlicesSavePath + "/" + "Graph1D_" + MomentumType + "_fits";
    std::string FitsDirByType = FitsDir + "/" + MomentumType + "_" + FitType + "_fits";
    std::string GraphSaveName = FitsDirByType + "/" + "0" + to_string(PolynomialDegree) + "_Fit_" + FitType + "_pol" + to_string(PolynomialDegree) + "_" + MomentumFitRange + ".pdf";

    system(("mkdir -p " + FitsDir).c_str());
    system(("mkdir -p " + FitsDirByType).c_str());

    std::cout << "\n", Fit_Canvas->SaveAs(GraphSaveName.c_str());
    Fit_Canvas->Clear();
    delete Fit_Canvas;
#pragma endregion
}
#pragma endregion

// DrawAndSaveResSlices function ----------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* DrawAndSaveResSlices function */
void MomentumResolution::DrawAndSaveResSlices(const std::string &SampleName, TCanvas *h1DCanvas, const std::string &plots_path, const std::string &MomentumResolutionDirectory) {
    std::string SampleNameTemp = SampleName;
    const char *ResSlicePlots_OutFile = MomResHistoListPDFFileName.c_str();

    if (isNeutron) {
        ResSlicePlots->Add(FittedTLNeutronResSlices), ResSlicePlots->Add(FittedTLNeutronResSlicesWidth), ResSlicePlots->Add(FittedTLNeutronResSlicesMean);
        ResSlicePlots->Add(FittedRecoNeutronResSlices), ResSlicePlots->Add(FittedRecoNeutronResSlicesWidth), ResSlicePlots->Add(FittedRecoNeutronResSlicesMean);
    } else if (isProton) {
        ResSlicePlots->Add(FittedTLProtonResSlices), ResSlicePlots->Add(FittedTLProtonResSlicesWidth), ResSlicePlots->Add(FittedTLProtonResSlicesMean);
        ResSlicePlots->Add(FittedRecoProtonResSlices), ResSlicePlots->Add(FittedRecoProtonResSlicesWidth), ResSlicePlots->Add(FittedRecoProtonResSlicesMean);
    }

    for (int i = 0; i < TL_NumberOfSlices; i++) { ResTLMomSlices.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, ResSlicePlots, MomResHistoList, false, true, 1., 9999, 9999, 0, false); }

    for (int i = 0; i < Reco_NumberOfSlices; i++) { ResRecoMomSlices.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, ResSlicePlots, MomResHistoList, false, true, 1., 9999, 9999, 0, false); }

    /* Save res and fitted res plots to plots directory: */
    TFile *PlotsFolder_fout = new TFile((plots_path + "/" + MomResParticle + "_resolution_plots_-_" + SampleName + ".root").c_str(), "recreate");
    PlotsFolder_fout->cd();
    ResSlicePlots->Write();
    PlotsFolder_fout->Write();
    PlotsFolder_fout->Close();
}
#pragma endregion

// LogResDataToFile function --------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* LogResDataToFile function */
void MomentumResolution::LogResDataToFile(const std::string &SampleName, const std::string &plots_path, const std::string &MomentumResolutionDirectory) {
    // TODO: reorder file save in test mode properly
    std::string SaveDateDir = MomentumResolutionDirectory + "Res_data_-_" + SampleName + "/";

    if (!momResTestMode) {
        system(("mkdir -p " + SaveDateDir).c_str());

        LogFitDataToFile(SampleName, plots_path, SaveDateDir);
        LogHistDataToFile(SampleName, plots_path, SaveDateDir);
    } else {
        LogFitDataToFile(SampleName, plots_path, SaveDateDir);
        LogHistDataToFile(SampleName, plots_path, SaveDateDir);
    }
}
#pragma endregion

// LogFitDataToFile function --------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* LogFitDataToFile function */
void MomentumResolution::LogFitDataToFile(const std::string &SampleName, const std::string &plots_path, const std::string &MomentumResolutionDirectory) {
    ofstream Neutron_res_fit_param;
    std::string Neutron_res_fit_paramFilePath;

    if (momResS2CalcMode) {
        if (!momResTestMode) {
            Neutron_res_fit_paramFilePath = MomentumResolutionDirectory + MomResParticle + "_momResS2_fit_param_-_" + SampleName + ".par";
        } else {
            Neutron_res_fit_paramFilePath = plots_path + "/" + MomResParticle + "_momResS2_fit_param_-_" + SampleName + ".par";
        }
    } else {
        if (momResS2RunMode) {
            if (!momResTestMode) {
                Neutron_res_fit_paramFilePath = MomentumResolutionDirectory + MomResParticle + "_momResS2_fit_param_-_" + SampleName + ".par";
            } else {
                Neutron_res_fit_paramFilePath = plots_path + "/" + MomResParticle + "_momResS2_fit_param_-_" + SampleName + ".par";
            }
        } else {
            if (!momResTestMode) {
                Neutron_res_fit_paramFilePath = MomentumResolutionDirectory + MomResParticle + "_momResS1_fit_param_-_" + SampleName + ".par";
            } else {
                Neutron_res_fit_paramFilePath = plots_path + "/" + MomResParticle + "_momResS1_fit_param_-_" + SampleName + ".par";
            }
        }
    }

#pragma region /* Writing log header */
    std::string momRes_calculation_mode;

    if (momResS2CalcMode && !momResS2RunMode) {
        momRes_calculation_mode = "Calculate momResS2";
    } else if (!momResS2CalcMode && momResS2RunMode) {
        momRes_calculation_mode = "Run on loaded momResS2";
    } else if (!momResS2CalcMode && !momResS2RunMode) {
        momRes_calculation_mode = "Calculate momResS1";
    }

    Neutron_res_fit_param.open(Neutron_res_fit_paramFilePath);
    Neutron_res_fit_param << "######################################################################\n";
    Neutron_res_fit_param << "# CLAS12 analysis cuts and parameters file (after nRes Gaussian fit) #\n";
    Neutron_res_fit_param << "######################################################################\n";
    Neutron_res_fit_param << "\n";
    Neutron_res_fit_param << "# " + MomResParticle + " resolution parameters are for:\n";
    Neutron_res_fit_param << "#sample:\t" << SampleName << "\n";
    Neutron_res_fit_param << "#particle:\t" << MomResParticle << "\n";
    Neutron_res_fit_param << "#momRes_calculation_mode:\t" << momRes_calculation_mode << "\n";
    Neutron_res_fit_param << "#delta:\t\t" << delta << "\n";
#pragma endregion

#pragma region /* Logging slice fit results */
    Neutron_res_fit_param << "#Parameters structure:\tSliceNumber:SliceLowerBoundary:SliceUpperBoundary:FitMean:FitSigma\n\n";

    Neutron_res_fit_param << "#Fitted TL slices:\n";
    for (int FittedSlice : FittedTLMomSlices) {
        DSCuts TempCut = ResTLMomSlicesFitVar.at(FittedSlice);
        Neutron_res_fit_param << TempCut.GetCutVariable() << "\t\t\t" << TempCut.GetSliceNumber() << ":" << TempCut.GetSliceLowerb() << ":" << TempCut.GetSliceUpperb() << ":"
                              << TempCut.GetMean() << ":" << TempCut.GetUpperCut() << "\n";
    }

    Neutron_res_fit_param << "\n\n#Fitted Reco slices:\n";
    for (int FittedSlice : FittedRecoMomSlices) {
        DSCuts TempCut = ResRecoMomSlicesFitVar.at(FittedSlice);
        Neutron_res_fit_param << TempCut.GetCutVariable() << "\t\t\t" << TempCut.GetSliceNumber() << ":" << TempCut.GetSliceLowerb() << ":" << TempCut.GetSliceUpperb() << ":"
                              << TempCut.GetMean() << ":" << TempCut.GetUpperCut() << "\n";
    }
#pragma endregion

#pragma region /* Logging correction and smear fit variables */
    if (isNeutron) {
        Neutron_res_fit_param << "\n\n#smearing fit variables:";
        Neutron_res_fit_param << "\n#smearing KC limits:\t" << SliceLowerMomLimKC_sigma << " to " << SliceUpperMomLimKC_sigma;

        Neutron_res_fit_param << "\n\n#pol1 fit variables (no KC):\n";
        AutoLogger(SampleName, plots_path, MomentumResolutionDirectory, "truth_Smear_pol1", TL_FitParam_Smear_pol1, Neutron_res_fit_param);
        AutoLogger(SampleName, plots_path, MomentumResolutionDirectory, "reco_Smear_pol1", Reco_FitParam_Smear_pol1, Neutron_res_fit_param);

        Neutron_res_fit_param << "\n\n#pol1 fit variables (with KC):\n";
        AutoLogger(SampleName, plots_path, MomentumResolutionDirectory, "truth_Smear_pol1_wKC", TL_FitParam_Smear_pol1_wKC, Neutron_res_fit_param);
        AutoLogger(SampleName, plots_path, MomentumResolutionDirectory, "reco_Smear_pol1_wKC", Reco_FitParam_Smear_pol1_wKC, Neutron_res_fit_param);

        Neutron_res_fit_param << "\n\n#pol2 fit variables (no KC):\n";
        AutoLogger(SampleName, plots_path, MomentumResolutionDirectory, "truth_Smear_pol2", TL_FitParam_Smear_pol2, Neutron_res_fit_param);
        AutoLogger(SampleName, plots_path, MomentumResolutionDirectory, "reco_Smear_pol2", Reco_FitParam_Smear_pol2, Neutron_res_fit_param);

        Neutron_res_fit_param << "\n\n#pol2 fit variables (with KC):\n";
        AutoLogger(SampleName, plots_path, MomentumResolutionDirectory, "truth_Smear_pol2_wKC", TL_FitParam_Smear_pol2_wKC, Neutron_res_fit_param);
        AutoLogger(SampleName, plots_path, MomentumResolutionDirectory, "reco_Smear_pol2_wKC", Reco_FitParam_Smear_pol2_wKC, Neutron_res_fit_param);

        Neutron_res_fit_param << "\n\n#pol3 fit variables (no KC):\n";
        AutoLogger(SampleName, plots_path, MomentumResolutionDirectory, "truth_Smear_pol3", TL_FitParam_Smear_pol3, Neutron_res_fit_param);
        AutoLogger(SampleName, plots_path, MomentumResolutionDirectory, "reco_Smear_pol3", Reco_FitParam_Smear_pol3, Neutron_res_fit_param);

        Neutron_res_fit_param << "\n\n#pol3 fit variables (with KC):\n";
        AutoLogger(SampleName, plots_path, MomentumResolutionDirectory, "truth_Smear_pol3_wKC", TL_FitParam_Smear_pol3_wKC, Neutron_res_fit_param);
        AutoLogger(SampleName, plots_path, MomentumResolutionDirectory, "reco_Smear_pol3_wKC", Reco_FitParam_Smear_pol3_wKC, Neutron_res_fit_param);

        Neutron_res_fit_param << "\n\n#correction fit variables:";
        Neutron_res_fit_param << "\n#correction KC limits:\t" << SliceLowerMomLimKC_mu << " to " << SliceUpperMomLimKC_mu;

        Neutron_res_fit_param << "\n\n#pol1 fit variables (no KC):\n";
        AutoLogger(SampleName, plots_path, MomentumResolutionDirectory, "truth_Corr_pol1", TL_FitParam_Corr_pol1, Neutron_res_fit_param);
        AutoLogger(SampleName, plots_path, MomentumResolutionDirectory, "reco_Corr_pol1", Reco_FitParam_Corr_pol1, Neutron_res_fit_param);

        Neutron_res_fit_param << "\n\n#pol1 fit variables (with KC):\n";
        AutoLogger(SampleName, plots_path, MomentumResolutionDirectory, "truth_Corr_pol1_wKC", TL_FitParam_Corr_pol1_wKC, Neutron_res_fit_param);
        AutoLogger(SampleName, plots_path, MomentumResolutionDirectory, "reco_Corr_pol1_wKC", Reco_FitParam_Corr_pol1_wKC, Neutron_res_fit_param);

        Neutron_res_fit_param << "\n\n#pol2 fit variables (no KC):\n";
        AutoLogger(SampleName, plots_path, MomentumResolutionDirectory, "truth_Corr_pol2", TL_FitParam_Corr_pol2, Neutron_res_fit_param);
        AutoLogger(SampleName, plots_path, MomentumResolutionDirectory, "reco_Corr_pol2", Reco_FitParam_Corr_pol2, Neutron_res_fit_param);

        Neutron_res_fit_param << "\n\n#pol2 fit variables (with KC):\n";
        AutoLogger(SampleName, plots_path, MomentumResolutionDirectory, "truth_Corr_pol2_wKC", TL_FitParam_Corr_pol2_wKC, Neutron_res_fit_param);
        AutoLogger(SampleName, plots_path, MomentumResolutionDirectory, "reco_Corr_pol2_wKC", Reco_FitParam_Corr_pol2_wKC, Neutron_res_fit_param);

        Neutron_res_fit_param << "\n\n#pol3 fit variables (no KC):\n";
        AutoLogger(SampleName, plots_path, MomentumResolutionDirectory, "truth_Corr_pol3", TL_FitParam_Corr_pol3, Neutron_res_fit_param);
        AutoLogger(SampleName, plots_path, MomentumResolutionDirectory, "reco_Corr_pol3", Reco_FitParam_Corr_pol3, Neutron_res_fit_param);

        Neutron_res_fit_param << "\n\n#pol3 fit variables (with KC):\n";
        AutoLogger(SampleName, plots_path, MomentumResolutionDirectory, "truth_Corr_pol3_wKC", TL_FitParam_Corr_pol3_wKC, Neutron_res_fit_param);
        AutoLogger(SampleName, plots_path, MomentumResolutionDirectory, "reco_Corr_pol3_wKC", Reco_FitParam_Corr_pol3_wKC, Neutron_res_fit_param);
    }
#pragma endregion

    Neutron_res_fit_param.close();

    if (!momResTestMode) {
        /* Copy fitted parameters file to plots folder for easy download from the ifarm */
        system(("cp " + Neutron_res_fit_paramFilePath + " " + plots_path).c_str());
    }
}
#pragma endregion

#pragma region /* AutoLogger function */
void MomentumResolution::AutoLogger(const std::string &SampleName, const std::string &plots_path, const std::string &MomentumResolutionDirectory, const std::string &LogHeader,
                                    const vector<vector<double>> &Vector2Log, ofstream &Neutron_res_fit_param) {
    if (Vector2Log.size() != 0) {
        vector<string> FitVarName = {"A", "B", "C", "D"}, FitVarErrName = {"A_Err", "B_Err", "C_Err", "D_Err"};
        std::string FitVarNameEnding = "# ", FitVarErrNameEnding = "# ";

        for (int i = 0; i < Vector2Log.at(0).size(); i++) {
            if (i > 0) {
                FitVarNameEnding = FitVarNameEnding + ":" + FitVarName.at(i);
                FitVarErrNameEnding = FitVarErrNameEnding + ":" + FitVarErrName.at(i);
            } else {
                FitVarNameEnding = FitVarNameEnding + FitVarName.at(i);
                FitVarErrNameEnding = FitVarErrNameEnding + FitVarErrName.at(i);
            }
        }

        for (int i = 0; i < Vector2Log.size(); i++) {
            if (i == 0) {
                Neutron_res_fit_param << LogHeader << ":" << "\t\t\t\t\t";

                for (int j = 0; j < Vector2Log.at(i).size(); j++) {
                    if (j == 0) {
                        Neutron_res_fit_param << Vector2Log.at(i).at(j);
                    } else if (j < (Vector2Log.at(i).size() - 1)) {
                        Neutron_res_fit_param << ":" << Vector2Log.at(i).at(j);
                    } else {
                        Neutron_res_fit_param << ":" << Vector2Log.at(i).at(j) << "     " << FitVarNameEnding << "\n";
                    }
                }
            } else if (i == 1) {
                Neutron_res_fit_param << LogHeader << "_error:" << "\t\t\t\t";

                for (int j = 0; j < Vector2Log.at(i).size(); j++) {
                    if (j == 0) {
                        Neutron_res_fit_param << Vector2Log.at(i).at(j);
                    } else if (j < (Vector2Log.at(i).size() - 1)) {
                        Neutron_res_fit_param << ":" << Vector2Log.at(i).at(j);
                    } else {
                        Neutron_res_fit_param << ":" << Vector2Log.at(i).at(j) << "     " << FitVarErrNameEnding << "\n";
                    }
                }
            } else if (i == 2) {
                Neutron_res_fit_param << LogHeader << "_FitGoodness:" << "\t\t";

                for (int j = 0; j < Vector2Log.at(i).size(); j++) {
                    if (j == 0) {
                        Neutron_res_fit_param << Vector2Log.at(i).at(j);
                    } else if (j < (Vector2Log.at(i).size() - 1)) {
                        Neutron_res_fit_param << ":" << Vector2Log.at(i).at(j);
                    } else {
                        Neutron_res_fit_param << ":" << Vector2Log.at(i).at(j) << "     # CHI2:NDF\n";
                    }
                }
            }
        }
    }
}
#pragma endregion

// LogHistDataToFile function --------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* LogHistDataToFile function */
void MomentumResolution::LogHistDataToFile(const std::string &SampleName, const std::string &plots_path, const std::string &MomentumResolutionDirectory) {
    ofstream Neutron_res_Hist_param;
    std::string Neutron_res_Hist_paramFilePath;

    if (momResS2CalcMode) {
        if (!momResTestMode) {
            Neutron_res_Hist_paramFilePath = MomentumResolutionDirectory + MomResParticle + "_momResS2_hist_param_-_" + SampleName + ".par";
        } else {
            Neutron_res_Hist_paramFilePath = plots_path + "/" + MomResParticle + "_momResS2_hist_param_-_" + SampleName + ".par";
        }
    } else {
        if (momResS2RunMode) {
            if (!momResTestMode) {
                Neutron_res_Hist_paramFilePath = MomentumResolutionDirectory + MomResParticle + "_momResS2_hist_param_-_" + SampleName + ".par";
            } else {
                Neutron_res_Hist_paramFilePath = plots_path + "/" + MomResParticle + "_momResS2_hist_param_-_" + SampleName + ".par";
            }
        } else {
            if (!momResTestMode) {
                Neutron_res_Hist_paramFilePath = MomentumResolutionDirectory + MomResParticle + "_momResS1_hist_param_-_" + SampleName + ".par";
            } else {
                Neutron_res_Hist_paramFilePath = plots_path + "/" + MomResParticle + "_momResS1_hist_param_-_" + SampleName + ".par";
            }
        }
    }

#pragma region /* Writing log header */
    std::string momRes_calculation_mode;

    if (momResS2CalcMode && !momResS2RunMode) {
        momRes_calculation_mode = "Calculate momResS2";
    } else if (!momResS2CalcMode && momResS2RunMode) {
        momRes_calculation_mode = "Run on loaded momResS2";
    } else if (!momResS2CalcMode && !momResS2RunMode) {
        momRes_calculation_mode = "Calculate momResS1";
    }

    Neutron_res_Hist_param.open(Neutron_res_Hist_paramFilePath);
    Neutron_res_Hist_param << "######################################################################\n";
    Neutron_res_Hist_param << "# CLAS12 analysis cuts and parameters file (after nRes Gaussian Hist) #\n";
    Neutron_res_Hist_param << "######################################################################\n";
    Neutron_res_Hist_param << "\n";
    Neutron_res_Hist_param << "# " + MomResParticle + " resolution parameters are for:\n";
    Neutron_res_Hist_param << "#sample:\t" << SampleName << "\n";
    Neutron_res_Hist_param << "#momRes_calculation_mode:\t" << momRes_calculation_mode << "\n";
    Neutron_res_Hist_param << "#delta:\t\t" << delta << "\n";
#pragma endregion

    Neutron_res_Hist_param << "#Parameters structure:\tSliceNumber:SliceLowerBoundary:SliceUpperBoundary:HistMean:HistSigma\n\n";  // TODO: confirm order!

    Neutron_res_Hist_param << "#Fitted TL slices:\n";
    for (DSCuts ResSlice : ResTLMomSlicesHistVar) {
        Neutron_res_Hist_param << ResSlice.GetCutVariable() << "\t\t\t" << ResSlice.GetSliceNumber() << ":" << ResSlice.GetSliceLowerb() << ":" << ResSlice.GetSliceUpperb() << ":"
                               << ResSlice.GetMean() << ":" << ResSlice.GetUpperCut() << "\n";
    }

    Neutron_res_Hist_param << "\n\n#Fitted Reco slices:\n";
    for (DSCuts ResSlice : ResRecoMomSlicesHistVar) {
        Neutron_res_Hist_param << ResSlice.GetCutVariable() << "\t\t\t" << ResSlice.GetSliceNumber() << ":" << ResSlice.GetSliceLowerb() << ":" << ResSlice.GetSliceUpperb() << ":"
                               << ResSlice.GetMean() << ":" << ResSlice.GetUpperCut() << "\n";
    }

    Neutron_res_Hist_param.close();

    if (!momResTestMode) {
        /* Copy histogram parameters file to plots folder for easy download from the ifarm */
        system(("cp " + Neutron_res_Hist_paramFilePath + " " + plots_path).c_str());
    }
}
#pragma endregion

// ReadResDataParam function --------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ReadResDataParam function */
void MomentumResolution::ReadResDataParam(const char *filename, const bool &Calculate_momResS2, const std::string &SampleName, const std::string &NucleonCutsDirectory,
                                          const std::string &MomentumType, const bool &Load_correction = false, const bool &Load_smearing = false) {
    ifstream infile;
    infile.open(filename);

    momResS2CalcMode = Calculate_momResS2;
    SName = SampleName;
    SetUpperMomCut(SampleName, NucleonCutsDirectory);

    if (Load_correction) { std::cout << "\033[33m\n\nLoading neutron correction from:\033[0m\n " << filename << "\n "; }

    if (Load_smearing) { std::cout << "\033[33m\nLoading proton smearing from:\033[0m\n " << filename << "\n "; }

    if (infile.is_open()) {
        std::string tp;

        // remove 3 lines of header
        for (int i = 0; i < 3; i++) { getline(infile, tp); }

        // getline(infile, tp) = read data from file object and put it into string.
        while (getline(infile, tp)) {
            stringstream ss(tp);
            std::string parameter, parameter2;
            ss >> parameter;  // get cut identifier

            if (!Load_correction && !Load_smearing) {
                if (FindSubstring(parameter, "fit_" + MomentumType + "_" + "Slice_#")) {
                    // get cut values
                    ss >> parameter2;
                    stringstream ss2(parameter2);
                    std::string SliceParam;
                    int count = 0;  // parameter number

                    std::string CutNameTemp = parameter;
                    int SliceNumberTemp;
                    double SliceLowerBoundaryTemp, SliceUpperBoundaryTemp, FitMeanTemp, FitSigmaTemp;

                    while (getline(ss2, SliceParam, ':')) {
                        if (count == 0) {
                            SliceNumberTemp = stoi(SliceParam);
                        } else if (count == 1) {
                            SliceLowerBoundaryTemp = stod(SliceParam);
                        } else if (count == 2) {
                            SliceUpperBoundaryTemp = stod(SliceParam);
                        } else if (count == 3) {
                            FitMeanTemp = stod(SliceParam);
                        } else if (count == 4) {
                            FitSigmaTemp = stod(SliceParam);
                        }

                        count++;
                    }

                    DSCuts TempFitCut = DSCuts(CutNameTemp, "FD", "Neutron", "1n", FitMeanTemp, -9999, FitSigmaTemp);
                    TempFitCut.SetSliceLowerb(SliceLowerBoundaryTemp);
                    TempFitCut.SetSliceUpperb(SliceUpperBoundaryTemp);
                    TempFitCut.SetSliceNumber(SliceNumberTemp);

                    Loaded_Res_Slices_FitVar.push_back(TempFitCut);
                } else if (FindSubstring(parameter, "hist_" + MomentumType + "_" + "Slice_#")) {
                    // get cut values
                    ss >> parameter2;
                    stringstream ss2(parameter2);
                    std::string SliceParam;
                    int count = 0;  // parameter number

                    std::string CutNameTemp = parameter;
                    int SliceNumberTemp;
                    double SliceLowerBoundaryTemp, SliceUpperBoundaryTemp, HistMeanTemp, HistSigmaTemp;

                    while (getline(ss2, SliceParam, ':')) {
                        if (count == 0) {
                            SliceNumberTemp = stoi(SliceParam);
                        } else if (count == 1) {
                            SliceLowerBoundaryTemp = stod(SliceParam);
                        } else if (count == 2) {
                            SliceUpperBoundaryTemp = stod(SliceParam);
                        } else if (count == 3) {
                            HistMeanTemp = stod(SliceParam);
                        } else if (count == 4) {
                            HistSigmaTemp = stod(SliceParam);
                        }

                        count++;
                    }

                    DSCuts TempHistCut = DSCuts(CutNameTemp, "FD", "Neutron", "1n", HistMeanTemp, -9999, HistSigmaTemp);
                    TempHistCut.SetSliceLowerb(SliceLowerBoundaryTemp);
                    TempHistCut.SetSliceUpperb(SliceUpperBoundaryTemp);
                    TempHistCut.SetSliceNumber(SliceNumberTemp);

                    Loaded_Res_Slices_HistVar.push_back(TempHistCut);
                }
            } else {
                ss >> parameter2;
                stringstream ss2(parameter2);

                // TODO: reorganize these into vectors!
                if (Load_correction && FindSubstring(parameter, MomentumType) && FindSubstring(parameter, "Corr")) {
                    Loaded_Corr_coefficients_path = filename;

#pragma region /* Safety checks for loading correction variables */
                    if (Loaded_Corr_coefficients_path == "") {
                        std::cout << "\n\nMomentumResolution::ReadResDataParam: Loaded_Corr_coefficients_path is empty!\n";
                        std::cout << "The file:\n" << filename << "\n";
                        std::cout << "is not found! Aborting...\n\n", exit(1);
                    }
#pragma endregion

                    if (FindSubstring(parameter, "pol1") && FindSubstring(CorrMode, "pol1")) {
                        if (FindSubstring(parameter, "pol1_wKC") && FindSubstring(CorrMode, "pol1_wKC")) {
                            if (MomentumType == "truth") {
                                AutoReader(MomentumType, 1, parameter, ss2, Loaded_TL_FitParam_Corr_pol1_wKC);
                                //                                AutoReader(MomentumType, 1, parameter, ss2, Loaded_TL_FitParam_Corr_pol1_wKC, Loaded_Corr_coefficients_values,
                                //                                           Loaded_Corr_coefficients_names);
                            } else if (MomentumType == "reco") {
                                AutoReader(MomentumType, 1, parameter, ss2, Loaded_Reco_FitParam_Corr_pol1_wKC);
                                //                                AutoReader(MomentumType, 1, parameter, ss2, Loaded_Reco_FitParam_Corr_pol1_wKC, Loaded_Corr_coefficients_values,
                                //                                           Loaded_Corr_coefficients_names);
                            }
                        } else if (FindSubstring(parameter, "pol1") && !FindSubstring(CorrMode, "pol1_wKC")) {
                            if (MomentumType == "truth") {
                                AutoReader(MomentumType, 1, parameter, ss2, Loaded_TL_FitParam_Corr_pol1);
                                //                                AutoReader(MomentumType, 1, parameter, ss2, Loaded_TL_FitParam_Corr_pol1, Loaded_Corr_coefficients_values,
                                //                                           Loaded_Corr_coefficients_names);
                            } else if (MomentumType == "reco") {
                                AutoReader(MomentumType, 1, parameter, ss2, Loaded_Reco_FitParam_Corr_pol1);
                                //                                AutoReader(MomentumType, 1, parameter, ss2, Loaded_Reco_FitParam_Corr_pol1, Loaded_Corr_coefficients_values,
                                //                                           Loaded_Corr_coefficients_names);
                            }
                        }
                    } else if (FindSubstring(parameter, "pol2") && FindSubstring(CorrMode, "pol2")) {
                        if (FindSubstring(parameter, "pol2_wKC") && FindSubstring(CorrMode, "pol2_wKC")) {
                            if (MomentumType == "truth") {
                                AutoReader(MomentumType, 2, parameter, ss2, Loaded_TL_FitParam_Corr_pol2_wKC);
                                //                                AutoReader(MomentumType, 2, parameter, ss2, Loaded_TL_FitParam_Corr_pol2_wKC, Loaded_Corr_coefficients_values,
                                //                                           Loaded_Corr_coefficients_names);
                            } else if (MomentumType == "reco") {
                                AutoReader(MomentumType, 2, parameter, ss2, Loaded_Reco_FitParam_Corr_pol2_wKC);
                                //                                AutoReader(MomentumType, 2, parameter, ss2, Loaded_Reco_FitParam_Corr_pol2_wKC, Loaded_Corr_coefficients_values,
                                //                                           Loaded_Corr_coefficients_names);
                            }
                        } else if (FindSubstring(parameter, "pol2") && !FindSubstring(CorrMode, "pol2_wKC")) {
                            if (MomentumType == "truth") {
                                AutoReader(MomentumType, 2, parameter, ss2, Loaded_TL_FitParam_Corr_pol2);
                                //                                AutoReader(MomentumType, 2, parameter, ss2, Loaded_TL_FitParam_Corr_pol2, Loaded_Corr_coefficients_values,
                                //                                           Loaded_Corr_coefficients_names);
                            } else if (MomentumType == "reco") {
                                AutoReader(MomentumType, 2, parameter, ss2, Loaded_Reco_FitParam_Corr_pol2);
                                //                                AutoReader(MomentumType, 2, parameter, ss2, Loaded_Reco_FitParam_Corr_pol2, Loaded_Corr_coefficients_values,
                                //                                           Loaded_Corr_coefficients_names);
                            }
                        }
                    } else if (FindSubstring(parameter, "pol3") && FindSubstring(CorrMode, "pol3")) {
                        if (FindSubstring(parameter, "pol3_wKC") && FindSubstring(CorrMode, "pol3_wKC")) {
                            if (MomentumType == "truth") {
                                AutoReader(MomentumType, 3, parameter, ss2, Loaded_TL_FitParam_Corr_pol3_wKC);
                                //                                AutoReader(MomentumType, 3, parameter, ss2, Loaded_TL_FitParam_Corr_pol3_wKC, Loaded_Corr_coefficients_values,
                                //                                           Loaded_Corr_coefficients_names);
                            } else if (MomentumType == "reco") {
                                AutoReader(MomentumType, 3, parameter, ss2, Loaded_Reco_FitParam_Corr_pol3_wKC);
                                //                                AutoReader(MomentumType, 3, parameter, ss2, Loaded_Reco_FitParam_Corr_pol3_wKC, Loaded_Corr_coefficients_values,
                                //                                           Loaded_Corr_coefficients_names);
                            }
                        } else if (FindSubstring(parameter, "pol3") && !FindSubstring(CorrMode, "pol3_wKC")) {
                            if (MomentumType == "truth") {
                                AutoReader(MomentumType, 3, parameter, ss2, Loaded_TL_FitParam_Corr_pol3);
                                //                                AutoReader(MomentumType, 3, parameter, ss2, Loaded_TL_FitParam_Corr_pol3, Loaded_Corr_coefficients_values,
                                //                                           Loaded_Corr_coefficients_names);
                            } else if (MomentumType == "reco") {
                                AutoReader(MomentumType, 3, parameter, ss2, Loaded_Reco_FitParam_Corr_pol3);
                                //                                AutoReader(MomentumType, 3, parameter, ss2, Loaded_Reco_FitParam_Corr_pol3, Loaded_Corr_coefficients_values,
                                //                                           Loaded_Corr_coefficients_names);
                            }
                        }
                    }
                } else if (Load_smearing && FindSubstring(parameter, MomentumType) && FindSubstring(parameter, "Smear")) {
                    Loaded_Std_coefficients_path = filename;

#pragma region /* Safety checks for loading smearing variables */
                    if (Load_smearing && Loaded_Std_coefficients_path == "") {
                        std::cout << "\n\nMomentumResolution::ReadResDataParam: Loaded_Std_coefficients_path is empty!\n";
                        std::cout << "The file:\n" << filename << "\n";
                        std::cout << "is not found! Aborting...\n\n", exit(1);
                    }
#pragma endregion

                    if (FindSubstring(parameter, "pol1") && FindSubstring(SmearMode, "pol1")) {
                        if (FindSubstring(parameter, "pol1_wKC") && FindSubstring(SmearMode, "pol1_wKC")) {
                            if (MomentumType == "truth") {
                                AutoReader(MomentumType, 1, parameter, ss2, Loaded_TL_FitParam_Smear_pol1_wKC);
                                //                                AutoReader(MomentumType, 1, parameter, ss2, Loaded_TL_FitParam_Smear_pol1_wKC, Loaded_Smear_coefficients_values,
                                //                                           Loaded_Smear_coefficients_names);
                            } else if (MomentumType == "reco") {
                                AutoReader(MomentumType, 1, parameter, ss2, Loaded_Reco_FitParam_Smear_pol1_wKC);
                            }
                        } else if (FindSubstring(parameter, "pol1") && !FindSubstring(SmearMode, "pol1_wKC")) {
                            if (MomentumType == "truth") {
                                AutoReader(MomentumType, 1, parameter, ss2, Loaded_TL_FitParam_Smear_pol1);
                                //                                AutoReader(MomentumType, 1, parameter, ss2, Loaded_TL_FitParam_Smear_pol1, Loaded_Smear_coefficients_values,
                                //                                           Loaded_Smear_coefficients_names);
                            } else if (MomentumType == "reco") {
                                AutoReader(MomentumType, 1, parameter, ss2, Loaded_Reco_FitParam_Smear_pol1);
                                //                                AutoReader(MomentumType, 1, parameter, ss2, Loaded_Reco_FitParam_Smear_pol1, Loaded_Smear_coefficients_values,
                                //                                           Loaded_Smear_coefficients_names);
                            }
                        }
                    } else if (FindSubstring(parameter, "pol2") && FindSubstring(SmearMode, "pol2")) {
                        if (FindSubstring(parameter, "pol2_wKC") && FindSubstring(SmearMode, "pol2_wKC")) {
                            if (MomentumType == "truth") {
                                AutoReader(MomentumType, 2, parameter, ss2, Loaded_TL_FitParam_Smear_pol2_wKC);
                                //                                AutoReader(MomentumType, 2, parameter, ss2, Loaded_TL_FitParam_Smear_pol2_wKC, Loaded_Smear_coefficients_values,
                                //                                           Loaded_Smear_coefficients_names);
                            } else if (MomentumType == "reco") {
                                AutoReader(MomentumType, 2, parameter, ss2, Loaded_Reco_FitParam_Smear_pol2_wKC);
                                //                                AutoReader(MomentumType, 2, parameter, ss2, Loaded_Reco_FitParam_Smear_pol2_wKC, Loaded_Smear_coefficients_values,
                                //                                           Loaded_Smear_coefficients_names);
                            }
                        } else if (FindSubstring(parameter, "pol2") && !FindSubstring(SmearMode, "pol2_wKC")) {
                            if (MomentumType == "truth") {
                                AutoReader(MomentumType, 2, parameter, ss2, Loaded_TL_FitParam_Smear_pol2);
                                //                                AutoReader(MomentumType, 2, parameter, ss2, Loaded_TL_FitParam_Smear_pol2, Loaded_Smear_coefficients_values,
                                //                                           Loaded_Smear_coefficients_names);
                            } else if (MomentumType == "reco") {
                                AutoReader(MomentumType, 2, parameter, ss2, Loaded_Reco_FitParam_Smear_pol2);
                                //                                AutoReader(MomentumType, 2, parameter, ss2, Loaded_Reco_FitParam_Smear_pol2, Loaded_Smear_coefficients_values,
                                //                                           Loaded_Smear_coefficients_names);
                            }
                        }
                    } else if (FindSubstring(parameter, "pol3") && FindSubstring(SmearMode, "pol3")) {
                        if (FindSubstring(parameter, "pol3_wKC") && FindSubstring(SmearMode, "pol3_wKC")) {
                            if (MomentumType == "truth") {
                                AutoReader(MomentumType, 3, parameter, ss2, Loaded_TL_FitParam_Smear_pol3_wKC);
                                //                                AutoReader(MomentumType, 3, parameter, ss2, Loaded_TL_FitParam_Smear_pol3_wKC, Loaded_Smear_coefficients_values,
                                //                                           Loaded_Smear_coefficients_names);
                            } else if (MomentumType == "reco") {
                                AutoReader(MomentumType, 3, parameter, ss2, Loaded_Reco_FitParam_Smear_pol3_wKC);
                                //                                AutoReader(MomentumType, 3, parameter, ss2, Loaded_Reco_FitParam_Smear_pol3_wKC, Loaded_Smear_coefficients_values,
                                //                                           Loaded_Smear_coefficients_names);
                            }
                        } else if (FindSubstring(parameter, "pol3") && !FindSubstring(SmearMode, "pol3_wKC")) {
                            if (MomentumType == "truth") {
                                AutoReader(MomentumType, 3, parameter, ss2, Loaded_TL_FitParam_Smear_pol3);
                                //                                AutoReader(MomentumType, 3, parameter, ss2, Loaded_TL_FitParam_Smear_pol3, Loaded_Smear_coefficients_values,
                                //                                           Loaded_Smear_coefficients_names);
                            } else if (MomentumType == "reco") {
                                AutoReader(MomentumType, 3, parameter, ss2, Loaded_Reco_FitParam_Smear_pol3);
                                //                                AutoReader(MomentumType, 3, parameter, ss2, Loaded_Reco_FitParam_Smear_pol3, Loaded_Smear_coefficients_values,
                                //                                           Loaded_Smear_coefficients_names);
                            }
                        }
                    }
                }

#pragma region /* Safety checks */
                if (Loaded_Smear_coefficients_values.size() != Loaded_Smear_coefficients_names.size()) {
                    std::cout << "\n\nMomentumResolution::ReadResDataParam: smearing coefficients load registered improperly! Aborting...\n\n", exit(1);
                }

                if (Loaded_Corr_coefficients_values.size() != Loaded_Corr_coefficients_names.size()) {
                    std::cout << "\n\nMomentumResolution::ReadResDataParam: correction coefficients load registered improperly! Aborting...\n\n", exit(1);
                }
#pragma endregion
            }
        }
    }
    // else {
    //     std::cout << "\n\nMomentumResolution::ReadResDataParam: file not found! Aborting...\n\n", exit(1);
    // }

    std::cout << "\033[35m\n\nMomentumResolution::ReadResDataParam:\033[36m Warning!\033[0m file:\n" << filename << "\nwas not found!\n\n";
}
#pragma endregion

#pragma region /* AutoReader function */
void MomentumResolution::AutoReader(const std::string &MomentumType, const int &PolynomialDegree, const std::string &parameter, basic_istream<char> &ss2,
                                    vector<vector<double>> &Loading_Dest, vector<double> &Loaded_coefficients_values, vector<string> &Loaded_coefficients_names) {
    if (FindSubstring(parameter, MomentumType)) {
        vector<string> VarNames = {"A", "B", "C", "D"};
        vector<double> Loaded_FitVarResults, Loaded_FitVarResultsErrors, Loaded_FitVarResultsGoodness;
        std::string Loaded_FitVar;

        int counter = 0;

        while (getline(ss2, Loaded_FitVar, ':')) {
            if (!FindSubstring(parameter, "error") && !FindSubstring(parameter, "FitGoodness")) {
                Loaded_FitVarResults.push_back(stod(Loaded_FitVar));
                Loaded_coefficients_values.push_back(stod(Loaded_FitVar));
                Loaded_coefficients_names.push_back((VarNames.at(counter) + " (" + MomentumType + ")"));
                ++counter;
            } else if (FindSubstring(parameter, "error") && !FindSubstring(parameter, "FitGoodness")) {
                Loaded_FitVarResultsErrors.push_back(stod(Loaded_FitVar));
            } else if (!FindSubstring(parameter, "error") && FindSubstring(parameter, "FitGoodness")) {
                Loaded_FitVarResultsGoodness.push_back(stod(Loaded_FitVar));
            }
        }

        if (!FindSubstring(parameter, "error") && !FindSubstring(parameter, "FitGoodness")) {
            Loading_Dest.push_back(Loaded_FitVarResults);
        } else if (FindSubstring(parameter, "error") && !FindSubstring(parameter, "FitGoodness")) {
            Loading_Dest.push_back(Loaded_FitVarResultsErrors);
        } else if (!FindSubstring(parameter, "error") && FindSubstring(parameter, "FitGoodness")) {
            Loading_Dest.push_back(Loaded_FitVarResultsGoodness);
        }

#pragma region /* Safety checks */
        if (!FindSubstring(parameter, "error") && !FindSubstring(parameter, "FitGoodness")) {
            if (Loaded_FitVarResults.size() != (PolynomialDegree + 1)) {
                std::cout << "\n\nMomentumResolution::AutoReader: the parameter " << parameter << " loaded improperly!\n";
                std::cout << "Loaded_FitVarResults.size() = " << Loaded_FitVarResults.size() << "\n";
                std::cout << "expected size (= PolynomialDegree) = " << PolynomialDegree << "\n";
                std::cout << "Loaded_FitVarResultsErrors elements:\n";
                for (int i = 0; i < Loaded_FitVarResults.size(); i++) { std::cout << Loaded_FitVarResults.at(i) << "\n"; }
                std::cout << "\nAborting...\n\n", exit(1);
            }
        }

        if (FindSubstring(parameter, "error") && !FindSubstring(parameter, "FitGoodness")) {
            if (Loaded_FitVarResultsErrors.size() != (PolynomialDegree + 1)) {
                std::cout << "\n\nMomentumResolution::AutoReader: the parameter " << parameter << " loaded improperly!\n";
                std::cout << "Loaded_FitVarResultsErrors.size() = " << Loaded_FitVarResultsErrors.size() << "\n";
                std::cout << "expected size (= PolynomialDegree) = " << PolynomialDegree << "\n";
                std::cout << "Loaded_FitVarResultsErrors elements:\n";
                for (int i = 0; i < Loaded_FitVarResultsErrors.size(); i++) { std::cout << Loaded_FitVarResultsErrors.at(i) << "\n"; }
                std::cout << "\nAborting...\n\n", exit(1);
            }
        }

        if (!FindSubstring(parameter, "error") && FindSubstring(parameter, "FitGoodness")) {
            if (Loaded_FitVarResultsGoodness.size() != 2) {
                std::cout << "\n\nMomentumResolution::AutoReader: the parameter " << parameter << " loaded improperly!\n";
                std::cout << "Loaded_FitVarResultsErrors.size() = " << Loaded_FitVarResultsGoodness.size() << "\n";
                std::cout << "expected size = " << 2 << "\n";
                std::cout << "Loaded_FitVarResultsErrors elements:\n";
                for (int i = 0; i < Loaded_FitVarResultsGoodness.size(); i++) { std::cout << Loaded_FitVarResultsGoodness.at(i) << "\n"; }
                std::cout << "\nAborting...\n\n", exit(1);
            }
        }
#pragma endregion
    }
}
#pragma endregion

#pragma region /* AutoReader function */
void MomentumResolution::AutoReader(const std::string &MomentumType, const int &PolynomialDegree, const std::string &parameter, basic_istream<char> &ss2,
                                    vector<vector<double>> &Loading_Dest) {
    bool PrintOut = false;
    bool PrintOutAndExit = false;
    int PolynomialDeg2Print = 3;
    std::string MomentumType2Print = "reco";

    if (FindSubstring(parameter, MomentumType)) {
        vector<string> VarNames = {"A", "B", "C", "D"};
        vector<double> Loaded_FitVarResults, Loaded_FitVarResultsErrors, Loaded_FitVarResultsGoodness;
        std::string Loaded_FitVar;

        if (PrintOut && (PolynomialDegree == PolynomialDeg2Print) && (MomentumType == MomentumType2Print)) { std::cout << "parameter = " << parameter << "\n"; }

        while (getline(ss2, Loaded_FitVar, ':')) {
            if (!FindSubstring(parameter, "error") && !FindSubstring(parameter, "FitGoodness")) {
                Loaded_FitVarResults.push_back(stod(Loaded_FitVar));

                if (PrintOut && (PolynomialDegree == PolynomialDeg2Print) && (MomentumType == MomentumType2Print)) { std::cout << "Loaded_FitVar = " << Loaded_FitVar << "\n"; }
            } else if (FindSubstring(parameter, "error") && !FindSubstring(parameter, "FitGoodness")) {
                Loaded_FitVarResultsErrors.push_back(stod(Loaded_FitVar));
            } else if (!FindSubstring(parameter, "error") && FindSubstring(parameter, "FitGoodness")) {
                Loaded_FitVarResultsGoodness.push_back(stod(Loaded_FitVar));
            }
        }

        if (PrintOutAndExit && (PolynomialDegree == PolynomialDeg2Print) && (MomentumType == MomentumType2Print)) { exit(1); }

        if (!FindSubstring(parameter, "error") && !FindSubstring(parameter, "FitGoodness")) {
            Loading_Dest.push_back(Loaded_FitVarResults);
        } else if (FindSubstring(parameter, "error") && !FindSubstring(parameter, "FitGoodness")) {
            Loading_Dest.push_back(Loaded_FitVarResultsErrors);
        } else if (!FindSubstring(parameter, "error") && FindSubstring(parameter, "FitGoodness")) {
            Loading_Dest.push_back(Loaded_FitVarResultsGoodness);
        }

#pragma region /* Safety checks */
        if (!FindSubstring(parameter, "error") && !FindSubstring(parameter, "FitGoodness")) {
            if (Loaded_FitVarResults.size() != (PolynomialDegree + 1)) {
                std::cout << "\n\nMomentumResolution::AutoReader: the parameter " << parameter << " loaded improperly!\n";
                std::cout << "Loaded_FitVarResults.size() = " << Loaded_FitVarResults.size() << "\n";
                std::cout << "expected size (= PolynomialDegree) = " << PolynomialDegree << "\n";
                std::cout << "Loaded_FitVarResultsErrors elements:\n";
                for (int i = 0; i < Loaded_FitVarResults.size(); i++) { std::cout << Loaded_FitVarResults.at(i) << "\n"; }
                std::cout << "\nAborting...\n\n", exit(1);
            }
        }

        if (FindSubstring(parameter, "error") && !FindSubstring(parameter, "FitGoodness")) {
            if (Loaded_FitVarResultsErrors.size() != (PolynomialDegree + 1)) {
                std::cout << "\n\nMomentumResolution::AutoReader: the parameter " << parameter << " loaded improperly!\n";
                std::cout << "Loaded_FitVarResultsErrors.size() = " << Loaded_FitVarResultsErrors.size() << "\n";
                std::cout << "expected size (= PolynomialDegree) = " << PolynomialDegree << "\n";
                std::cout << "Loaded_FitVarResultsErrors elements:\n";
                for (int i = 0; i < Loaded_FitVarResultsErrors.size(); i++) { std::cout << Loaded_FitVarResultsErrors.at(i) << "\n"; }
                std::cout << "\nAborting...\n\n", exit(1);
            }
        }

        if (!FindSubstring(parameter, "error") && FindSubstring(parameter, "FitGoodness")) {
            if (Loaded_FitVarResultsGoodness.size() != 2) {
                std::cout << "\n\nMomentumResolution::AutoReader: the parameter " << parameter << " loaded improperly!\n";
                std::cout << "Loaded_FitVarResultsErrors.size() = " << Loaded_FitVarResultsGoodness.size() << "\n";
                std::cout << "expected size = " << 2 << "\n";
                std::cout << "Loaded_FitVarResultsErrors elements:\n";
                for (int i = 0; i < Loaded_FitVarResultsGoodness.size(); i++) { std::cout << Loaded_FitVarResultsGoodness.at(i) << "\n"; }
                std::cout << "\nAborting...\n\n", exit(1);
            }
        }
#pragma endregion
    }
}
#pragma endregion

// PSmear function ------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* PSmear function */
/* A function to smear protons by fitted neutron resolution */

double MomentumResolution::PSmear(const bool &apply_nucleon_SmearAndCorr, const double &Momentum) {
    bool PrintOut = false;
    bool PrintOut_Smear_Variables = false;
    bool PrintOut_And_Exit = false;

    if (!apply_nucleon_SmearAndCorr) {
        /* Smearing and correction are disabled */
        return Momentum;
    } else {
        /* Smearing and correction are enabled */

        /* Setting momentum powers */
        double SmearedMomentum, Momentum2 = Momentum * Momentum, Momentum3 = Momentum2 * Momentum;

        if (SmearMode == "slices") {
            /* Smear using slice fit results */
            double Smearing;

#pragma region                                  /* Original (smearing from loaded nRes fit variables) */
            if (Momentum < SliceUpperMomLim) {  // NOTE: changed according to upper neutron mom. th.
                for (DSCuts Loaded_res_slice : Loaded_Res_Slices_FitVar) {
                    if ((Loaded_res_slice.GetSliceLowerb() < Momentum) && (Loaded_res_slice.GetSliceUpperb() >= Momentum)) {
                        if (RandomNumGenerator == "TRandom1") {
                            Smearing = Rand1->Gaus(1, Loaded_res_slice.GetUpperCut());
                        } else if (RandomNumGenerator == "TRandom2") {
                            Smearing = Rand2->Gaus(1, Loaded_res_slice.GetUpperCut());
                        } else if (RandomNumGenerator == "TRandom3") {
                            Smearing = Rand3->Gaus(1, Loaded_res_slice.GetUpperCut());
                        } else {
                            std::cout << "\n\nMomentumResolution::PSmear: RandomNumGenerator illegal! Aborting...\n", exit(1);
                        }

                        //                        double Smearing = Rand->Gaus(1, Loaded_res_slice.GetUpperCut());

                        if (PrintOut) {
                            std::cout << "\n\nLoaded_res_slice.GetUpperCut() = " << Loaded_res_slice.GetUpperCut() << "\n";
                            std::cout << "RandomNumGenerator = " << RandomNumGenerator << "\n";
                            std::cout << "Momentum = " << Momentum << "\n";
                            std::cout << "Smearing = " << Smearing << "\n";
                            std::cout << "Smearing * Momentum = " << Smearing * Momentum << "\n\n";
                        }

                        return Smearing * Momentum;
                    }
                }
            } else {
                double Arg = Loaded_Res_Slices_FitVar.at(Loaded_Res_Slices_FitVar.size() - 1).GetUpperCut();

                if (RandomNumGenerator == "TRandom1") {
                    Smearing = Rand1->Gaus(1, Arg);
                } else if (RandomNumGenerator == "TRandom2") {
                    Smearing = Rand2->Gaus(1, Arg);
                } else if (RandomNumGenerator == "TRandom3") {
                    Smearing = Rand3->Gaus(1, Arg);
                } else {
                    std::cout << "\n\nMomentumResolution::PSmear: RandomNumGenerator illegal! Aborting...\n", exit(1);
                }

                if (PrintOut) {
                    std::cout << "\n\nLoaded_Res_Slices_FitVar.GetUpperCut() = " << Loaded_Res_Slices_FitVar.at(Loaded_Res_Slices_FitVar.size() - 1).GetUpperCut() << "\n";
                    std::cout << "Loaded_res_slice.GetSliceLowerb() = " << Loaded_Res_Slices_FitVar.at(Loaded_Res_Slices_FitVar.size() - 1).GetSliceLowerb() << "\n";
                    std::cout << "Loaded_res_slice.GetSliceUpperb() = " << Loaded_Res_Slices_FitVar.at(Loaded_Res_Slices_FitVar.size() - 1).GetSliceUpperb() << "\n";
                    std::cout << "RandomNumGenerator = " << RandomNumGenerator << "\n";
                    std::cout << "Arg = " << Arg << "\n";
                    std::cout << "Momentum = " << Momentum << "\n";
                    std::cout << "Smearing = " << Smearing << "\n";
                    std::cout << "Smearing * Momentum = " << Smearing * Momentum << "\n\n";
                }

                return Smearing * Momentum;
            }
#pragma endregion
        } else if ((SmearMode == "pol1") || (SmearMode == "pol2") || (SmearMode == "pol3") || (SmearMode == "pol1_wKC") || (SmearMode == "pol2_wKC") || (SmearMode == "pol3_wKC")) {
            /* Smear using pol fit results */
            double Smearing, Arg;
            vector<string> VarNames = {"A", "B", "C", "D"};

            if (SmearMode == "pol1") {
#pragma region /* Safety checks */
                if (Loaded_Reco_FitParam_Smear_pol1.size() == 0) { std::cout << "\n\nMomentumResolution::PSmear: Loaded_Reco_FitParam_Smear_pol1 wasn't loaded! Aborting...\n", exit(1); }
                if (Loaded_Reco_FitParam_Smear_pol1.at(0).size() == 0) {
                    std::cout << "\n\nMomentumResolution::PSmear: Loaded_Reco_FitParam_Smear_pol1 smearing wasn't loaded! Aborting...\n", exit(1);
                }
#pragma endregion

                Arg = Loaded_Reco_FitParam_Smear_pol1.at(0).at(0) * Momentum + Loaded_Reco_FitParam_Smear_pol1.at(0).at(1);

                if ((Loaded_Smear_coefficients_values.size() == 0) || (Loaded_Smear_coefficients_names.size() == 0)) {
                    Loaded_Smear_coefficients_values.push_back(Loaded_Reco_FitParam_Smear_pol1.at(0).at(0));
                    Loaded_Smear_coefficients_names.push_back(VarNames.at(0));
                    Loaded_Smear_coefficients_values.push_back(Loaded_Reco_FitParam_Smear_pol1.at(0).at(1));
                    Loaded_Smear_coefficients_names.push_back(VarNames.at(1));
                }

                if (PrintOut_Smear_Variables) {
                    std::cout << "\n\n\n#(FitParam groups) = " << Loaded_Reco_FitParam_Smear_pol1.size() << "\n";
                    std::cout << "#(FitParam variables) = " << Loaded_Reco_FitParam_Smear_pol1.at(0).size() << "\n";
                    std::cout << "#(FitParam errors) = " << Loaded_Reco_FitParam_Smear_pol1.at(1).size() << "\n";
                    std::cout << "#(FitParam fit goodness variables) = " << Loaded_Reco_FitParam_Smear_pol1.at(2).size() << "\n\n";

                    std::cout << "Loaded_A_Std_pol1 = " << Loaded_Reco_FitParam_Smear_pol1.at(0).at(0) << "\n";
                    std::cout << "Loaded_B_Std_pol1 = " << Loaded_Reco_FitParam_Smear_pol1.at(0).at(1) << "\n\n";
                }
            } else if (SmearMode == "pol1_wKC") {
#pragma region /* Safety checks */
                if (Loaded_Reco_FitParam_Smear_pol1_wKC.size() == 0) {
                    std::cout << "\n\nMomentumResolution::PSmear: Loaded_Reco_FitParam_Smear_pol1_wKC wasn't loaded! Aborting...\n", exit(1);
                }
                if (Loaded_Reco_FitParam_Smear_pol1_wKC.at(0).size() == 0) {
                    std::cout << "\n\nMomentumResolution::PSmear: Loaded_Reco_FitParam_Smear_pol1_wKC smearing wasn't loaded! Aborting...\n", exit(1);
                }
#pragma endregion

                Arg = Loaded_Reco_FitParam_Smear_pol1_wKC.at(0).at(0) * Momentum + Loaded_Reco_FitParam_Smear_pol1_wKC.at(0).at(1);

                if ((Loaded_Smear_coefficients_values.size() == 0) || (Loaded_Smear_coefficients_names.size() == 0)) {
                    Loaded_Smear_coefficients_values.push_back(Loaded_Reco_FitParam_Smear_pol1_wKC.at(0).at(0));
                    Loaded_Smear_coefficients_names.push_back(VarNames.at(0));
                    Loaded_Smear_coefficients_values.push_back(Loaded_Reco_FitParam_Smear_pol1_wKC.at(0).at(1));
                    Loaded_Smear_coefficients_names.push_back(VarNames.at(1));
                }

                if (PrintOut_Smear_Variables) {
                    std::cout << "\n\n\n#(FitParam groups) = " << Loaded_Reco_FitParam_Smear_pol1_wKC.size() << "\n";
                    std::cout << "#(FitParam variables) = " << Loaded_Reco_FitParam_Smear_pol1_wKC.at(0).size() << "\n";
                    std::cout << "#(FitParam errors) = " << Loaded_Reco_FitParam_Smear_pol1_wKC.at(1).size() << "\n";
                    std::cout << "#(FitParam fit goodness variables) = " << Loaded_Reco_FitParam_Smear_pol1_wKC.at(2).size() << "\n\n";

                    std::cout << "Loaded_A_Std_pol1_wKC = " << Loaded_Reco_FitParam_Smear_pol1_wKC.at(0).at(0) << "\n";
                    std::cout << "Loaded_B_Std_pol1_wKC = " << Loaded_Reco_FitParam_Smear_pol1_wKC.at(0).at(1) << "\n\n";
                }
            } else if (SmearMode == "pol2") {
#pragma region /* Safety checks */
                if (Loaded_Reco_FitParam_Smear_pol2.size() == 0) { std::cout << "\n\nMomentumResolution::PSmear: Loaded_Reco_FitParam_Smear_pol2 wasn't loaded! Aborting...\n", exit(1); }
                if (Loaded_Reco_FitParam_Smear_pol2.at(0).size() == 0) {
                    std::cout << "\n\nMomentumResolution::PSmear: Loaded_Reco_FitParam_Smear_pol2 smearing wasn't loaded! Aborting...\n", exit(1);
                }
#pragma endregion

                Arg = Loaded_Reco_FitParam_Smear_pol2.at(0).at(0) * Momentum2 + Loaded_Reco_FitParam_Smear_pol2.at(0).at(1) * Momentum + Loaded_Reco_FitParam_Smear_pol2.at(0).at(2);

                if ((Loaded_Smear_coefficients_values.size() == 0) || (Loaded_Smear_coefficients_names.size() == 0)) {
                    Loaded_Smear_coefficients_values.push_back(Loaded_Reco_FitParam_Smear_pol2.at(0).at(0));
                    Loaded_Smear_coefficients_names.push_back(VarNames.at(0));
                    Loaded_Smear_coefficients_values.push_back(Loaded_Reco_FitParam_Smear_pol2.at(0).at(1));
                    Loaded_Smear_coefficients_names.push_back(VarNames.at(1));
                    Loaded_Smear_coefficients_values.push_back(Loaded_Reco_FitParam_Smear_pol2.at(0).at(2));
                    Loaded_Smear_coefficients_names.push_back(VarNames.at(2));
                }

                if (PrintOut_Smear_Variables) {
                    std::cout << "\n\n\n#(FitParam groups) = " << Loaded_Reco_FitParam_Smear_pol2.size() << "\n";
                    std::cout << "#(FitParam variables) = " << Loaded_Reco_FitParam_Smear_pol2.at(0).size() << "\n";
                    std::cout << "#(FitParam errors) = " << Loaded_Reco_FitParam_Smear_pol2.at(1).size() << "\n";
                    std::cout << "#(FitParam fit goodness variables) = " << Loaded_Reco_FitParam_Smear_pol2.at(2).size() << "\n\n";

                    std::cout << "Loaded_A_Std_pol2 = " << Loaded_Reco_FitParam_Smear_pol2.at(0).at(0) << "\n";
                    std::cout << "Loaded_B_Std_pol2 = " << Loaded_Reco_FitParam_Smear_pol2.at(0).at(1) << "\n";
                    std::cout << "Loaded_C_Std_pol2 = " << Loaded_Reco_FitParam_Smear_pol2.at(0).at(2) << "\n\n";
                }
            } else if (SmearMode == "pol2_wKC") {
#pragma region /* Safety checks */
                if (Loaded_Reco_FitParam_Smear_pol2_wKC.size() == 0) {
                    std::cout << "\n\nMomentumResolution::PSmear: Loaded_Reco_FitParam_Smear_pol2_wKC wasn't loaded! Aborting...\n", exit(1);
                }
                if (Loaded_Reco_FitParam_Smear_pol2_wKC.at(0).size() == 0) {
                    std::cout << "\n\nMomentumResolution::PSmear: Loaded_Reco_FitParam_Smear_pol2_wKC smearing wasn't loaded! Aborting...\n", exit(1);
                }
#pragma endregion

                Arg = Loaded_Reco_FitParam_Smear_pol2_wKC.at(0).at(0) * Momentum2 + Loaded_Reco_FitParam_Smear_pol2_wKC.at(0).at(1) * Momentum +
                      Loaded_Reco_FitParam_Smear_pol2_wKC.at(0).at(2);

                if ((Loaded_Smear_coefficients_values.size() == 0) || (Loaded_Smear_coefficients_names.size() == 0)) {
                    Loaded_Smear_coefficients_values.push_back(Loaded_Reco_FitParam_Smear_pol2_wKC.at(0).at(0));
                    Loaded_Smear_coefficients_names.push_back(VarNames.at(0));
                    Loaded_Smear_coefficients_values.push_back(Loaded_Reco_FitParam_Smear_pol2_wKC.at(0).at(1));
                    Loaded_Smear_coefficients_names.push_back(VarNames.at(1));
                    Loaded_Smear_coefficients_values.push_back(Loaded_Reco_FitParam_Smear_pol2_wKC.at(0).at(2));
                    Loaded_Smear_coefficients_names.push_back(VarNames.at(2));
                }

                if (PrintOut_Smear_Variables) {
                    std::cout << "\n\n\n#(FitParam groups) = " << Loaded_Reco_FitParam_Smear_pol2_wKC.size() << "\n";
                    std::cout << "#(FitParam variables) = " << Loaded_Reco_FitParam_Smear_pol2_wKC.at(0).size() << "\n";
                    std::cout << "#(FitParam errors) = " << Loaded_Reco_FitParam_Smear_pol2_wKC.at(1).size() << "\n";
                    std::cout << "#(FitParam fit goodness variables) = " << Loaded_Reco_FitParam_Smear_pol2_wKC.at(2).size() << "\n\n";

                    std::cout << "Loaded_A_Std_pol2_wKC = " << Loaded_Reco_FitParam_Smear_pol2_wKC.at(0).at(0) << "\n";
                    std::cout << "Loaded_B_Std_pol2_wKC = " << Loaded_Reco_FitParam_Smear_pol2_wKC.at(0).at(1) << "\n";
                    std::cout << "Loaded_C_Std_pol2_wKC = " << Loaded_Reco_FitParam_Smear_pol2_wKC.at(0).at(2) << "\n\n";
                }
            } else if (SmearMode == "pol3") {
#pragma region /* Safety checks */
                if (Loaded_Reco_FitParam_Smear_pol3.size() == 0) { std::cout << "\n\nMomentumResolution::PSmear: Loaded_Reco_FitParam_Smear_pol3 wasn't loaded! Aborting...\n", exit(1); }
                if (Loaded_Reco_FitParam_Smear_pol3.at(0).size() == 0) {
                    std::cout << "\n\nMomentumResolution::PSmear: Loaded_Reco_FitParam_Smear_pol3 smearing wasn't loaded! Aborting...\n", exit(1);
                }
#pragma endregion

                Arg = Loaded_Reco_FitParam_Smear_pol3.at(0).at(0) * Momentum3 + Loaded_Reco_FitParam_Smear_pol3.at(0).at(1) * Momentum2 +
                      Loaded_Reco_FitParam_Smear_pol3.at(0).at(2) * Momentum + Loaded_Reco_FitParam_Smear_pol3.at(0).at(3);

                if ((Loaded_Smear_coefficients_values.size() == 0) || (Loaded_Smear_coefficients_names.size() == 0)) {
                    Loaded_Smear_coefficients_values.push_back(Loaded_Reco_FitParam_Smear_pol3.at(0).at(0));
                    Loaded_Smear_coefficients_names.push_back(VarNames.at(0));
                    Loaded_Smear_coefficients_values.push_back(Loaded_Reco_FitParam_Smear_pol3.at(0).at(1));
                    Loaded_Smear_coefficients_names.push_back(VarNames.at(1));
                    Loaded_Smear_coefficients_values.push_back(Loaded_Reco_FitParam_Smear_pol3.at(0).at(2));
                    Loaded_Smear_coefficients_names.push_back(VarNames.at(2));
                    Loaded_Smear_coefficients_values.push_back(Loaded_Reco_FitParam_Smear_pol3.at(0).at(3));
                    Loaded_Smear_coefficients_names.push_back(VarNames.at(3));
                }

                if (PrintOut_Smear_Variables) {
                    std::cout << "\n\n\n#(FitParam groups) = " << Loaded_Reco_FitParam_Smear_pol3.size() << "\n";
                    std::cout << "#(FitParam variables) = " << Loaded_Reco_FitParam_Smear_pol3.at(0).size() << "\n";
                    std::cout << "#(FitParam errors) = " << Loaded_Reco_FitParam_Smear_pol3.at(1).size() << "\n";
                    std::cout << "#(FitParam fit goodness variables) = " << Loaded_Reco_FitParam_Smear_pol3.at(2).size() << "\n\n";

                    std::cout << "Loaded_A_Std_pol3 = " << Loaded_Reco_FitParam_Smear_pol3.at(0).at(0) << "\n";
                    std::cout << "Loaded_B_Std_pol3 = " << Loaded_Reco_FitParam_Smear_pol3.at(0).at(1) << "\n";
                    std::cout << "Loaded_C_Std_pol3 = " << Loaded_Reco_FitParam_Smear_pol3.at(0).at(2) << "\n";
                    std::cout << "Loaded_D_Std_pol3 = " << Loaded_Reco_FitParam_Smear_pol3.at(0).at(3) << "\n\n";
                }
            } else if (SmearMode == "pol3_wKC") {
#pragma region /* Safety checks */
                if (Loaded_Reco_FitParam_Smear_pol3_wKC.size() == 0) {
                    std::cout << "\n\nMomentumResolution::PSmear: Loaded_Reco_FitParam_Smear_pol3_wKC wasn't loaded! Aborting...\n", exit(1);
                }
                if (Loaded_Reco_FitParam_Smear_pol3_wKC.at(0).size() == 0) {
                    std::cout << "\n\nMomentumResolution::PSmear: Loaded_Reco_FitParam_Smear_pol3_wKC smearing wasn't loaded! Aborting...\n", exit(1);
                }
#pragma endregion

                Arg = Loaded_Reco_FitParam_Smear_pol3_wKC.at(0).at(0) * Momentum3 + Loaded_Reco_FitParam_Smear_pol3_wKC.at(0).at(1) * Momentum2 +
                      Loaded_Reco_FitParam_Smear_pol3_wKC.at(0).at(2) * Momentum + Loaded_Reco_FitParam_Smear_pol3_wKC.at(0).at(3);

                if ((Loaded_Smear_coefficients_values.size() == 0) || (Loaded_Smear_coefficients_names.size() == 0)) {
                    Loaded_Smear_coefficients_values.push_back(Loaded_Reco_FitParam_Smear_pol3_wKC.at(0).at(0));
                    Loaded_Smear_coefficients_names.push_back(VarNames.at(0));
                    Loaded_Smear_coefficients_values.push_back(Loaded_Reco_FitParam_Smear_pol3_wKC.at(0).at(1));
                    Loaded_Smear_coefficients_names.push_back(VarNames.at(1));
                    Loaded_Smear_coefficients_values.push_back(Loaded_Reco_FitParam_Smear_pol3_wKC.at(0).at(2));
                    Loaded_Smear_coefficients_names.push_back(VarNames.at(2));
                    Loaded_Smear_coefficients_values.push_back(Loaded_Reco_FitParam_Smear_pol3_wKC.at(0).at(3));
                    Loaded_Smear_coefficients_names.push_back(VarNames.at(3));
                }

                if (PrintOut_Smear_Variables) {
                    std::cout << "\n\n\n#(FitParam groups) = " << Loaded_Reco_FitParam_Smear_pol3_wKC.size() << "\n";
                    std::cout << "#(FitParam variables) = " << Loaded_Reco_FitParam_Smear_pol3_wKC.at(0).size() << "\n";
                    std::cout << "#(FitParam errors) = " << Loaded_Reco_FitParam_Smear_pol3_wKC.at(1).size() << "\n";
                    std::cout << "#(FitParam fit goodness variables) = " << Loaded_Reco_FitParam_Smear_pol3_wKC.at(2).size() << "\n\n";

                    std::cout << "Loaded_A_Std_pol3_wKC = " << Loaded_Reco_FitParam_Smear_pol3_wKC.at(0).at(0) << "\n";
                    std::cout << "Loaded_B_Std_pol3_wKC = " << Loaded_Reco_FitParam_Smear_pol3_wKC.at(0).at(1) << "\n";
                    std::cout << "Loaded_C_Std_pol3_wKC = " << Loaded_Reco_FitParam_Smear_pol3_wKC.at(0).at(2) << "\n";
                    std::cout << "Loaded_D_Std_pol3_wKC = " << Loaded_Reco_FitParam_Smear_pol3_wKC.at(0).at(3) << "\n\n";
                }
            } else {
                std::cout << "\n\nMomentumResolution::PSmear: SmearMode is not a polynomial! Aborting...\n", exit(1);
            }

            if (RandomNumGenerator == "TRandom1") {
                Smearing = Rand1->Gaus(1, Arg);
            } else if (RandomNumGenerator == "TRandom2") {
                Smearing = Rand2->Gaus(1, Arg);
            } else if (RandomNumGenerator == "TRandom3") {
                Smearing = Rand3->Gaus(1, Arg);  // TODO: recheck if this smearing can be negative!
            } else {
                std::cout << "\n\nMomentumResolution::PSmear: RandomNumGenerator illegal! Aborting...\n", exit(1);
            }

            SmearedMomentum = Smearing * Momentum;

            if (PrintOut) {
                std::cout << "\n\nSmearMode = " << SmearMode << "\n";
                std::cout << "RandomNumGenerator = " << RandomNumGenerator << "\n";
                std::cout << "Arg = " << Arg << "\n";
                std::cout << "Smearing = " << Smearing << "\n";
                std::cout << "Momentum = " << Momentum << "\n";
                std::cout << "SmearedMomentum = " << SmearedMomentum << "\n";
                std::cout << "==========================================================================\n\n";
            }

            if (PrintOut_And_Exit) { exit(1); }
        } else {
            std::cout << "\n\nMomentumResolution::PSmear: SmearMode illegal! Aborting...\n", exit(1);
        }

        return SmearedMomentum;
    }
}
#pragma endregion

// NCorr function -------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* NCorr function */
/* A function to correction (calibrate) neutron momentum by fitted neutron correction */

double MomentumResolution::NCorr(const bool &apply_nucleon_SmearAndCorr, const double &Momentum) {
    bool PrintOut = false;
    bool PrintOut_Corr_Variables = false;

    if (!apply_nucleon_SmearAndCorr) {
        /* Smearing and correction are disabled */
        return Momentum;
    } else {
        /* Smearing and correction are enabled */

        /* Setting corrected momentum & momentum powers */
        double CorrectedMomentum, Momentum2 = Momentum * Momentum, Momentum3 = Momentum2 * Momentum;

        if (CorrMode == "slices") {
            /* Correction using slice fit results */

#pragma region /* Original (correction from loaded nRes fit variables) */
            for (DSCuts Loaded_res_slice : Loaded_Res_Slices_HistVar) {
                if ((Loaded_res_slice.GetSliceLowerb() < Momentum) && (Loaded_res_slice.GetSliceUpperb() >= Momentum)) {
                    CorrectedMomentum = Momentum * (1 + Loaded_res_slice.GetMean());  // minus for protons and plus for neutrons

                    if (PrintOut) {
                        std::cout << "\n\nLoaded_res_slice.GetMean() = " << Loaded_res_slice.GetMean() << "\n";
                        std::cout << "Momentum = " << Momentum << "\n";
                        std::cout << "Correction = " << Loaded_res_slice.GetMean() << "\n";
                        std::cout << "CorrectedMomentum = " << CorrectedMomentum << "\n\n";
                    }
                }
            }
#pragma endregion
        } else if ((CorrMode == "pol1") || (CorrMode == "pol2") || (CorrMode == "pol3") || (CorrMode == "pol1_wKC") || (CorrMode == "pol2_wKC") || (CorrMode == "pol3_wKC")) {
            /* Correction using pol fit results */
            double Mu, CorrectionFactor;
            vector<string> VarNames = {"A", "B", "C", "D"};

            // TODO: add choice mechanism between truth and reco correction factors

            if (CorrMode == "pol1") {
#pragma region /* Safety checks */
                if (Loaded_Reco_FitParam_Corr_pol1.size() == 0) { std::cout << "\n\nMomentumResolution::NCorr: Loaded_Reco_FitParam_Corr_pol1 wasn't loaded! Aborting...\n", exit(1); }
                if (Loaded_Reco_FitParam_Corr_pol1.at(0).size() == 0) {
                    std::cout << "\n\nMomentumResolution::NCorr: Loaded_Reco_FitParam_Corr_pol1 correction wasn't loaded! Aborting...\n", exit(1);
                }
#pragma endregion

                Mu = Loaded_Reco_FitParam_Corr_pol1.at(0).at(0) * Momentum + Loaded_Reco_FitParam_Corr_pol1.at(0).at(1);

                if ((Loaded_Corr_coefficients_values.size() == 0) || (Loaded_Corr_coefficients_names.size() == 0)) {
                    Loaded_Corr_coefficients_values.push_back(Loaded_Reco_FitParam_Corr_pol1.at(0).at(0));
                    Loaded_Corr_coefficients_names.push_back(VarNames.at(0));
                    Loaded_Corr_coefficients_values.push_back(Loaded_Reco_FitParam_Corr_pol1.at(0).at(1));
                    Loaded_Corr_coefficients_names.push_back(VarNames.at(1));
                }

                if (PrintOut_Corr_Variables) {
                    std::cout << "\n\n\n#(FitParam groups) = " << Loaded_Reco_FitParam_Corr_pol1.size() << "\n";
                    std::cout << "#(FitParam variables) = " << Loaded_Reco_FitParam_Corr_pol1.at(0).size() << "\n";
                    std::cout << "#(FitParam errors) = " << Loaded_Reco_FitParam_Corr_pol1.at(1).size() << "\n";
                    std::cout << "#(FitParam fit goodness variables) = " << Loaded_Reco_FitParam_Corr_pol1.at(2).size() << "\n\n";

                    std::cout << "Loaded_A_Corr_pol1 = " << Loaded_Reco_FitParam_Corr_pol1.at(0).at(0) << "\n";
                    std::cout << "Loaded_B_Corr_pol1 = " << Loaded_Reco_FitParam_Corr_pol1.at(0).at(1) << "\n\n";
                }
            } else if (CorrMode == "pol1_wKC") {
#pragma region /* Safety checks */
                if (Loaded_Reco_FitParam_Corr_pol1_wKC.size() == 0) { std::cout << "\n\nMomentumResolution::NCorr: Loaded_Reco_FitParam_Corr_pol1_wKC wasn't loaded! Aborting...\n", exit(1); }
                if (Loaded_Reco_FitParam_Corr_pol1_wKC.at(0).size() == 0) {
                    std::cout << "\n\nMomentumResolution::NCorr: Loaded_Reco_FitParam_Corr_pol1_wKC correction wasn't loaded! Aborting...\n", exit(1);
                }
#pragma endregion

                Mu = Loaded_Reco_FitParam_Corr_pol1_wKC.at(0).at(0) * Momentum + Loaded_Reco_FitParam_Corr_pol1_wKC.at(0).at(1);

                if ((Loaded_Corr_coefficients_values.size() == 0) || (Loaded_Corr_coefficients_names.size() == 0)) {
                    Loaded_Corr_coefficients_values.push_back(Loaded_Reco_FitParam_Corr_pol1_wKC.at(0).at(0));
                    Loaded_Corr_coefficients_names.push_back(VarNames.at(0));
                    Loaded_Corr_coefficients_values.push_back(Loaded_Reco_FitParam_Corr_pol1_wKC.at(0).at(1));
                    Loaded_Corr_coefficients_names.push_back(VarNames.at(1));
                }

                if (PrintOut_Corr_Variables) {
                    std::cout << "\n\n\n#(FitParam groups) = " << Loaded_Reco_FitParam_Corr_pol1_wKC.size() << "\n";
                    std::cout << "#(FitParam variables) = " << Loaded_Reco_FitParam_Corr_pol1_wKC.at(0).size() << "\n";
                    std::cout << "#(FitParam errors) = " << Loaded_Reco_FitParam_Corr_pol1_wKC.at(1).size() << "\n";
                    std::cout << "#(FitParam fit goodness variables) = " << Loaded_Reco_FitParam_Corr_pol1_wKC.at(2).size() << "\n\n";

                    std::cout << "Loaded_A_Corr_pol1_wKC = " << Loaded_Reco_FitParam_Corr_pol1_wKC.at(0).at(0) << "\n";
                    std::cout << "Loaded_B_Corr_pol1_wKC = " << Loaded_Reco_FitParam_Corr_pol1_wKC.at(0).at(1) << "\n\n";
                }
            } else if (CorrMode == "pol2") {
#pragma region /* Safety checks */
                if (Loaded_Reco_FitParam_Corr_pol2.size() == 0) { std::cout << "\n\nMomentumResolution::NCorr: Loaded_Reco_FitParam_Corr_pol2 wasn't loaded! Aborting...\n", exit(1); }
                if (Loaded_Reco_FitParam_Corr_pol2.at(0).size() == 0) {
                    std::cout << "\n\nMomentumResolution::NCorr: Loaded_Reco_FitParam_Corr_pol2 correction wasn't loaded! Aborting...\n", exit(1);
                }
#pragma endregion

                Mu = Loaded_Reco_FitParam_Corr_pol2.at(0).at(0) * Momentum2 + Loaded_Reco_FitParam_Corr_pol2.at(0).at(1) * Momentum + Loaded_Reco_FitParam_Corr_pol2.at(0).at(2);

                if ((Loaded_Corr_coefficients_values.size() == 0) || (Loaded_Corr_coefficients_names.size() == 0)) {
                    Loaded_Corr_coefficients_values.push_back(Loaded_Reco_FitParam_Corr_pol2.at(0).at(0));
                    Loaded_Corr_coefficients_names.push_back(VarNames.at(0));
                    Loaded_Corr_coefficients_values.push_back(Loaded_Reco_FitParam_Corr_pol2.at(0).at(1));
                    Loaded_Corr_coefficients_names.push_back(VarNames.at(1));
                    Loaded_Corr_coefficients_values.push_back(Loaded_Reco_FitParam_Corr_pol2.at(0).at(2));
                    Loaded_Corr_coefficients_names.push_back(VarNames.at(2));
                }

                if (PrintOut_Corr_Variables) {
                    std::cout << "\n\n\n#(FitParam groups) = " << Loaded_Reco_FitParam_Corr_pol2.size() << "\n";
                    std::cout << "#(FitParam variables) = " << Loaded_Reco_FitParam_Corr_pol2.at(0).size() << "\n";
                    std::cout << "#(FitParam errors) = " << Loaded_Reco_FitParam_Corr_pol2.at(1).size() << "\n";
                    std::cout << "#(FitParam fit goodness variables) = " << Loaded_Reco_FitParam_Corr_pol2.at(2).size() << "\n\n";

                    std::cout << "Loaded_A_Corr_pol2 = " << Loaded_Reco_FitParam_Corr_pol2.at(0).at(0) << "\n";
                    std::cout << "Loaded_B_Corr_pol2 = " << Loaded_Reco_FitParam_Corr_pol2.at(0).at(1) << "\n";
                    std::cout << "Loaded_C_Corr_pol2 = " << Loaded_Reco_FitParam_Corr_pol2.at(0).at(2) << "\n\n";
                }
            } else if (CorrMode == "pol2_wKC") {
#pragma region /* Safety checks */
                if (Loaded_Reco_FitParam_Corr_pol2_wKC.size() == 0) { std::cout << "\n\nMomentumResolution::NCorr: Loaded_Reco_FitParam_Corr_pol2_wKC wasn't loaded! Aborting...\n", exit(1); }
                if (Loaded_Reco_FitParam_Corr_pol2_wKC.at(0).size() == 0) {
                    std::cout << "\n\nMomentumResolution::NCorr: Loaded_Reco_FitParam_Corr_pol2_wKC correction wasn't loaded! Aborting...\n", exit(1);
                }
#pragma endregion

                Mu = Loaded_Reco_FitParam_Corr_pol2_wKC.at(0).at(0) * Momentum2 + Loaded_Reco_FitParam_Corr_pol2_wKC.at(0).at(1) * Momentum + Loaded_Reco_FitParam_Corr_pol2_wKC.at(0).at(2);

                if ((Loaded_Corr_coefficients_values.size() == 0) || (Loaded_Corr_coefficients_names.size() == 0)) {
                    Loaded_Corr_coefficients_values.push_back(Loaded_Reco_FitParam_Corr_pol2_wKC.at(0).at(0));
                    Loaded_Corr_coefficients_names.push_back(VarNames.at(0));
                    Loaded_Corr_coefficients_values.push_back(Loaded_Reco_FitParam_Corr_pol2_wKC.at(0).at(1));
                    Loaded_Corr_coefficients_names.push_back(VarNames.at(1));
                    Loaded_Corr_coefficients_values.push_back(Loaded_Reco_FitParam_Corr_pol2_wKC.at(0).at(2));
                    Loaded_Corr_coefficients_names.push_back(VarNames.at(2));
                }

                if (PrintOut_Corr_Variables) {
                    std::cout << "\n\n\n#(FitParam groups) = " << Loaded_Reco_FitParam_Corr_pol2_wKC.size() << "\n";
                    std::cout << "#(FitParam variables) = " << Loaded_Reco_FitParam_Corr_pol2_wKC.at(0).size() << "\n";
                    std::cout << "#(FitParam errors) = " << Loaded_Reco_FitParam_Corr_pol2_wKC.at(1).size() << "\n";
                    std::cout << "#(FitParam fit goodness variables) = " << Loaded_Reco_FitParam_Corr_pol2_wKC.at(2).size() << "\n\n";

                    std::cout << "Loaded_A_Corr_pol2_wKC = " << Loaded_Reco_FitParam_Corr_pol2_wKC.at(0).at(0) << "\n";
                    std::cout << "Loaded_B_Corr_pol2_wKC = " << Loaded_Reco_FitParam_Corr_pol2_wKC.at(0).at(1) << "\n";
                    std::cout << "Loaded_C_Corr_pol2_wKC = " << Loaded_Reco_FitParam_Corr_pol2_wKC.at(0).at(2) << "\n\n";
                }
            } else if (CorrMode == "pol3") {
#pragma region /* Safety checks */
                if (Loaded_Reco_FitParam_Corr_pol3.size() == 0) { std::cout << "\n\nMomentumResolution::NCorr: Loaded_Reco_FitParam_Corr_pol3 wasn't loaded! Aborting...\n", exit(1); }
                if (Loaded_Reco_FitParam_Corr_pol3.at(0).size() == 0) {
                    std::cout << "\n\nMomentumResolution::NCorr: Loaded_Reco_FitParam_Corr_pol3 correction wasn't loaded! Aborting...\n", exit(1);
                }
#pragma endregion

                Mu = Loaded_Reco_FitParam_Corr_pol3.at(0).at(0) * Momentum3 + Loaded_Reco_FitParam_Corr_pol3.at(0).at(1) * Momentum2 + Loaded_Reco_FitParam_Corr_pol3.at(0).at(2) * Momentum +
                     Loaded_Reco_FitParam_Corr_pol3.at(0).at(3);

                if ((Loaded_Corr_coefficients_values.size() == 0) || (Loaded_Corr_coefficients_names.size() == 0)) {
                    Loaded_Corr_coefficients_values.push_back(Loaded_Reco_FitParam_Corr_pol3.at(0).at(0));
                    Loaded_Corr_coefficients_names.push_back(VarNames.at(0));
                    Loaded_Corr_coefficients_values.push_back(Loaded_Reco_FitParam_Corr_pol3.at(0).at(1));
                    Loaded_Corr_coefficients_names.push_back(VarNames.at(1));
                    Loaded_Corr_coefficients_values.push_back(Loaded_Reco_FitParam_Corr_pol3.at(0).at(2));
                    Loaded_Corr_coefficients_names.push_back(VarNames.at(2));
                    Loaded_Corr_coefficients_values.push_back(Loaded_Reco_FitParam_Corr_pol3.at(0).at(3));
                    Loaded_Corr_coefficients_names.push_back(VarNames.at(3));
                }

                if (PrintOut_Corr_Variables) {
                    std::cout << "\n\n\n#(FitParam groups) = " << Loaded_Reco_FitParam_Corr_pol3.size() << "\n";
                    std::cout << "#(FitParam variables) = " << Loaded_Reco_FitParam_Corr_pol3.at(0).size() << "\n";
                    std::cout << "#(FitParam errors) = " << Loaded_Reco_FitParam_Corr_pol3.at(1).size() << "\n";
                    std::cout << "#(FitParam fit goodness variables) = " << Loaded_Reco_FitParam_Corr_pol3.at(2).size() << "\n\n";

                    std::cout << "Loaded_A_Corr_pol3 = " << Loaded_Reco_FitParam_Corr_pol3.at(0).at(0) << "\n";
                    std::cout << "Loaded_B_Corr_pol3 = " << Loaded_Reco_FitParam_Corr_pol3.at(0).at(1) << "\n";
                    std::cout << "Loaded_C_Corr_pol3 = " << Loaded_Reco_FitParam_Corr_pol3.at(0).at(2) << "\n";
                    std::cout << "Loaded_D_Corr_pol3 = " << Loaded_Reco_FitParam_Corr_pol3.at(0).at(3) << "\n\n";
                }
            } else if (CorrMode == "pol3_wKC") {
#pragma region /* Safety checks */
                if (Loaded_Reco_FitParam_Corr_pol3_wKC.size() == 0) { std::cout << "\n\nMomentumResolution::NCorr: Loaded_Reco_FitParam_Corr_pol3_wKC wasn't loaded! Aborting...\n", exit(1); }
                if (Loaded_Reco_FitParam_Corr_pol3_wKC.at(0).size() == 0) {
                    std::cout << "\n\nMomentumResolution::NCorr: Loaded_Reco_FitParam_Corr_pol3_wKC correction wasn't loaded! Aborting...\n", exit(1);
                }
#pragma endregion

                Mu = Loaded_Reco_FitParam_Corr_pol3_wKC.at(0).at(0) * Momentum3 + Loaded_Reco_FitParam_Corr_pol3_wKC.at(0).at(1) * Momentum2 +
                     Loaded_Reco_FitParam_Corr_pol3_wKC.at(0).at(2) * Momentum + Loaded_Reco_FitParam_Corr_pol3_wKC.at(0).at(3);

                if ((Loaded_Corr_coefficients_values.size() == 0) || (Loaded_Corr_coefficients_names.size() == 0)) {
                    Loaded_Corr_coefficients_values.push_back(Loaded_Reco_FitParam_Corr_pol3_wKC.at(0).at(0));
                    Loaded_Corr_coefficients_names.push_back(VarNames.at(0));
                    Loaded_Corr_coefficients_values.push_back(Loaded_Reco_FitParam_Corr_pol3_wKC.at(0).at(1));
                    Loaded_Corr_coefficients_names.push_back(VarNames.at(1));
                    Loaded_Corr_coefficients_values.push_back(Loaded_Reco_FitParam_Corr_pol3_wKC.at(0).at(2));
                    Loaded_Corr_coefficients_names.push_back(VarNames.at(2));
                    Loaded_Corr_coefficients_values.push_back(Loaded_Reco_FitParam_Corr_pol3_wKC.at(0).at(3));
                    Loaded_Corr_coefficients_names.push_back(VarNames.at(3));
                }

                if (PrintOut_Corr_Variables) {
                    std::cout << "\n\n\n#(FitParam groups) = " << Loaded_Reco_FitParam_Corr_pol3_wKC.size() << "\n";
                    std::cout << "#(FitParam variables) = " << Loaded_Reco_FitParam_Corr_pol3_wKC.at(0).size() << "\n";
                    std::cout << "#(FitParam errors) = " << Loaded_Reco_FitParam_Corr_pol3_wKC.at(1).size() << "\n";
                    std::cout << "#(FitParam fit goodness variables) = " << Loaded_Reco_FitParam_Corr_pol3_wKC.at(2).size() << "\n\n";

                    std::cout << "Loaded_A_Corr_pol3_wKC = " << Loaded_Reco_FitParam_Corr_pol3_wKC.at(0).at(0) << "\n";
                    std::cout << "Loaded_B_Corr_pol3_wKC = " << Loaded_Reco_FitParam_Corr_pol3_wKC.at(0).at(1) << "\n";
                    std::cout << "Loaded_C_Corr_pol3_wKC = " << Loaded_Reco_FitParam_Corr_pol3_wKC.at(0).at(2) << "\n";
                    std::cout << "Loaded_D_Corr_pol3_wKC = " << Loaded_Reco_FitParam_Corr_pol3_wKC.at(0).at(3) << "\n\n";
                }
            }

            CorrectionFactor = 1 / (1 - Mu);
            CorrectedMomentum = Momentum * CorrectionFactor;

            if (PrintOut) {
                std::cout << "\n\nCorrMode = " << CorrMode << "\n";
                std::cout << "Momentum = " << Momentum << "\n";
                std::cout << "Mu = " << Mu << "\n";
                std::cout << "CorrectionFactor = " << CorrectionFactor << "\n";
                std::cout << "CorrectedMomentum = " << CorrectedMomentum << "\n";
                std::cout << "==========================================================================\n\n";
            }
        } else {
            std::cout << "\n\nMomentumResolution::NCorr: CorrMode illegal! Aborting...\n", exit(1);
        }

        return CorrectedMomentum;
    }
}
#pragma endregion

#endif  // MOMENTUMRESOLUTION_H
