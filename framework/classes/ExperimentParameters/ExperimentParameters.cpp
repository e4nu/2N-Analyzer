#ifndef EXPERIMENTPARAMETERS_H
#define EXPERIMENTPARAMETERS_H

#include "ExperimentParameters.h"

// ======================================================================================================================================================================
// ExperimentParameters
//
// This class derives from TargetParameters and centralizes all run-time configuration derived from a sample path:
//   - Target type (H1/D2/C12/Cx4/Ar40/Uniform)
//   - Sample type (GENIE simulation / data / uniform)
//   - GENIE tune (G18 vs SuSa)
//   - Beam energy bucket (2, 4, 6 GeV) inferred from the path
//   - Q2 cut category (for GENIE truth generation)
//   - A canonical `SampleName` string used throughout the analysis
//   - Vz and dVz cut objects chosen by sample name
//
// The constructor runs a sequence of \"Config*\" methods that parse `RecoSamplePath` and set member variables.
// IMPORTANT: These Config* methods are *mutators* (void functions). They must be called from the constructor body
// (not from the initializer list) because initializer lists can only initialize data members and base classes.
// ======================================================================================================================================================================

// ======================================================================================================================================================================
// Constructor
// ======================================================================================================================================================================

#pragma region /* Constructor */
ExperimentParameters::ExperimentParameters(const std::string& RecoSamplePath, const std::string& ReconHipoDir, const std::string& HipoFilesPrefix)
    : RecoSamplePath(RecoSamplePath),    // Path to top directory containing reconstructed samples (e.g. "/path/to/samples")
      ReconHipoDir(ReconHipoDir),        // Subdirectory within RecoSamplePath containing the .hipo files (either "reconhipo" for simulations or run number for data)
      HipoFilesPrefix(HipoFilesPrefix),  // Prefix for .hipo files (e.g. "*hipo"), free paramater for custom selection of hipo files within the ReconHipoDir directory
      InputFilesGlobal("/" + RecoSamplePath + "/" + ReconHipoDir + "/" + HipoFilesPrefix)  // Full glob pattern for input files, e.g. "/path/to/samples/*hipo"
{
    // InputFilesGlobal = "/" + RecoSamplePath + "/" + ReconHipoDir + "/" + this->HipoFilesPrefix;
    bool PrintOut = false;

    if (this->RecoSamplePath.empty()) {
        ExitWithError("ExperimentParameters::ExperimentParameters", __FILE__, __LINE__,
                      "RecoSamplePath cannot be empty. Please provide a valid path to the directory containing the samples.");
    }

    if (this->ReconHipoDir.empty()) {
        ExitWithError("ExperimentParameters::ExperimentParameters", __FILE__, __LINE__,
                      "ReconHipoDir cannot be empty. Please provide a valid subdirectory name containing the .hipo files (e.g. 'reconhipo' for simulations or run number for data).");
    }

    if (PrintOut) {
        std::cout << env::SYSTEM_COLOR << "ExperimentParameters constructor called with:\n" << env::RESET_COLOR << std::flush;
        std::cout << env::SYSTEM_COLOR << "\tRecoSamplePath: " << env::RESET_COLOR << this->RecoSamplePath << "\n" << std::endl;
        std::cout << env::SYSTEM_COLOR << "\tReconHipoDir: " << env::RESET_COLOR << this->ReconHipoDir << "\n" << std::endl;
        std::cout << env::SYSTEM_COLOR << "\tHipoFilesPrefix: " << env::RESET_COLOR << this->HipoFilesPrefix << "\n" << std::endl;
        std::cout << env::SYSTEM_COLOR << "\tInputFilesGlobal: " << env::RESET_COLOR << this->InputFilesGlobal << "\n" << std::endl;
    }

    // 1) Determine the target element from the sample path (also sets PDG and nuclear numbers in TargetParameters).
    this->ConfigSampleTarget();
    if (PrintOut) {
        std::cout << env::SYSTEM_COLOR << "1) Determine the target element from the sample path (also sets PDG and nuclear numbers in TargetParameters):\n" << env::RESET_COLOR << std::flush;
        std::cout << env::SYSTEM_COLOR << "\tInferred SampleTargetStr: " << env::RESET_COLOR << this->SampleTargetStr << "\n" << std::endl;
        std::cout << env::SYSTEM_COLOR << "\tInferred TargetElementPDG: " << env::RESET_COLOR << this->TargetElementPDG << "\n" << std::endl;
        std::cout << env::SYSTEM_COLOR << "\tInferred TotalBaryonNumber_A: " << env::RESET_COLOR << this->TotalBaryonNumber_A << "\n" << std::endl;
        std::cout << env::SYSTEM_COLOR << "\tInferred TotalChargeNumber_Z: " << env::RESET_COLOR << this->TotalChargeNumber_Z << "\n" << std::endl;
    }

    // 2) Determine whether this is GENIE simulation, farm data, or a uniform sample.
    this->ConfigSampleType();
    if (PrintOut) {
        std::cout << env::SYSTEM_COLOR << "2) Determine whether this is GENIE simulation, farm data, or a uniform sample:\n" << env::RESET_COLOR << std::flush;
        std::cout << env::SYSTEM_COLOR << "\tInferred SampleType: " << env::RESET_COLOR << this->SampleTypeStr << "\n" << std::endl;
    }

    // 3) If this is GENIE simulation, infer the GENIE tune from the path (G18 vs SuSa).
    this->ConfigGENIETune();
    if (PrintOut && sampleType == GENIE_SIMULATION_TYPE) {
        std::cout << env::SYSTEM_COLOR << "3) If this is GENIE simulation, infer the GENIE tune from the path (G18 vs SuSa):\n" << env::RESET_COLOR << std::flush;
        std::cout << env::SYSTEM_COLOR << "\tInferred SampleGENIETuneStr: " << env::RESET_COLOR << this->SampleGENIETuneStr << "\n" << std::endl;
    }

    // 4) Infer the beam-energy bucket from the path (2/4/6 GeV). This also sets `SampleBeamEnergyStr`.
    this->ConfigBeamEnergy();
    if (PrintOut) {
        std::cout << env::SYSTEM_COLOR << "4) Infer the beam-energy bucket from the path (2/4/6 GeV):\n" << env::RESET_COLOR << std::flush;
        std::cout << env::SYSTEM_COLOR << "\tInferred SampleBeamEnergyStr: " << env::RESET_COLOR << this->SampleBeamEnergyStr << "\n" << std::endl;
        std::cout << env::SYSTEM_COLOR << "\tInferred BeamEnergyValue: " << env::RESET_COLOR << this->BeamEnergyValue << " GeV\n" << std::flush;
    }

    // 5) For GENIE simulation, infer the Q2 cut category based on beam energy.
    this->ConfigQ2Cut();
    if (PrintOut && sampleType == GENIE_SIMULATION_TYPE) {
        std::cout << env::SYSTEM_COLOR << "5) For GENIE simulation, infer the Q2 cut category based on beam energy:\n" << env::RESET_COLOR << std::flush;
        std::cout << env::SYSTEM_COLOR << "\tInferred SampleQ2CutStr: " << env::RESET_COLOR << this->SampleQ2CutStr << "\n" << std::endl;
    }

    // 6) Build a canonical analysis sample name string (depends on target/type/tune/Q2/energy, or run directory for data).
    this->ConfigSampleName();
    if (PrintOut) {
        std::cout << env::SYSTEM_COLOR << "6) Build a canonical analysis sample name string (depends on target/type/tune/Q2/energy, or run directory for data):\n"
                  << env::RESET_COLOR << std::flush;
        std::cout << env::SYSTEM_COLOR << "\tConstructed SampleName: " << env::RESET_COLOR << this->SampleName << "\n" << std::endl;
    }

    // 7) Derive a "varying" sample name used by acceptance maps / weight maps / neutron resolution inputs.
    this->ConfigureVaryingSampleName();
    if (PrintOut) {
        std::cout << env::SYSTEM_COLOR << "7) Derive a \"varying\" sample name used by acceptance maps / weight maps / neutron resolution inputs:\n" << env::RESET_COLOR << std::flush;
        std::cout << env::SYSTEM_COLOR << "\tConstructed VaryingSampleName: " << env::RESET_COLOR << this->VaryingSampleName << "\n" << std::endl;
    }

    // 8) Configure vertex-z cuts (Vz) and delta-vertex cuts (dVz) based on the canonical sample name.
    this->ConfigureVz_cuts();
    this->ConfiguredVz_cuts();
    if (PrintOut) {
        std::cout << env::SYSTEM_COLOR << "8) Configure vertex-z cuts (Vz) and delta-vertex cuts (dVz) based on the canonical sample name:\n" << env::RESET_COLOR << std::flush;
        std::cout << env::SYSTEM_COLOR << "\tConfigured Vz_cuts: " << env::RESET_COLOR << this->Vz_cuts.GetCutVariable() << env::SYSTEM_COLOR << " [" << env::RESET_COLOR
                  << this->Vz_cuts.GetLowerCut() << env::SYSTEM_COLOR << ", " << env::RESET_COLOR << this->Vz_cuts.GetUpperCut() << env::SYSTEM_COLOR << "]\n"
                  << env::RESET_COLOR << std::flush;
        std::cout << env::SYSTEM_COLOR << "\tConfigured dVz_cuts: " << env::RESET_COLOR << this->dVz_cuts_def.GetCutVariable() << env::SYSTEM_COLOR << " [" << env::RESET_COLOR
                  << this->dVz_cuts_def.GetLowerCut() << env::SYSTEM_COLOR << ", " << env::RESET_COLOR << this->dVz_cuts_def.GetUpperCut() << env::SYSTEM_COLOR << "]\n"
                  << env::RESET_COLOR << std::flush;
    }

    // 9) Check if sample is local or on the ifarm:
    // Currently based on path, but can be changed to a more robust way in the future.
    // If the path contains "volatile", it is simulation. If it contains "clas12/rg-m/production", data on the ifarm. Otherwise, it is local.
    LocalSample = (bt::FindSubstring(this->RecoSamplePath, "volatile") || bt::FindSubstring(this->RecoSamplePath, "clas12/rg-m/production")) ? false : true;
    if (PrintOut) {
        std::cout << env::SYSTEM_COLOR << "9) Check if sample is local or on the ifarm:\n" << env::RESET_COLOR << std::flush;
        std::cout << env::SYSTEM_COLOR << "\tInferred LocalSample: " << env::RESET_COLOR << (this->LocalSample ? "true" : "false") << "\n" << std::endl;
    }
}
#pragma endregion

