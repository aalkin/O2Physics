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

/// \file onTheFlyDetectorGeometryProvider.cxx
///
/// \brief Reader and and provider of the detector geometry for on-the-fly simulation
///
/// \author Nicolò Jacazio <nicolo.jacazio@cern.ch>, Universita del Piemonte Orientale (IT)
///

#include "ALICE3/Core/FastTracker.h"
#include "ALICE3/DataModel/OTFLUT.h"

#include <CCDB/BasicCCDBManager.h>
#include <Framework/AnalysisTask.h>
#include <Framework/HistogramRegistry.h>
#include <Framework/runDataProcessing.h>

#include <map>
#include <string>
#include <vector>

using namespace o2::framework;

struct OnTheFlyDetectorGeometryProvider {
  HistogramRegistry histos{"Histos", {}, o2::framework::OutputObjHandlingPolicy::AnalysisObject};
  Configurable<bool> cleanLutWhenLoaded{"cleanLutWhenLoaded", true, "clean LUTs after being loaded to save disk space"};
  Configurable<std::vector<std::string>> detectorConfiguration{"detectorConfiguration",
                                                                              std::vector<std::string>{"$O2PHYSICS_ROOT/share/alice3/a3geometry_v3.ini"},
                                                                              "Paths of the detector geometry configuration files"};
  Produces<o2::aod::Timestamps> tableTimestamps;
  Service<o2::ccdb::BasicCCDBManager> ccdb;

  void process(o2::aod::McCollisions const&, o2::aod::McParticles const&)
  {
    tableTimestamps(o2::upgrade::TimestampLUT); // needs to be done for each timeframe - no extra fetches will happen!
  }
};

// #define VERIFY_ALICE3
#ifdef VERIFY_ALICE3
struct OnTheFlyDetectorGeometryUser {
  void init(o2::framework::InitContext& initContext)
  {
    o2::fastsim::GeometryContainer geometryContainer; // Checking that the configuration can be inherited
    geometryContainer.init(initContext);
  }
  void run(o2::framework::ProcessingContext& pc)
  {
    pc.services().get<o2::framework::ControlService>().endOfStream();
    pc.services().get<o2::framework::ControlService>().readyToQuit(o2::framework::QuitRequest::Me);
  }
};
#endif

o2::framework::WorkflowSpec defineDataProcessing(o2::framework::ConfigContext const& cfgc)
{
  o2::framework::WorkflowSpec spec;
  spec.push_back(adaptAnalysisTask<OnTheFlyDetectorGeometryProvider>(cfgc));
#ifdef VERIFY_ALICE3
  spec.push_back(adaptAnalysisTask<OnTheFlyDetectorGeometryUser>(cfgc));
#endif
  return spec;
}