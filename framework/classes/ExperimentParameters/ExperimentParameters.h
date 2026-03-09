
// #ifndef EXPERIMENTPARAMETERS_H
// #define EXPERIMENTPARAMETERS_H
#ifdef EXPERIMENTPARAMETERS_H

    #include <iostream>

    // Include libraries:
    #include "../../namespaces/general_utilities/lists.h"
    #include "../../namespaces/general_utilities/utilities.h"
    #include "../../namespaces/setup/debugging.h"
    #include "../../namespaces/setup/path_definitions.h"

    // Include classes:
    #include "../DSCuts/DSCuts.h"

    // Include inherited classes:
    #include "../TargetParameters/TargetParameters.h"

    // Include CLAS12 libraries:
    #include "../../includes/clas12_include.h"

namespace bt = basic_tools;
namespace db = debugging;
namespace pd = path_definitions;

/**
 * @class ExperimentParameters
 *
 * Central configuration object derived from TargetParameters.
 *
 * Purpose:
 *   Parse a reconstructed sample path (RecoSamplePath) and derive all parameters needed to run the analysis:
 *     - target element (H1/D2/C12/Cx4/Ar40/Uniform)
 *     - sample type (GENIE simulation / farm data / uniform)
 *     - GENIE tune label (G18 vs SuSa)
 *     - beam energy bucket + numeric beam energy value
 *     - Q2 cut category (for GENIE truth generation)
 *     - canonical sample-name string used everywhere in the analysis
 *     - vertex-z and delta-vertex-z (Vz/dVz) cut windows
 *
 * Notes:
 *   The Config* methods are mutators (void). They configure member variables.
 *   Because of that, they must be called in the constructor body, not in an initializer list.
 */
class ExperimentParameters : public TargetParameters {
   public:
    // Input paths and glob patterns ------------------------------------------------------------------------------------------------------------------------------------

    // InputFilesGlobal = Full glob pattern for input files, e.g. "/path/to/samples/*hipo"
    const std::string RecoSamplePath, ReconHipoDir, HipoFilesPrefix, InputFilesGlobal;

    // Sample type ------------------------------------------------------------------------------------------------------------------------------------------------------

    /**
     * @enum SampleType
     * Defines the sample type inferred from the sample path.
     */
    enum SampleType { GENIE_SIMULATION_TYPE, DATA_TYPE, UNIFORM_TYPE, UNKNOWN_TYPE, UNSET_TYPE };

    // GENIE tune -------------------------------------------------------------------------------------------------------------------------------------------------------

    /**
     * @enum GENIETune
     * Defines the GENIE tune used in simulation samples.
     */
    enum GENIETune {
        G18_TUNE,   // G18_10a_00_000
        SUSA_TUNE,  // GEM21_11a_00_000
        UNKNOWN_TUNE,
        UNSET_TUNE
    };

    // Beam energy ------------------------------------------------------------------------------------------------------------------------------------------------------

    /**
     * @enum BeamEnergy
     * Beam energy bucket inferred from the path.
     */
    enum BeamEnergy { BEAM_AT_2GEV, BEAM_AT_4GEV, BEAM_AT_6GEV, UNKNOWN_ENERGY, UNSET_ENERGY };

    // Q^2 cut ----------------------------------------------------------------------------------------------------------------------------------------------------------

    /**
     * @enum Q2Cut
     * Q2 cut category used for GENIE truth-level generation.
     */
    enum Q2Cut {
        GENIE_DEF_CUT,  // 0.02 GeV^2/c^2; For 2 GeV samples
        Q2_0_25_CUT,    // 0.25 GeV^2/c^2; For 4 GeV samples
        Q2_0_4_CUT,     // 0.40 GeV^2/c^2; For 6 GeV samples
        UNKNOWN_CUT,
        UNSET_CUT
    };

    // ==================================================================================================================================================================
    // Constructor
    // ==================================================================================================================================================================

    // Constructor: configures everything based on the sample path and optional run directory label.
    ExperimentParameters(const std::string& RecoSamplePath, const std::string& ReconHipoDir, const std::string& HipoFilesPrefix = pd::HipoFilesPrefix);

