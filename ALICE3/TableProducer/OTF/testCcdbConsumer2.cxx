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

#include "ALICE3/Core/FlatTrackSmearer.h"
#include "ALICE3/Core/TrackUtilities.h"
#include "ALICE3/DataModel/OTFLUT.h"

#include <CCDB/BasicCCDBManager.h>
#include <Framework/AnalysisTask.h>
#include <Framework/HistogramRegistry.h>
#include <Framework/O2DatabasePDGPlugin.h>
#include <Framework/runDataProcessing.h>

#include <TPDGCode.h>

#include <string>
#include <vector>

struct TestCcdbConsumer2 {
  o2::framework::HistogramRegistry histos{"Histos", {}, o2::framework::OutputObjHandlingPolicy::AnalysisObject};
  o2::framework::ConfigurableAxis axisDeltaPt{"axisDeltaPt", {200, -0.2f, +0.2f}, "#Delta p_{T}"};
  o2::framework::ConfigurableAxis axisPt{"axisPt", {o2::framework::VARIABLE_WIDTH, 0.0f, 0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f, 1.0f, 1.1f, 1.2f, 1.3f, 1.4f, 1.5f, 1.6f, 1.7f, 1.8f, 1.9f, 2.0f, 2.2f, 2.4f, 2.6f, 2.8f, 3.0f, 3.2f, 3.4f, 3.6f, 3.8f, 4.0f, 4.4f, 4.8f, 5.2f, 5.6f, 6.0f, 6.5f, 7.0f, 7.5f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 17.0f, 19.0f, 21.0f, 23.0f, 25.0f, 30.0f, 35.0f, 40.0f, 50.0f}, "#it{p} (GeV/#it{c})"};

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
    histos.add("h2dDeltaPtEl", "h2dhDeltaPtEl", o2::framework::kTH2D, {axisPt, axisDeltaPt});
    histos.add("h2dDeltaPtMu", "h2dhDeltaPtMu", o2::framework::kTH2D, {axisPt, axisDeltaPt});
    histos.add("h2dDeltaPtPi", "h2dhDeltaPtPi", o2::framework::kTH2D, {axisPt, axisDeltaPt});
    histos.add("h2dDeltaPtKa", "h2dhDeltaPtKa", o2::framework::kTH2D, {axisPt, axisDeltaPt});
    histos.add("h2dDeltaPtPr", "h2dhDeltaPtPr", o2::framework::kTH2D, {axisPt, axisDeltaPt});
    auto hBookkeeping = histos.add<TH1>("hBookkeeping", "hBookkeeping", o2::framework::kTH1D, {{2, 0.5f, 2.5f}});
    hBookkeeping->GetXaxis()->SetBinLabel(1, "Entry found");
    hBookkeeping->GetXaxis()->SetBinLabel(2, "Entry not found");
    LOG(info) << "Initialization completed";
  }

  void process(o2::aod::McCollisions const& mcCollisions, o2::aod::McParticles const& mcParticles, o2::aod::A3LookUpTables const& luts)
  {
    const auto thisTable = luts.begin();
    smearer.viewTable(PDG_t::kElectron, thisTable.lutEl());
    smearer.viewTable(PDG_t::kMuonMinus, thisTable.lutMu());
    smearer.viewTable(PDG_t::kPiPlus, thisTable.lutPi());
    smearer.viewTable(PDG_t::kKPlus, thisTable.lutKa());
    smearer.viewTable(PDG_t::kProton, thisTable.lutPr());

    for (const auto& _ : mcCollisions) {
      for (const auto& mcParticle : mcParticles) {
        if (!pdgsToBeHandled.count(std::abs(mcParticle.pdgCode()))) {
          continue;
        }

        o2::track::TrackParCov trackParCov = o2::upgrade::convertMCParticleToO2Track(mcParticle, pdgDB);
        bool success = smearer.smearTrack(trackParCov, std::abs(mcParticle.pdgCode()), mcParticles.size());
        if (!success) {
          histos.fill(HIST("hBookkeeping"), 1.0f);
          continue;
        }
        histos.fill(HIST("hBookkeeping"), 2.0f);

        switch (std::abs(mcParticle.pdgCode())) {
          case PDG_t::kElectron:
            histos.fill(HIST("hDeltaPtEl"), (trackParCov.getPt() - mcParticle.pt()) / trackParCov.getPt());
            histos.fill(HIST("h2dDeltaPtEl"), trackParCov.getPt(), (trackParCov.getPt() - mcParticle.pt()) / trackParCov.getPt());
            break;
          case PDG_t::kMuonMinus:
            histos.fill(HIST("hDeltaPtMu"), (trackParCov.getPt() - mcParticle.pt()) / trackParCov.getPt());
            histos.fill(HIST("h2dDeltaPtMu"), trackParCov.getPt(), (trackParCov.getPt() - mcParticle.pt()) / trackParCov.getPt());
            break;
          case PDG_t::kPiPlus:
            histos.fill(HIST("hDeltaPtPi"), (trackParCov.getPt() - mcParticle.pt()) / trackParCov.getPt());
            histos.fill(HIST("h2dDeltaPtPi"), trackParCov.getPt(), (trackParCov.getPt() - mcParticle.pt()) / trackParCov.getPt());
            break;
          case PDG_t::kKPlus:
            histos.fill(HIST("hDeltaPtKa"), (trackParCov.getPt() - mcParticle.pt()) / trackParCov.getPt());
            histos.fill(HIST("h2dDeltaPtKa"), trackParCov.getPt(), (trackParCov.getPt() - mcParticle.pt()) / trackParCov.getPt());
            break;
          case PDG_t::kProton:
            histos.fill(HIST("hDeltaPtPr"), (trackParCov.getPt() - mcParticle.pt()) / trackParCov.getPt());
            histos.fill(HIST("h2dDeltaPtPr"), trackParCov.getPt(), (trackParCov.getPt() - mcParticle.pt()) / trackParCov.getPt());
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

