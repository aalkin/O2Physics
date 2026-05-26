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

/// \file TestCcdbConsumer0.cxx
///
/// \brief Reader and and provider of the detector geometry for on-the-fly simulation
///
/// \author Nicolò Jacazio <nicolo.jacazio@cern.ch>, Universita del Piemonte Orientale (IT)
///

#include "ALICE3/Core/FlatLutEntry.h"
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

struct TestCcdbConsumer0 {
  o2::framework::HistogramRegistry histos{"Histos", {}, o2::framework::OutputObjHandlingPolicy::AnalysisObject};
  o2::framework::ConfigurableAxis axisDeltaPt{"axisDeltaPt", {200, -0.2f, +0.2f}, "#Delta p_{T}"};
  o2::framework::ConfigurableAxis axisPt{"axisPt", {o2::framework::VARIABLE_WIDTH, 0.0f, 0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f, 1.0f, 1.1f, 1.2f, 1.3f, 1.4f, 1.5f, 1.6f, 1.7f, 1.8f, 1.9f, 2.0f, 2.2f, 2.4f, 2.6f, 2.8f, 3.0f, 3.2f, 3.4f, 3.6f, 3.8f, 4.0f, 4.4f, 4.8f, 5.2f, 5.6f, 6.0f, 6.5f, 7.0f, 7.5f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 17.0f, 19.0f, 21.0f, 23.0f, 25.0f, 30.0f, 35.0f, 40.0f, 50.0f}, "#it{p} (GeV/#it{c})"};

  o2::framework::Service<o2::framework::O2DatabasePDG> pdgDB;
  const std::unordered_set<int> pdgsToBeHandled = {PDG_t::kElectron, PDG_t::kMuonMinus, PDG_t::kPiPlus, PDG_t::kKPlus, PDG_t::kProton};
  // const std::unordered_set<int> pdgsToBeHandled = {PDG_t::kMuonMinus};

  void init(o2::framework::InitContext&)
  {
    histos.add("flat/hDeltaPtEl", "hDeltaPtEl", o2::framework::kTH1D, {axisDeltaPt});
    histos.add("flat/hDeltaPtMu", "hDeltaPtMu", o2::framework::kTH1D, {axisDeltaPt});
    histos.add("flat/hDeltaPtPi", "hDeltaPtPi", o2::framework::kTH1D, {axisDeltaPt});
    histos.add("flat/hDeltaPtKa", "hDeltaPtKa", o2::framework::kTH1D, {axisDeltaPt});
    histos.add("flat/hDeltaPtPr", "hDeltaPtPr", o2::framework::kTH1D, {axisDeltaPt});
    histos.add("flat/h2dDeltaPtEl", "h2dhDeltaPtEl", o2::framework::kTH2D, {axisPt, axisDeltaPt});
    histos.add("flat/h2dDeltaPtMu", "h2dhDeltaPtMu", o2::framework::kTH2D, {axisPt, axisDeltaPt});
    histos.add("flat/h2dDeltaPtPi", "h2dhDeltaPtPi", o2::framework::kTH2D, {axisPt, axisDeltaPt});
    histos.add("flat/h2dDeltaPtKa", "h2dhDeltaPtKa", o2::framework::kTH2D, {axisPt, axisDeltaPt});
    histos.add("flat/h2dDeltaPtPr", "h2dhDeltaPtPr", o2::framework::kTH2D, {axisPt, axisDeltaPt});
    histos.add("full/hDeltaPtEl", "hDeltaPtEl", o2::framework::kTH1D, {axisDeltaPt});
    histos.add("full/hDeltaPtMu", "hDeltaPtMu", o2::framework::kTH1D, {axisDeltaPt});
    histos.add("full/hDeltaPtPi", "hDeltaPtPi", o2::framework::kTH1D, {axisDeltaPt});
    histos.add("full/hDeltaPtKa", "hDeltaPtKa", o2::framework::kTH1D, {axisDeltaPt});
    histos.add("full/hDeltaPtPr", "hDeltaPtPr", o2::framework::kTH1D, {axisDeltaPt});
    histos.add("full/h2dDeltaPtEl", "h2dhDeltaPtEl", o2::framework::kTH2D, {axisPt, axisDeltaPt});
    histos.add("full/h2dDeltaPtMu", "h2dhDeltaPtMu", o2::framework::kTH2D, {axisPt, axisDeltaPt});
    histos.add("full/h2dDeltaPtPi", "h2dhDeltaPtPi", o2::framework::kTH2D, {axisPt, axisDeltaPt});
    histos.add("full/h2dDeltaPtKa", "h2dhDeltaPtKa", o2::framework::kTH2D, {axisPt, axisDeltaPt});
    histos.add("full/h2dDeltaPtPr", "h2dhDeltaPtPr", o2::framework::kTH2D, {axisPt, axisDeltaPt});
    auto hFlatBookkeeping = histos.add<TH1>("flat/hBookkeeping", "hBookkeeping", o2::framework::kTH1D, {{2, 0.5f, 2.5f}});
    hFlatBookkeeping->GetXaxis()->SetBinLabel(1, "Entry found");
    hFlatBookkeeping->GetXaxis()->SetBinLabel(2, "Entry not found");
    auto hFullBookkeeping = histos.add<TH1>("full/hBookkeeping", "hBookkeeping", o2::framework::kTH1D, {{2, 0.5f, 2.5f}});
    hFullBookkeeping->GetXaxis()->SetBinLabel(1, "Entry found");
    hFullBookkeeping->GetXaxis()->SetBinLabel(2, "Entry not found");
    LOG(info) << "Initialization completed";
  }