// ======================================================================================================================================================================
// Configure functions
// ======================================================================================================================================================================

// ConfigSampleTarget function ------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ConfigSampleTarget function */
/**
 * Infer the target element from the sample path.
 *
 * This fills both:
 *   - ExperimentParameters members: SampleTargetStr
 *   - TargetParameters members: TargetElement, PDG code, A/Z/L/I
 *
 * Matching is substring-based so the path can include additional tokens (run-group tags, suffixes, etc.).
 */
void ExperimentParameters::ConfigSampleTarget() {
    const bool PrintOut = false;

    if (bt::FindSubstring(this->RecoSamplePath, "/H1/") || bt::FindSubstring(this->RecoSamplePath, "/H/") || bt::FindSubstring(this->RecoSamplePath, "/H1_")) {  // Hydrogen
        this->TargetElement = this->SampleTargetStr = "H1";
        this->TargetElementPDG = 1000010010;
        this->TotalBaryonNumber_A = 1;
        this->TotalChargeNumber_Z = 1;
        this->StrangeQuarksNumber_L = this->IsomerNumber_I = 0;
    } else if (bt::FindSubstring(this->RecoSamplePath, "/D2/") || bt::FindSubstring(this->RecoSamplePath, "/D/") || bt::FindSubstring(this->RecoSamplePath, "/D2_")) {  // Deuterium
        this->TargetElement = this->SampleTargetStr = "D2";
        this->TargetElementPDG = 1000010020;
        this->TotalBaryonNumber_A = 2;
        this->TotalChargeNumber_Z = 1;
        this->StrangeQuarksNumber_L = this->IsomerNumber_I = 0;
    } else if ((bt::FindSubstring(this->RecoSamplePath, "/C12/") || bt::FindSubstring(this->RecoSamplePath, "/C/") || bt::FindSubstring(this->RecoSamplePath, "/C12_"))  // Carbon (1-foil)
               || bt::FindSubstring(this->RecoSamplePath, "/Cx4/") || bt::FindSubstring(this->RecoSamplePath, "/Cx4_")                                                   // Carbon (4-foil)
    ) {
        this->TargetElement = this->SampleTargetStr = "C12";
        this->TargetElementPDG = 1000060120;
        this->TotalBaryonNumber_A = 12;
        this->TotalChargeNumber_Z = 6;
        this->StrangeQuarksNumber_L = this->IsomerNumber_I = 0;
    } else if (bt::FindSubstring(this->RecoSamplePath, "/Ar40/") || bt::FindSubstring(this->RecoSamplePath, "/Ar/") || bt::FindSubstring(this->RecoSamplePath, "/Ar40_")) {  // Argon
        this->TargetElement = this->SampleTargetStr = "Ar40";
        this->TargetElementPDG = 1000180400;
        this->TotalBaryonNumber_A = 40;
        this->TotalChargeNumber_Z = 18;
        this->StrangeQuarksNumber_L = this->IsomerNumber_I = 0;
    } else if (bt::FindSubstring(this->RecoSamplePath, "/Uniform")) {  // Uniform samples
        this->TargetElement = this->SampleTargetStr = "Uniform";
        this->TargetElementPDG = -9999;
        this->TotalBaryonNumber_A = this->TotalChargeNumber_Z = this->StrangeQuarksNumber_L = this->IsomerNumber_I = 0;
    } else {
        this->SampleTargetStr = "UNKOWN";
        this->TargetElementPDG = -9999;
        this->TotalBaryonNumber_A = this->TotalChargeNumber_Z = this->StrangeQuarksNumber_L = this->IsomerNumber_I = 0;
    }

    if (this->SampleTargetStr == "UNKOWN") { bt::ExitWithError("ExperimentParameters::ConfigSampleTarget", __FILE__, __LINE__, "Unable to configure sample target!"); }

    if (PrintOut) {
        std::cout << std::endl;
        std::cout << env::SYSTEM_COLOR << "Target element:          " << env::RESET_COLOR << this->TargetElement << std::endl;
        std::cout << env::SYSTEM_COLOR << "Sample target:           " << env::RESET_COLOR << this->SampleTargetStr << std::endl;
        std::cout << env::SYSTEM_COLOR << "Target element PDG:      " << env::RESET_COLOR << this->TargetElementPDG << std::endl;
        std::cout << env::SYSTEM_COLOR << "Total Baryon Number A:   " << env::RESET_COLOR << this->TotalBaryonNumber_A << std::endl;
        std::cout << env::SYSTEM_COLOR << "Total Charge Number Z:   " << env::RESET_COLOR << this->TotalChargeNumber_Z << std::endl;
        std::cout << env::SYSTEM_COLOR << "Strange Quarks Number L: " << env::RESET_COLOR << this->StrangeQuarksNumber_L << std::endl;
        std::cout << env::SYSTEM_COLOR << "Isomer Number I:         " << env::RESET_COLOR << this->IsomerNumber_I << std::endl;
        std::cout << std::endl;
    }
}
#pragma endregion

