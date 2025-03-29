set(CLASS_SOURCES 
    AMaps/AMaps.cpp
    AMaps/CustomSliceLimits.cpp
    clas12ana/clas12ana.cpp
    DataAnalyser/DataAnalyser.cpp
    Debugger/Debugger.cpp
    DEfficiency/DEfficiency.cpp
    Directories/Directories.cpp
    # DSCuts/DSCuts.cpp
    ExperimentParameters/ExperimentParameters.cpp
    # gst/gst.cpp
    hPlots/hPlot1D.cpp
    hPlots/hPlot2D.cpp
    MomentumResolution/MomentumResolution.cpp
    ParticleID/ParticleID.cpp
    Settings/CustomPlotsDirectory.cpp
    Settings/Settings.cpp
    # TargetParameters/TargetParameters.cpp
    TLCuts/TLCuts.cpp
)

# Create a library
add_library(AnalysisTools ${CLASS_SOURCES})

# Link ROOT properly (without -lEG)
target_link_libraries(AnalysisTools PUBLIC ${ROOT_LIBRARIES})

# Link Clas12 libraries correctly (no -l)
target_link_libraries(AnalysisTools PUBLIC Clas12Banks Hipo4 Clas12Root)
