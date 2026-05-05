//
// Created by Alon Sportes on 29/03/2026.
//

#ifndef BETACURVES_H
#define BETACURVES_H

#include "TF1.h"

struct BetaCurves {
    TF1* beta_neutron = nullptr;
    TF1* beta_proton = nullptr;
    TF1* beta_Kplus = nullptr;
    TF1* beta_Kminus = nullptr;
    TF1* beta_Kzero = nullptr;
    TF1* beta_piplus = nullptr;
    TF1* beta_piminus = nullptr;
    TF1* beta_pizero = nullptr;
    TF1* beta_electron = nullptr;
    TF1* beta_photon = nullptr;
    TF1* beta_deuteron = nullptr;

    BetaCurves() = default;
    explicit BetaCurves(double beamEMax);
    ~BetaCurves();

    BetaCurves(const BetaCurves&) = delete;
    BetaCurves& operator=(const BetaCurves&) = delete;
    BetaCurves(BetaCurves&& other) noexcept;
    BetaCurves& operator=(BetaCurves&& other) noexcept;

   private:
    static TF1* MakeBetaCurve(const char* name, double mass, double beamEMax);
    void Reset() noexcept;
};

#endif  // BETACURVES_H
