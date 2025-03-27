cmake_minimum_required(VERSION 3.16.3)
project(2N_Analyser_e4nu)

set(CMAKE_CXX_STANDARD 14)

# Find ROOT
find_package(ROOT REQUIRED)

# Include ROOT
include_directories(${ROOT_INCLUDE_DIRS})

# # Set the path to CLAS12ROOT includes and libraries
# set(CLAS12ROOT_INCLUDE_DIR /home/alon/clas12root/Clas12Root)
# set(CLAS12BANKS_INCLUDE_DIR /home/alon/clas12root/Clas12Banks)
# set(HIPO4_INCLUDE_DIR /home/alon/clas12root/hipo4)

# # Include directories for CLAS12ROOT
# include_directories(${CLAS12ROOT_INCLUDE_DIR})
# include_directories(${CLAS12BANKS_INCLUDE_DIR})
# include_directories(${HIPO4_INCLUDE_DIR})

# # Set library paths
# set(CLAS12ROOT_LIBRARY /home/alon/clas12root/lib/libClas12Root.so)
# set(CLAS12BANKS_LIBRARY /home/alon/clas12root/lib/libClas12Banks.so)
# set(HIPO4_LIBRARY /home/alon/clas12root/lib/libHipo4.so)

# # Check if the library files exist
# if (NOT EXISTS ${CLAS12ROOT_LIBRARY})
#     message(FATAL_ERROR "CLAS12ROOT_LIBRARY does not exist at ${CLAS12ROOT_LIBRARY}")
# endif()

# if (NOT EXISTS ${CLAS12BANKS_LIBRARY})
#     message(FATAL_ERROR "CLAS12BANKS_LIBRARY does not exist at ${CLAS12BANKS_LIBRARY}")
# endif()

# if (NOT EXISTS ${HIPO4_LIBRARY})
#     message(FATAL_ERROR "HIPO4_LIBRARY does not exist at ${HIPO4_LIBRARY}")
# endif()

# # Collect all .cpp files in the src directory
# file(GLOB CLAS12ROOT_SOURCES "${CLAS12ROOT_INCLUDE_DIR}/*.cpp")
# file(GLOB CLAS12BANKS_SOURCES "${CLAS12BANKS_INCLUDE_DIR}/*.cpp")
# file(GLOB HIPO4_SOURCES "${HIPO4_INCLUDE_DIR}/*.cpp")

# Add executable
#add_executable(project ${CLAS12ROOT_SOURCES} ${CLAS12BANKS_SOURCES} ${HIPO4_SOURCES})

include_directories(.)
include_directories(macros)
include_directories(setup)
include_directories(setup/CLAS12DAnalysis)
include_directories(setup/CLAS12DAnalysis/CLAS12DAnalyser)
include_directories(setup/CLAS12DAnalysis/CLAS12DASettings)
include_directories(setup/CLAS12DAnalysis/CLAS12DHistograms)
include_directories(framework)
include_directories(framework/classes)
include_directories(framework/classes/AMaps)
include_directories(framework/classes/clas12ana)
include_directories(framework/classes/clas12ana/clas12ana_ver_1_myedit)
include_directories(framework/classes/clas12ana/other)
include_directories(framework/classes/DEfficiency)
include_directories(framework/classes/Directories)
include_directories(framework/classes/DSCuts)
include_directories(framework/classes/ExperimentParameters)
include_directories(framework/classes/hData)
include_directories(framework/classes/hPlots)
include_directories(framework/classes/MomentumResolution)
include_directories(framework/classes/TargetParameters)
include_directories(framework/classes/TLCuts)
include_directories(source/data)
include_directories(framework/functions)
include_directories(framework/functions/AngleCalc)
include_directories(framework/functions/DrawAndSaveFunctions)
include_directories(framework/functions/EventProperties)
include_directories(framework/functions/FitFunctions)
include_directories(framework/functions/Math_func)
include_directories(framework/functions/other)
include_directories(framework/functions/PID_functions)
include_directories(framework/other)
include_directories(framework/structures)