// ConfigSampleType function --------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ConfigSampleType function */
/**
 * Infer sample type from path heuristics:
 *   - GENIE simulation: path contains \"GENIE_Reco_Samples\"
 *   - farm data: path contains the canonical production directory \"clas12/rg-m/production\"
 *   - uniform: path contains \"Uniform\"
 *
 * If none match, we abort because subsequent configuration depends on knowing which naming rules to apply.
 */
void ExperimentParameters::ConfigSampleType() {
    const bool PrintOut = false;

    this->sampleType = (bt::FindSubstring(this->RecoSamplePath, "GENIE_Reco_Samples"))       ? GENIE_SIMULATION_TYPE
                       : (bt::FindSubstring(this->RecoSamplePath, "clas12/rg-m/production")) ? DATA_TYPE
                       : (bt::FindSubstring(this->RecoSamplePath, "Uniform"))                ? UNIFORM_TYPE
                                                                                             : UNKNOWN_TYPE;
    if (this->sampleType == UNKNOWN_TYPE) { bt::ExitWithError("ExperimentParameters::ConfigSampleType", __FILE__, __LINE__, "Unable to configure sample type!"); }

    this->SampleTypeStr = (this->sampleType == GENIE_SIMULATION_TYPE) ? "sim" : (this->sampleType == DATA_TYPE) ? "data" : "";

    if (PrintOut) {
        std::cout << std::endl;
        std::cout << env::SYSTEM_COLOR << "Sample type: " << env::RESET_COLOR << this->sampleType << std::endl;
        std::cout << env::SYSTEM_COLOR << "Sample type string: " << env::RESET_COLOR << this->SampleTypeStr << std::endl;
        std::cout << std::endl;
    }
}
#pragma endregion

