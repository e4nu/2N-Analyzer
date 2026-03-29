#
# Created by Alon Sportes on 09/03/2026.
#

# apps/CMakeLists.txt
# -----------------------------------------------------------------------------
# This file defines the executable targets of the project.
# In this case, it builds the main reconstructed-analysis runner and links it
# against the core analysis library that is defined under source/.
# -----------------------------------------------------------------------------

# Create the executable that will be run from the command line.
# The source file listed here must contain the program entry point: main().
add_executable(TwoNAnalyzerReco
    # Main entry point for the reconstructed-analysis application.
    TwoNAnalyzer/RecoAnalyzer/main.cpp
)

# Link the executable against the core project library.
# This gives the executable access to the full analysis framework code that was
# compiled in source/CMakeLists.txt.
target_link_libraries(TwoNAnalyzerReco
    PRIVATE
    # Internal core library containing the analysis implementation.
    TwoNAnalysisCore
)
