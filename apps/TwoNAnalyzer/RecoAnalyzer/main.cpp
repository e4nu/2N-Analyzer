#include "TwoNAnalyzer/RecoAnalyzer/RecoApp.h"

// Keep main() as a thin wrapper so this TU does not pull in definition-heavy
// analysis headers that would duplicate symbols already compiled in core.
int main(int argc, char** argv) { return RunTwoNAnalyzerReco(argc, argv); }
