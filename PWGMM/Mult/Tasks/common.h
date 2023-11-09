// Copyright 2019-2020 CERN and copyright holders of ALICE O2.
// See https://alice-o2.web.cern.ch/copyright for details of the copyright holders.
// All rights not expressly granted are reserved.
//
// This software is distributed under the terms of the GNU General Public
// License v3 (GPL Version 3), copied verbatim in the file "COPYING".
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

#ifndef PWGMM_DNDETA_H
#define PWGMM_DNDETA_H
#include <cmath>
#include "Framework/HistogramSpec.h"
#include "Framework/AnalysisDataModel.h"

namespace pwgmm::dndeta {
using namespace o2;
using namespace o2::framework;
//common axis definitions
AxisSpec ZAxis = {301, -30.1, 30.1};          // Z vertex in cm
AxisSpec DeltaZAxis = {61, -6.1, 6.1};        // Z vertex difference in cm
AxisSpec DCAAxis = {601, -3.01, 3.01};        // DCA in cm
AxisSpec EtaAxis = {22, -2.2, 2.2};           // Eta

AxisSpec PhiAxis = {629, 0, 2 * M_PI};        // Phi (azimuthal angle)
AxisSpec PtAxis = {2401, -0.005, 24.005};     // Large fine-binned Pt
// Large wide-binned Pt (for efficiency)
AxisSpec PtAxisEff = {{0.1, 0.12, 0.14, 0.16, 0.18, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6,
                       1.7, 1.8, 1.9, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.4, 3.6, 3.8, 4.0, 4.5, 5.0, 5.5, 6.0, 6.5, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 18.0, 20.0}};
AxisSpec PtAxis_wide = {1041, -0.05, 104.05}; // Smaller wider-binned Pt
AxisSpec FT0CAxis = {1001, -0.5, 1000.5};     // FT0C amplitudes
AxisSpec FT0AAxis = {3001, -0.5, 3000.5};     // FT0A amplitudes
AxisSpec FDDAxis = {3001, -0.5, 3000.5};      // FDD amplitudes

// event selection/efficiency binning
enum struct EvSelBins : int {
  kAll = 1,
  kSelected = 2,
  kSelectedgt0 = 3,
  kSelectedPVgt0 = 4,
  kRejected = 5
};

std::array<std::string_view, static_cast<size_t>(EvSelBins::kRejected)> EvSelBinLabels{
  "All",
  "Selected",
  "Selected INEL>0",
  "Selected INEL>0 (PV)",
  "Rejected"};

enum struct EvEffBins : int {
  kGen = 1,
  kGengt0 = 2,
  kRec = 3,
  kSelected = 4,
  kSelectedgt0 = 5,
  kSelectedPVgt0 = 6
};

std::array<std::string_view, static_cast<size_t>(EvEffBins::kSelectedPVgt0)> EvEffBinLabels{
  "Generated",
  "Generated INEL>0",
  "Reconstructed",
  "Selected",
  "Selected INEL>0",
  "Selected INEL>0 (PV)"};

// helper function to determine if collision/mccollison type contains centrality
namespace
{
template <typename T>
static constexpr bool hasCent()
{
  if constexpr (!soa::is_soa_join_v<T>) {
    return false;
  } else if constexpr (T::template contains<aod::HepMCHeavyIons>()) {
    return true;
  } else {
    return false;
  }
}
} // namespace
}

#endif // PWGMM_DNDETA_H
