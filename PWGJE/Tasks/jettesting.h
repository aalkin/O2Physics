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
/// \brief Table definitions for reduced data model for jets
///
/// \author Nima Zardoshti <nima.zardoshti@cern.ch>

#ifndef PWGJE_DATAMODEL_JETTESTING_H_
#define PWGJE_DATAMODEL_JETTESTING_H_

#include <cmath>
#include "Framework/AnalysisDataModel.h"
#include "PWGJE/DataModel/EMCALClusters.h"
#include "PWGJE/Core/JetDerivedDataUtilities.h"

namespace o2::aod
{

namespace testing
{

DECLARE_SOA_COLUMN(PosZ, posZ, float);

}

DECLARE_SOA_TABLE(JTests, "AOD", "JTEST",
                  testing::PosZ);

DECLARE_SOA_TABLE(StoredJTests, "AOD1", "JTEST",
                  testing::PosZ,
                  o2::soa::Marker<1>);

namespace testingagain
{
DECLARE_SOA_INDEX_COLUMN_FULL(Prong0, prong0, int, JTests, "_0");
} // namespace testingagain

DECLARE_SOA_TABLE(JTestIndexs, "AOD1", "JTESTINDEX",
                  testingagain::Prong0Id);

DECLARE_SOA_TABLE(StoredJTestIndexs, "AOD", "JTESTINDEX",
                  testingagain::Prong0Id,
                  o2::soa::Marker<1>);

} // namespace o2::aod

#endif // PWGJE_DATAMODEL_JETTESTING_H_
