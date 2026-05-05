# Physics Modules

This page groups the analysis-facing modules by purpose.

## Event Content And Selection

- [clas12ana](AnalysisTools/clas12ana.md): reconstructed-particle access and event-level analysis utilities
- [ParticleID](AnalysisTools/ParticleID.md): particle classification and detector-response helpers
- [TLCuts](AnalysisTools/TLCuts.md): truth-level event selection
- [EventCounters](AnalysisTools/EventCounters.md): aggregated counters used during event processing

## Sample And Detector Context

- [ExperimentParameters](AnalysisTools/ExperimentParameters.md): infer target, sample type, tune, beam energy, and derived names from the sample path
- [TargetParameters](AnalysisTools/TargetParameters.md): target-nucleus metadata
- [HipoChainLoader](AnalysisTools/HipoChainLoader.md): safe HIPO chain construction

## Detector Acceptance And Efficiency

- [AMaps](AnalysisTools/AMaps.md): acceptance maps and weight maps
- [DEfficiency](AnalysisTools/DEfficiency.md): reconstructed/truth efficiency and acceptance-correction plots

## Resolution And Cut Extraction

- [DSCuts](AnalysisTools/DSCuts.md): per-cut parameter container
- [MomentumResolution](AnalysisTools/MomentumResolution.md): slice fitting, coefficient loading, correction, and smearing workflows
- [clas12debug](AnalysisTools/clas12debug.md): debugging helpers around CLAS12-facing analysis paths
- [Debugger](AnalysisTools/Debugger.md): runtime safety checks and diagnostic printing

## Plotting Helpers

- [hPlot1D](AnalysisTools/hPlot1D.md)
- [hPlot2D](AnalysisTools/hPlot2D.md)
- [hsPlots](AnalysisTools/hsPlots.md)
- [Legacy plotting functions](functions/HistogramPlottingFunctions.md)
