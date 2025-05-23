/** @file

  Copyright (c) 2017 - 2023, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "DmaProtection.h"

/**
  Create extended context entry.

  @param[in]  VtdIndex  The index of the VTd engine.

  @retval EFI_SUCCESS          The extended context entry is created.
  @retval EFI_OUT_OF_RESOURCE  No enough resource to create extended context entry.
**/
EFI_STATUS
CreateExtContextEntry (
  IN UINTN  VtdIndex
  );

/**
  Allocate zero pages.

  @param[in]  Pages the number of pages.

  @return the page address.
  @retval NULL No resource to allocate pages.
**/
VOID *
EFIAPI
AllocateZeroPages (
  IN UINTN  Pages
  )
{
  VOID *Addr;

  Addr = AllocatePages (Pages);
  if (Addr == NULL) {
    return NULL;
  }
  ZeroMem (Addr, EFI_PAGES_TO_SIZE(Pages));
  return Addr;
}

/**
  Set second level paging entry attribute based upon IoMmuAccess.

  @param[in]  PtEntry      The paging entry.
  @param[in]  IoMmuAccess  The IOMMU access.
**/
VOID
SetSecondLevelPagingEntryAttribute (
  IN VTD_SECOND_LEVEL_PAGING_ENTRY  *PtEntry,
  IN UINT64                         IoMmuAccess
  )
{
  PtEntry->Bits.Read  = ((IoMmuAccess & EDKII_IOMMU_ACCESS_READ) != 0);
  PtEntry->Bits.Write = ((IoMmuAccess & EDKII_IOMMU_ACCESS_WRITE) != 0);
}

/**
  Create context entry.

  @param[in]  VtdIndex  The index of the VTd engine.

  @retval EFI_SUCCESS          The context entry is created.
  @retval EFI_OUT_OF_RESOURCE  No enough resource to create context entry.
**/
EFI_STATUS
CreateContextEntry (
  IN UINTN  VtdIndex
  )
{
  UINTN                  Index;
  VOID                   *Buffer;
  UINTN                  RootPages;
  UINTN                  ContextPages;
  VTD_ROOT_ENTRY         *RootEntry;
  VTD_CONTEXT_ENTRY      *ContextEntryTable;
  VTD_CONTEXT_ENTRY      *ContextEntry;
  VTD_SOURCE_ID          *PciSourceId;
  VTD_SOURCE_ID          SourceId;
  UINTN                  MaxBusNumber;
  UINTN                  EntryTablePages;

  MaxBusNumber = 0;
  for (Index = 0; Index < mVtdUnitInformation[VtdIndex].PciDeviceInfo->PciDeviceDataNumber; Index++) {
    PciSourceId = &mVtdUnitInformation[VtdIndex].PciDeviceInfo->PciDeviceData[Index].PciSourceId;
    if (PciSourceId->Bits.Bus > MaxBusNumber) {
      MaxBusNumber = PciSourceId->Bits.Bus;
    }
  }
  DEBUG ((DEBUG_INFO,"  MaxBusNumber - 0x%x\n", MaxBusNumber));

  RootPages = EFI_SIZE_TO_PAGES (sizeof (VTD_ROOT_ENTRY) * VTD_ROOT_ENTRY_NUMBER);
  ContextPages = EFI_SIZE_TO_PAGES (sizeof (VTD_CONTEXT_ENTRY) * VTD_CONTEXT_ENTRY_NUMBER);
  EntryTablePages = RootPages + ContextPages * (MaxBusNumber + 1);
  Buffer = AllocateZeroPages (EntryTablePages);
  if (Buffer == NULL) {
    DEBUG ((DEBUG_INFO,"Could not Alloc Root Entry Table.. \n"));
    return EFI_OUT_OF_RESOURCES;
  }
  mVtdUnitInformation[VtdIndex].RootEntryTable = (VTD_ROOT_ENTRY *)Buffer;
  Buffer = (UINT8 *)Buffer + EFI_PAGES_TO_SIZE (RootPages);

  for (Index = 0; Index < mVtdUnitInformation[VtdIndex].PciDeviceInfo->PciDeviceDataNumber; Index++) {
    PciSourceId = &mVtdUnitInformation[VtdIndex].PciDeviceInfo->PciDeviceData[Index].PciSourceId;

    SourceId.Bits.Bus = PciSourceId->Bits.Bus;
    SourceId.Bits.Device = PciSourceId->Bits.Device;
    SourceId.Bits.Function = PciSourceId->Bits.Function;

    RootEntry = &mVtdUnitInformation[VtdIndex].RootEntryTable[SourceId.Index.RootIndex];
    if (RootEntry->Bits.Present == 0) {
      RootEntry->Bits.ContextTablePointerLo  = (UINT32) RShiftU64 ((UINT64)(UINTN)Buffer, 12);
      RootEntry->Bits.ContextTablePointerHi  = (UINT32) RShiftU64 ((UINT64)(UINTN)Buffer, 32);
      RootEntry->Bits.Present = 1;
      Buffer = (UINT8 *)Buffer + EFI_PAGES_TO_SIZE (ContextPages);
    }

    ContextEntryTable = (VTD_CONTEXT_ENTRY *)(UINTN)VTD_64BITS_ADDRESS(RootEntry->Bits.ContextTablePointerLo, RootEntry->Bits.ContextTablePointerHi) ;
    ContextEntry = &ContextEntryTable[SourceId.Index.ContextIndex];
    ContextEntry->Bits.TranslationType = 0;
    ContextEntry->Bits.FaultProcessingDisable = 0;
    ContextEntry->Bits.Present = 0;

    DEBUG ((DEBUG_INFO,"Source: S%04x B%02x D%02x F%02x\n", mVtdUnitInformation[VtdIndex].Segment, SourceId.Bits.Bus, SourceId.Bits.Device, SourceId.Bits.Function));

    mVtdUnitInformation[VtdIndex].Is5LevelPaging = FALSE;
    if ((mVtdUnitInformation[VtdIndex].CapReg.Bits.SAGAW & BIT3) != 0) {
      mVtdUnitInformation[VtdIndex].Is5LevelPaging = TRUE;
      if ((mAcpiDmarTable->HostAddressWidth <= 48) &&
          ((mVtdUnitInformation[VtdIndex].CapReg.Bits.SAGAW & BIT2) != 0)) {
        mVtdUnitInformation[VtdIndex].Is5LevelPaging = FALSE;
      }
    } else if ((mVtdUnitInformation[VtdIndex].CapReg.Bits.SAGAW & BIT2) == 0) {
      DEBUG((DEBUG_ERROR, "!!!! Page-table type is not supported on VTD %d !!!!\n", VtdIndex));
      return EFI_UNSUPPORTED;
    }

    if (mVtdUnitInformation[VtdIndex].Is5LevelPaging) {
      ContextEntry->Bits.AddressWidth = 0x3;
      DEBUG((DEBUG_INFO, "Using 5-level page-table on VTD %d\n", VtdIndex));
    } else {
      ContextEntry->Bits.AddressWidth = 0x2;
      DEBUG((DEBUG_INFO, "Using 4-level page-table on VTD %d\n", VtdIndex));
    }
  }

  FlushPageTableMemory (VtdIndex, (UINTN)mVtdUnitInformation[VtdIndex].RootEntryTable, EFI_PAGES_TO_SIZE(EntryTablePages));

  return EFI_SUCCESS;
}