    // ==================================================================================================================================================================
    // Configure functions
    // ==================================================================================================================================================================

    // ConfigSampleTarget function --------------------------------------------------------------------------------------------------------------------------------------

    void ConfigSampleTarget();

    // ConfigSampleType function ----------------------------------------------------------------------------------------------------------------------------------------

    void ConfigSampleType();

    // ConfigGENIETune function -----------------------------------------------------------------------------------------------------------------------------------------

    void ConfigGENIETune();

    // ConfigQ2Cut function ---------------------------------------------------------------------------------------------------------------------------------------------

    void ConfigQ2Cut();

    // ConfigBeamEnergy function ----------------------------------------------------------------------------------------------------------------------------------------

    void ConfigBeamEnergy();

    // ConfigSampleName function ----------------------------------------------------------------------------------------------------------------------------------------

    void ConfigSampleName();

    // ConfigureVaryingSampleName function ------------------------------------------------------------------------------------------------------------------------------

    void ConfigureVaryingSampleName();

    // ConfigureVz_cuts function ----------------------------------------------------------------------------------------------------------------------------------------

    void ConfigureVz_cuts();

    // ConfiguredVz_cuts function ---------------------------------------------------------------------------------------------------------------------------------------

    void ConfiguredVz_cuts();

    // ConfigureBeamEnergy function -------------------------------------------------------------------------------------------------------------------------------------

    double ConfigureBeamEnergy(const std::string& sn);

    // ==================================================================================================================================================================
    // Getter = functions
    // ==================================================================================================================================================================

    // Get input paths and glob patterns --------------------------------------------------------------------------------------------------------------------------------

    std::string GetRecoSamplePath() const { return RecoSamplePath; }
    std::string GetReconHipoDir() const { return ReconHipoDir; }
    std::string GetHipoFilesPrefix() const { return HipoFilesPrefix; }
    std::string GetInputFiles() const { return InputFilesGlobal; }

    // GetSampleType function -------------------------------------------------------------------------------------------------------------------------------------------

    ExperimentParameters::SampleType GetSampleType() const { return sampleType; }

    // GetGENIETune function --------------------------------------------------------------------------------------------------------------------------------------------

    ExperimentParameters::GENIETune GetGENIETune() const { return genieTune; }

    // GetGENIETune function --------------------------------------------------------------------------------------------------------------------------------------------

    ExperimentParameters::BeamEnergy GetBeamEnergy() const { return beamEnergy; }

    // GetQ2Cut function ------------------------------------------------------------------------------------------------------------------------------------------------

    ExperimentParameters::Q2Cut GetQ2Cut() const { return Q2cut; }

    // IsSimulationSample function --------------------------------------------------------------------------------------------------------------------------------------

    bool IsSimulationSample() const { return (sampleType == GENIE_SIMULATION_TYPE || sampleType == UNIFORM_TYPE); };

    // IsDataSample function --------------------------------------------------------------------------------------------------------------------------------------------

    bool IsDataSample() const { return (sampleType == DATA_TYPE); };

    // GetBeamEnergy function -------------------------------------------------------------------------------------------------------------------------------------------

    double GetBeamEnergy() { return BeamEnergyValue; }

    // Check beam energy functions --------------------------------------------------------------------------------------------------------------------------------------

    bool IsBeamAt2GeV() const { return (beamEnergy == BEAM_AT_2GEV); };
    bool IsBeamAt4GeV() const { return (beamEnergy == BEAM_AT_4GEV); };
    bool IsBeamAt6GeV() const { return (beamEnergy == BEAM_AT_6GEV); };

    // Get sample name functions ----------------------------------------------------------------------------------------------------------------------------------------

    std::string GetSampleName() const { return SampleName; }
    std::string GetVaryingSampleName() const { return VaryingSampleName; };

    // Get vertex cuts functions ----------------------------------------------------------------------------------------------------------------------------------------

