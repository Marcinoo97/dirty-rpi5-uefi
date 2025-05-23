/** @file
  Header file for CpuPcieInfoFruLib.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _CPU_PCIE_INFO_FRU_LIB_H_
#define _CPU_PCIE_INFO_FRU_LIB_H_

#include <CpuPcieInfo.h>

#define CPU_PCIE_MAX_ROOT_PORTS       4
#define CPU_PCIE_MAX_CONTROLLERS      3

#define CPU_PCIE_ULT_MAX_ROOT_PORT         3
#define CPU_PCIE_ULX_MAX_ROOT_PORT         1

/**
  Get CPU Maximum Pcie Root Port Number

  @retval PcieMaxRootPort         Pch Maximum Pcie Root Port Number
**/
UINT8
GetMaxCpuPciePortNum (
  VOID
  );

/**
  Get CPU Pcie Root Port Device and Function Number by Root Port physical Number

  @param[in]  RpNumber              Root port physical number. (0-based)
  @param[out] RpDev                 Return corresponding root port device number.
  @param[out] RpFun                 Return corresponding root port function number.

  @retval     EFI_SUCCESS           Root port device and function is retrieved
  @retval     EFI_INVALID_PARAMETER RpNumber is invalid
**/
EFI_STATUS
EFIAPI
GetCpuPcieRpDevFun (
  IN  UINTN   RpNumber,
  OUT UINTN   *RpDev,
  OUT UINTN   *RpFun
  );
#endif // _CPU_PCIE_INFO_FRU_LIB_H_
