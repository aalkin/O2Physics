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

/// \file TestCcdbConsumer2.cxx
///
/// \brief Reader and and provider of the detector geometry for on-the-fly simulation
///
/// \author Nicolò Jacazio <nicolo.jacazio@cern.ch>, Universita del Piemonte Orientale (IT)
///

#include "ALICE3/Core/FastTracker.h"
#include "ALICE3/Core/TrackUtilities.h"
#include "ALICE3/DataModel/OTFLUT.h"
#include <Framework/O2DatabasePDGPlugin.h>
#include "ALICE3/Core/FlatTrackSmearer.h"
#include "ALICE3/Core/FlatLutEntry.h"
#include <CCDB/BasicCCDBManager.h>
#include <Framework/AnalysisTask.h>
#include <Framework/HistogramRegistry.h>
#include <Framework/runDataProcessing.h>
#include <TPDGCode.h>

#include <map>
#include <string>
#include <vector>

struct TestCcdbConsumer2 {
  o2::framework::HistogramRegistry histos{"Histos", {}, o2::framework::OutputObjHandlingPolicy::AnalysisObject};
  o2::framework::ConfigurableAxis axisDeltaPt{"axisDeltaPt", {200, -0.2f, +0.2f}, "#Delta p_{T}"};

  o2::delphes::TrackSmearer smearer;
  o2::framework::Service<o2::framework::O2DatabasePDG> pdgDB;
  const std::unordered_set<int> pdgsToBeHandled = {PDG_t::kElectron, PDG_t::kMuonMinus, PDG_t::kPiPlus, PDG_t::kKPlus, PDG_t::kProton};

  void init(o2::framework::InitContext&)
  {
    histos.add("hDeltaPtEl", "hDeltaPtEl", o2::framework::kTH1D, {axisDeltaPt});
    histos.add("hDeltaPtMu", "hDeltaPtMu", o2::framework::kTH1D, {axisDeltaPt});
    histos.add("hDeltaPtPi", "hDeltaPtPi", o2::framework::kTH1D, {axisDeltaPt});
    histos.add("hDeltaPtKa", "hDeltaPtKa", o2::framework::kTH1D, {axisDeltaPt});
    histos.add("hDeltaPtPr", "hDeltaPtPr", o2::framework::kTH1D, {axisDeltaPt});
    LOG(info) << "Initialization completed";
  }

  void process(o2::aod::McCollisions const& mcCollisions, o2::aod::McParticles const& mcParticles, o2::aod::A3LookUpTables const& luts)
  {
    const auto thisTable = luts.rawIteratorAt(0);
    auto lutEl = thisTable.lutEl();
    auto lutMu = thisTable.lutMu();
    auto lutPi = thisTable.lutPi();
    auto lutKa = thisTable.lutKa();
    auto lutPr = thisTable.lutPr();
    smearer.viewTable(PDG_t::kElectron, reinterpret_cast<const uint8_t*>(lutEl.data()), lutEl.size());
    smearer.viewTable(PDG_t::kMuonMinus, reinterpret_cast<const uint8_t*>(lutMu.data()), lutMu.size());
    smearer.viewTable(PDG_t::kPiPlus, reinterpret_cast<const uint8_t*>(lutPi.data()), lutPi.size());
    smearer.viewTable(PDG_t::kKPlus, reinterpret_cast<const uint8_t*>(lutKa.data()), lutKa.size());
    smearer.viewTable(PDG_t::kProton, reinterpret_cast<const uint8_t*>(lutPr.data()), lutPr.size());

    for (const auto& mcCollision : mcCollisions) {
      for (const auto& mcParticle : mcParticles) {
        if (!pdgsToBeHandled.count(std::abs(mcParticle.pdgCode()))) {
          continue;
        }

        o2::track::TrackParCov trackParCov = o2::upgrade::convertMCParticleToO2Track(mcParticle, pdgDB);
        smearer.smearTrack(trackParCov, std::abs(mcParticle.pdgCode()), mcParticles.size());

        const int pdg = std::abs(mcParticle.pdgCode());
        switch (pdg) {
          case PDG_t::kElectron:
            histos.fill(HIST("hDeltaPtEl"), (trackParCov.getPt() - mcParticle.pt()) / trackParCov.getPt());
            break;
          case PDG_t::kMuonMinus:
            histos.fill(HIST("hDeltaPtMu"), (trackParCov.getPt() - mcParticle.pt()) / trackParCov.getPt());
            break;
          case PDG_t::kPiPlus:
            histos.fill(HIST("hDeltaPtPi"), (trackParCov.getPt() - mcParticle.pt()) / trackParCov.getPt());
            break;
          case PDG_t::kKPlus:
            histos.fill(HIST("hDeltaPtKa"), (trackParCov.getPt() - mcParticle.pt()) / trackParCov.getPt());
            break;
          case PDG_t::kProton:
            histos.fill(HIST("hDeltaPtPr"), (trackParCov.getPt() - mcParticle.pt()) / trackParCov.getPt());
            break;
          default:
            break;
        }
      }
    }
  }
};


o2::framework::WorkflowSpec defineDataProcessing(o2::framework::ConfigContext const& cfgc)
{
  o2::framework::WorkflowSpec spec;
  spec.push_back(adaptAnalysisTask<TestCcdbConsumer2>(cfgc));
  return spec;
}