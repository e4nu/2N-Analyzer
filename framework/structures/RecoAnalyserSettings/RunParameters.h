//
// Created by Alon Sportes on 01/04/2025.
//

#ifndef RUNPARAMETERS_H
#define RUNPARAMETERS_H

#include <iostream>

// Include classes:
#include "../../classes/ExperimentParameters/ExperimentParameters.cpp"

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

    RunParameters(ExperimentParameters& Experiment, const std::string& AnalyzeFilePath, const std::string& AnalyzeFileSample)
        : SampleName(Experiment.ConfigureSampleName(AnalyzeFilePath, AnalyzeFileSample)),  // Configure SampleName from input
          VaryingSampleName(Experiment.GetVaryingSampleName()),                            // Get VaryingSampleName (configured from SampleName) - for data runs!
          beamE(Experiment.GetBeamEnergy()),                                               // Configure beam energy from SampleName
          Target(Experiment.GetTargetElement()),                                           // Configure target (element) from SampleName
          TargetPDG(Experiment.GetTargetElementPDG()),                                     // Configure target PDG from SampleName
          isLocal(Experiment.SLocal()),
          isMC(Experiment.SSample()),
          isData(Experiment.DSample()),
          is2GeVSample(Experiment.IsBeamAt2GeV()),
          is4GeVSample(Experiment.IsBeamAt4GeV()),
          is6GeVSample(Experiment.IsBeamAt6GeV()) {}
};

#endif  // RUNPARAMETERS_H
