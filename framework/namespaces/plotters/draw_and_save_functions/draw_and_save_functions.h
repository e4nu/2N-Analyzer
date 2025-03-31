//
// Created by Alon Sportes on 26/03/2025.
//

#ifndef DRAW_AND_SAVE_FUNCTIONS_H
#define DRAW_AND_SAVE_FUNCTIONS_H

#if CMakeBuidIncludes
    #include "draw_and_save_acorr_plots.h"
    #include "draw_and_save_efficiency_plots.h"
    #include "draw_and_save_fs_ratio.h"
#else
    #include "draw_and_save_acorr_plots.cpp"
    #include "draw_and_save_efficiency_plots.cpp"
    #include "draw_and_save_fs_ratio.cpp"
#endif

namespace draw_and_save_functions {
using namespace draw_and_save_acorr_plots;
using namespace draw_and_save_efficiency_plots;
using namespace draw_and_save_fs_ratio;
};  // namespace draw_and_save_functions

#endif  // DRAW_AND_SAVE_FUNCTIONS_H
