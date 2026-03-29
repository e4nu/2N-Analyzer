#
# Created by Alon Sportes on 09/03/2026.
#

# include/CMakeLists.txt
# -----------------------------------------------------------------------------
# This file currently defines a small helper library from source files that are
# located under the include tree.
# Note: in a conventional C++ project, header-only files live under include/
# while .cpp implementation files usually belong under src/ or source/.
# -----------------------------------------------------------------------------

# Make sure ROOT is available before defining targets that depend on it.
find_package(ROOT REQUIRED)

# List the implementation files used to build the helper library.
# These paths are relative to the include/ directory because this CMake file is
# physically located there.
set(NAMESPACE_SOURCES
    general_utilities/analysis_math/poly_solver.cpp
    general_utilities/Yaml_loader/Yaml_loader.cpp
)

# Build a helper library from the listed implementation files.
add_library(AnalysisLibraries ${NAMESPACE_SOURCES})

# Link the helper library against the ROOT libraries found by CMake.
# ROOT_LIBRARIES is provided by find_package(ROOT).
target_link_libraries(AnalysisLibraries PUBLIC ${ROOT_LIBRARIES})

# Link the helper library against the CLAS12 and HIPO libraries.
# These names rely on the linker search paths already being configured.
target_link_libraries(AnalysisLibraries PUBLIC Clas12Banks Hipo4 Clas12Root)

# Legacy file kept only to avoid accidental use.
# The project now builds implementation files exclusively from source/CMakeLists.txt.
# Do not add this directory with add_subdirectory(include).
# Legacy file kept only to avoid accidental use.
# The project now builds implementation files exclusively from source/CMakeLists.txt.
# Do not add this directory with add_subdirectory(include).