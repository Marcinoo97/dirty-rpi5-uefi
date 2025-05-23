/** @file
  RISC-V CPU library definitions.

  Copyright (c) 2016 - 2022, Hewlett Packard Enterprise Development LP. All rights reserved.<BR>
  Copyright (c) 2023, Intel Corporation. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef RISCV_CPU_LIB_H_
#define RISCV_CPU_LIB_H_

#include "RiscVImpl.h"

/**
  RISCV_TRAP_HANDLER
**/
typedef
VOID
(EFIAPI *RISCV_TRAP_HANDLER)(
  VOID
  );

VOID
RiscVSetMachineScratch (
  RISCV_MACHINE_MODE_CONTEXT  *RiscvContext
  );

UINT32
RiscVGetMachineScratch (
  VOID
  );

UINT32
RiscVGetMachineTrapCause (
  VOID
  );

UINT64
RiscVReadMachineTimer (
  VOID
  );

UINT64
RiscVReadMachineTimerInterface (
  VOID
  );

VOID
  RiscVSetMachineTimerCmp (UINT64);

UINT64
RiscVReadMachineTimerCmp (
  VOID
  );

UINT64
RiscVReadMachineInterruptEnable (
  VOID
  );

UINT64
RiscVReadMachineInterruptPending (
  VOID
  );

UINT64
RiscVReadMachineStatus (
  VOID
  );

VOID
  RiscVWriteMachineStatus (UINT64);

UINT64
RiscVReadMachineTrapVector (
  VOID
  );

UINT64
RiscVReadMachineIsa (
  VOID
  );

UINT64
RiscVReadMachineVendorId (
  VOID
  );

UINT64
RiscVReadMachineArchitectureId (
  VOID
  );

UINT64
RiscVReadMachineImplementId (
  VOID
  );

#endif
