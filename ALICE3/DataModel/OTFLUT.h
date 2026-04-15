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
/// \file   OTFLUT.h
/// \since  23/02/2026
/// \author Jesper & Nicolò
/// \brief  Set of tables for ALICE 3 tracker
///

#ifndef ALICE3_DATAMODEL_OTFLUT_H_
#define ALICE3_DATAMODEL_OTFLUT_H_

#include "Framework/AnalysisDataModel.h"
#include <TClass.h>
#include <TBufferFile.h>

namespace o2
{
namespace aod
{
namespace otf::lut
{

DECLARE_SOA_CCDB_COLUMN(LutEl, lutEl, std::span<std::byte>, "Users/j/jekarlss/ccdbUploadTest3/luts/lutEl"); //!
DECLARE_SOA_CCDB_COLUMN(LutMu, lutMu, std::span<std::byte>, "Users/j/jekarlss/ccdbUploadTest3/luts/lutMu"); //!
DECLARE_SOA_CCDB_COLUMN(LutPi, lutPi, std::span<std::byte>, "Users/j/jekarlss/ccdbUploadTest3/luts/lutPi"); //!
DECLARE_SOA_CCDB_COLUMN(LutKa, lutKa, std::span<std::byte>, "Users/j/jekarlss/ccdbUploadTest3/luts/lutKa"); //!
DECLARE_SOA_CCDB_COLUMN(LutPr, lutPr, std::span<std::byte>, "Users/j/jekarlss/ccdbUploadTest3/luts/lutPr"); //!

} // namespace otf::lut

DECLARE_SOA_TIMESTAMPED_TABLE(A3LookUpTables, aod::Timestamps, o2::aod::timestamp::Timestamp, 1, "LUTs", //!
                              otf::lut::LutEl,
                              otf::lut::LutMu,
                              otf::lut::LutPi,
                              otf::lut::LutKa,
                              otf::lut::LutPr);
} // namespace aod

namespace upgrade
{
static constexpr uint64_t TimestampLUT = 1747442464000;
} // namespace upgrade
} // namespace o2

#endif // ALICE3_DATAMODEL_OTFLUT_H_