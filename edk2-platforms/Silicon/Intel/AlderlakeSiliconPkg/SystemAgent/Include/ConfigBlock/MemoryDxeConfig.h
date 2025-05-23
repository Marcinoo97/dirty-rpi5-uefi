/** @file
  Memory DXE Policy definitions

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _MEMORY_DXE_CONFIG_H_
#define _MEMORY_DXE_CONFIG_H_

#pragma pack(push, 1)

/**
 Making any setup structure change after code frozen
 will need to maintain backward compatibility, bump up
 structure revision and update below history table\n
  <b>Revision 1</b>:  - Initial version.
**/
#define MEMORY_DXE_CONFIG_REVISION 2

typedef struct _MEMORY_DXE_CONFIG  MEMORY_DXE_CONFIG;

/**
  Retrieves the OEM custom string for the SMBIOS Type 17 Table DeviceLocator field.
  Implementation of this function is optional, if this function pointer is NULL then
  the reference implementation of DeviceLocator will be used.

  @param[in]  This                          A pointer to this instance of MEMORY_DXE_CONFIG.
  @param[in]  Controller                    Desired Controller to get a DeviceLocator string for.
  @param[in]  Dimm                          Desired DIMM to get a DeviceLocator string for.
  @param[in]  MdSocket                      0 = Memory Down, 1 = Socketed.

  @retval                                   The DeviceLocator string
  @retval     NULL                          If the return value is NULL, the default value will be used.
**/
typedef
CHAR8*
(EFIAPI *MEMORY_DXE_CONFIG_GET_DEVICE_LOCATOR_STRING)(
  IN CONST    MEMORY_DXE_CONFIG   *This,
  IN          UINT8               Controller,
  IN          UINT8               Dimm,
  IN          UINT8               MdSocket
  );

/**
  Retrieves the OEM custom string for the SMBIOS Type 17 Table BankLocator field.
  Implementation of this function is optional, if this function pointer is NULL then
  the reference implementation of DeviceLocator will be used.

  @param[in]  This                          A pointer to this instance of MEMORY_DXE_CONFIG.
  @param[in]  Controller                    Desired Controller to get a BankLocator string for.
  @param[in]  Dimm                          Desired DIMM to get a BankLocator string for.
  @param[in]  MdSocket                      0 = Memory Down, 1 = Socketed.

  @retval                                   The BankLocator string
  @retval     NULL                          If the return value is NULL, the default value will be used.
**/
typedef
CHAR8*
(EFIAPI *MEMORY_DXE_CONFIG_GET_BANK_LOCATOR_STRING)(
  IN CONST    MEMORY_DXE_CONFIG   *This,
  IN          UINT8               Controller,
  IN          UINT8               Dimm,
  IN          UINT8               MdSocket
  );

/**
  The Memory Configuration includes DIMM SPD address Map and DIMM Slot Mechanical present bit map.
  The data elements should be initialized by a Platform Module.\n
  <b>Revision 1</b>:
  - Initial version.
**/
struct _MEMORY_DXE_CONFIG {
  CONFIG_BLOCK_HEADER   Header;                   ///< Offset 0-27: Config Block Header
/**
  Offset 28:
  Dimm SPD address
  Only Server support 2 channels * 3 slots per channel = 6 sockets totally
  The Desktop and mobile only support 2 channels * 2 slots per channel = 4 sockets totally
  So there is mapping rule here for Desktop and mobile that there are no more 4 DIMMS totally in a system:
    Channel A/ Slot 0 --> Dimm 0 --> SpdAddressTable[0]
    Channel A/ Slot 1 --> Dimm 1 --> SpdAddressTable[1]
    Channel B/ Slot 0 --> Dimm 2 --> SpdAddressTable[2]
    Channel B/ Slot 1 --> Dimm 3 --> SpdAddressTable[3]
  Refer to SmbiosMemory.c for use
  If change the mapping rule, please update the Revision number.
**/
  UINT8                                           *SpdAddressTable;
/**
  Offset 36:
  ChannelASlotMap and ChannelBSlotMap change to use SlotMap as the DIMM slot map solution.
  Reserve 2-bytes in order to comply with 4-bytes alignment and keep the offset of other members.
**/
  UINT8                                           ChannelASlotMap;
  UINT8                                           ChannelBSlotMap;
/**
  Offset 38:
  MRC execution time measurement: <b>0=Disable</b>, 1=Enable
**/
  UINT8                                           MrcTimeMeasure;
/**
  Offset 39:
  Fast boot: 0=Disable, <b>1=Enable</b>
**/
  UINT8                                           MrcFastBoot;
/**
  Offset 40:
  Retrieves the OEM custom string for the SMBIOS Type 17 Table DeviceLocator field.
**/
  MEMORY_DXE_CONFIG_GET_DEVICE_LOCATOR_STRING     GetDeviceLocatorString;
/**
  Offset 48:
  Retrieves the OEM custom string for the SMBIOS Type 17 Table BankLocator field.
**/
  MEMORY_DXE_CONFIG_GET_BANK_LOCATOR_STRING       GetBankLocatorString;
/**
  Offset 56:
  Each DIMM Slot Mechanical present bit map, bit 0 -> DIMM 0, bit 1 -> DIMM1, ...
  if the bit is 1, the related DIMM slot is present.
  E.g. if memory controller 0 channel 3 has 2 DIMMs,  SlotMap[0][3] = 0x03;
  E.g. if memory controller 1 channel 2 has only 1 DIMMs,  SlotMap[1][2] = 0x0;
**/
  UINT8                                           **SlotMap;
};
#pragma pack(pop)

#endif // _MEMORY_DXE_CONFIG_H_
