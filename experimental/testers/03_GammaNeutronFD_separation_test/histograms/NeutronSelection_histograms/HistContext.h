#pragma once

// Put this somewhere shared (e.g. header or top of your .cpp)
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "TH1D.h"
#include "TH2D.h"
#include "TObject.h"

struct HistContext {
    std::string selection;  // "ECALveto", "matched", ...
    std::string selectionDescription;
    std::string nTag;  // "nFD" or "LnFD"
    bool selectionFirstPrint;

    // =======================
    // Constructors
    // =======================
    HistContext(const std::string& sel, const std::string& selDesc, const std::string& n, bool firstPrint)
        : selection(sel), selectionDescription(selDesc), nTag(n), selectionFirstPrint(firstPrint) {}

    HistContext(const std::string& sel, const std::string& selDesc /* , const std::string& n */, bool firstPrint)
        : selection(sel), selectionDescription(selDesc), nTag(""), selectionFirstPrint(firstPrint) {}

    HistContext(const std::string& sel, const std::string& selDesc, const std::string& n /* , bool firstPrint */)
        : selection(sel), selectionDescription(selDesc), nTag(n), selectionFirstPrint(false) {}
};