// ConfigGENIETune function ---------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ConfigGENIETune function */
/**
 * Configure GENIE tune information.
 *
 * Only meaningful for GENIE simulation samples. For data and uniform samples we leave the tune unset/empty.
 * Mapping is heuristic:
 *   - if path contains \"G18\"      -> G18_TUNE
 *   - else if contains \"GEM21\"   -> SUSA_TUNE (SuSa)
 *   - else                          UNKNOWN_TUNE (will later trigger an error for GENIE samples)
 */
void ExperimentParameters::ConfigGENIETune() {
    if (this->sampleType == GENIE_SIMULATION_TYPE) {
        this->genieTune = (bt::FindSubstring(this->RecoSamplePath, "G18")) ? G18_TUNE : (bt::FindSubstring(this->RecoSamplePath, "GEM21")) ? SUSA_TUNE : UNKNOWN_TUNE;

        this->SampleGENIETuneStr = (this->genieTune == G18_TUNE) ? "G18" : (this->genieTune == SUSA_TUNE) ? "SuSa" : "";
    } else {
        this->genieTune = UNSET_TUNE;

        this->SampleGENIETuneStr = "";
    }
}
#pragma endregion

// ConfigQ2Cut function -------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ConfigQ2Cut function */
/**
 * Infer beam-energy bucket from path substrings.
 *
 * Heuristic approach:
 *   - 2070 or 2GeV tokens -> BEAM_AT_2GEV
 *   - 4029 or 4GeV tokens -> BEAM_AT_4GEV
 *   - 5986 or 6GeV tokens -> BEAM_AT_6GEV
 *
 * Note: substring heuristics can produce false matches if the digits appear in unrelated tokens.
 * In practice your directory naming convention is the contract that makes this safe.
 */
void ExperimentParameters::ConfigQ2Cut() {
    if (this->sampleType == GENIE_SIMULATION_TYPE) {
        this->Q2cut = (this->beamEnergy == BEAM_AT_2GEV) ? GENIE_DEF_CUT : (this->beamEnergy == BEAM_AT_4GEV) ? Q2_0_25_CUT : (this->beamEnergy == BEAM_AT_6GEV) ? Q2_0_4_CUT : UNKNOWN_CUT;

        this->SampleQ2CutStr = (this->Q2cut == GENIE_DEF_CUT) ? "Q2_0_02" : (this->Q2cut == Q2_0_25_CUT) ? "Q2_0_25" : (this->Q2cut == Q2_0_4_CUT) ? "Q2_0_40" : "";
    } else {
        this->Q2cut = UNSET_CUT;

        this->SampleQ2CutStr = "";
    }
}
#pragma endregion

// ConfigBeamEnergy function --------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ConfigBeamEnergy function */
/**
 * Configure Q2 cut category for GENIE simulation samples.
 *
 * In this analysis code the Q2 cut label is inferred from beam-energy bucket:
 *   - 2 GeV -> GENIE_DEF_CUT -> \"Q2_0_02\"
 *   - 4 GeV -> Q2_0_25_CUT   -> \"Q2_0_25\"
 *   - 6 GeV -> Q2_0_4_CUT    -> \"Q2_0_40\"
 *
 * For data/uniform, we leave it unset/empty.
 */
void ExperimentParameters::ConfigBeamEnergy() {
    this->beamEnergy = (bt::FindSubstring(this->RecoSamplePath, "2070") ||
                        (bt::FindSubstring(this->RecoSamplePath, "2GeV") || bt::FindSubstring(this->RecoSamplePath, "2Gev") || bt::FindSubstring(this->RecoSamplePath, "2gev")))
                           ? BEAM_AT_2GEV
                       : (bt::FindSubstring(this->RecoSamplePath, "4029") ||
                          (bt::FindSubstring(this->RecoSamplePath, "4GeV") || bt::FindSubstring(this->RecoSamplePath, "4Gev") || bt::FindSubstring(this->RecoSamplePath, "4gev")))
                           ? BEAM_AT_4GEV
                       : (bt::FindSubstring(this->RecoSamplePath, "5986") ||
                          (bt::FindSubstring(this->RecoSamplePath, "6GeV") || bt::FindSubstring(this->RecoSamplePath, "6Gev") || bt::FindSubstring(this->RecoSamplePath, "6gev")))
                           ? BEAM_AT_6GEV
                           : UNKNOWN_ENERGY;
    if (this->beamEnergy == UNKNOWN_ENERGY) { bt::ExitWithError("ExperimentParameters::ConfigBeamEnergy", __FILE__, __LINE__, "Unable to configure beam energy!"); }

    if (this->beamEnergy == BEAM_AT_2GEV) {
        this->SampleBeamEnergyStr = "2070MeV";
        this->BeamEnergyValue = 2.07052;
    } else if (this->beamEnergy == BEAM_AT_4GEV) {
        this->SampleBeamEnergyStr = "4029MeV";
        this->BeamEnergyValue = 4.02962;
    } else if (this->beamEnergy == BEAM_AT_6GEV) {
        this->SampleBeamEnergyStr = "5986MeV";
        this->BeamEnergyValue = 5.98636;
    }
}
#pragma endregion

