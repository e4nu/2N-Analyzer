#include "TChain.h"
#include "TSelector.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TApplication.h"
#include <iostream>

int main() {
    TChain RootChain("chain");

    RootChain.Process("RunCode.cpp+");
}
