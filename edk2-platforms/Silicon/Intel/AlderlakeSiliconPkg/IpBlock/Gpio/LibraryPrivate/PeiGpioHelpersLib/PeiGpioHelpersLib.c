/** @file
  This file contains routines for PEI GPIO Helpers Lib

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include <Library/HobLib.h>
#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/GpioNativeLib.h>
#include <Library/GpioPrivateLib.h>
#include <Library/GpioHelpersLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/ConfigBlockLib.h>
#include <Ppi/SiPolicy.h>

extern EFI_GUID gGpioLibUnlockHobGuid;

//
//  GPIO Lock HOB
//  Stores information on GPIO pads that should be left unlocked
//
typedef struct {
  //
  // GPIO PadConfig unlock data
  //
  UINT32  PadConfig;
  //
  // GPIO Output unlock data
  //
  UINT32  OutputState;
} GPIO_UNLOCK_HOB_DATA;

/**
  This procedure will get index of GPIO Unlock HOB structure for selected GroupIndex and DwNum.

  @param[in]  GroupIndex          GPIO group index
  @param[in]  DwNum               DWORD index for a group.
                                  For group which has less then 32 pads per group DwNum must be 0.

  @retval GpioUnlockHobIndex
**/
STATIC
UINT32
GpioUnlockDataIndex (
  IN UINT32                       GroupIndex,
  IN UINT32                       DwNum
  )
{
  UINT32         GpioUnlockDataIndex;
  UINT32         Index;

  GpioUnlockDataIndex = 0;

  for (Index = 0; Index < GroupIndex; Index++) {
    GpioUnlockDataIndex += GPIO_GET_DW_NUM (GpioGetPadPerGroup (GpioGetGroupFromGroupIndex (Index))) + 1;
  }

  GpioUnlockDataIndex += DwNum;
  return GpioUnlockDataIndex;
}

/**
  This procedure will create GPIO HOB for storing unlock data

  @retval Pointer to GPIO Unlock data structure
**/
STATIC
GPIO_UNLOCK_HOB_DATA*
GpioCreateUnlockData (
  VOID
  )
{
  VOID           *HobData;
  GPIO_GROUP     Group;
  GPIO_GROUP     GroupMin;
  GPIO_GROUP     GroupMax;
  UINT32         GpioUnlockDataRecords;

  GroupMin = GpioGetLowestGroup ();
  GroupMax = GpioGetHighestGroup ();
  GpioUnlockDataRecords = 0;

  for (Group = GroupMin; Group <= GroupMax; Group++) {
    GpioUnlockDataRecords += GPIO_GET_DW_NUM (GpioGetPadPerGroup (Group)) + 1;
  }

  HobData = BuildGuidHob (&gGpioLibUnlockHobGuid, GpioUnlockDataRecords * sizeof (GPIO_UNLOCK_HOB_DATA));
  if (HobData == NULL) {
    return NULL;
  }

  ZeroMem (HobData, GpioUnlockDataRecords * sizeof (GPIO_UNLOCK_HOB_DATA));

  return (GPIO_UNLOCK_HOB_DATA*)HobData;
}

/**
  This procedure will Get GPIO Unlock data structure for storing unlock data.
  If HOB doesn't exist it will be created.

  @param[out] GpioUnlockData          pointer to GPIO Unlock data structure

  @retval Length                      number of GPIO unlock data records
**/
STATIC
UINT32
GpioGetUnlockData (
  GPIO_UNLOCK_HOB_DATA  **GpioUnlockData
  )
{
  VOID  *Hob;

  Hob = GetFirstGuidHob (&gGpioLibUnlockHobGuid);
  if (Hob == NULL) {
    //
    // It is the first time this function is used so create the HOB
    //
    *GpioUnlockData = GpioCreateUnlockData ();
    if (*GpioUnlockData == NULL) {
      return 0;
    }
    Hob = GetFirstGuidHob (&gGpioLibUnlockHobGuid);
  } else {
    *GpioUnlockData = (GPIO_UNLOCK_HOB_DATA*) GET_GUID_HOB_DATA (Hob);
  }
  return GET_GUID_HOB_DATA_SIZE (Hob) / sizeof (GPIO_UNLOCK_HOB_DATA);
}

/**
  This procedure stores GPIO group data about pads which PadConfig needs to be unlocked.

  @param[in]  GroupIndex          GPIO group index
  @param[in]  DwNum               DWORD index for a group.
                                  For group which has less then 32 pads per group DwNum must be 0.
  @param[in]  UnlockedPads        DWORD bitmask for pads which are going to be left unlocked
                                  Bit position - PadNumber
                                  Bit value - 0: Skip, 1: Leave unlocked

  @retval Status
**/
EFI_STATUS
GpioStoreGroupDwUnlockPadConfigData (
  IN UINT32                       GroupIndex,
  IN UINT32                       DwNum,
  IN UINT32                       UnlockedPads
  )
{
  GPIO_UNLOCK_HOB_DATA *GpioUnlockData;
  UINT32               Length;
  UINT32               Index;

  if (UnlockedPads == 0) {
    //
    // No pads to be left unlocked
    //
    return EFI_SUCCESS;
  }

  Length = GpioGetUnlockData (&GpioUnlockData);
  if (Length == 0) {
    return EFI_NOT_FOUND;
  }

  Index = GpioUnlockDataIndex (GroupIndex, DwNum);
  if (Index >= Length) {
    return EFI_INVALID_PARAMETER;
  }

  GpioUnlockData[Index].PadConfig |= UnlockedPads;
  return EFI_SUCCESS;
}

/**
  This procedure stores GPIO group data about pads which Output state needs to be unlocked.

  @param[in]  GroupIndex          GPIO group index
  @param[in]  DwNum               DWORD index for a group.
                                  For group which has less then 32 pads per group DwNum must be 0.
  @param[in]  UnlockedPads        DWORD bitmask for pads which are going to be left unlocked
                                  Bit position - PadNumber
                                  Bit value - 0: Skip, 1: Leave unlocked
  @retval Status
**/
EFI_STATUS
GpioStoreGroupDwUnlockOutputData (
  IN UINT32                       GroupIndex,
  IN UINT32                       DwNum,
  IN UINT32                       UnlockedPads
  )
{
  GPIO_UNLOCK_HOB_DATA *GpioUnlockData;
  UINT32               Length;
  UINT32               Index;

  if (UnlockedPads == 0) {
    //
    // No pads to be left unlocked
    //
    return EFI_SUCCESS;
  }

  Length = GpioGetUnlockData (&GpioUnlockData);
  if (Length == 0) {
    return EFI_NOT_FOUND;
  }

  Index = GpioUnlockDataIndex (GroupIndex, DwNum);
  if (Index >= Length) {
    return EFI_INVALID_PARAMETER;
  }

  GpioUnlockData[Index].OutputState |= UnlockedPads;
  return EFI_SUCCESS;
}
