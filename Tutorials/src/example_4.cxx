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

#include "Framework/runDataProcessing.h"
#include "Framework/AnalysisTask.h"

using namespace o2;
using namespace o2::framework;
using namespace o2::framework::expressions;

constexpr int each = 10;

struct ExampleOne {
  HistogramRegistry registry{
    "registry",
    {
      {"hpt", " ; p_{T} (GeV/c)", {HistType::kTH1F, {{101, -0.05, 10.05}}}}, //
      {"hptMC", " ; p_{T} (GeV/c)", {HistType::kTH1F, {{101, -0.05, 10.05}}}},
      {"havpt", "; <p_{T}> (GeV/c)", {HistType::kTH1F, {{21, -0.05, 2.05}}}},  //
      {"havptMC", "; <p_{T}> (GeV/c)", {HistType::kTH1F, {{21, -0.05, 2.05}}}} //
    }                                                                          //
  };

  Configurable<float> etaCut{"etaCut", 0.8, "track eta cut"};
  Filter etaFilter = nabs(aod::track::eta) <= etaCut;
  Filter etaFilterMC = nabs(aod::mcparticle::eta) <= etaCut;

  void process(aod::Collision const& collision, soa::Filtered<aod::Tracks> const& tracks)
  {
    auto avpt = 0.f;
    for (auto& track : tracks) {
      if (!isnan(track.pt())) {
        avpt += track.pt();
      }
      registry.fill(HIST("hpt"), track.pt());
    }
    if (tracks.size() > 0) {
      avpt /= (float)tracks.size();
    }
    registry.fill(HIST("havpt"), avpt);
    if (collision.index() % each == 0) {
      LOGP(info, "Collision {} has {} tracks, average pt = {}", collision.index(), tracks.size(), avpt);
    }
  }

  void processMC(aod::McCollision const& mccollision, soa::Filtered<aod::McParticles> const& particles)
  {
    auto avpt = 0.f;
    auto count = 0;
    for (auto& particle : particles) {
      if (particle.isPhysicalPrimary()) {
        count++;
        if (!isnan(particle.pt())) {
          avpt += particle.pt();
        }
        registry.fill(HIST("hptMC"), particle.pt());
      }
    }
    if (count > 0) {
      avpt /= (float)count;
    }
    registry.fill(HIST("havptMC"), avpt);
    if (mccollision.index() % each == 0) {
      LOGP(info, "MC Collision {} has {} primary particles, average pt = {}", mccollision.index(), count, avpt);
    }
  }

  PROCESS_SWITCH(ExampleOne, processMC, "Process MC info", false);
};

WorkflowSpec defineDataProcessing(ConfigContext const& cfgc)
{
  return WorkflowSpec{adaptAnalysisTask<ExampleOne>(cfgc)};
}