// ConfigSampleName function --------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ConfigSampleName function */
/**
 * Build the canonical `SampleName` used across the analysis.
 *
 * Naming conventions:
 *   - GENIE simulation:
 *       <Target>_<sim>_<Tune>_<Q2Label>_<EnergyLabel>
 *       Example: C12_sim_G18_Q2_0_02_2070MeV
 *
 *   - Data:
 *       <Target>_<data>_<EnergyLabel>[_run_<ReconHipoDir>]
 *       Example: C12_data_2070MeV_run_015664
 *
 *   - Uniform:
 *       Either tester labels, or:
 *       <Target>_<ParticleType>_sample_<EnergyLabel>
 *       Example: Uniform_en_sample_2070MeV
 */
void ExperimentParameters::ConfigSampleName() {
    if (this->sampleType == UNSET_TYPE) { bt::ExitWithError("ExperimentParameters::ConfigSampleName", __FILE__, __LINE__, "Unable to configure sample name - type is unset!"); }

    if (this->sampleType == GENIE_SIMULATION_TYPE) {
        if (this->genieTune == UNKNOWN_TUNE) { bt::ExitWithError("ExperimentParameters::ConfigSampleName", __FILE__, __LINE__, "Unable to configure sample name - GENIE tune is unknown!"); }
        if (this->Q2cut == UNKNOWN_CUT) { bt::ExitWithError("ExperimentParameters::ConfigSampleName", __FILE__, __LINE__, "Unable to configure sample name - Q2 cut is unknown!"); }
    }

    if (this->beamEnergy == UNSET_ENERGY) { bt::ExitWithError("ExperimentParameters::ConfigSampleName", __FILE__, __LINE__, "Unable to configure sample name - beam energy is unset!"); }

    if (this->sampleType == GENIE_SIMULATION_TYPE) {
        this->SampleName = this->SampleTargetStr + "_" + this->SampleTypeStr + "_" + this->SampleGENIETuneStr + "_" + this->SampleQ2CutStr + "_" + this->SampleBeamEnergyStr;
    } else if (this->sampleType == DATA_TYPE) {
        this->SampleName = this->SampleTargetStr + "_" + this->SampleTypeStr + "_" + this->SampleBeamEnergyStr + ((this->ReconHipoDir != "") ? "_run_" + this->ReconHipoDir : "");
    } else if (this->sampleType == UNIFORM_TYPE) {
        if (bt::FindSubstring(this->RecoSamplePath, "OutPut_Tester_e_Tester_e")) {  // Electron tester samples
            if (this->beamEnergy == BEAM_AT_2GEV) {
                this->SampleName = "Uniform_sample_e_tester_2070MeV";
            } else if (this->beamEnergy == BEAM_AT_4GEV) {
                this->SampleName = "Uniform_sample_e_tester_4029MeV";
            } else if (this->beamEnergy == BEAM_AT_6GEV) {
                this->SampleName = "Uniform_sample_e_tester_5986MeV";
            }
        } else {
            std::string ParticleType = (bt::FindSubstring(this->RecoSamplePath, "OutPut_1e"))   ? "1e"
                                       : (bt::FindSubstring(this->RecoSamplePath, "OutPut_ep")) ? "ep"
                                       : (bt::FindSubstring(this->RecoSamplePath, "OutPut_en")) ? "en"
                                                                                                : "";
            if (ParticleType == "") {
                bt::ExitWithError("ExperimentParameters::ConfigSampleName", __FILE__, __LINE__, "Unable to configure sample name - unknown particle type in unform sample!");
            }

            this->SampleName = this->SampleTargetStr + "_" + ParticleType + "_sample_" + this->SampleBeamEnergyStr;
        }
    } else {
        bt::ExitWithError("ExperimentParameters::ConfigSampleName", __FILE__, __LINE__, "Unable to configure sample name!");
    }
}
#pragma endregion

// ConfigureVaryingSampleName function ----------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ConfigureVaryingSampleName function */
/**
 * Configure the name used to select varying inputs (acceptance maps, weight maps, neutron resolution, etc.).
 *
 * Current behavior:
 *   - Use the configured sample name directly for all supported sample types.
 */
void ExperimentParameters::ConfigureVaryingSampleName() {
    if ((this->sampleType == GENIE_SIMULATION_TYPE) || (this->sampleType == UNIFORM_TYPE) || (this->sampleType == DATA_TYPE)) {
        this->VaryingSampleName = this->SampleName;
    } else {
        bt::ExitWithError("ExperimentParameters::ConfigureVaryingSampleName", __FILE__, __LINE__, "Varying sample name can't be configured!");
    }
}
#pragma endregion

