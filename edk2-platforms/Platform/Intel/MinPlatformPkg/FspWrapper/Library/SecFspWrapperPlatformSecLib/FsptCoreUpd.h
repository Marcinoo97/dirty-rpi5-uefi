/** @file

Copyright (c) 2017 - 2024, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __FSPT_CORE_UPD_H__
#define __FSPT_CORE_UPD_H__

#pragma pack(1)

#if FixedPcdGet8 (PcdFsptArchUpdRevision) <= 1
/** Fsp T Core UPD
**/
typedef struct {

/** Offset 0x0020
**/
  UINT32                      MicrocodeRegionBase;

/** Offset 0x0024
**/
  UINT32                      MicrocodeRegionSize;

/** Offset 0x0028
**/
  UINT32                      CodeRegionBase;

/** Offset 0x002C
**/
  UINT32                      CodeRegionSize;

/** Offset 0x0030
**/
  UINT8                       Reserved[16];
} FSPT_CORE_UPD;
#else
/** Fsp T Core UPD
**/
typedef struct {

/** Offset 0x0040
**/
  EFI_PHYSICAL_ADDRESS        MicrocodeRegionBase;

/** Offset 0x0048
**/
  UINT64                      MicrocodeRegionSize;

/** Offset 0x0050
**/
  EFI_PHYSICAL_ADDRESS        CodeRegionBase;

/** Offset 0x0058
**/
  UINT64                      CodeRegionSize;
} FSPT_CORE_UPD;
#endif

#pragma pack()

#endif
