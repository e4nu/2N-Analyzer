find_package(ROOT REQUIRED)  # Ensure ROOT is found

set(NAMESPACE_SOURCES 
    general_utilities/analysis_math/poly_solver.cpp
    general_utilities/Yaml_loader/Yaml_loader.cpp
)

# Create a library
add_library(AnalysisLibraries ${NAMESPACE_SOURCES})

# Link ROOT properly (without -lEG)
target_link_libraries(AnalysisLibraries PUBLIC ${ROOT_LIBRARIES})

# Link Clas12 libraries correctly (no -l)
target_link_libraries(AnalysisLibraries PUBLIC Clas12Banks Hipo4 Clas12Root)
