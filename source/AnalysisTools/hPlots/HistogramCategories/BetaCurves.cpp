//
// Created by Alon Sportes on 29/03/2026.
//

#include "AnalysisTools/hPlots/HistogramCategories/BetaCurves.h"

#include <string>
#include <utility>

#include "libraries/general_utilities/constants.h"

TF1* BetaCurves::MakeBetaCurve(const char* name, const double mass, const double beamEMax) {
    const std::string formula = "x/sqrt(x*x + " + std::to_string(mass * mass) + ")";
    return new TF1(name, formula.c_str(), 0, beamEMax);
}

BetaCurves::BetaCurves(const double beamEMax) {
    beta_neutron = BetaCurves::MakeBetaCurve("beta_neutron", constants::m_n, beamEMax);
    beta_proton = BetaCurves::MakeBetaCurve("beta_proton", constants::m_p, beamEMax);
    beta_Kplus = BetaCurves::MakeBetaCurve("beta_Kplus", constants::m_Kplus, beamEMax);
    beta_Kminus = BetaCurves::MakeBetaCurve("beta_Kminus", constants::m_Kminus, beamEMax);
    beta_Kzero = BetaCurves::MakeBetaCurve("beta_Kzero", constants::m_Kzero, beamEMax);
    beta_piplus = BetaCurves::MakeBetaCurve("beta_piplus", constants::m_piplus, beamEMax);
    beta_piminus = BetaCurves::MakeBetaCurve("beta_piminus", constants::m_piminus, beamEMax);
    beta_pizero = BetaCurves::MakeBetaCurve("beta_pizero", constants::m_pizero, beamEMax);
    beta_electron = BetaCurves::MakeBetaCurve("beta_electron", constants::m_e, beamEMax);
    beta_photon = BetaCurves::MakeBetaCurve("beta_photon", 0., beamEMax);
    beta_deuteron = BetaCurves::MakeBetaCurve("beta_deuteron", constants::m_d, beamEMax);
}

BetaCurves::~BetaCurves() { Reset(); }

BetaCurves::BetaCurves(BetaCurves&& other) noexcept
    : beta_neutron(std::exchange(other.beta_neutron, nullptr)),
      beta_proton(std::exchange(other.beta_proton, nullptr)),
      beta_Kplus(std::exchange(other.beta_Kplus, nullptr)),
      beta_Kminus(std::exchange(other.beta_Kminus, nullptr)),
      beta_Kzero(std::exchange(other.beta_Kzero, nullptr)),
      beta_piplus(std::exchange(other.beta_piplus, nullptr)),
      beta_piminus(std::exchange(other.beta_piminus, nullptr)),
      beta_pizero(std::exchange(other.beta_pizero, nullptr)),
      beta_electron(std::exchange(other.beta_electron, nullptr)),
      beta_photon(std::exchange(other.beta_photon, nullptr)),
      beta_deuteron(std::exchange(other.beta_deuteron, nullptr)) {}

BetaCurves& BetaCurves::operator=(BetaCurves&& other) noexcept {
    if (this != &other) {
        Reset();
        beta_neutron = std::exchange(other.beta_neutron, nullptr);
        beta_proton = std::exchange(other.beta_proton, nullptr);
        beta_Kplus = std::exchange(other.beta_Kplus, nullptr);
        beta_Kminus = std::exchange(other.beta_Kminus, nullptr);
        beta_Kzero = std::exchange(other.beta_Kzero, nullptr);
        beta_piplus = std::exchange(other.beta_piplus, nullptr);
        beta_piminus = std::exchange(other.beta_piminus, nullptr);
        beta_pizero = std::exchange(other.beta_pizero, nullptr);
        beta_electron = std::exchange(other.beta_electron, nullptr);
        beta_photon = std::exchange(other.beta_photon, nullptr);
        beta_deuteron = std::exchange(other.beta_deuteron, nullptr);
    }
    return *this;
}

void BetaCurves::Reset() noexcept {
    delete beta_neutron;
    beta_neutron = nullptr;
    delete beta_proton;
    beta_proton = nullptr;
    delete beta_Kplus;
    beta_Kplus = nullptr;
    delete beta_Kminus;
    beta_Kminus = nullptr;
    delete beta_Kzero;
    beta_Kzero = nullptr;
    delete beta_piplus;
    beta_piplus = nullptr;
    delete beta_piminus;
    beta_piminus = nullptr;
    delete beta_pizero;
    beta_pizero = nullptr;
    delete beta_electron;
    beta_electron = nullptr;
    delete beta_photon;
    beta_photon = nullptr;
    delete beta_deuteron;
    beta_deuteron = nullptr;
}