// ConfigureVz_cuts function --------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ConfigureVz_cuts function */
void ExperimentParameters::ConfigureVz_cuts() {
    const char* kTitle = "Vertex z component";
    const char* kCutLabel = "1e cut";

    auto set_cuts = [&](double lo, double hi, double loFD, double hiFD, double loCD, double hiCD) {
        this->Vz_cuts = DSCuts(kTitle, "", "", kCutLabel, 0, lo, hi);
        this->Vz_cuts_FD = DSCuts(kTitle, "FD", "", kCutLabel, 0, loFD, hiFD);
        this->Vz_cuts_CD = DSCuts(kTitle, "CD", "", kCutLabel, 0, loCD, hiCD);
    };

    auto set_same_all = [&](double lo, double hi) { set_cuts(lo, hi, lo, hi, lo, hi); };

    if (this->sampleType == GENIE_SIMULATION_TYPE) {                 // Sample is simulation
        if (this->SampleName == "C12x4_simulation_G18_Q204_6GeV") {  // 4-foil
            set_cuts(-7., 2., -8., 3., -7., 2.);
        } else if (this->SampleName == "C12_simulation_G18_Q204_6GeV") {  // 1-foil
            set_cuts(-5., 5., -5., 5., -2., 1.);
        } else if (this->SampleName == "C12_simulation_G18_Q2_0_02_2070MeV" || this->SampleName == "C12_simulation_G18_Q2_0_02_2070MeV_wFC" ||
                   this->SampleName == "C12_simulation_G18_Q2_0_25_4029MeV" || this->SampleName == "C12_simulation_G18_Q2_0_25_4029MeV_wFC" ||
                   this->SampleName == "C12_simulation_SuSa_Q2_0_02_2070MeV" || this->SampleName == "C12_simulation_SuSa_Q2_0_02_2070MeV_wFC" ||
                   this->SampleName == "C12_simulation_SuSa_Q2_0_25_4029MeV" || this->SampleName == "C12_simulation_SuSa_Q2_0_25_4029MeV_wFC") {
            set_cuts(-5., 4., -5., 4., -2., 1.);
        }
        // V_{z} cuts for the new simulation samples with corrected geometry. Cuts to be defined based on the RGM analysis note (RGM_Analysis_Note_Feb11-2026.pdf):
        else if (this->SampleName == "C12_sim_G18_Q2_0_02_2070MeV" || this->SampleName == "C12_sim_SuSa_Q2_0_02_2070MeV"     // C12; 1-foil GENIE G18/SuSa samples at 2 GeV (small foils)
                 || this->SampleName == "C12_sim_G18_Q2_0_25_4029MeV" || this->SampleName == "C12_sim_SuSa_Q2_0_25_4029MeV"  // C12; 1-foil GENIE G18/SuSa samples at 4 GeV (large foils)
        ) {
            set_cuts(-5., 0., -6., 0.7, -2.8, -1.5);  // Cuts from RGM_Analysis_Note_Feb11-2026.pdf
        } else if (this->SampleName == "Ar40_sim_G18_Q2_0_02_2070MeV" || this->SampleName == "Ar40_sim_G18_Q2_0_25_4029MeV" || this->SampleName == "Ar40_sim_SuSa_Q2_0_02_2070MeV" ||
                   this->SampleName == "Ar40_sim_SuSa_Q2_0_25_4029MeV") {
            set_cuts(-8.5, -3.5, -9.9, -2.6, -6.7, -4.8);  // Cuts from RGM_Analysis_Note_Feb11-2026.pdf
        }
    } else if (this->sampleType == DATA_TYPE) {  // Sample is data
        if (this->SampleName == "Ar40_data_2GeV_run_015672") {
            set_cuts(-7., 2., -12., -3., -7., -4.);
        } else if (this->SampleName == "Ar40_data_4GeV_run_015743") {
            set_cuts(-7., 2., -9., -2., -7., -4.);
        } else if (this->SampleName == "Ar40_data_6GeV_run_015792") {
            set_cuts(-7., 2., -9., -2., -7., -4.);
        } else if (this->SampleName == "C12_data_2GeV_run_015664") {
            set_cuts(-7., 2., -9., -1., -4., 0.);
        } else if (this->SampleName == "C12_data_4GeV_run_015778") {
            set_cuts(-7., 2., -6., 1., -4., -1.);
        } else if (this->SampleName == "D2_data_2GeV") {
            set_cuts(-7., 2., -10., 1., -7., 0.);
        } else if (this->SampleName == "D2_data_6GeV_run_015443") {
            set_cuts(-7., 2., -7., 3., -7., 1.);
        } else if (this->SampleName == "H1_data_2GeV_run_015634") {
            set_cuts(-7., 2., -11., 1., -7., 0.);
        } else if (this->SampleName == "H1_data_6GeV_run_015028") {
            set_cuts(-7., 2., -7., 1., -7., 1.);
        } else if (bt::FindSubstring(this->SampleName, "C12x4_data_6GeV_run_0151")) {
            if (bt::FindSubstring(this->SampleName, "C12x4_data_6GeV_run_015186") || bt::FindSubstring(this->SampleName, "C12x4_data_6GeV_run_015187")) {  // 4-foil
                set_same_all(-7., 2.);
            } else if (bt::FindSubstring(this->SampleName, "C12x4_data_6GeV_run_015188")) {  // 4-foil
                set_cuts(-7., 2., -8., 3., -7., 2.);
            } else {
                this->Vz_cuts = this->Vz_cuts_def;
                this->Vz_cuts_FD = this->Vz_cuts_FD_def;
                this->Vz_cuts_CD = this->Vz_cuts_CD_def;
            }
        } else if (this->SampleName == "C12x4_data_6GeV") {  // Sample is data (full 4-foil run)
            set_cuts(-7., 2., -8., 3., -7., 2.);

        } else if (this->SampleName == "LH2_data_6GeV_run_015032") {
            set_same_all(-15., 5.);
        }
    } else if (this->sampleType == UNIFORM_TYPE) {  // Sample is uniform simulation
        if (this->beamEnergy == BEAM_AT_2GEV) {
            if (this->SampleName == "Uniform_1e_sample_2070MeV") {
                set_same_all(-6., 0.);
            } else if (this->SampleName == "Uniform_ep_sample_2070MeV") {
                set_cuts(-6., 0., -6., 0., -5., -1.);
            } else if (this->SampleName == "Uniform_en_sample_2070MeV") {
                set_same_all(-6., 0.);
            }
        } else if (this->beamEnergy == BEAM_AT_4GEV) {
            if (this->SampleName == "Uniform_1e_sample_4029MeV") {
                set_same_all(-5., -1.);
            } else if (this->SampleName == "Uniform_ep_sample_4029MeV") {
                set_cuts(-6., 0., -6., 0., -5., -1.);
            } else if (this->SampleName == "Uniform_en_sample_4029MeV") {
                set_same_all(-6., 0.);
            }
        } else if (this->beamEnergy == BEAM_AT_6GEV) {
            if (this->SampleName == "Uniform_1e_sample_5986MeV") {
                set_same_all(-5., -1.);
            } else if (this->SampleName == "Uniform_ep_sample_5986MeV") {
                set_cuts(-6., 0., -6., 0., -5., -1.);
            } else if (this->SampleName == "Uniform_en_sample_5986MeV") {
                set_same_all(-6., 0.);
            }
        }
    } else {
        set_same_all(-5., 5.);
    }
}
#pragma endregion