add_executable(project ${CLAS12ROOT_SOURCES} ${CLAS12BANKS_SOURCES} ${HIPO4_SOURCES}
        macros/Other/process_events.C
        macros/PlotsEffAndACorr2x3/PlotsACorr2x3Electron.cpp
        macros/PlotsEffAndACorr2x3/PlotsACorr2x3Nucleon.cpp
        macros/PlotsEffAndACorr2x3/PlotsEff2x3.cpp
        macros/PlotsEffAndACorr2x3/PlotsEff2x3Electron.cpp
        macros/PlotsEffAndACorr2x3/PlotsEff2x3Nucleon.cpp
        macros/AMapsDiveTest.cpp
        macros/BetaFitAndSave.cpp
        macros/BetaFitAndSaveApprax.cpp
        "macros/MCtoDATAcomp - Copy (2).cpp"
        "macros/MCtoDATAcomp - Copy.cpp"
        macros/MCtoDATAcomp.cpp
        macros/plots4x4.txt
        macros/PlotsBeta4x4.cpp
        macros/PlotsEffAndACorr2x3.cpp
        macros/plotter_1x1.cpp
        macros/plotter_1x2.cpp
        macros/plotter_2x2.cpp
        "macros/PoliFitter - Copy.cpp"
        macros/PoliFitter.cpp
        macros/process_events.C
        setup/codeSetup.h
        framework/classes/gst/gst.h
        framework/classes/AMaps/AMaps.cpp
        framework/classes/AMaps/AMaps.h
        framework/classes/clas12ana/clas12ana_ver_1_myedit/clas12ana.h
        framework/classes/clas12ana/clas12ana_ver_1_myedit/example_ana.cpp
        "framework/classes/clas12ana/other/clas12ana - Copy (2).h"
        "framework/classes/clas12ana/other/clas12ana - Copy.h"
        framework/classes/clas12ana/clas12ana.h
        framework/classes/clas12ana/clas12ana_ver_1_myedit.h
        "framework/classes/clas12ana/example_ana - Copy.cpp"
        framework/classes/clas12ana/example_ana.cpp
        framework/classes/clas12ana/skim_events.cpp
        framework/classes/DEfficiency/DEfficiency.cpp
        framework/classes/DEfficiency/DEfficiency.h
        framework/classes/Directories/Directories.cpp
        framework/classes/Directories/Directories.h
        framework/classes/DSCuts/DSCuts.h
        framework/classes/ExperimentParameters/ExperimentParameters.cpp
        framework/classes/ExperimentParameters/ExperimentParameters.h
        framework/classes/hPlots/hPlot1D.cpp
        framework/classes/hPlots/hPlot1D.h
        framework/classes/hPlots/hPlot1DStack.cpp
        framework/classes/hPlots/hPlot1DStack.h
        framework/classes/hPlots/hPlot2D.cpp
        framework/classes/hPlots/hPlot2D.h
        framework/classes/MomentumResolution/MomentumResolution.cpp
        framework/classes/MomentumResolution/MomentumResolution.h
        framework/classes/TargetParameters/TargetParameters.h
        framework/classes/TLCuts/TLCuts.cpp
        framework/classes/TLCuts/TLCuts.h
        source/data/AcceptanceCorrections/C12_simulation_6GeV_T5/ACorr_data_-_C12_simulation_6GeV_T5.root
        source/data/AcceptanceCorrections/C12_simulation_6GeV_T5_first_10/ACorr_data_-_C12_simulation_6GeV_T5_first_10.root
        source/data/AcceptanceCorrections/C12_simulation_6GeV_T5_first_100/ACorr_data_-_C12_simulation_6GeV_T5_first_100.root
        source/data/AcceptanceCorrections/C12_simulation_6GeV_T5_first_250/ACorr_data_-_C12_simulation_6GeV_T5_first_250.root
        source/data/AcceptanceCorrections/C12_simulation_G18_2GeV/ACorr_data_-_C12_simulation_G18_2GeV.root
        source/data/AcceptanceCorrections/C12_simulation_G18_Q208_6GeV/ACorr_data_-_C12_simulation_G18_Q208_6GeV.root
        source/data/AcceptanceCorrections/Ca48_simulation_G18_10a_02_11b_6GeV/ACorr_data_-_Ca48_simulation_G18_10a_02_11b_6GeV.root
        source/data/PIDCuts/clas12ana_ver_1_myedit/ana.par
        source/data/PIDCuts/clas12ana_ver_1_myedit/ecal.par
        source/data/PIDCuts/clas12ana_ver_1_myedit/Fitted_PID_Cuts__C12_simulation_6GeV_T5.par
        source/data/PIDCuts/clas12ana_ver_1_myedit/Fitted_PID_Cuts__C12_simulation_6GeV_T5_first_10.par
        source/data/PIDCuts/clas12ana_ver_1_myedit/Fitted_PID_Cuts__C12_simulation_6GeV_T5_first_100.par
        source/data/PIDCuts/clas12ana_ver_1_myedit/Fitted_PID_Cuts__Ca48_data_6GeV_run_015832.par
        source/data/PIDCuts/clas12ana_ver_1_myedit/Fitted_PID_Cuts__Ca48_simulation_G18_10a_02_11b_6GeV.par
        source/data/PIDCuts/clas12ana_ver_1_myedit/Fitted_PID_Cuts__LH2_data_6GeV_run_015032.par
        source/data/PIDCuts/ana.par
        source/data/PIDCuts/ecal.par

        source/data/PIDCuts/paramsPI_40Ca_x2.dat
        source/data/PIDCuts/paramsPI_LD2_x2.dat
        source/data/PIDCuts/paramsSF_40Ca_x2.dat
        source/data/PIDCuts/paramsSF_LD2_x2.dat
        framework/functions/FillByInt.h
        framework/functions/GetTruthLevelCuts.h
        framework/functions/HistogramPlottingFunctions.h
        framework/functions/neutron_veto.h
        framework/functions/NeutronECAL_Cut_Veto.h
        framework/functions/PlotsEff2x3.h
        framework/functions/StackPlot3.h
        framework/functions/StackPlot4.h
        framework/namespaces/general_utilities/utilities/utilities.h
        framework/namespaces/general_utilities/analysis_math/analysis_math.h
        framework/namespaces/general_utilities/analysis_math/analysis_math_reco_extentions.h
        framework/namespaces/general_utilities/basic_tools/basic_tools.h
        framework/namespaces/general_utilities/constants/constants.h
        framework/namespaces/general_utilities/data_processor/data_processor.h
        framework/namespaces/general_utilities/lists/lists.h
        framework/namespaces/general_utilities/reco_analysis_functions/reco_analysis_functions.h
        framework/namespaces/settings/general_settings/general_settings.h
        framework/namespaces/general_utilities/truth_analysis_functions/truth_analysis_functions.h
        "framework/other/fitexample - Copy.c"
        framework/other/fitexample.c
        framework/other/hPlot1D_example.cpp
        framework/structures/Histogram1D_beta/Histogram1D.cpp
        macros/Other/db_script.txt
        Old_analysers/DetectorSimulationAnalyser.C
        git_handle.sh
        main.c
        run.csh
        Old_analysers/TruthLevelAnalyser.c
        framework/classes/Settings/Settings.cpp
        framework/classes/Settings/Settings.h
        framework/classes/Settings/CustomPlotsDirectory.cpp
        framework/classes/Settings/CustomPlotsDirectory.h
        framework/classes/ParticleID/ParticleID.cpp
        framework/classes/ParticleID/ParticleID.h
        framework/classes/AMaps/CustomSliceLimits.h
        macros/zoomed_in_histogram.cpp
        framework/classes/DataAnalyser/DataAnalyser.cpp
        framework/classes/DataAnalyser/DataAnalyser.h
        testers/nFD_eff_test/nFD_eff_test_functions.cpp
        testers/nFD_eff_test/nFD_eff_test.cpp
        macros/plotHistogramsWithLabels.cpp
        testers/reaction_cuts_test/reaction_cuts_test.cpp
        testers/reaction_cuts_test/reaction_cuts_test_functions.cpp
        framework/namespaces/general_utilities/analysis_math/poly_solver.h
        framework/namespaces/general_utilities/analysis_math/poly_solver.cpp
        framework/namespaces/fitters/fitter_functions/fitter_functions.h
        framework/namespaces/setup/path_definitions/path_definitions.h
        framework/namespaces/settings/debugging_settings/debugging_settings.h
        framework/namespaces/setup/debugging/debugging.h
        framework/namespaces/setup/code_setup/code_setup.h
        framework/includes/work_dir_include.h
        framework/structures/CodeDirectories/CodeDirectories.h
)

## Link against ROOT and CLAS12ROOT libraries
#target_link_libraries(project ${ROOT_LIBRARIES})

target_link_libraries(project ${ROOT_LIBRARIES})
# target_link_libraries(project ${ROOT_LIBRARIES} ${CLAS12ROOT_LIBRARY} ${CLAS12BANKS_LIBRARY} ${HIPO4_LIBRARY})
