/*

 git pull && clas12root -b -q main.c

 * Copy commend (local -> JLab):
 scp -r /home/alon/project/temp/BankHist/Analyser.C -J asportes@ftp.jlab.org:/u/home/asportes/clas12project/

 * Copy commend (JLab -> local):
 scp -r asportes@ftp.jlab.org:/u/home/asportes/Analyser/plots -J /home/alon/project/temp/
 scp -r asportes@ftp.jlab.org:/w/hallb-scshelf2102/clas12/asportes/recon_c12_6gev.hipo -J /home/alon/project/temp/

*/

#ifndef DETECTORSIMULATIONANALYSER_H
#define DETECTORSIMULATIONANALYSER_H

#include <THStack.h>

// #include "../namespaces/setup/code_setup/code_setup.cpp"
// #include "../setup/codeSetup.h"

#if CMakeBuidIncludes
    #include "../namespaces/fitters/fitter_functions/fitter_functions.h"
    #include "../namespaces/general_utilities/utilities/utilities.h"
    #include "../namespaces/plotters/draw_and_save_functions/draw_and_save_functions.h"
    //
    #include "../classes/AMaps/AMaps.h"
    #include "../classes/DEfficiency/DEfficiency.h"
    #include "../classes/DSCuts/DSCuts.h"
    #include "../classes/clas12ana/clas12ana.h"
    #include "../classes/hPlots/hPlot1D.h"
    #include "../classes/hPlots/hPlot2D.h"

    #if !defined(MOMENTUMRESOLUTION_H)
        #include "../classes/MomentumResolution/MomentumResolution.h"
    #endif

    #if !defined(PARTICLEID_H)
        #include "../classes/ParticleID/ParticleID.h"
    #endif

    #include "../classes/Settings/Settings.h"
    #include "../classes/TLCuts/TLCuts.h"
#else
    #include "../namespaces/fitters/fitter_functions/fitter_functions.cpp"
    #include "../namespaces/general_utilities/utilities/utilities.h"
    #include "../namespaces/plotters/draw_and_save_functions/draw_and_save_functions.h"
    //
    #include "../classes/AMaps/AMaps.cpp"
    #include "../classes/DEfficiency/DEfficiency.cpp"
    #include "../classes/DSCuts/DSCuts.h"
    #include "../classes/clas12ana/clas12ana.cpp"
    #include "../classes/hPlots/hPlot1D.cpp"
    #include "../classes/hPlots/hPlot2D.cpp"

    #if !defined(MOMENTUMRESOLUTION_H)
        #include "../classes/MomentumResolution/MomentumResolution.cpp"
    #endif

    #if !defined(PARTICLEID_H)
        #include "../classes/ParticleID/ParticleID.cpp"
    #endif

    #include "../classes/Settings/Settings.cpp"
    #include "../classes/TLCuts/TLCuts.cpp"
#endif

#include "HipoChain.h"
#include "clas12reader.h"

// using namespace std;
using namespace clas12;
using namespace utilities;
using namespace draw_and_save_functions;

void EventAnalyser(const std::string &AnalyseFilePath, const std::string &AnalyseFileSample, const std::string &AnalyseFile);

#endif  // DETECTORSIMULATIONANALYSER_H
