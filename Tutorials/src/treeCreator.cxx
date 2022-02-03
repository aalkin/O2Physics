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
///
/// \brief A task to create a flat tree for ML model training as an input
///        for ML tutorial task
/// \author
/// \since

#include "Framework/runDataProcessing.h"
#include "Framework/AnalysisTask.h"
#include "Common/DataModel/Multiplicity.h"

namespace o2::aod
{
namespace vars
{
DECLARE_SOA_COLUMN(VertexZ, vertexZ, float);
DECLARE_SOA_COLUMN(AveragePt, averagePt, float);
DECLARE_SOA_COLUMN(CentralMult, centralMult, int);
DECLARE_SOA_COLUMN(ForwardMultT0, forwardMultT0, float);
DECLARE_SOA_COLUMN(ForwardMultV0A, forwardMultV0A, float);
} // namespace vars

DECLARE_SOA_TABLE(TrainingTree, "AOD", "TRTR", o2::soa::Index<>,
                  vars::VertexZ,
                  vars::AveragePt,
                  vars::CentralMult,
                  vars::ForwardMultT0,
                  vars::ForwardMultV0A);
} // namespace o2::aod

using namespace o2;
using namespace o2::framework;
using namespace o2::framework::expressions;

struct CreateTree {
  Configurable<float> centralEtaCut{"centralEtaCut", 0.8, "central eta limit"};

  Produces<aod::TrainingTree> tt;
  Filter centralTracks = nabs(aod::track::eta) < centralEtaCut;

  void process(soa::Join<aod::Collisions, aod::Mults>::iterator const& collision, soa::Filtered<aod::Tracks> const& tracks)
  {
    auto apt = 0.;
    auto npt = 0;
    auto cm = 0;
    for (auto& track : tracks) {
      ++cm;
      if (isfinite(track.pt())) {
        ++npt;
        apt += track.pt();
      }
    }
    tt(collision.posZ(), apt / npt, cm, collision.multT0M(), collision.multV0A());
  }
};

WorkflowSpec defineDataProcessing(ConfigContext const& cfgc)
{
  return WorkflowSpec{adaptAnalysisTask<CreateTree>(cfgc)};
}