  void process(o2::aod::McCollisions const& mcCollisions, o2::aod::McParticles const& mcParticles, o2::aod::A3LookUpTables const& luts)
  {
    o2::delphes::TrackSmearer flatSmearer, fullSmearer;
    const auto thisTable = luts.begin();
    flatSmearer.viewTable(PDG_t::kElectron, thisTable.lutEl());
    flatSmearer.viewTable(PDG_t::kMuonMinus, thisTable.lutMu());
    flatSmearer.viewTable(PDG_t::kPiPlus, thisTable.lutPi());
    flatSmearer.viewTable(PDG_t::kKPlus, thisTable.lutKa());
    flatSmearer.viewTable(PDG_t::kProton, thisTable.lutPr());

    std::ifstream fileEl("lutCovm.el.20kG.rmin20.geometry_Petal.dat", std::ios::binary);
    std::ifstream fileMu("lutCovm.mu.20kG.rmin20.geometry_Petal.dat", std::ios::binary);
    std::ifstream filePi("lutCovm.pi.20kG.rmin20.geometry_Petal.dat", std::ios::binary);
    std::ifstream fileKa("lutCovm.ka.20kG.rmin20.geometry_Petal.dat", std::ios::binary);
    std::ifstream filePr("lutCovm.pr.20kG.rmin20.geometry_Petal.dat", std::ios::binary);

    o2::delphes::FlatLutData lutDataEl = o2::delphes::FlatLutData::loadFromFile(fileEl, "lutCovm.el.20kG.rmin20.geometry_Petal.dat");
    auto lutDataElHeader = lutDataEl.getHeader();
    lutDataEl.initialize(lutDataElHeader);
    o2::delphes::FlatLutData lutDataMu = o2::delphes::FlatLutData::loadFromFile(fileMu, "lutCovm.mu.20kG.rmin20.geometry_Petal.dat");
    auto lutDataMuHeader = lutDataMu.getHeader();
    lutDataMu.initialize(lutDataMuHeader);
    o2::delphes::FlatLutData lutDataPi = o2::delphes::FlatLutData::loadFromFile(filePi, "lutCovm.pi.20kG.rmin20.geometry_Petal.dat");
    auto lutDataPiHeader = lutDataPi.getHeader();
    lutDataPi.initialize(lutDataPiHeader);
    o2::delphes::FlatLutData lutDataKa = o2::delphes::FlatLutData::loadFromFile(fileKa, "lutCovm.ka.20kG.rmin20.geometry_Petal.dat");
    auto lutDataKaHeader = lutDataKa.getHeader();
    lutDataKa.initialize(lutDataKaHeader);
    o2::delphes::FlatLutData lutDataPr = o2::delphes::FlatLutData::loadFromFile(filePr, "lutCovm.pr.20kG.rmin20.geometry_Petal.dat");
    auto lutDataPrHeader = lutDataPr.getHeader();
    lutDataPr.initialize(lutDataPrHeader);

    for (const auto& _ : mcCollisions) {
      for (const auto& mcParticle : mcParticles) {
        if (!pdgsToBeHandled.count(std::abs(mcParticle.pdgCode()))) {
          continue;
        }

        if (std::abs(mcParticle.eta()) > 1.) {
          continue;
        }

        o2::track::TrackParCov flatTrack = o2::upgrade::convertMCParticleToO2Track(mcParticle, pdgDB);
        bool success = flatSmearer.smearTrack(flatTrack, std::abs(mcParticle.pdgCode()), mcParticles.size());
        if (!success) {
          histos.fill(HIST("flat/hBookkeeping"), 1.0f);
        } else {
          histos.fill(HIST("flat/hBookkeeping"), 2.0f);

          switch (std::abs(mcParticle.pdgCode())) {
            case PDG_t::kElectron:
              histos.fill(HIST("flat/hDeltaPtEl"), (mcParticle.pt() - flatTrack.getPt()) / flatTrack.getPt());
              histos.fill(HIST("flat/h2dDeltaPtEl"), flatTrack.getPt(), (mcParticle.pt() - flatTrack.getPt()) / flatTrack.getPt());
              break;
            case PDG_t::kMuonMinus:
              histos.fill(HIST("flat/hDeltaPtMu"), (flatTrack.getPt() - mcParticle.pt()) / flatTrack.getPt());
              histos.fill(HIST("flat/h2dDeltaPtMu"), flatTrack.getPt(), (mcParticle.pt() - flatTrack.getPt()) / flatTrack.getPt());
              break;
            case PDG_t::kPiPlus:
              histos.fill(HIST("flat/hDeltaPtPi"), (flatTrack.getPt() - mcParticle.pt()) / flatTrack.getPt());
              histos.fill(HIST("flat/h2dDeltaPtPi"), flatTrack.getPt(), (mcParticle.pt() - flatTrack.getPt()) / flatTrack.getPt());
              break;
            case PDG_t::kKPlus:
              histos.fill(HIST("flat/hDeltaPtKa"), (flatTrack.getPt() - mcParticle.pt()) / flatTrack.getPt());
              histos.fill(HIST("flat/h2dDeltaPtKa"), flatTrack.getPt(), (mcParticle.pt() - flatTrack.getPt()) / flatTrack.getPt());
              break;
            case PDG_t::kProton:
              histos.fill(HIST("flat/hDeltaPtPr"), (flatTrack.getPt() - mcParticle.pt()) / flatTrack.getPt());
              histos.fill(HIST("flat/h2dDeltaPtPr"), flatTrack.getPt(), (mcParticle.pt() - flatTrack.getPt()) / flatTrack.getPt());
              break;

            default:
              break;
          }
        }

        o2::track::TrackParCov fullTrack = o2::upgrade::convertMCParticleToO2Track(mcParticle, pdgDB);
        float interpolatedEff;
        const o2::delphes::lutEntry_t* lutEntry = fullSmearer.getLUTEntry(std::abs(mcParticle.pdgCode()), mcParticles.size(), 0.f, mcParticle.eta(), mcParticle.pt(), interpolatedEff);
        switch (std::abs(mcParticle.pdgCode())) {
          case PDG_t::kElectron:
            success = fullSmearer.smearTrack(fullTrack, lutEntry, mcParticles.size());
            if (success) {
              histos.fill(HIST("full/hDeltaPtEl"), (mcParticle.pt() - fullTrack.getPt()) / fullTrack.getPt());
              histos.fill(HIST("full/h2dDeltaPtEl"), fullTrack.getPt(), (mcParticle.pt() - fullTrack.getPt()) / fullTrack.getPt());
            }
            break;
          case PDG_t::kMuonMinus:
            success = fullSmearer.smearTrack(fullTrack, lutEntry, mcParticles.size());
            histos.fill(HIST("full/hDeltaPtMu"), (fullTrack.getPt() - mcParticle.pt()) / fullTrack.getPt());
            histos.fill(HIST("full/h2dDeltaPtMu"), fullTrack.getPt(), (mcParticle.pt() - fullTrack.getPt()) / fullTrack.getPt());
            break;
          case PDG_t::kPiPlus:
            success = fullSmearer.smearTrack(fullTrack, lutEntry, mcParticles.size());
            if (success) {
              histos.fill(HIST("full/hDeltaPtPi"), (fullTrack.getPt() - mcParticle.pt()) / fullTrack.getPt());
              histos.fill(HIST("full/h2dDeltaPtPi"), fullTrack.getPt(), (mcParticle.pt() - fullTrack.getPt()) / fullTrack.getPt());
            }
            break;
          case PDG_t::kKPlus:
            success = fullSmearer.smearTrack(fullTrack, lutEntry, mcParticles.size());
            if (success) {
              histos.fill(HIST("full/hDeltaPtKa"), (fullTrack.getPt() - mcParticle.pt()) / fullTrack.getPt());
              histos.fill(HIST("full/h2dDeltaPtKa"), fullTrack.getPt(), (mcParticle.pt() - fullTrack.getPt()) / fullTrack.getPt());
            }
            break;
          case PDG_t::kProton:
            success = fullSmearer.smearTrack(fullTrack, lutEntry, mcParticles.size());
            if (success) {
              histos.fill(HIST("full/hDeltaPtPr"), (fullTrack.getPt() - mcParticle.pt()) / fullTrack.getPt());
              histos.fill(HIST("full/h2dDeltaPtPr"), fullTrack.getPt(), (mcParticle.pt() - fullTrack.getPt()) / fullTrack.getPt());
            }
            break;

          default:
            break;
        }

        if (success) {
          histos.fill(HIST("full/hBookkeeping"), 1.0f);
        } else {
          histos.fill(HIST("full/hBookkeeping"), 2.0f);
        }
      }
    }
  }
};

o2::framework::WorkflowSpec defineDataProcessing(o2::framework::ConfigContext const& cfgc)
{
  o2::framework::WorkflowSpec spec;
  spec.push_back(adaptAnalysisTask<TestCcdbConsumer0>(cfgc));
  return spec;
}