// ConfiguredVz_cuts function -------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ConfiguredVz_cuts function */
void ExperimentParameters::ConfiguredVz_cuts() {
    const char* kTitle = "dVz";
    const char* kCutLabel = "1e cut";

    auto set_cuts = [&](double lo, double hi, double loFD, double hiFD, double loCD, double hiCD) {
        this->dVz_cuts = DSCuts(kTitle, "", "", kCutLabel, 0, lo, hi);
        this->dVz_cuts_FD = DSCuts(kTitle, "FD", "", kCutLabel, 0, loFD, hiFD);
        this->dVz_cuts_CD = DSCuts(kTitle, "CD", "", kCutLabel, 0, loCD, hiCD);
    };

    auto set_same_all = [&](double lo, double hi) { set_cuts(lo, hi, lo, hi, lo, hi); };

    if (this->sampleType == UNIFORM_TYPE) {  // Sample is uniform simulation
        if (this->beamEnergy == BEAM_AT_2GEV) {
            if (this->SampleName == "Uniform_1e_sample_2070MeV") {
                set_same_all(-9999., 9999.);
            } else if (this->SampleName == "Uniform_ep_sample_2070MeV") {
                set_same_all(-2.5, 2.5);
            } else if (this->SampleName == "Uniform_en_sample_2070MeV") {
                set_same_all(-9999., 9999.);
            }
        } else if (this->beamEnergy == BEAM_AT_4GEV) {
            if (this->SampleName == "Uniform_1e_sample_4029MeV") {
                set_same_all(-9999., 9999.);
            } else if (this->SampleName == "Uniform_ep_sample_4029MeV") {
                set_same_all(-2.5, 2.5);
            } else if (this->SampleName == "Uniform_en_sample_4029MeV") {
                set_same_all(-9999., 9999.);
            }
        } else if (this->beamEnergy == BEAM_AT_6GEV) {
            if (this->SampleName == "Uniform_1e_sample_5986MeV") {
                set_same_all(-9999., 9999.);
            } else if (this->SampleName == "Uniform_ep_sample_5986MeV") {
                set_same_all(-2.5, 2.5);
            } else if (this->SampleName == "Uniform_en_sample_5986MeV") {
                set_same_all(-9999., 9999.);
            }
        }
    } else if (this->sampleType == GENIE_SIMULATION_TYPE) {          // Sample is simulation
        if (this->SampleName == "C12x4_simulation_G18_Q204_6GeV") {  // 4-foil
            set_same_all(-5., 4.);
        } else if (this->SampleName == "C12_simulation_G18_Q204_6GeV") {  // 1-foil
            set_same_all(-5., 4.);
        } else if (this->SampleName == "C12_simulation_G18_Q2_0_02_2070MeV" || this->SampleName == "C12_simulation_G18_Q2_0_02_2070MeV_wFC" ||
                   this->SampleName == "C12_simulation_G18_Q2_0_25_4029MeV" || this->SampleName == "C12_simulation_G18_Q2_0_25_4029MeV_wFC" ||
                   this->SampleName == "C12_simulation_SuSa_Q2_0_02_2070MeV" || this->SampleName == "C12_simulation_SuSa_Q2_0_02_2070MeV_wFC" ||
                   this->SampleName == "C12_simulation_SuSa_Q2_0_25_4029MeV" || this->SampleName == "C12_simulation_SuSa_Q2_0_25_4029MeV_wFC") {
            set_cuts(-6., 6., -6., 6., -4., 4.);
        }
        // dV_{z} cuts for the new simulation samples with corrected geometry. Cuts to be defined based on the RGM analysis note (RGM_Analysis_Note_Feb11-2026.pdf):
        else if (this->SampleName == "C12_sim_G18_Q2_0_02_2070MeV" || this->SampleName == "C12_sim_SuSa_Q2_0_02_2070MeV"     // C12; 1-foil GENIE G18/SuSa samples at 2 GeV (small foils)
                 || this->SampleName == "C12_sim_G18_Q2_0_25_4029MeV" || this->SampleName == "C12_sim_SuSa_Q2_0_25_4029MeV"  // C12; 1-foil GENIE G18/SuSa samples at 4 GeV (large foils)
        ) {
            set_cuts(-3.5, 5.8, -3.5, 5.8, -1.8, 3.1);  // Cuts from RGM_Analysis_Note_Feb11-2026.pdf
        } else if (this->SampleName == "Ar40_sim_G18_Q2_0_02_2070MeV" || this->SampleName == "Ar40_sim_G18_Q2_0_25_4029MeV" || this->SampleName == "Ar40_sim_SuSa_Q2_0_02_2070MeV" ||
                   this->SampleName == "Ar40_sim_SuSa_Q2_0_25_4029MeV") {
            set_cuts(-3.5, 5.8, -3.5, 5.8, -1.8, 3.1);  // Cuts from RGM_Analysis_Note_Feb11-2026.pdf
        }
    } else if (this->sampleType == DATA_TYPE) {  // Sample is data
        if (this->SampleName == "Ar40_data_2GeV_run_015672") {
            set_cuts(-5., 4., -4., 8., -2., 6.);
        } else if (this->SampleName == "Ar40_data_4GeV_run_015743") {
            set_cuts(-5., 4., -8., 4., -4., 2.);
        } else if (this->SampleName == "Ar40_data_6GeV_run_015792") {
            set_cuts(-5., 4., -8., 4., -4., 2.);
        } else if (this->SampleName == "C12_data_2GeV_run_015664") {
            set_cuts(-5., 4., -3., 5., -2., 5.);
        } else if (this->SampleName == "C12_data_4GeV_run_015778") {
            set_cuts(-5., 4., -6., 3., -4., 2.);
        } else if (this->SampleName == "D2_data_2GeV") {
            set_cuts(-5., 4., -5., 7., -2., 5.);
        } else if (this->SampleName == "D2_data_6GeV_run_015443") {
            set_cuts(-5., 4., -7., 3., -5., 3.);
        } else if (this->SampleName == "H1_data_2GeV_run_015634") {
            set_cuts(-5., 4., -3., 6., -2., 5.);
        } else if (this->SampleName == "H1_data_6GeV_run_015028") {
            set_cuts(-5., 4., -6., 2., -4., 2.);
        } else if (bt::FindSubstring(this->SampleName, "C12x4_data_6GeV_run_0151")) {
            if (bt::FindSubstring(this->SampleName, "C12x4_data_6GeV_run_015186") || bt::FindSubstring(this->SampleName, "C12x4_data_6GeV_run_015187") ||
                bt::FindSubstring(this->SampleName, "C12x4_data_6GeV_run_015188")) {  // 4-foil
                set_same_all(-5., 4.);
            } else {
                this->dVz_cuts = dVz_cuts_def;
                this->dVz_cuts_FD = dVz_cuts_FD_def;
                this->dVz_cuts_CD = dVz_cuts_CD_def;
            }
        } else if (this->SampleName == "C12x4_data_6GeV") {  // Sample is data (full 4-foil run)
            set_same_all(-5., 4.);

        } else if (this->SampleName == "LH2_data_6GeV_run_015032") {
            set_same_all(-8., 4.);
        }
    } else {
        set_same_all(-8., 4.);
    }
}
#pragma endregion

