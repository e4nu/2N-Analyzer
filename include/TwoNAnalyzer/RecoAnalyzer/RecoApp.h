//
// Created by Alon Sportes on 09/03/2026.
//

#ifndef RECO_APP_H
#define RECO_APP_H

// App entrypoint implementation lives in core so only one translation unit owns
// setup/utility definitions, preventing duplicate-symbol link errors.
int RunTwoNAnalyzerReco(int argc, char** argv);

#endif  // RECO_APP_H