    DSCuts GetVz_cuts() { return Vz_cuts; };
    DSCuts GetVz_cuts_FD() { return Vz_cuts_FD; };
    DSCuts GetVz_cuts_CD() { return Vz_cuts_CD; };
    DSCuts GetdVz_cuts() { return dVz_cuts; };
    DSCuts GetdVz_cuts_FD() { return dVz_cuts_FD; };
    DSCuts GetdVz_cuts_CD() { return dVz_cuts_CD; };

    // IsLocalSample function -------------------------------------------------------------------------------------------------------------------------------------------

    bool IsLocalSample() const { return LocalSample; };

   protected:
    // ==================================================================================================================================================================
    // Sample parameters
    // ==================================================================================================================================================================

    // Sample target (element) ------------------------------------------------------------------------------------------------------------------------------------------

    // Sample target (element). Example values: "H1", "D2", "C12", "Ar40", "Uniform"
    std::string SampleTargetStr = "";

    // Sample type ------------------------------------------------------------------------------------------------------------------------------------------------------

    // Configured by ConfigSampleType()
    SampleType sampleType = UNSET_TYPE;

    // A short string label used when constructing SampleName (e.g. "sim" or "data")
    std::string SampleTypeStr = "";

    // bool SimulationSample = false;
    // bool DataSample = false;

    // GENIE tune -------------------------------------------------------------------------------------------------------------------------------------------------------

    // Configured by ConfigGENIETune()
    GENIETune genieTune = UNSET_TUNE;

    // Short label used in SampleName (e.g. "G18" or "SuSa")
    std::string SampleGENIETuneStr = "";

    // Beam energy ------------------------------------------------------------------------------------------------------------------------------------------------------

    // Configured by ConfigBeamEnergy()
    BeamEnergy beamEnergy = UNSET_ENERGY;

    // Numeric beam energy value used in analysis (GeV), configured after SampleName is set by ConfigureBeamEnergy()
    double BeamEnergyValue = 0.0;

    // Short label used in SampleName (e.g. "2070MeV", "4029MeV", "5986MeV")
    std::string SampleBeamEnergyStr = "";

    // bool BeamAt2GeV = false, BeamAt4GeV = false, BeamAt6GeV = false;

    // Q^2 cut ----------------------------------------------------------------------------------------------------------------------------------------------------------

    // Configured by ConfigQ2Cut()
    Q2Cut Q2cut = UNSET_CUT;

    // Short label used in SampleName (e.g. "Q2_0_02")
    std::string SampleQ2CutStr = "";

    // Sample names -----------------------------------------------------------------------------------------------------------------------------------------------------

    // Canonical sample name used across the analysis
    std::string SampleName = "";

    // Name used for acceptance maps / weight maps / neutron resolution inputs that vary with sample. Currently set to the same as SampleName, but can be changed in
    // ConfigureVaryingSampleName() if needed.
    std::string VaryingSampleName = "";

    // Vertex cuts ------------------------------------------------------------------------------------------------------------------------------------------------------

    // Default vertex-z cuts and configured cuts for the sample. Configured by ConfigureVz_cuts() and ConfiguredVz_cuts().
    DSCuts Vz_cuts_def = DSCuts("Vertex z component", "", "", "1e cut", 0, -15, 5);
    DSCuts Vz_cuts, Vz_cuts_FD, Vz_cuts_CD, Vz_cuts_FD_def = Vz_cuts_def, Vz_cuts_CD_def = Vz_cuts_def;
    DSCuts dVz_cuts_def = DSCuts("dVz", "", "", "1e cut", 0, -8, 4);
    DSCuts dVz_cuts, dVz_cuts_FD, dVz_cuts_CD, dVz_cuts_FD_def = dVz_cuts_def, dVz_cuts_CD_def = dVz_cuts_def;

    // Other ------------------------------------------------------------------------------------------------------------------------------------------------------------

    // Whether the sample is considered local (heuristic). Used to decide how to add files to the HipoChain (e.g. from a local directory or from the ifarm).
    bool LocalSample = false;
};

#endif  // EXPERIMENTPARAMETERS_H
