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
#include "Common/DataModel/Multiplicity.h"

using namespace o2;
using namespace o2::framework;
using namespace o2::framework::expressions;

using CollisionsWLabels = soa::Join<aod::Collisions, aod::McCollisionLabels>;

namespace o2::aod::idx
{
DECLARE_SOA_INDEX_COLUMN(CollisionsWLabel, collision);
DECLARE_SOA_INDEX_COLUMN(McCollision, mccollision);
} // namespace o2::aod::idx

namespace o2::aod
{
DECLARE_SOA_INDEX_TABLE_USER(MatchedMCRec, McCollisions, "MMCR", idx::McCollisionId, idx::CollisionsWLabelId)
} // namespace o2::aod

struct PreTask {
  Builds<aod::MatchedMCRec> idx;
  void init(InitContext const&){};
};

constexpr int each = 10;

struct ExampleOne {
  HistogramRegistry registry{
    "registry",
    {
      {"hpt", " ; p_{T} (GeV/c)", {HistType::kTH1F, {{101, -0.05, 10.05}}}},                             //
      {"hptMC", " ; p_{T} (GeV/c)", {HistType::kTH1F, {{101, -0.05, 10.05}}}},                           //
      {"havpt", "; <p_{T}> (GeV/c) ; V0M", {HistType::kTH2F, {{21, -0.05, 2.05}, {101, -0.5, 100.5}}}},  //
      {"havptMC", "; <p_{T}> (GeV/c) ; V0M", {HistType::kTH2F, {{21, -0.05, 2.05}, {101, -0.5, 100.5}}}} //
    }                                                                                                    //
  };

  Configurable<float> etaCut{"etaCut", 0.8, "track eta cut"};
  Filter etaFilter = nabs(aod::track::eta) <= etaCut;

  using MyTracks = soa::Filtered<aod::Tracks>;

  void process(soa::Join<aod::Collisions, aod::Mults>::iterator const& collision, MyTracks const& tracks)
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
    registry.fill(HIST("havpt"), avpt, collision.multV0M());
    if (collision.index() % each == 0) {
      LOGP(info, "Collision {} has {} tracks, average pt = {}", collision.index(), tracks.size(), avpt);
    }
  }

  Partition<aod::McParticles> central = nabs(aod::mcparticle::eta) <= etaCut;
  Partition<aod::McParticles> v0m = (aod::mcparticle::eta > 2.7f && aod::mcparticle::eta < 5.1f) || (aod::mcparticle::eta > -3.7f && aod::mcparticle::eta < -1.7f);

  void processMC(soa::Join<aod::McCollisions, aod::MatchedMCRec>::iterator const& matched, aod::McParticles const&)
  {
    if (!matched.has_collision()) {
      return;
    }
    auto avpt = 0.f;
    auto count = 0;
    for (auto& particle : central) {
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
    auto pcount = 0;
    for (auto& particle : v0m) {
      if (particle.isPhysicalPrimary()) {
          pcount++;
      }
    }
    registry.fill(HIST("havptMC"), avpt, pcount);
    if (matched.index() % each == 0) {
      LOGP(info, "MC Collision {} has {} primary particles, average pt = {}", matched.index(), count, avpt);
    }
  }

  PROCESS_SWITCH(ExampleOne, processMC, "Process MC info", false);
};

WorkflowSpec defineDataProcessing(ConfigContext const& cfgc)
{
  return WorkflowSpec{
    adaptAnalysisTask<PreTask>(cfgc),   //
    adaptAnalysisTask<ExampleOne>(cfgc) //
  };
}
