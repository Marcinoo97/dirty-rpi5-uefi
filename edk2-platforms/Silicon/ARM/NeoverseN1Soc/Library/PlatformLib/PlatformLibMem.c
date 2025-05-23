/** @file

  Copyright (c) 2018 - 2024, ARM Limited. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/ArmPlatformLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesLib.h>
#include <NeoverseN1Soc.h>

// The total number of descriptors, including the final "end-of-table" descriptor.
#define MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS 19

/**
  Returns the Virtual Memory Map of the platform.

  This Virtual Memory Map is used by MemoryInitPei Module to initialize the MMU
  on your platform.

  @param[in]   VirtualMemoryMap  Array of ARM_MEMORY_REGION_DESCRIPTOR describing
                                 a Physical-to-Virtual Memory mapping. This array
                                 must be ended by a zero-filled entry.
**/
VOID
ArmPlatformGetVirtualMemoryMap (
  IN     ARM_MEMORY_REGION_DESCRIPTOR  **VirtualMemoryMap
  )
{
  UINTN                          Index;
  ARM_MEMORY_REGION_DESCRIPTOR   *VirtualMemoryTable;
  EFI_RESOURCE_ATTRIBUTE_TYPE    ResourceAttributes;
  CONST NEOVERSEN1SOC_PLAT_INFO  *PlatInfo;
  UINT64                         DramBlock2Size;
  UINT64                         RemoteDdrSize;
  EFI_STATUS                     Status;

  Index = 0;

  Status = PeiServicesLocatePpi (
             &gArmNeoverseN1SocPlatformInfoDescriptorPpiGuid,
             0,
             NULL,
             (VOID **)&PlatInfo
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "[%a]: failed to locate gArmNeoverseN1SocPlatformInfoDescriptorPpiGuid - %r\n",
      gEfiCallerBaseName,
      Status
      ));
      *VirtualMemoryMap = NULL;
      return;
  }

  DramBlock2Size = ((UINT64)(PlatInfo->LocalDdrSize -
                             NEOVERSEN1SOC_DRAM_BLOCK1_SIZE / SIZE_1GB) *
                            (UINT64)SIZE_1GB);

  ResourceAttributes =
    EFI_RESOURCE_ATTRIBUTE_PRESENT |
    EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
    EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
    EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
    EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE |
    EFI_RESOURCE_ATTRIBUTE_TESTED;

  BuildResourceDescriptorHob (
    EFI_RESOURCE_SYSTEM_MEMORY,
    ResourceAttributes,
    FixedPcdGet64 (PcdDramBlock2Base),
    DramBlock2Size);

  if (PlatInfo->MultichipMode == 1) {
    RemoteDdrSize = ((UINT64)(PlatInfo->RemoteDdrSize - 2) * SIZE_1GB);

    BuildResourceDescriptorHob (
      EFI_RESOURCE_SYSTEM_MEMORY,
      ResourceAttributes,
      FixedPcdGet64 (PcdExtMemorySpace) + FixedPcdGet64 (PcdSystemMemoryBase),
      PcdGet64 (PcdSystemMemorySize)
      );

    BuildResourceDescriptorHob (
      EFI_RESOURCE_SYSTEM_MEMORY,
      ResourceAttributes,
      FixedPcdGet64 (PcdExtMemorySpace) + FixedPcdGet64 (PcdDramBlock2Base),
      RemoteDdrSize
      );
  }

  ASSERT (VirtualMemoryMap != NULL);
  Index = 0;

  VirtualMemoryTable = AllocatePool (sizeof (ARM_MEMORY_REGION_DESCRIPTOR) *
                                     MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS);
  if (VirtualMemoryTable == NULL) {
    return;
  }

  // SubSystem Peripherals - Generic Watchdog
  VirtualMemoryTable[Index].PhysicalBase    = NEOVERSEN1SOC_GENERIC_WDOG_BASE;
  VirtualMemoryTable[Index].VirtualBase     = NEOVERSEN1SOC_GENERIC_WDOG_BASE;
  VirtualMemoryTable[Index].Length          = NEOVERSEN1SOC_GENERIC_WDOG_SZ;
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;

  // SubSystem Peripherals - GIC-600
  VirtualMemoryTable[++Index].PhysicalBase  = NEOVERSEN1SOC_GIC_BASE;
  VirtualMemoryTable[Index].VirtualBase     = NEOVERSEN1SOC_GIC_BASE;
  VirtualMemoryTable[Index].Length          = NEOVERSEN1SOC_GIC_SZ;
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;

  // SubSystem Peripherals - GICR-600
  VirtualMemoryTable[++Index].PhysicalBase  = NEOVERSEN1SOC_GICR_BASE;
  VirtualMemoryTable[Index].VirtualBase     = NEOVERSEN1SOC_GICR_BASE;
  VirtualMemoryTable[Index].Length          = NEOVERSEN1SOC_GICR_SZ;
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;

  // OnChip non-secure SRAM
  VirtualMemoryTable[++Index].PhysicalBase  = NEOVERSEN1SOC_NON_SECURE_SRAM_BASE;
  VirtualMemoryTable[Index].VirtualBase     = NEOVERSEN1SOC_NON_SECURE_SRAM_BASE;
  VirtualMemoryTable[Index].Length          = NEOVERSEN1SOC_NON_SECURE_SRAM_SZ;
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_UNCACHED_UNBUFFERED;

  // PCIe RC Configuration Space
  VirtualMemoryTable[++Index].PhysicalBase  = PcdGet32 (PcdPcieRootPortConfigBaseAddress);
  VirtualMemoryTable[Index].VirtualBase     = PcdGet32 (PcdPcieRootPortConfigBaseAddress);
  VirtualMemoryTable[Index].Length          = PcdGet32 (PcdPcieRootPortConfigBaseSize);
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;

  // PCIe ECAM Configuration Space
  VirtualMemoryTable[++Index].PhysicalBase  = PcdGet64 (PcdPcieExpressBaseAddress);
  VirtualMemoryTable[Index].VirtualBase     = PcdGet64 (PcdPcieExpressBaseAddress);
  VirtualMemoryTable[Index].Length          = (FixedPcdGet32 (PcdPcieBusMax) -
                                               FixedPcdGet32 (PcdPcieBusMin) + 1) *
                                              SIZE_1MB;
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;

  // PCIe MMIO32 Memory Space
  VirtualMemoryTable[++Index].PhysicalBase  = PcdGet32 (PcdPcieMmio32Base);
  VirtualMemoryTable[Index].VirtualBase     = PcdGet32 (PcdPcieMmio32Base);
  VirtualMemoryTable[Index].Length          = PcdGet32 (PcdPcieMmio32Size);
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;

  // PCIe MMIO64 Memory Space
  VirtualMemoryTable[++Index].PhysicalBase  = PcdGet64 (PcdPcieMmio64Base);
  VirtualMemoryTable[Index].VirtualBase     = PcdGet64 (PcdPcieMmio64Base);
  VirtualMemoryTable[Index].Length          = PcdGet64 (PcdPcieMmio64Size);
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;

  // CCIX RC Configuration Space
  VirtualMemoryTable[++Index].PhysicalBase  = PcdGet32 (PcdCcixRootPortConfigBaseAddress);
  VirtualMemoryTable[Index].VirtualBase     = PcdGet32 (PcdCcixRootPortConfigBaseAddress);
  VirtualMemoryTable[Index].Length          = PcdGet32 (PcdCcixRootPortConfigBaseSize);
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;

  // CCIX ECAM Configuration Space
  VirtualMemoryTable[++Index].PhysicalBase  = PcdGet32 (PcdCcixExpressBaseAddress);
  VirtualMemoryTable[Index].VirtualBase     = PcdGet32 (PcdCcixExpressBaseAddress);
  VirtualMemoryTable[Index].Length          = (FixedPcdGet32 (PcdCcixBusMax) -
                                               FixedPcdGet32 (PcdCcixBusMin) + 1) *
                                               SIZE_1MB;
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;

  // CCIX MMIO32 Memory Space
  VirtualMemoryTable[++Index].PhysicalBase  = PcdGet32 (PcdCcixMmio32Base);
  VirtualMemoryTable[Index].VirtualBase     = PcdGet32 (PcdCcixMmio32Base);
  VirtualMemoryTable[Index].Length          = PcdGet32 (PcdCcixMmio32Size);
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;

 // CCIX MMIO64 Memory Space
  VirtualMemoryTable[++Index].PhysicalBase  = PcdGet64 (PcdCcixMmio64Base);
  VirtualMemoryTable[Index].VirtualBase     = PcdGet64 (PcdCcixMmio64Base);
  VirtualMemoryTable[Index].Length          = PcdGet64 (PcdCcixMmio64Size);
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;

  // SubSystem Pheripherals - UART0
  VirtualMemoryTable[++Index].PhysicalBase  = NEOVERSEN1SOC_UART0_BASE;
  VirtualMemoryTable[Index].VirtualBase     = NEOVERSEN1SOC_UART0_BASE;
  VirtualMemoryTable[Index].Length          = NEOVERSEN1SOC_UART0_SZ;
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;

  // DDR Primary (2GB)
  VirtualMemoryTable[++Index].PhysicalBase  = PcdGet64 (PcdSystemMemoryBase);
  VirtualMemoryTable[Index].VirtualBase     = PcdGet64 (PcdSystemMemoryBase);
  VirtualMemoryTable[Index].Length          = PcdGet64 (PcdSystemMemorySize);
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK;

  // DDR Secondary
  VirtualMemoryTable[++Index].PhysicalBase  = PcdGet64 (PcdDramBlock2Base);
  VirtualMemoryTable[Index].VirtualBase     = PcdGet64 (PcdDramBlock2Base);
  VirtualMemoryTable[Index].Length          = DramBlock2Size;
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK;

  // Expansion Peripherals
  VirtualMemoryTable[++Index].PhysicalBase  = NEOVERSEN1SOC_EXP_PERIPH_BASE0;
  VirtualMemoryTable[Index].VirtualBase     = NEOVERSEN1SOC_EXP_PERIPH_BASE0;
  VirtualMemoryTable[Index].Length          = NEOVERSEN1SOC_EXP_PERIPH_BASE0_SZ;
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;

  if (PlatInfo->MultichipMode == 1) {
    //Remote DDR (2GB)
    VirtualMemoryTable[++Index].PhysicalBase  = PcdGet64 (PcdExtMemorySpace) +
                                                PcdGet64 (PcdSystemMemoryBase);
    VirtualMemoryTable[Index].VirtualBase     = PcdGet64 (PcdExtMemorySpace) +
                                                PcdGet64 (PcdSystemMemoryBase);
    VirtualMemoryTable[Index].Length          = PcdGet64 (PcdSystemMemorySize);
    VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_WRITE_THROUGH;

    //Remote DDR
    VirtualMemoryTable[++Index].PhysicalBase  = PcdGet64 (PcdExtMemorySpace) +
                                                PcdGet64 (PcdDramBlock2Base);
    VirtualMemoryTable[Index].VirtualBase     = PcdGet64 (PcdExtMemorySpace) +
                                                PcdGet64 (PcdDramBlock2Base);
    VirtualMemoryTable[Index].Length          = RemoteDdrSize;
    VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_WRITE_THROUGH;
  }

  // End of Table
  VirtualMemoryTable[++Index].PhysicalBase  = 0;
  VirtualMemoryTable[Index].VirtualBase     = 0;
  VirtualMemoryTable[Index].Length          = 0;
  VirtualMemoryTable[Index].Attributes      = (ARM_MEMORY_REGION_ATTRIBUTES)0;

  ASSERT((Index) < MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS);
  DEBUG ((DEBUG_INIT, "Virtual Memory Table setup complete.\n"));

  *VirtualMemoryMap = VirtualMemoryTable;
}