/**
  Create second level paging entry table.

  @param[in]  VtdIndex                    The index of the VTd engine.
  @param[in]  SecondLevelPagingEntry      The second level paging entry.
  @param[in]  MemoryBase                  The base of the memory.
  @param[in]  MemoryLimit                 The limit of the memory.
  @param[in]  IoMmuAccess                 The IOMMU access.
  @param[in]  Is5LevelPaging              If it is the 5 level paging.

  @return The second level paging entry.
**/
VTD_SECOND_LEVEL_PAGING_ENTRY *
CreateSecondLevelPagingEntryTable (
  IN UINTN                         VtdIndex,
  IN VTD_SECOND_LEVEL_PAGING_ENTRY *SecondLevelPagingEntry,
  IN UINT64                        MemoryBase,
  IN UINT64                        MemoryLimit,
  IN UINT64                        IoMmuAccess,
  IN BOOLEAN                       Is5LevelPaging
  )
{
  UINTN                          Index5;
  UINTN                          Index4;
  UINTN                          Index3;
  UINTN                          Index2;
  UINTN                          Lvl5Start;
  UINTN                          Lvl5End;
  UINTN                          Lvl4PagesStart;
  UINTN                          Lvl4PagesEnd;
  UINTN                          Lvl4Start;
  UINTN                          Lvl4End;
  UINTN                          Lvl3Start;
  UINTN                          Lvl3End;
  VTD_SECOND_LEVEL_PAGING_ENTRY  *Lvl5PtEntry;
  VTD_SECOND_LEVEL_PAGING_ENTRY  *Lvl4PtEntry;
  VTD_SECOND_LEVEL_PAGING_ENTRY  *Lvl3PtEntry;
  VTD_SECOND_LEVEL_PAGING_ENTRY  *Lvl2PtEntry;
  UINT64                         BaseAddress;
  UINT64                         EndAddress;

  if (MemoryLimit == 0) {
    return NULL;
  }

  Lvl4PagesStart = 0;
  Lvl4PagesEnd   = 0;
  Lvl4PtEntry    = NULL;
  Lvl5PtEntry    = NULL;

  BaseAddress = ALIGN_VALUE_LOW(MemoryBase, SIZE_2MB);
  EndAddress = ALIGN_VALUE_UP(MemoryLimit, SIZE_2MB);
  DEBUG ((DEBUG_INFO,"CreateSecondLevelPagingEntryTable: BaseAddress - 0x%016lx, EndAddress - 0x%016lx\n", BaseAddress, EndAddress));

  if (SecondLevelPagingEntry == NULL) {
    SecondLevelPagingEntry = AllocateZeroPages (1);
    if (SecondLevelPagingEntry == NULL) {
      DEBUG ((DEBUG_ERROR,"Could not Alloc LVL4 or LVL5 PT. \n"));
      return NULL;
    }
    FlushPageTableMemory (VtdIndex, (UINTN)SecondLevelPagingEntry, EFI_PAGES_TO_SIZE(1));
  }

  //
  // If no access is needed, just create not present entry.
  //
  if (IoMmuAccess == 0) {
    return SecondLevelPagingEntry;
  }

  if (Is5LevelPaging) {
    Lvl5Start = RShiftU64 (BaseAddress, 48) & 0x1FF;
    Lvl5End = RShiftU64 (EndAddress - 1, 48) & 0x1FF;
    DEBUG ((DEBUG_INFO,"  Lvl5Start - 0x%x, Lvl5End - 0x%x\n", Lvl5Start, Lvl5End));

    Lvl4Start = RShiftU64 (BaseAddress, 39) & 0x1FF;
    Lvl4End = RShiftU64 (EndAddress - 1, 39) & 0x1FF;

    Lvl4PagesStart = (Lvl5Start<<9) | Lvl4Start;
    Lvl4PagesEnd = (Lvl5End<<9) | Lvl4End;
    DEBUG ((DEBUG_INFO,"  Lvl4PagesStart - 0x%x, Lvl4PagesEnd - 0x%x\n", Lvl4PagesStart, Lvl4PagesEnd));

    Lvl5PtEntry = (VTD_SECOND_LEVEL_PAGING_ENTRY *)SecondLevelPagingEntry;
  } else {
    Lvl5Start = RShiftU64 (BaseAddress, 48) & 0x1FF;
    Lvl5End = Lvl5Start;

    Lvl4Start = RShiftU64 (BaseAddress, 39) & 0x1FF;
    Lvl4End = RShiftU64 (EndAddress - 1, 39) & 0x1FF;
    DEBUG ((DEBUG_INFO,"  Lvl4Start - 0x%x, Lvl4End - 0x%x\n", Lvl4Start, Lvl4End));

    Lvl4PtEntry = (VTD_SECOND_LEVEL_PAGING_ENTRY *)SecondLevelPagingEntry;
  }

  for (Index5 = Lvl5Start; Index5 <= Lvl5End; Index5++) {
    if (Is5LevelPaging) {
      if (Lvl5PtEntry[Index5].Uint64 == 0) {
        Lvl5PtEntry[Index5].Uint64 = (UINT64)(UINTN)AllocateZeroPages (1);
        if (Lvl5PtEntry[Index5].Uint64 == 0) {
          DEBUG ((DEBUG_ERROR,"!!!!!! ALLOCATE LVL4 PAGE FAIL (0x%x)!!!!!!\n", Index5));
          ASSERT(FALSE);
          return NULL;
        }
        FlushPageTableMemory (VtdIndex, (UINTN)Lvl5PtEntry[Index5].Uint64, SIZE_4KB);
        SetSecondLevelPagingEntryAttribute (&Lvl5PtEntry[Index5], EDKII_IOMMU_ACCESS_READ | EDKII_IOMMU_ACCESS_WRITE);
      }
      Lvl4Start = Lvl4PagesStart & 0x1FF;
      if (((Index5+1)<<9) > Lvl4PagesEnd) {
        Lvl4End = SIZE_4KB/sizeof(VTD_SECOND_LEVEL_PAGING_ENTRY) - 1;;
        Lvl4PagesStart = (Index5+1)<<9;
      } else {
        Lvl4End = Lvl4PagesEnd & 0x1FF;
      }
      DEBUG ((DEBUG_INFO,"  Lvl5(0x%x): Lvl4Start - 0x%x, Lvl4End - 0x%x\n", Index5, Lvl4Start, Lvl4End));
      Lvl4PtEntry = (VTD_SECOND_LEVEL_PAGING_ENTRY *)(UINTN)VTD_64BITS_ADDRESS(Lvl5PtEntry[Index5].Bits.AddressLo, Lvl5PtEntry[Index5].Bits.AddressHi);
    }

    for (Index4 = Lvl4Start; Index4 <= Lvl4End; Index4++) {
      if (Lvl4PtEntry[Index4].Uint64 == 0) {
        Lvl4PtEntry[Index4].Uint64 = (UINT64)(UINTN)AllocateZeroPages (1);
        if (Lvl4PtEntry[Index4].Uint64 == 0) {
          DEBUG ((DEBUG_ERROR,"!!!!!! ALLOCATE LVL4 PAGE FAIL (0x%x)!!!!!!\n", Index4));
          ASSERT(FALSE);
          return NULL;
        }
        FlushPageTableMemory (VtdIndex, (UINTN)Lvl4PtEntry[Index4].Uint64, SIZE_4KB);
        SetSecondLevelPagingEntryAttribute (&Lvl4PtEntry[Index4], EDKII_IOMMU_ACCESS_READ | EDKII_IOMMU_ACCESS_WRITE);
      }

      Lvl3Start = RShiftU64 (BaseAddress, 30) & 0x1FF;
      if (ALIGN_VALUE_LOW(BaseAddress + SIZE_1GB, SIZE_1GB) <= EndAddress) {
        Lvl3End = SIZE_4KB/sizeof(VTD_SECOND_LEVEL_PAGING_ENTRY) - 1;
      } else {
        Lvl3End = RShiftU64 (EndAddress - 1, 30) & 0x1FF;
      }
      DEBUG ((DEBUG_INFO,"  Lvl4(0x%x): Lvl3Start - 0x%x, Lvl3End - 0x%x\n", Index4, Lvl3Start, Lvl3End));

      Lvl3PtEntry = (VTD_SECOND_LEVEL_PAGING_ENTRY *)(UINTN)VTD_64BITS_ADDRESS(Lvl4PtEntry[Index4].Bits.AddressLo, Lvl4PtEntry[Index4].Bits.AddressHi);
      for (Index3 = Lvl3Start; Index3 <= Lvl3End; Index3++) {
        if (Lvl3PtEntry[Index3].Uint64 == 0) {
          Lvl3PtEntry[Index3].Uint64 = (UINT64)(UINTN)AllocateZeroPages (1);
          if (Lvl3PtEntry[Index3].Uint64 == 0) {
            DEBUG ((DEBUG_ERROR,"!!!!!! ALLOCATE LVL3 PAGE FAIL (0x%x, 0x%x)!!!!!!\n", Index4, Index3));
            ASSERT(FALSE);
            return NULL;
          }
          FlushPageTableMemory (VtdIndex, (UINTN)Lvl3PtEntry[Index3].Uint64, SIZE_4KB);
          SetSecondLevelPagingEntryAttribute (&Lvl3PtEntry[Index3], EDKII_IOMMU_ACCESS_READ | EDKII_IOMMU_ACCESS_WRITE);
        }

        Lvl2PtEntry = (VTD_SECOND_LEVEL_PAGING_ENTRY *)(UINTN)VTD_64BITS_ADDRESS(Lvl3PtEntry[Index3].Bits.AddressLo, Lvl3PtEntry[Index3].Bits.AddressHi);
        for (Index2 = 0; Index2 < SIZE_4KB/sizeof(VTD_SECOND_LEVEL_PAGING_ENTRY); Index2++) {
          Lvl2PtEntry[Index2].Uint64 = BaseAddress;
          SetSecondLevelPagingEntryAttribute (&Lvl2PtEntry[Index2], IoMmuAccess);
          Lvl2PtEntry[Index2].Bits.PageSize = 1;
          BaseAddress += SIZE_2MB;
          if (BaseAddress >= MemoryLimit) {
            break;
          }
        }
        FlushPageTableMemory (VtdIndex, (UINTN)Lvl2PtEntry, SIZE_4KB);
        if (BaseAddress >= MemoryLimit) {
          break;
        }
      }
      FlushPageTableMemory (VtdIndex, (UINTN)&Lvl3PtEntry[Lvl3Start], (UINTN)&Lvl3PtEntry[Lvl3End + 1] - (UINTN)&Lvl3PtEntry[Lvl3Start]);
      if (BaseAddress >= MemoryLimit) {
        break;
      }
    }
    FlushPageTableMemory (VtdIndex, (UINTN)&Lvl4PtEntry[Lvl4Start], (UINTN)&Lvl4PtEntry[Lvl4End + 1] - (UINTN)&Lvl4PtEntry[Lvl4Start]);
  }
  FlushPageTableMemory (VtdIndex, (UINTN)&Lvl5PtEntry[Lvl5Start], (UINTN)&Lvl5PtEntry[Lvl5End + 1] - (UINTN)&Lvl5PtEntry[Lvl5Start]);

  return SecondLevelPagingEntry;
}