// ConfigureBeamEnergy function -----------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ConfigureBeamEnergy function */
double ExperimentParameters::ConfigureBeamEnergy(const std::string& sn) {
    double be;

    if (bt::FindSubstring(sn, "598636MeV") || bt::FindSubstring(sn, "598636mev") || bt::FindSubstring(sn, "598636") || bt::FindSubstring(sn, "5986MeV") || bt::FindSubstring(sn, "5986mev") ||
        bt::FindSubstring(sn, "5986") || bt::FindSubstring(sn, "6GeV") || bt::FindSubstring(sn, "6Gev") || bt::FindSubstring(sn, "6gev")) {
        be = 5.98636;
    } else if (bt::FindSubstring(sn, "402962MeV") || bt::FindSubstring(sn, "402962mev") || bt::FindSubstring(sn, "402962") || bt::FindSubstring(sn, "4029MeV") ||
               bt::FindSubstring(sn, "4029mev") || bt::FindSubstring(sn, "4029") || bt::FindSubstring(sn, "4GeV") || bt::FindSubstring(sn, "4Gev") || bt::FindSubstring(sn, "4gev")) {
        be = 4.02962;
    } else if (bt::FindSubstring(sn, "207052MeV") || bt::FindSubstring(sn, "207052mev") || bt::FindSubstring(sn, "207052") || bt::FindSubstring(sn, "2070MeV") ||
               bt::FindSubstring(sn, "2070mev") || bt::FindSubstring(sn, "2070") || bt::FindSubstring(sn, "2GeV") || bt::FindSubstring(sn, "2Gev") || bt::FindSubstring(sn, "2gev")) {
        be = 2.07052;
    }

    return be;
}
#pragma endregion

#endif  // EXPERIMENTPARAMETERS_H
