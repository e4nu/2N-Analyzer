set(STRUCTURES_SOURCES 
    /w/hallb-scshelf2102/clas12/asportes/2N-Analyser/framework/structures/CodeDirectories/CodeDirectories.cpp
    # /w/hallb-scshelf2102/clas12/asportes/2N-Analyser/framework/structures/DataSample/DataSample.cpp
    # /w/hallb-scshelf2102/clas12/asportes/2N-Analyser/framework/structures/Histogram1D_beta/Histogram1D.cpp
)

# Create a library
add_library(AnalysisDataStructures ${STRUCTURES_SOURCES})

# Link ROOT properly (without -lEG)
target_link_libraries(AnalysisDataStructures PUBLIC ${ROOT_LIBRARIES})

# Link Clas12 libraries correctly (no -l)
target_link_libraries(AnalysisDataStructures PUBLIC Clas12Banks Hipo4 Clas12Root)