/**
  Create second level paging entry.

  @param[in]  VtdIndex                    The index of the VTd engine.
  @param[in]  IoMmuAccess                 The IOMMU access.
  @param[in]  Is5LevelPaging              If it is the 5 level paging.

  @return The second level paging entry.
**/
VTD_SECOND_LEVEL_PAGING_ENTRY *
CreateSecondLevelPagingEntry (
  IN UINTN   VtdIndex,
  IN UINT64  IoMmuAccess,
  IN BOOLEAN Is5LevelPaging
  )
{
  VTD_SECOND_LEVEL_PAGING_ENTRY *SecondLevelPagingEntry;

  SecondLevelPagingEntry = NULL;
  SecondLevelPagingEntry = CreateSecondLevelPagingEntryTable (VtdIndex, SecondLevelPagingEntry, 0, mBelow4GMemoryLimit, IoMmuAccess, Is5LevelPaging);
  if (SecondLevelPagingEntry == NULL) {
    return NULL;
  }

  if (mAbove4GMemoryLimit != 0) {
    ASSERT (mAbove4GMemoryLimit > BASE_4GB);
    SecondLevelPagingEntry = CreateSecondLevelPagingEntryTable (VtdIndex, SecondLevelPagingEntry, SIZE_4GB, mAbove4GMemoryLimit, IoMmuAccess, Is5LevelPaging);
    if (SecondLevelPagingEntry == NULL) {
      return NULL;
    }
  }

  return SecondLevelPagingEntry;
}

