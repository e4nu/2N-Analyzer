
// #ifndef EXPERIMENTPARAMETERS_H
// #define EXPERIMENTPARAMETERS_H
#ifdef EXPERIMENTPARAMETERS_H

    #include <iostream>

    // Include libraries:
    #include "../../namespaces/general_utilities/lists.h"
    #include "../../namespaces/general_utilities/utilities.h"

    // Include classes:
    #include "../DSCuts/DSCuts.h"

    // Include inherited classes:
    #include "../TargetParameters/TargetParameters.h"

// #include "HipoChain.h"
// #include "clas12reader.h"

    #include "../../includes/clas12_include.h"

// using namespace clas12;
using namespace utilities;

class ExperimentParameters : public TargetParameters {
   protected:
    std::string SampleName;
    std::string VaryingSampleName;  // for AMaps, WMaps and nRes
    double BeamEnergy;

    bool LocalSample = false;

    bool SimulationSample = false;
    bool DataSample = false;

    bool BeamAt2GeV = false;
    bool BeamAt4GeV = false;
    bool BeamAt6GeV = false;

    DSCuts Vz_cuts_def = DSCuts("Vertex z component", "", "", "1e cut", 0, -15, 5);
    DSCuts Vz_cuts, Vz_cuts_FD, Vz_cuts_CD, Vz_cuts_FD_def = Vz_cuts_def, Vz_cuts_CD_def = Vz_cuts_def;
    DSCuts dVz_cuts_def = DSCuts("dVz", "", "", "1e cut", 0, -8, 4);
    DSCuts dVz_cuts, dVz_cuts_FD, dVz_cuts_CD, dVz_cuts_FD_def = dVz_cuts_def, dVz_cuts_CD_def = dVz_cuts_def;

   public:
    // Constructor ----------------------------------------------------------------------------------------------------------------------------------------

    ExperimentParameters(const std::string &AnalyzeFilePath, const std::string &AnalyzeFileSample);

    // ConfigureSampleName function -----------------------------------------------------------------------------------------------------------------------------------------

    std::string ConfigureSampleName(const std::string &AnalyzeFilePath, const std::string &AnalyzeFileSample);

    // ConfigureVaryingSampleName function -----------------------------------------------------------------------------------------------------------------------------------

    void ConfigureVaryingSampleName(const std::string &sn);

    // ConfigureVz_cuts function --------------------------------------------------------------------------------------------------------------------------------------------

    void ConfigureVz_cuts(const std::string &sn);

    // ConfiguredVz_cuts function -------------------------------------------------------------------------------------------------------------------------------------------

    void ConfiguredVz_cuts(const std::string &sn);

    // ConfigureBeamEnergy function -----------------------------------------------------------------------------------------------------------------------------------------

    double ConfigureBeamEnergy(const std::string &sn);

    // GetBeamEnergy function -----------------------------------------------------------------------------------------------------------------------------------------------

    double GetBeamEnergy();

    // GetVz_cuts function -----------------------------------------------------------------------------------------------------------------------------------------------

    DSCuts GetVz_cuts() { return Vz_cuts; };

    // GetVz_cuts_FD function -----------------------------------------------------------------------------------------------------------------------------------------------

    DSCuts GetVz_cuts_FD() { return Vz_cuts_FD; };

    // GetVz_cuts_CD function -----------------------------------------------------------------------------------------------------------------------------------------------

    DSCuts GetVz_cuts_CD() { return Vz_cuts_CD; };

    // GetdVz_cuts function -----------------------------------------------------------------------------------------------------------------------------------------------

    DSCuts GetdVz_cuts() { return dVz_cuts; };

    // GetdVz_cuts_FD function -----------------------------------------------------------------------------------------------------------------------------------------------

    DSCuts GetdVz_cuts_FD() { return dVz_cuts_FD; };

    // GetdVz_cuts_CD function -----------------------------------------------------------------------------------------------------------------------------------------------

    DSCuts GetdVz_cuts_CD() { return dVz_cuts_CD; };

    // AddToHipoChain function ----------------------------------------------------------------------------------------------------------------------------------------

    // This is the old function used to add runs to the HipoChain
    void AddToHipoChain(HipoChain &chain, const std::string &sn, const std::string &AnalyzeFilePath, const std::string &AnalyzeFileSample, const std::string &AnalyzeFile);

    // AddToHipoChainFromList function ----------------------------------------------------------------------------------------------------------------------------------------

    // This is the old function used to add runs to the HipoChain
    void AddToHipoChainFromList(HipoChain &chain, const std::string &sn, const std::string &AnalyzeFilePath, const std::string &AnalyzeFileSample, const std::string &AnalyzeFile);

    // other functions ------------------------------------------------------------------------------------------------------------------------------------------------------

    bool SLocal() const { return LocalSample; };

    bool SSample() const { return SimulationSample; };

    bool DSample() const { return DataSample; };

    bool IsBeamAt2GeV() const { return BeamAt2GeV; };

    bool IsBeamAt4GeV() const { return BeamAt4GeV; };

    bool IsBeamAt6GeV() const { return BeamAt6GeV; };

    std::string GetVaryingSampleName() { return VaryingSampleName; };
};

#endif  // EXPERIMENTPARAMETERS_H
