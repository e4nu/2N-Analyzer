find_package(ROOT REQUIRED)  # Ensure ROOT is found

set(NAMESPACE_SOURCES 
    fitters/fitter_functions/fitter_functions.cpp
    general_utilities/analysis_math/analysis_math.cpp
    general_utilities/analysis_math/polynomial_solver.cpp
    general_utilities/basic_tools/basic_tools.cpp
    general_utilities/constants/constants.cpp
    general_utilities/data_processor/data_processor.cpp
    general_utilities/lists/lists.cpp
    # general_utilities/reco_analysis_functions/reco_analysis_functions.cpp
    # general_utilities/truth_analysis_functions/truth_analysis_functions.cpp
    # general_utilities/utilities/utilities.cpp
    plotters/draw_and_save_functions/draw_and_save_acorr_plots.cpp
    plotters/draw_and_save_functions/draw_and_save_efficiency_plots.cpp
    plotters/draw_and_save_functions/draw_and_save_fs_ratio.cpp
    # plotters/PDF_canvas/PDF_canvas.cpp
    # settings/debugging_settings/debugging_settings.cpp
    # settings/general_settings/general_settings.cpp
    settings/PDF_canvas_settings/PDF_canvas_settings.cpp
    # setup/analysis_samples/analysis_samples.cpp
    # setup/analysis_samples/data_samples.cpp
    # setup/analysis_samples/uniform_samples.cpp
    # setup/code_setup/code_setup.cpp
    setup/debugging/debugging.cpp
    setup/path_definitions/path_definitions.cpp
    # setup/truth_analysis_variables/truth_analysis_variables.cpp
)

# Create a library
add_library(AnalysisLibraries ${NAMESPACE_SOURCES})

# Link ROOT properly (without -lEG)
target_link_libraries(AnalysisLibraries PUBLIC ${ROOT_LIBRARIES})

# Link Clas12 libraries correctly (no -l)
target_link_libraries(AnalysisLibraries PUBLIC Clas12Banks Hipo4 Clas12Root)
