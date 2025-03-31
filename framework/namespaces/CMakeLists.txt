find_package(ROOT REQUIRED)  # Ensure ROOT is found

set(NAMESPACE_SOURCES 
    fitters/fitter_functions.h
    general_utilities/analysis_math/analysis_math.cpp
    general_utilities/analysis_math/polynomial_solver.cpp
    general_utilities/basic_tools.cpp
    general_utilities/constants.cpp
    general_utilities/data_processor.cpp
    general_utilities/histogram_functions/histogram_functions.cpp
    general_utilities/lists.h
    # general_utilities/reco_analysis_functions.cpp
    # general_utilities/truth_analysis_functions.cpp
    # general_utilities/utilities.cpp
    plotters/draw_and_save_functions/draw_and_save_acorr_plots.h
    plotters/draw_and_save_functions/draw_and_save_efficiency_plots.h
    plotters/draw_and_save_functions/draw_and_save_fs_ratio.h
    plotters/PDF_canvas/PDF_canvas.h
    # settings/debugging_settings.cpp
    # settings/general_settings/general_settings.cpp
    settings/PDF_canvas_settings.h
    # setup/analysis_samples/analysis_samples.cpp
    # setup/analysis_samples/data_samples.cpp
    # setup/analysis_samples/uniform_samples.cpp
    setup/code_setup.h
    setup/debugging.h
    setup/path_definitions.h
    # setup/truth_analysis_variables/truth_analysis_variables.cpp
)

# Create a library
add_library(AnalysisLibraries ${NAMESPACE_SOURCES})

# Link ROOT properly (without -lEG)
target_link_libraries(AnalysisLibraries PUBLIC ${ROOT_LIBRARIES})

# Link Clas12 libraries correctly (no -l)
target_link_libraries(AnalysisLibraries PUBLIC Clas12Banks Hipo4 Clas12Root)