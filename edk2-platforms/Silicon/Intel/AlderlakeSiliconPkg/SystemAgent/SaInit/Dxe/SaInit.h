/** @file
  Header file for SA Common Initialization Driver.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _SA_INITIALIZATION_DRIVER_H_
#define _SA_INITIALIZATION_DRIVER_H_

#include <Uefi.h>
#include <IndustryStandard/Acpi.h>
#include <IndustryStandard/Pci.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/UefiLib.h>
#include <Library/IoLib.h>
#include <Library/PciCf8Lib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/S3BootScriptLib.h>
#include <Guid/EventGroup.h>
#include <CpuRegs.h>
#include <Library/CpuPlatformLib.h>
#include <Protocol/SaPolicy.h>

typedef struct {
  UINT64  BaseAddr;
  UINT32  Offset;
  UINT32  AndMask;
  UINT32  OrMask;
} BOOT_SCRIPT_REGISTER_SETTING;

/**
  SystemAgent Initialization Common Function.

  @retval EFI_SUCCESS   - Always.
**/
VOID
SaInitEntryPoint (
  VOID
  );
#endif