/**
  Setup VTd translation table.

  @retval EFI_SUCCESS           Setup translation table successfully.
  @retval EFI_OUT_OF_RESOURCE   Setup translation table fail.
**/
EFI_STATUS
SetupTranslationTable (
  VOID
  )
{
  EFI_STATUS      Status;
  UINTN           Index;

  for (Index = 0; Index < mVtdUnitNumber; Index++) {
    DEBUG((DEBUG_INFO, "CreateContextEntry - %d\n", Index));

    if (mVtdUnitInformation[Index].ECapReg.Bits.SMTS) {
      if (mVtdUnitInformation[Index].ECapReg.Bits.DEP_24) {
        DEBUG ((DEBUG_ERROR,"ECapReg.bit24 is not zero\n"));
        ASSERT(FALSE);
        Status = EFI_UNSUPPORTED;
      } else {
        Status = CreateContextEntry (Index);
      }
    } else {
      if (mVtdUnitInformation[Index].ECapReg.Bits.DEP_24) {
        //
        // To compatible with pervious VTd engine
        // It was ECS(Extended Context Support) bit.
        //
        Status = CreateExtContextEntry (Index);
      } else {
        Status = CreateContextEntry (Index);
      }
    }

    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  return EFI_SUCCESS;
}

/**
  Dump DMAR second level paging entry.

  @param[in]  SecondLevelPagingEntry  The second level paging entry.
  @param[in]  Is5LevelPaging          If it is the 5 level paging.
**/
VOID
DumpSecondLevelPagingEntry (
  IN VOID *SecondLevelPagingEntry,
  IN BOOLEAN Is5LevelPaging
  )
{
  UINTN                          Index5;
  UINTN                          Index4;
  UINTN                          Index3;
  UINTN                          Index2;
  UINTN                          Index1;
  UINTN                          Lvl5IndexEnd;
  VTD_SECOND_LEVEL_PAGING_ENTRY  *Lvl5PtEntry;
  VTD_SECOND_LEVEL_PAGING_ENTRY  *Lvl4PtEntry;
  VTD_SECOND_LEVEL_PAGING_ENTRY  *Lvl3PtEntry;
  VTD_SECOND_LEVEL_PAGING_ENTRY  *Lvl2PtEntry;
  VTD_SECOND_LEVEL_PAGING_ENTRY  *Lvl1PtEntry;

  DEBUG ((DEBUG_VERBOSE,"================\n"));
  DEBUG ((DEBUG_VERBOSE,"DMAR Second Level Page Table:\n"));
  DEBUG ((DEBUG_VERBOSE,"SecondLevelPagingEntry Base - 0x%x, Is5LevelPaging - %d\n", SecondLevelPagingEntry, Is5LevelPaging));

  Lvl5IndexEnd = Is5LevelPaging ? SIZE_4KB/sizeof(VTD_SECOND_LEVEL_PAGING_ENTRY) : 1;
  Lvl4PtEntry = (VTD_SECOND_LEVEL_PAGING_ENTRY *)SecondLevelPagingEntry;
  Lvl5PtEntry = (VTD_SECOND_LEVEL_PAGING_ENTRY *)SecondLevelPagingEntry;

  for (Index5 = 0; Index5 < Lvl5IndexEnd; Index5++) {
    if (Is5LevelPaging) {
      if (Lvl5PtEntry[Index5].Uint64 != 0) {
        DEBUG ((DEBUG_VERBOSE,"  Lvl5Pt Entry(0x%03x) - 0x%016lx\n", Index5, Lvl5PtEntry[Index5].Uint64));
      }
      if (Lvl5PtEntry[Index5].Uint64 == 0) {
        continue;
      }
      Lvl4PtEntry = (VTD_SECOND_LEVEL_PAGING_ENTRY *)(UINTN)VTD_64BITS_ADDRESS(Lvl5PtEntry[Index5].Bits.AddressLo, Lvl5PtEntry[Index5].Bits.AddressHi);
    }

    for (Index4 = 0; Index4 < SIZE_4KB/sizeof(VTD_SECOND_LEVEL_PAGING_ENTRY); Index4++) {
      if (Lvl4PtEntry[Index4].Uint64 != 0) {
        DEBUG ((DEBUG_VERBOSE,"  Lvl4Pt Entry(0x%03x) - 0x%016lx\n", Index4, Lvl4PtEntry[Index4].Uint64));
      }
      if (Lvl4PtEntry[Index4].Uint64 == 0) {
        continue;
      }
      Lvl3PtEntry = (VTD_SECOND_LEVEL_PAGING_ENTRY *)(UINTN)VTD_64BITS_ADDRESS(Lvl4PtEntry[Index4].Bits.AddressLo, Lvl4PtEntry[Index4].Bits.AddressHi);
      for (Index3 = 0; Index3 < SIZE_4KB/sizeof(VTD_SECOND_LEVEL_PAGING_ENTRY); Index3++) {
        if (Lvl3PtEntry[Index3].Uint64 != 0) {
          DEBUG ((DEBUG_VERBOSE,"   Lvl3Pt Entry(0x%03x) - 0x%016lx\n", Index3, Lvl3PtEntry[Index3].Uint64));
        }
        if (Lvl3PtEntry[Index3].Uint64 == 0) {
          continue;
        }

        Lvl2PtEntry = (VTD_SECOND_LEVEL_PAGING_ENTRY *)(UINTN)VTD_64BITS_ADDRESS(Lvl3PtEntry[Index3].Bits.AddressLo, Lvl3PtEntry[Index3].Bits.AddressHi);
        for (Index2 = 0; Index2 < SIZE_4KB/sizeof(VTD_SECOND_LEVEL_PAGING_ENTRY); Index2++) {
          if (Lvl2PtEntry[Index2].Uint64 != 0) {
            DEBUG ((DEBUG_VERBOSE,"    Lvl2Pt Entry(0x%03x) - 0x%016lx\n", Index2, Lvl2PtEntry[Index2].Uint64));
          }
          if (Lvl2PtEntry[Index2].Uint64 == 0) {
            continue;
          }
          if (Lvl2PtEntry[Index2].Bits.PageSize == 0) {
            Lvl1PtEntry = (VTD_SECOND_LEVEL_PAGING_ENTRY *)(UINTN)VTD_64BITS_ADDRESS(Lvl2PtEntry[Index2].Bits.AddressLo, Lvl2PtEntry[Index2].Bits.AddressHi);
            for (Index1 = 0; Index1 < SIZE_4KB/sizeof(VTD_SECOND_LEVEL_PAGING_ENTRY); Index1++) {
              if (Lvl1PtEntry[Index1].Uint64 != 0) {
                DEBUG ((DEBUG_VERBOSE,"      Lvl1Pt Entry(0x%03x) - 0x%016lx\n", Index1, Lvl1PtEntry[Index1].Uint64));
              }
            }
          }
        }
      }
    }
  }
  DEBUG ((DEBUG_VERBOSE,"================\n"));
}

/**
  Invalid page entry.

  @param VtdIndex  The VTd engine index.
**/
VOID
InvalidatePageEntry (
  IN UINTN                 VtdIndex
  )
{
  if (mVtdUnitInformation[VtdIndex].HasDirtyContext || mVtdUnitInformation[VtdIndex].HasDirtyPages) {
    InvalidateVtdIOTLBGlobal (VtdIndex);
  }
  mVtdUnitInformation[VtdIndex].HasDirtyContext = FALSE;
  mVtdUnitInformation[VtdIndex].HasDirtyPages = FALSE;
}

#define VTD_PG_R                   BIT0
#define VTD_PG_W                   BIT1
#define VTD_PG_X                   BIT2
#define VTD_PG_EMT                 (BIT3 | BIT4 | BIT5)
#define VTD_PG_TM                  (BIT62)

#define VTD_PG_PS                  BIT7

#define PAGE_PROGATE_BITS          (VTD_PG_TM | VTD_PG_EMT | VTD_PG_W | VTD_PG_R)

#define PAGING_4K_MASK  0xFFF
#define PAGING_2M_MASK  0x1FFFFF
#define PAGING_1G_MASK  0x3FFFFFFF

#define PAGING_VTD_INDEX_MASK     0x1FF

#define PAGING_4K_ADDRESS_MASK_64 0x000FFFFFFFFFF000ull
#define PAGING_2M_ADDRESS_MASK_64 0x000FFFFFFFE00000ull
#define PAGING_1G_ADDRESS_MASK_64 0x000FFFFFC0000000ull

typedef enum {
  PageNone,
  Page4K,
  Page2M,
  Page1G,
} PAGE_ATTRIBUTE;

typedef struct {
  PAGE_ATTRIBUTE   Attribute;
  UINT64           Length;
  UINT64           AddressMask;
} PAGE_ATTRIBUTE_TABLE;

PAGE_ATTRIBUTE_TABLE mPageAttributeTable[] = {
  {Page4K,  SIZE_4KB, PAGING_4K_ADDRESS_MASK_64},
  {Page2M,  SIZE_2MB, PAGING_2M_ADDRESS_MASK_64},
  {Page1G,  SIZE_1GB, PAGING_1G_ADDRESS_MASK_64},
};

/**
  Return length according to page attributes.

  @param[in]  PageAttributes   The page attribute of the page entry.

  @return The length of page entry.
**/
UINTN
PageAttributeToLength (
  IN PAGE_ATTRIBUTE  PageAttribute
  )
{
  UINTN  Index;
  for (Index = 0; Index < sizeof(mPageAttributeTable)/sizeof(mPageAttributeTable[0]); Index++) {
    if (PageAttribute == mPageAttributeTable[Index].Attribute) {
      return (UINTN)mPageAttributeTable[Index].Length;
    }
  }
  return 0;
}

/**
  Return page table entry to match the address.

  @param[in]   VtdIndex                 The index used to identify a VTd engine.
  @param[in]   SecondLevelPagingEntry   The second level paging entry in VTd table for the device.
  @param[in]   Address                  The address to be checked.
  @param[in]   Is5LevelPaging           If it is the 5 level paging.
  @param[out]  PageAttributes           The page attribute of the page entry.

  @return The page entry.
**/
VOID *
GetSecondLevelPageTableEntry (
  IN  UINTN                         VtdIndex,
  IN  VTD_SECOND_LEVEL_PAGING_ENTRY *SecondLevelPagingEntry,
  IN  PHYSICAL_ADDRESS              Address,
  IN  BOOLEAN                       Is5LevelPaging,
  OUT PAGE_ATTRIBUTE                *PageAttribute
  )
{
  UINTN                 Index1;
  UINTN                 Index2;
  UINTN                 Index3;
  UINTN                 Index4;
  UINTN                 Index5;
  UINT64                *L1PageTable;
  UINT64                *L2PageTable;
  UINT64                *L3PageTable;
  UINT64                *L4PageTable;
  UINT64                *L5PageTable;

  Index5 = ((UINTN)RShiftU64 (Address, 48)) & PAGING_VTD_INDEX_MASK;
  Index4 = ((UINTN)RShiftU64 (Address, 39)) & PAGING_VTD_INDEX_MASK;
  Index3 = ((UINTN)Address >> 30) & PAGING_VTD_INDEX_MASK;
  Index2 = ((UINTN)Address >> 21) & PAGING_VTD_INDEX_MASK;
  Index1 = ((UINTN)Address >> 12) & PAGING_VTD_INDEX_MASK;

  if (Is5LevelPaging) {
    L5PageTable = (UINT64 *)SecondLevelPagingEntry;
    if (L5PageTable[Index5] == 0) {
      L5PageTable[Index5] = (UINT64)(UINTN)AllocateZeroPages (1);
      if (L5PageTable[Index5] == 0) {
        DEBUG ((DEBUG_ERROR,"!!!!!! ALLOCATE LVL5 PAGE FAIL (0x%x)!!!!!!\n", Index4));
        ASSERT(FALSE);
        *PageAttribute = PageNone;
        return NULL;
      }
      FlushPageTableMemory (VtdIndex, (UINTN)L5PageTable[Index5], SIZE_4KB);
      SetSecondLevelPagingEntryAttribute ((VTD_SECOND_LEVEL_PAGING_ENTRY *)&L5PageTable[Index5], EDKII_IOMMU_ACCESS_READ | EDKII_IOMMU_ACCESS_WRITE);
      FlushPageTableMemory (VtdIndex, (UINTN)&L5PageTable[Index5], sizeof(L5PageTable[Index5]));
    }
    L4PageTable = (UINT64 *)(UINTN)(L5PageTable[Index5] & PAGING_4K_ADDRESS_MASK_64);
  } else {
    L4PageTable = (UINT64 *)SecondLevelPagingEntry;
  }

  if (L4PageTable[Index4] == 0) {
    L4PageTable[Index4] = (UINT64)(UINTN)AllocateZeroPages (1);
    if (L4PageTable[Index4] == 0) {
      DEBUG ((DEBUG_ERROR,"!!!!!! ALLOCATE LVL4 PAGE FAIL (0x%x)!!!!!!\n", Index4));
      ASSERT(FALSE);
      *PageAttribute = PageNone;
      return NULL;
    }
    FlushPageTableMemory (VtdIndex, (UINTN)L4PageTable[Index4], SIZE_4KB);
    SetSecondLevelPagingEntryAttribute ((VTD_SECOND_LEVEL_PAGING_ENTRY *)&L4PageTable[Index4], EDKII_IOMMU_ACCESS_READ | EDKII_IOMMU_ACCESS_WRITE);
    FlushPageTableMemory (VtdIndex, (UINTN)&L4PageTable[Index4], sizeof(L4PageTable[Index4]));
  }

  L3PageTable = (UINT64 *)(UINTN)(L4PageTable[Index4] & PAGING_4K_ADDRESS_MASK_64);
  if (L3PageTable[Index3] == 0) {
    L3PageTable[Index3] = (UINT64)(UINTN)AllocateZeroPages (1);
    if (L3PageTable[Index3] == 0) {
      DEBUG ((DEBUG_ERROR,"!!!!!! ALLOCATE LVL3 PAGE FAIL (0x%x, 0x%x)!!!!!!\n", Index4, Index3));
      ASSERT(FALSE);
      *PageAttribute = PageNone;
      return NULL;
    }
    FlushPageTableMemory (VtdIndex, (UINTN)L3PageTable[Index3], SIZE_4KB);
    SetSecondLevelPagingEntryAttribute ((VTD_SECOND_LEVEL_PAGING_ENTRY *)&L3PageTable[Index3], EDKII_IOMMU_ACCESS_READ | EDKII_IOMMU_ACCESS_WRITE);
    FlushPageTableMemory (VtdIndex, (UINTN)&L3PageTable[Index3], sizeof(L3PageTable[Index3]));
  }
  if ((L3PageTable[Index3] & VTD_PG_PS) != 0) {
    // 1G
    *PageAttribute = Page1G;
    return &L3PageTable[Index3];
  }

  L2PageTable = (UINT64 *)(UINTN)(L3PageTable[Index3] & PAGING_4K_ADDRESS_MASK_64);
  if (L2PageTable[Index2] == 0) {
    L2PageTable[Index2] = Address & PAGING_2M_ADDRESS_MASK_64;
    SetSecondLevelPagingEntryAttribute ((VTD_SECOND_LEVEL_PAGING_ENTRY *)&L2PageTable[Index2], 0);
    L2PageTable[Index2] |= VTD_PG_PS;
    FlushPageTableMemory (VtdIndex, (UINTN)&L2PageTable[Index2], sizeof(L2PageTable[Index2]));
  }
  if ((L2PageTable[Index2] & VTD_PG_PS) != 0) {
    // 2M
    *PageAttribute = Page2M;
    return &L2PageTable[Index2];
  }

  // 4k
  L1PageTable = (UINT64 *)(UINTN)(L2PageTable[Index2] & PAGING_4K_ADDRESS_MASK_64);
  if ((L1PageTable[Index1] == 0) && (Address != 0)) {
    *PageAttribute = PageNone;
    return NULL;
  }
  *PageAttribute = Page4K;
  return &L1PageTable[Index1];
}

/**
  Modify memory attributes of page entry.

  @param[in]   VtdIndex         The index used to identify a VTd engine.
  @param[in]   PageEntry        The page entry.
  @param[in]   IoMmuAccess      The IOMMU access.
  @param[out]  IsModified       TRUE means page table modified. FALSE means page table not modified.
**/
VOID
ConvertSecondLevelPageEntryAttribute (
  IN  UINTN                             VtdIndex,
  IN  VTD_SECOND_LEVEL_PAGING_ENTRY     *PageEntry,
  IN  UINT64                            IoMmuAccess,
  OUT BOOLEAN                           *IsModified
  )
{
  UINT64  CurrentPageEntry;
  UINT64  NewPageEntry;

  CurrentPageEntry = PageEntry->Uint64;
  SetSecondLevelPagingEntryAttribute (PageEntry, IoMmuAccess);
  FlushPageTableMemory (VtdIndex, (UINTN)PageEntry, sizeof(*PageEntry));
  NewPageEntry = PageEntry->Uint64;
  if (CurrentPageEntry != NewPageEntry) {
    *IsModified = TRUE;
    DEBUG ((DEBUG_VERBOSE, "ConvertSecondLevelPageEntryAttribute 0x%lx", CurrentPageEntry));
    DEBUG ((DEBUG_VERBOSE, "->0x%lx\n", NewPageEntry));
  } else {
    *IsModified = FALSE;
  }
}

/**
  This function returns if there is need to split page entry.

  @param[in]  BaseAddress      The base address to be checked.
  @param[in]  Length           The length to be checked.
  @param[in]  PageAttribute    The page attribute of the page entry.

  @retval SplitAttributes on if there is need to split page entry.
**/
PAGE_ATTRIBUTE
NeedSplitPage (
  IN  PHYSICAL_ADDRESS                  BaseAddress,
  IN  UINT64                            Length,
  IN  PAGE_ATTRIBUTE                    PageAttribute
  )
{
  UINT64                PageEntryLength;

  PageEntryLength = PageAttributeToLength (PageAttribute);

  if (((BaseAddress & (PageEntryLength - 1)) == 0) && (Length >= PageEntryLength)) {
    return PageNone;
  }

  if (((BaseAddress & PAGING_2M_MASK) != 0) || (Length < SIZE_2MB)) {
    return Page4K;
  }

  return Page2M;
}

/**
  This function splits one page entry to small page entries.

  @param[in]  VtdIndex         The index used to identify a VTd engine.
  @param[in]  PageEntry        The page entry to be splitted.
  @param[in]  PageAttribute    The page attribute of the page entry.
  @param[in]  SplitAttribute   How to split the page entry.

  @retval RETURN_SUCCESS            The page entry is splitted.
  @retval RETURN_UNSUPPORTED        The page entry does not support to be splitted.
  @retval RETURN_OUT_OF_RESOURCES   No resource to split page entry.
**/
RETURN_STATUS
SplitSecondLevelPage (
  IN  UINTN                             VtdIndex,
  IN  VTD_SECOND_LEVEL_PAGING_ENTRY     *PageEntry,
  IN  PAGE_ATTRIBUTE                    PageAttribute,
  IN  PAGE_ATTRIBUTE                    SplitAttribute
  )
{
  UINT64   BaseAddress;
  UINT64   *NewPageEntry;
  UINTN    Index;

  ASSERT (PageAttribute == Page2M || PageAttribute == Page1G);

  if (PageAttribute == Page2M) {
    //
    // Split 2M to 4K
    //
    ASSERT (SplitAttribute == Page4K);
    if (SplitAttribute == Page4K) {
      NewPageEntry = AllocateZeroPages (1);
      DEBUG ((DEBUG_VERBOSE, "Split - 0x%x\n", NewPageEntry));
      if (NewPageEntry == NULL) {
        return RETURN_OUT_OF_RESOURCES;
      }
      BaseAddress = PageEntry->Uint64 & PAGING_2M_ADDRESS_MASK_64;
      for (Index = 0; Index < SIZE_4KB / sizeof(UINT64); Index++) {
        NewPageEntry[Index] = (BaseAddress + SIZE_4KB * Index) | (PageEntry->Uint64 & PAGE_PROGATE_BITS);
      }
      FlushPageTableMemory (VtdIndex, (UINTN)NewPageEntry, SIZE_4KB);

      PageEntry->Uint64 = (UINT64)(UINTN)NewPageEntry;
      SetSecondLevelPagingEntryAttribute (PageEntry, EDKII_IOMMU_ACCESS_READ | EDKII_IOMMU_ACCESS_WRITE);
      FlushPageTableMemory (VtdIndex, (UINTN)PageEntry, sizeof(*PageEntry));
      return RETURN_SUCCESS;
    } else {
      return RETURN_UNSUPPORTED;
    }
  } else if (PageAttribute == Page1G) {
    //
    // Split 1G to 2M
    // No need support 1G->4K directly, we should use 1G->2M, then 2M->4K to get more compact page table.
    //
    ASSERT (SplitAttribute == Page2M || SplitAttribute == Page4K);
    if ((SplitAttribute == Page2M || SplitAttribute == Page4K)) {
      NewPageEntry = AllocateZeroPages (1);
      DEBUG ((DEBUG_VERBOSE, "Split - 0x%x\n", NewPageEntry));
      if (NewPageEntry == NULL) {
        return RETURN_OUT_OF_RESOURCES;
      }
      BaseAddress = PageEntry->Uint64 & PAGING_1G_ADDRESS_MASK_64;
      for (Index = 0; Index < SIZE_4KB / sizeof(UINT64); Index++) {
        NewPageEntry[Index] = (BaseAddress + SIZE_2MB * Index) | VTD_PG_PS | (PageEntry->Uint64 & PAGE_PROGATE_BITS);
      }
      FlushPageTableMemory (VtdIndex, (UINTN)NewPageEntry, SIZE_4KB);

      PageEntry->Uint64 = (UINT64)(UINTN)NewPageEntry;
      SetSecondLevelPagingEntryAttribute (PageEntry, EDKII_IOMMU_ACCESS_READ | EDKII_IOMMU_ACCESS_WRITE);
      FlushPageTableMemory (VtdIndex, (UINTN)PageEntry, sizeof(*PageEntry));
      return RETURN_SUCCESS;
    } else {
      return RETURN_UNSUPPORTED;
    }
  } else {
    return RETURN_UNSUPPORTED;
  }
}

/**
  Set VTd attribute for a system memory on second level page entry

  @param[in]  VtdIndex                The index used to identify a VTd engine.
  @param[in]  DomainIdentifier        The domain ID of the source.
  @param[in]  SecondLevelPagingEntry  The second level paging entry in VTd table for the device.
  @param[in]  BaseAddress             The base of device memory address to be used as the DMA memory.
  @param[in]  Length                  The length of device memory address to be used as the DMA memory.
  @param[in]  IoMmuAccess             The IOMMU access.

  @retval EFI_SUCCESS            The IoMmuAccess is set for the memory range specified by BaseAddress and Length.
  @retval EFI_INVALID_PARAMETER  BaseAddress is not IoMmu Page size aligned.
  @retval EFI_INVALID_PARAMETER  Length is not IoMmu Page size aligned.
  @retval EFI_INVALID_PARAMETER  Length is 0.
  @retval EFI_INVALID_PARAMETER  IoMmuAccess specified an illegal combination of access.
  @retval EFI_UNSUPPORTED        The bit mask of IoMmuAccess is not supported by the IOMMU.
  @retval EFI_UNSUPPORTED        The IOMMU does not support the memory range specified by BaseAddress and Length.
  @retval EFI_OUT_OF_RESOURCES   There are not enough resources available to modify the IOMMU access.
  @retval EFI_DEVICE_ERROR       The IOMMU device reported an error while attempting the operation.
**/
EFI_STATUS
SetSecondLevelPagingAttribute (
  IN UINTN                         VtdIndex,
  IN UINT16                        DomainIdentifier,
  IN VTD_SECOND_LEVEL_PAGING_ENTRY *SecondLevelPagingEntry,
  IN UINT64                        BaseAddress,
  IN UINT64                        Length,
  IN UINT64                        IoMmuAccess
  )
{
  VTD_SECOND_LEVEL_PAGING_ENTRY  *PageEntry;
  PAGE_ATTRIBUTE                 PageAttribute;
  UINTN                          PageEntryLength;
  PAGE_ATTRIBUTE                 SplitAttribute;
  EFI_STATUS                     Status;
  BOOLEAN                        IsEntryModified;

  DEBUG ((DEBUG_VERBOSE,"SetSecondLevelPagingAttribute (%d) (0x%016lx - 0x%016lx : %x) \n", VtdIndex, BaseAddress, Length, IoMmuAccess));
  DEBUG ((DEBUG_VERBOSE,"  SecondLevelPagingEntry Base - 0x%x\n", SecondLevelPagingEntry));

  if (BaseAddress != ALIGN_VALUE(BaseAddress, SIZE_4KB)) {
    DEBUG ((DEBUG_ERROR, "SetSecondLevelPagingAttribute - Invalid Alignment\n"));
    return EFI_UNSUPPORTED;
  }
  if (Length != ALIGN_VALUE(Length, SIZE_4KB)) {
    DEBUG ((DEBUG_ERROR, "SetSecondLevelPagingAttribute - Invalid Alignment\n"));
    return EFI_UNSUPPORTED;
  }

  while (Length != 0) {
    PageEntry = GetSecondLevelPageTableEntry (VtdIndex, SecondLevelPagingEntry, BaseAddress, mVtdUnitInformation[VtdIndex].Is5LevelPaging, &PageAttribute);
    if (PageEntry == NULL) {
      DEBUG ((DEBUG_ERROR, "PageEntry - NULL\n"));
      return RETURN_UNSUPPORTED;
    }
    PageEntryLength = PageAttributeToLength (PageAttribute);
    SplitAttribute = NeedSplitPage (BaseAddress, Length, PageAttribute);
    if (SplitAttribute == PageNone) {
      ConvertSecondLevelPageEntryAttribute (VtdIndex, PageEntry, IoMmuAccess, &IsEntryModified);
      if (IsEntryModified) {
        mVtdUnitInformation[VtdIndex].HasDirtyPages = TRUE;
      }
      //
      // Convert success, move to next
      //
      BaseAddress += PageEntryLength;
      Length -= PageEntryLength;
    } else {
      Status = SplitSecondLevelPage (VtdIndex, PageEntry, PageAttribute, SplitAttribute);
      if (RETURN_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "SplitSecondLevelPage - %r\n", Status));
        return RETURN_UNSUPPORTED;
      }
      mVtdUnitInformation[VtdIndex].HasDirtyPages = TRUE;
      //
      // Just split current page
      // Convert success in next around
      //
    }
  }

  return EFI_SUCCESS;
}

