/** @file
 Source code for the board SA configuration Pcd init functions in Pre-Memory init phase.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "BoardSaConfigPreMem.h"
#include <Library/CpuPlatformLib.h>
#include <Pins/GpioPinsVer2Lp.h>
#include <PlatformBoardId.h>
#include <PlatformBoardConfig.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Ppi/ReadOnlyVariable2.h>
/**
  MRC configuration init function for PEI pre-memory phase.

  @param[in]  VOID

  @retval VOID
**/
VOID
AdlPSaMiscConfigInit (
  VOID
  )
{
  PcdSet8S (PcdSaMiscUserBd, 6);
  return;
}

/**
  Board Memory Init related configuration init function for PEI pre-memory phase.

  @param[in]  VOID

  @retval VOID
**/
VOID
AdlPMrcConfigInit (
  VOID
  )
{
  UINT16    BoardId;
  BOOLEAN   ExternalSpdPresent;
  MRC_DQS   *MrcDqs;
  MRC_DQ    *MrcDq;
  SPD_DATA  *SpdData;

  BoardId = PcdGet16(PcdBoardId);

  // SPD is the same size for all boards
  PcdSet16S (PcdMrcSpdDataSize, 512);

  ExternalSpdPresent = PcdGetBool (PcdSpdPresent);

  // Assume internal SPD is used
  PcdSet8S (PcdMrcSpdAddressTable0,  0);
  PcdSet8S (PcdMrcSpdAddressTable1,  0);
  PcdSet8S (PcdMrcSpdAddressTable2,  0);
  PcdSet8S (PcdMrcSpdAddressTable3,  0);
  PcdSet8S (PcdMrcSpdAddressTable4,  0);
  PcdSet8S (PcdMrcSpdAddressTable5,  0);
  PcdSet8S (PcdMrcSpdAddressTable6,  0);
  PcdSet8S (PcdMrcSpdAddressTable7,  0);
  PcdSet8S (PcdMrcSpdAddressTable8,  0);
  PcdSet8S (PcdMrcSpdAddressTable9,  0);
  PcdSet8S (PcdMrcSpdAddressTable10, 0);
  PcdSet8S (PcdMrcSpdAddressTable11, 0);
  PcdSet8S (PcdMrcSpdAddressTable12, 0);
  PcdSet8S (PcdMrcSpdAddressTable13, 0);
  PcdSet8S (PcdMrcSpdAddressTable14, 0);
  PcdSet8S (PcdMrcSpdAddressTable15, 0);

  // Check for external SPD presence
  if (ExternalSpdPresent) {
    switch (BoardId) {
      case BoardIdAdlPDdr5Rvp:
        PcdSet8S (PcdMrcSpdAddressTable0,  0xA0);
        PcdSet8S (PcdMrcSpdAddressTable1,  0xA2);
        PcdSet8S (PcdMrcSpdAddressTable8,  0xA4);
        PcdSet8S (PcdMrcSpdAddressTable9,  0xA6);
        break;
      default:
        break;
    }
  }

  // Setting the default DQ Byte Map. It may be overriden to board specific settings below.
  PcdSet32S (PcdMrcDqByteMap, (UINTN) DqByteMapAdlP);
  PcdSet16S (PcdMrcDqByteMapSize, sizeof (DqByteMapAdlP));

  // ADL uses the same RCOMP resistors for all DDR types
  PcdSet32S (PcdMrcRcompResistor, (UINTN) AdlPRcompResistorZero);

  // Use default RCOMP target values for all boards
  PcdSet32S (PcdMrcRcompTarget, (UINTN) RcompTargetAdlP);

  // Default is NIL
  PcdSetBoolS (PcdMrcDqPinsInterleavedControl, TRUE);
  PcdSetBoolS (PcdMrcDqPinsInterleaved, FALSE);

  // DqsMapCpu2Dram is the same size for all boards
  PcdSet16S (PcdMrcDqsMapCpu2DramSize, sizeof (MRC_DQS));
  // DqMapCpu2Dram is the same size for all boards
  PcdSet16S (PcdMrcDqMapCpu2DramSize, sizeof (MRC_DQ));
      PcdSet8S (PcdMrcLp5CccConfig, 0x0);

  // CPU-DRAM DQ mapping
  MrcDq = PcdGetPtr (VpdPcdMrcDqMapCpu2Dram);
  if (MrcDq != NULL) {
    PcdSet32S (PcdMrcDqMapCpu2Dram, (UINTN)MrcDq->DqMapCpu2Dram);
  }

  // CPU-DRAM DQS mapping
  MrcDqs = PcdGetPtr (VpdPcdMrcDqsMapCpu2Dram);
  if (MrcDqs != NULL) {
    PcdSet32S (PcdMrcDqsMapCpu2Dram, (UINTN)MrcDqs->DqsMapCpu2Dram);
  }

  // DRAM SPD Data
  SpdData = PcdGetPtr (VpdPcdMrcSpdData);
  if (SpdData != NULL) {
    if (SpdData->OverrideSpd == TRUE) {
      PcdSet32S (PcdMrcSpdData, (UINTN)SpdData->SpdData);
    }
  }

  return;
}

/**
  SA Display DDI configuration init function for PEI pre-memory phase.

  @param[in]  VOID

  @retval     VOID
**/
VOID
AdlPSaDisplayConfigInit (
  VOID
  )
{
  UINT16    BoardId;

  BoardId   = PcdGet16 (PcdBoardId);

  switch (BoardId) {
    case BoardIdAdlPDdr5Rvp:
        DEBUG ((DEBUG_INFO, "DDI Configuration ADLP Edp DP\n"));
        PcdSet32S (PcdSaDisplayConfigTable, (UINTN) mAdlPDdr5RvpDisplayDdiConfig);
        PcdSet16S (PcdSaDisplayConfigTableSize, sizeof (mAdlPDdr5RvpDisplayDdiConfig));
      break;
    default:
      break;
  }

  return;
}
