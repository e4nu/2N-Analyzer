//
// Created by Alon Sportes on 29/03/2026.
//

#ifndef HISTOGRAMSBYREACTION_H
#define HISTOGRAMSBYREACTION_H

#include <type_traits>
#include <utility>

#include "TH1D.h"
#include "TH2D.h"
#include "THStack.h"

template <typename HistogramT>
struct HistogramsByReaction {
    static_assert(std::is_same_v<HistogramT, TH1D> || std::is_same_v<HistogramT, TH2D>, "HistogramsByReaction supports TH1D and TH2D only.");

    THStack* stack = nullptr;
    HistogramT* all_int = nullptr;
    HistogramT* qe = nullptr;
    HistogramT* mec = nullptr;
    HistogramT* res = nullptr;
    HistogramT* dis = nullptr;

    HistogramsByReaction() = default;
    ~HistogramsByReaction() = default;

    HistogramsByReaction(const HistogramsByReaction&) = delete;
    HistogramsByReaction& operator=(const HistogramsByReaction&) = delete;

    HistogramsByReaction(HistogramsByReaction&& other) noexcept
        : stack(std::exchange(other.stack, nullptr)),
          all_int(std::exchange(other.all_int, nullptr)),
          qe(std::exchange(other.qe, nullptr)),
          mec(std::exchange(other.mec, nullptr)),
          res(std::exchange(other.res, nullptr)),
          dis(std::exchange(other.dis, nullptr)) {}

    HistogramsByReaction& operator=(HistogramsByReaction&& other) noexcept {
        if (this != &other) {
            Reset();
            stack = std::exchange(other.stack, nullptr);
            all_int = std::exchange(other.all_int, nullptr);
            qe = std::exchange(other.qe, nullptr);
            mec = std::exchange(other.mec, nullptr);
            res = std::exchange(other.res, nullptr);
            dis = std::exchange(other.dis, nullptr);
        }
        return *this;
    }

    void SetHistograms(HistogramT* allIntHistogram, HistogramT* qeHistogram, HistogramT* mecHistogram, HistogramT* resHistogram, HistogramT* disHistogram) {
        all_int = allIntHistogram;
        qe = qeHistogram;
        mec = mecHistogram;
        res = resHistogram;
        dis = disHistogram;
    }

    void SetStack(THStack* histogramStack) { stack = histogramStack; }

    void AddHistogramsToStack() {
        if constexpr (std::is_same_v<HistogramT, TH1D>) {
            if (stack == nullptr) { return; }
            if (all_int != nullptr) { stack->Add(all_int); }
            if (qe != nullptr) { stack->Add(qe); }
            if (mec != nullptr) { stack->Add(mec); }
            if (res != nullptr) { stack->Add(res); }
            if (dis != nullptr) { stack->Add(dis); }
        }
    }

    void Reset() noexcept {
        stack = nullptr;
        all_int = nullptr;
        qe = nullptr;
        mec = nullptr;
        res = nullptr;
        dis = nullptr;
    }
};

#endif  // HISTOGRAMSBYREACTION_H