/**
  Set VTd attribute for a system memory.

  @param[in]  VtdIndex                The index used to identify a VTd engine.
  @param[in]  DomainIdentifier        The domain ID of the source.
  @param[in]  SecondLevelPagingEntry  The second level paging entry in VTd table for the device.
  @param[in]  BaseAddress             The base of device memory address to be used as the DMA memory.
  @param[in]  Length                  The length of device memory address to be used as the DMA memory.
  @param[in]  IoMmuAccess             The IOMMU access.

  @retval EFI_SUCCESS            The IoMmuAccess is set for the memory range specified by BaseAddress and Length.
  @retval EFI_INVALID_PARAMETER  BaseAddress is not IoMmu Page size aligned.
  @retval EFI_INVALID_PARAMETER  Length is not IoMmu Page size aligned.
  @retval EFI_INVALID_PARAMETER  Length is 0.
  @retval EFI_INVALID_PARAMETER  IoMmuAccess specified an illegal combination of access.
  @retval EFI_UNSUPPORTED        The bit mask of IoMmuAccess is not supported by the IOMMU.
  @retval EFI_UNSUPPORTED        The IOMMU does not support the memory range specified by BaseAddress and Length.
  @retval EFI_OUT_OF_RESOURCES   There are not enough resources available to modify the IOMMU access.
  @retval EFI_DEVICE_ERROR       The IOMMU device reported an error while attempting the operation.
**/
EFI_STATUS
SetPageAttribute (
  IN UINTN                         VtdIndex,
  IN UINT16                        DomainIdentifier,
  IN VTD_SECOND_LEVEL_PAGING_ENTRY *SecondLevelPagingEntry,
  IN UINT64                        BaseAddress,
  IN UINT64                        Length,
  IN UINT64                        IoMmuAccess
  )
{
  EFI_STATUS Status;
  Status = EFI_NOT_FOUND;
  if (SecondLevelPagingEntry != NULL) {
    Status = SetSecondLevelPagingAttribute (VtdIndex, DomainIdentifier, SecondLevelPagingEntry, BaseAddress, Length, IoMmuAccess);
  }
  return Status;
}

