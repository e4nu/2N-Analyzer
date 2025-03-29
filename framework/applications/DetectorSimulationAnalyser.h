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

#include "../setup/codeSetup.h"
//
#include "../framework/namespaces/fitters/fitter_functions/fitter_functions.h"
#include "../framework/namespaces/general_utilities/utilities/utilities.h"
#include "../framework/namespaces/plotters/draw_and_save_functions/draw_and_save_functions.h"
//
#include "../framework/classes/AMaps/AMaps.h"
#include "../framework/classes/DEfficiency/DEfficiency.h"
#include "../framework/classes/DSCuts/DSCuts.h"
#include "../framework/classes/clas12ana/clas12ana.h"
#include "../framework/classes/hPlots/hPlot1D.h"
#include "../framework/classes/hPlots/hPlot2D.h"
// #include "../framework/includes/clas12_include.h"

#if !defined(MOMENTUMRESOLUTION_H)
#include "../framework/classes/MomentumResolution/MomentumResolution.h"
#endif

#if !defined(PARTICLEID_H)
#include "../framework/classes/ParticleID/ParticleID.h"
#endif

#include "../framework/classes/Settings/Settings.h"
#include "../framework/classes/TLCuts/TLCuts.h"
// #include "../framework/functions/FillByInt.h"
// #include "../framework/functions/FitFunctions/BetaFit.h"
// #include "../framework/functions/FitFunctions/BetaFitApprax.h"
//
#include "HipoChain.h"
#include "clas12reader.h"

// using namespace std;
using namespace clas12;
using namespace utilities;
using namespace draw_and_save_functions;

void EventAnalyser(const std::string &AnalyseFilePath, const std::string &AnalyseFileSample, const std::string &AnalyseFile);

#endif  // DETECTORSIMULATIONANALYSER_H
