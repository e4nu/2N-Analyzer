//
// Created by Alon Sportes on 01/04/2025.
//

#ifndef RUNPARAMETERS_H
#define RUNPARAMETERS_H

#include <iostream>

// Include classes:
#include "../../classes/ExperimentParameters/ExperimentParameters.cpp"

/**
 * @struct RunParameters
 * @brief A struct to store the run parameters for the analysis, including sample name, beam energy, target information, and flags for local, MC, and data samples.
 * @details This struct is used to store the run parameters for the analysis, which are initialized from the ExperimentParameters class based on the RecoSamplePath and ReconHipoDir. The run
 * parameters include the sample name, beam energy, target information (element and PDG), and flags for whether the sample is local, MC, or data. The run parameters are used in the main code
 * to control various aspects of the analysis based on the characteristics of the sample being analyzed.
 * @note The run parameters are initialized from the ExperimentParameters class, which allows for flexibility in configuring different samples based on their RecoSamplePath and ReconHipoDir.
 * It is important to set these parameters appropriately based on the goals of the analysis and the characteristics of the data being analyzed. For example, the beam energy and target
 * information can affect the kinematics of the events being analyzed, while the flags for local, MC, and data samples can affect how the analysis is performed and how the results are
 * interpreted.
 */
// TODO: discard this struct and merge it into the ExperimentParameters class!
struct RunParameters {
    const std::string SampleName;
    const std::string VaryingSampleName;

    const double beamE;

    const std::string Target;
    const int TargetPDG;

    const bool isLocal;

    const bool isMC;
    const bool isData;

    const bool is2GeVSample;
    const bool is4GeVSample;
    const bool is6GeVSample;

    RunParameters(ExperimentParameters& Experiment, const std::string& RecoSamplePath, const std::string& ReconHipoDir)
        : SampleName(Experiment.GetSampleName()),                // Get SampleName from ExperimentParameters (configured from RecoSamplePath and ReconHipoDir)
          VaryingSampleName(Experiment.GetVaryingSampleName()),  // Get VaryingSampleName (configured from SampleName) - for data runs!
          beamE(Experiment.GetBeamEnergy()),                     // Configure beam energy from SampleName
          Target(Experiment.GetTargetElement()),                 // Configure target (element) from SampleName
          TargetPDG(Experiment.GetTargetElementPDG()),           // Configure target PDG from SampleName
          isLocal(Experiment.IsLocalSample()),
          isMC(Experiment.IsSimulationSample()),
          isData(Experiment.IsDataSample()),
          is2GeVSample(Experiment.IsBeamAt2GeV()),
          is4GeVSample(Experiment.IsBeamAt4GeV()),
          is6GeVSample(Experiment.IsBeamAt6GeV()) {}
};

#endif  // RUNPARAMETERS_H