/**
  Set VTd attribute for a system memory.

  @param[in]  Segment           The Segment used to identify a VTd engine.
  @param[in]  SourceId          The SourceId used to identify a VTd engine and table entry.
  @param[in]  BaseAddress       The base of device memory address to be used as the DMA memory.
  @param[in]  Length            The length of device memory address to be used as the DMA memory.
  @param[in]  IoMmuAccess       The IOMMU access.

  @retval EFI_SUCCESS            The IoMmuAccess is set for the memory range specified by BaseAddress and Length.
  @retval EFI_INVALID_PARAMETER  BaseAddress is not IoMmu Page size aligned.
  @retval EFI_INVALID_PARAMETER  Length is not IoMmu Page size aligned.
  @retval EFI_INVALID_PARAMETER  Length is 0.
  @retval EFI_INVALID_PARAMETER  IoMmuAccess specified an illegal combination of access.
  @retval EFI_UNSUPPORTED        The bit mask of IoMmuAccess is not supported by the IOMMU.
  @retval EFI_UNSUPPORTED        The IOMMU does not support the memory range specified by BaseAddress and Length.
  @retval EFI_OUT_OF_RESOURCES   There are not enough resources available to modify the IOMMU access.
  @retval EFI_DEVICE_ERROR       The IOMMU device reported an error while attempting the operation.
**/
EFI_STATUS
SetAccessAttribute (
  IN UINT16                Segment,
  IN VTD_SOURCE_ID         SourceId,
  IN UINT64                BaseAddress,
  IN UINT64                Length,
  IN UINT64                IoMmuAccess
  )
{
  UINTN                         VtdIndex;
  EFI_STATUS                    Status;
  VTD_EXT_CONTEXT_ENTRY         *ExtContextEntry;
  VTD_CONTEXT_ENTRY             *ContextEntry;
  VTD_SECOND_LEVEL_PAGING_ENTRY *SecondLevelPagingEntry;
  UINT64                        Pt;
  UINTN                         PciDataIndex;
  UINT16                        DomainIdentifier;

  SecondLevelPagingEntry = NULL;

  DEBUG ((DEBUG_VERBOSE,"SetAccessAttribute (S%04x B%02x D%02x F%02x) (0x%016lx - 0x%08x, %x)\n", Segment, SourceId.Bits.Bus, SourceId.Bits.Device, SourceId.Bits.Function, BaseAddress, (UINTN)Length, IoMmuAccess));

  VtdIndex = FindVtdIndexByPciDevice (Segment, SourceId, &ExtContextEntry, &ContextEntry);
  if (VtdIndex == (UINTN)-1) {
    DEBUG ((DEBUG_ERROR,"SetAccessAttribute - Pci device (S%04x B%02x D%02x F%02x) not found!\n", Segment, SourceId.Bits.Bus, SourceId.Bits.Device, SourceId.Bits.Function));
    return EFI_DEVICE_ERROR;
  }

  PciDataIndex = GetPciDataIndex (VtdIndex, Segment, SourceId);
  mVtdUnitInformation[VtdIndex].PciDeviceInfo->PciDeviceData[PciDataIndex].AccessCount++;
  //
  // DomainId should not be 0.
  //
  DomainIdentifier = (UINT16)(PciDataIndex + 1);

  if (ExtContextEntry != NULL) {
    if (ExtContextEntry->Bits.Present == 0) {
      SecondLevelPagingEntry = CreateSecondLevelPagingEntry (VtdIndex, 0, mVtdUnitInformation[VtdIndex].Is5LevelPaging);
      DEBUG ((DEBUG_VERBOSE,"SecondLevelPagingEntry - 0x%x (S%04x B%02x D%02x F%02x) New\n", SecondLevelPagingEntry, Segment, SourceId.Bits.Bus, SourceId.Bits.Device, SourceId.Bits.Function));
      Pt = (UINT64)RShiftU64 ((UINT64)(UINTN)SecondLevelPagingEntry, 12);

      ExtContextEntry->Bits.SecondLevelPageTranslationPointerLo = (UINT32) Pt;
      ExtContextEntry->Bits.SecondLevelPageTranslationPointerHi = (UINT32) RShiftU64(Pt, 20);
      ExtContextEntry->Bits.DomainIdentifier = DomainIdentifier;
      ExtContextEntry->Bits.Present = 1;
      FlushPageTableMemory (VtdIndex, (UINTN)ExtContextEntry, sizeof(*ExtContextEntry));
      VtdLibDumpDmarExtContextEntryTable (NULL, NULL, mVtdUnitInformation[VtdIndex].ExtRootEntryTable, mVtdUnitInformation[VtdIndex].Is5LevelPaging);
      mVtdUnitInformation[VtdIndex].HasDirtyContext = TRUE;
    } else {
      SecondLevelPagingEntry = (VOID *)(UINTN)VTD_64BITS_ADDRESS(ExtContextEntry->Bits.SecondLevelPageTranslationPointerLo, ExtContextEntry->Bits.SecondLevelPageTranslationPointerHi);
      DEBUG ((DEBUG_VERBOSE,"SecondLevelPagingEntry - 0x%x (S%04x B%02x D%02x F%02x)\n", SecondLevelPagingEntry, Segment, SourceId.Bits.Bus, SourceId.Bits.Device, SourceId.Bits.Function));
    }
  } else if (ContextEntry != NULL) {
    if (ContextEntry->Bits.Present == 0) {
      SecondLevelPagingEntry = CreateSecondLevelPagingEntry (VtdIndex, 0, mVtdUnitInformation[VtdIndex].Is5LevelPaging);
      DEBUG ((DEBUG_VERBOSE,"SecondLevelPagingEntry - 0x%x (S%04x B%02x D%02x F%02x) New\n", SecondLevelPagingEntry, Segment, SourceId.Bits.Bus, SourceId.Bits.Device, SourceId.Bits.Function));
      Pt = (UINT64)RShiftU64 ((UINT64)(UINTN)SecondLevelPagingEntry, 12);

      ContextEntry->Bits.SecondLevelPageTranslationPointerLo = (UINT32) Pt;
      ContextEntry->Bits.SecondLevelPageTranslationPointerHi = (UINT32) RShiftU64(Pt, 20);
      ContextEntry->Bits.DomainIdentifier = DomainIdentifier;
      ContextEntry->Bits.Present = 1;
      FlushPageTableMemory (VtdIndex, (UINTN)ContextEntry, sizeof(*ContextEntry));
      VtdLibDumpDmarContextEntryTable (NULL, NULL, mVtdUnitInformation[VtdIndex].RootEntryTable, mVtdUnitInformation[VtdIndex].Is5LevelPaging);
      mVtdUnitInformation[VtdIndex].HasDirtyContext = TRUE;
    } else {
      SecondLevelPagingEntry = (VOID *)(UINTN)VTD_64BITS_ADDRESS(ContextEntry->Bits.SecondLevelPageTranslationPointerLo, ContextEntry->Bits.SecondLevelPageTranslationPointerHi);
      DEBUG ((DEBUG_VERBOSE,"SecondLevelPagingEntry - 0x%x (S%04x B%02x D%02x F%02x)\n", SecondLevelPagingEntry, Segment, SourceId.Bits.Bus, SourceId.Bits.Device, SourceId.Bits.Function));
    }
  }

  //
  // Do not update FixedSecondLevelPagingEntry
  //
  if (SecondLevelPagingEntry != mVtdUnitInformation[VtdIndex].FixedSecondLevelPagingEntry) {
    Status = SetPageAttribute (
               VtdIndex,
               DomainIdentifier,
               SecondLevelPagingEntry,
               BaseAddress,
               Length,
               IoMmuAccess
               );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR,"SetPageAttribute - %r\n", Status));
      return Status;
    }
  }

  InvalidatePageEntry (VtdIndex);

  return EFI_SUCCESS;
}

