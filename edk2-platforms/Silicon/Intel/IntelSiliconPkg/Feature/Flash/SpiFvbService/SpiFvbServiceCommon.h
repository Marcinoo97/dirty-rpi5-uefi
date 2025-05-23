/** @file
  Common source definitions used in serial flash drivers

  Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
  Copyright (c) Microsoft Corporation.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef SPI_FVB_SERVICE_COMMON_H_
#define SPI_FVB_SERVICE_COMMON_H_

#include <Guid/EventGroup.h>
#include <Guid/FirmwareFileSystem2.h>
#include <Guid/SystemNvDataGuid.h>
#include <Pi/PiFirmwareVolume.h>
#include <Protocol/DevicePath.h>
#include <Protocol/FirmwareVolumeBlock.h>

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/CacheMaintenanceLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/DevicePathLib.h>
#include <Library/HobLib.h>
#include <Library/VariableFlashInfoLib.h>
#include <Library/SafeIntLib.h>

#include <Library/SpiFlashCommonLib.h>

//
// Define two helper macro to extract the Capability field or Status field in FVB
// bit fields
//
#define EFI_FVB2_CAPABILITIES (EFI_FVB2_READ_DISABLED_CAP | \
                              EFI_FVB2_READ_ENABLED_CAP | \
                              EFI_FVB2_WRITE_DISABLED_CAP | \
                              EFI_FVB2_WRITE_ENABLED_CAP | \
                              EFI_FVB2_LOCK_CAP \
                              )

#define EFI_FVB2_STATUS (EFI_FVB2_READ_STATUS | EFI_FVB2_WRITE_STATUS | EFI_FVB2_LOCK_STATUS)

#define FVB_INSTANCE_SIGNATURE       SIGNATURE_32('F','V','B','I')

typedef struct {
  UINT32                                Signature;
  UINTN                                 FvBase;
  UINTN                                 NumOfBlocks;
  EFI_DEVICE_PATH_PROTOCOL              *DevicePath;
  EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL    FvbProtocol;
  EFI_FIRMWARE_VOLUME_HEADER            FvHeader;
} EFI_FVB_INSTANCE;

typedef struct {
  EFI_FVB_INSTANCE            *FvbInstance;
  UINT32                      NumFv;
} FVB_GLOBAL;

//
// Firmware Volume Block (FVB) Protocol instance data
//
#define FVB_INSTANCE_FROM_THIS(a) CR(a, EFI_FVB_INSTANCE, FvbProtocol, FVB_INSTANCE_SIGNATURE)

typedef struct {
  MEDIA_FW_VOL_DEVICE_PATH  FvDevPath;
  EFI_DEVICE_PATH_PROTOCOL  EndDevPath;
} FV_PIWG_DEVICE_PATH;

typedef struct {
  MEMMAP_DEVICE_PATH          MemMapDevPath;
  EFI_DEVICE_PATH_PROTOCOL    EndDevPath;
} FV_MEMMAP_DEVICE_PATH;

typedef struct {
  UINT32              FvBase;
  UINT32              FvSize;
} FV_INFO;

//
// Firmware Volume Block (FVB) Protocol APIs
//
EFI_STATUS
EFIAPI
FvbProtocolGetAttributes (
  IN CONST EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL   *This,
  OUT EFI_FVB_ATTRIBUTES_2                      *Attributes
  );

EFI_STATUS
EFIAPI
FvbProtocolSetAttributes (
  IN CONST EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL   *This,
  IN OUT EFI_FVB_ATTRIBUTES_2                   *Attributes
  );

EFI_STATUS
EFIAPI
FvbProtocolGetPhysicalAddress (
  IN CONST EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL  *This,
       OUT EFI_PHYSICAL_ADDRESS                *Address
  );

EFI_STATUS
EFIAPI
FvbProtocolGetBlockSize (
  IN CONST EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL  *This,
  IN  EFI_LBA                                  Lba,
  OUT UINTN                                    *BlockSize,
  OUT UINTN                                    *NumOfBlocks
  );

EFI_STATUS
EFIAPI
FvbProtocolRead (
  IN CONST EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL   *This,
  IN EFI_LBA                                    Lba,
  IN UINTN                                      Offset,
  IN OUT UINTN                                  *NumBytes,
  OUT UINT8                                     *Buffer
  );

EFI_STATUS
EFIAPI
FvbProtocolWrite (
  IN CONST EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL   *This,
  IN EFI_LBA                                    Lba,
  IN UINTN                                      Offset,
  IN OUT UINTN                                  *NumBytes,
  IN UINT8                                      *Buffer
  );

EFI_STATUS
EFIAPI
FvbProtocolEraseBlocks (
  IN CONST EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL    *This,
  ...
  );

BOOLEAN
IsFvHeaderValid (
  IN       EFI_PHYSICAL_ADDRESS          FvBase,
  IN CONST EFI_FIRMWARE_VOLUME_HEADER    *FwVolHeader
  );

//
// Module Local Functions
//

/**
  Returns an empty firmware volume for the firmware volume at the given base address.

  @param[in]    FvBaseAddress       The base address of the firmware volume requested.
  @param[out]   FvbInfo             A pointer that will be set to a buffer for the firmware volume header
                                    at the given base address.

  @retval     EFI_SUCCESS           The firmware volume was returned successfully.
  @retval     EFI_NOT_FOUND         The firmware volume was not found for the given base address.

**/
EFI_STATUS
GetGeneratedFvByAddress (
  IN  EFI_PHYSICAL_ADDRESS         FvBaseAddress,
  OUT EFI_FIRMWARE_VOLUME_HEADER   **FvbInfo
  );

/**
  Get the total size of the firmware volume on flash used for variable store operations.

  @param[out] BaseAddress         Base address of the variable store firmware volume.
  @param[out] Length              Length in bytes of the firmware volume used for variable store operations.

**/
VOID
GetVariableFvInfo (
  OUT EFI_PHYSICAL_ADDRESS      *BaseAddress,
  OUT UINT32                    *Length
  );

extern FVB_GLOBAL                         mFvbModuleGlobal;
extern FV_MEMMAP_DEVICE_PATH              mFvMemmapDevicePathTemplate;
extern FV_PIWG_DEVICE_PATH                mFvPIWGDevicePathTemplate;
extern EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL mFvbProtocolTemplate;
extern FV_INFO                            mPlatformFvBaseAddress[];

#endif