/**
  Always enable the VTd page attribute for the device.

  @param[in]  Segment           The Segment used to identify a VTd engine.
  @param[in]  SourceId          The SourceId used to identify a VTd engine and table entry.

  @retval EFI_SUCCESS           The VTd entry is updated to always enable all DMA access for the specific device.
**/
EFI_STATUS
AlwaysEnablePageAttribute (
  IN UINT16                  Segment,
  IN VTD_SOURCE_ID           SourceId
  )
{
  UINTN                         VtdIndex;
  VTD_EXT_CONTEXT_ENTRY         *ExtContextEntry;
  VTD_CONTEXT_ENTRY             *ContextEntry;
  VTD_SECOND_LEVEL_PAGING_ENTRY *SecondLevelPagingEntry;
  UINT64                        Pt;

  DEBUG ((DEBUG_INFO,"AlwaysEnablePageAttribute (S%04x B%02x D%02x F%02x)\n", Segment, SourceId.Bits.Bus, SourceId.Bits.Device, SourceId.Bits.Function));

  VtdIndex = FindVtdIndexByPciDevice (Segment, SourceId, &ExtContextEntry, &ContextEntry);
  if (VtdIndex == (UINTN)-1) {
    DEBUG ((DEBUG_ERROR,"AlwaysEnablePageAttribute - Pci device (S%04x B%02x D%02x F%02x) not found!\n", Segment, SourceId.Bits.Bus, SourceId.Bits.Device, SourceId.Bits.Function));
    return EFI_DEVICE_ERROR;
  }

  if (mVtdUnitInformation[VtdIndex].FixedSecondLevelPagingEntry == 0) {
    DEBUG((DEBUG_INFO, "CreateSecondLevelPagingEntry - %d\n", VtdIndex));
    mVtdUnitInformation[VtdIndex].FixedSecondLevelPagingEntry = CreateSecondLevelPagingEntry (VtdIndex, EDKII_IOMMU_ACCESS_READ | EDKII_IOMMU_ACCESS_WRITE, mVtdUnitInformation[VtdIndex].Is5LevelPaging);
  }

  SecondLevelPagingEntry = mVtdUnitInformation[VtdIndex].FixedSecondLevelPagingEntry;
  Pt = (UINT64)RShiftU64 ((UINT64)(UINTN)SecondLevelPagingEntry, 12);
  if (ExtContextEntry != NULL) {
    ExtContextEntry->Bits.SecondLevelPageTranslationPointerLo = (UINT32) Pt;
    ExtContextEntry->Bits.SecondLevelPageTranslationPointerHi = (UINT32) RShiftU64(Pt, 20);
    ExtContextEntry->Bits.DomainIdentifier = ((1 << (UINT8)((UINTN)mVtdUnitInformation[VtdIndex].CapReg.Bits.ND * 2 + 4)) - 1);
    ExtContextEntry->Bits.Present = 1;
    FlushPageTableMemory (VtdIndex, (UINTN)ExtContextEntry, sizeof(*ExtContextEntry));
  } else if (ContextEntry != NULL) {
    ContextEntry->Bits.SecondLevelPageTranslationPointerLo = (UINT32) Pt;
    ContextEntry->Bits.SecondLevelPageTranslationPointerHi = (UINT32) RShiftU64(Pt, 20);
    ContextEntry->Bits.DomainIdentifier = ((1 << (UINT8)((UINTN)mVtdUnitInformation[VtdIndex].CapReg.Bits.ND * 2 + 4)) - 1);
    ContextEntry->Bits.Present = 1;
    FlushPageTableMemory (VtdIndex, (UINTN)ContextEntry, sizeof(*ContextEntry));
  }

  return EFI_SUCCESS;
}
