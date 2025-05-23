/** @file
  IPMI common hook functions head file

  @copyright
  Copyright 2014 - 2021 Intel Corporation. <BR>
  Copyright (c) 1985 - 2023, American Megatrends International LLC. <BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _IPMI_HOOKS_H
#define _IPMI_HOOKS_H

#include <Protocol/IpmiTransportProtocol.h>
#include <Protocol/IpmiTransport2Protocol.h>
#include <Library/BtInterfaceLib.h>
#include <Library/SsifInterfaceLib.h>
#include <Library/IpmbInterfaceLib.h>
#include <Library/DebugLib.h>
#include <IpmiBmcCommon.h>
#include <IpmiBmc.h>

//
// Internal(hook) function list
//
EFI_STATUS
EFIAPI
IpmiSendCommand (
  IN      IPMI_TRANSPORT  *This,
  IN      UINT8           NetFunction,
  IN      UINT8           Lun,
  IN      UINT8           Command,
  IN      UINT8           *CommandData,
  IN      UINT32          CommandDataSize,
  IN OUT  UINT8           *ResponseData,
  IN OUT  UINT32          *ResponseDataSize
  )

/*++

Routine Description:

  Send IPMI command to BMC

Arguments:

  This              - Pointer to IPMI protocol instance
  NetFunction       - Net Function of command to send
  Lun               - LUN of command to send
  Command           - IPMI command to send
  CommandData       - Pointer to command data buffer, if needed
  CommandDataSize   - Size of command data buffer
  ResponseData      - Pointer to response data buffer
  ResponseDataSize  - Pointer to response data buffer size

Returns:

  EFI_INVALID_PARAMETER - One of the input values is bad
  EFI_DEVICE_ERROR      - IPMI command failed
  EFI_BUFFER_TOO_SMALL  - Response buffer is too small
  EFI_UNSUPPORTED       - Command is not supported by BMC
  EFI_SUCCESS           - Command completed successfully

--*/
;

EFI_STATUS
EFIAPI
IpmiSendCommand2 (
  IN      IPMI_TRANSPORT2  *This,
  IN      UINT8            NetFunction,
  IN      UINT8            Lun,
  IN      UINT8            Command,
  IN      UINT8            *CommandData,
  IN      UINT32           CommandDataSize,
  IN OUT  UINT8            *ResponseData,
  IN OUT  UINT32           *ResponseDataSize
  )

/*++

Routine Description:

  This API use the default interface (PcdDefaultSystemInterface) to send IPMI command
  in the right mode to the appropiate device, ME or BMC.

Arguments:

  This              - Pointer to IPMI protocol instance
  NetFunction       - Net Function of command to send
  Lun               - LUN of command to send
  Command           - IPMI command to send
  CommandData       - Pointer to command data buffer, if needed
  CommandDataSize   - Size of command data buffer
  ResponseData      - Pointer to response data buffer
  ResponseDataSize  - Pointer to response data buffer size

Returns:

  EFI_INVALID_PARAMETER - One of the input values is bad
  EFI_DEVICE_ERROR      - IPMI command failed
  EFI_BUFFER_TOO_SMALL  - Response buffer is too small
  EFI_UNSUPPORTED       - Command is not supported by BMC
  EFI_SUCCESS           - Command completed successfully

--*/
;

EFI_STATUS
EFIAPI
IpmiSendCommand2Ex (
  IN      IPMI_TRANSPORT2        *This,
  IN      UINT8                  NetFunction,
  IN      UINT8                  Lun,
  IN      UINT8                  Command,
  IN      UINT8                  *CommandData,
  IN      UINT32                 CommandDataSize,
  IN OUT  UINT8                  *ResponseData,
  IN OUT  UINT32                 *ResponseDataSize,
  IN      SYSTEM_INTERFACE_TYPE  InterfaceType
  )

/*++
Routine Description:

  This API use the specific interface type to send IPMI command
  in the right mode to the appropiate device, ME or BMC.

Arguments:

  This              - Pointer to IPMI protocol instance
  NetFunction       - Net Function of command to send
  Lun               - LUN of command to send
  Command           - IPMI command to send
  CommandData       - Pointer to command data buffer, if needed
  CommandDataSize   - Size of command data buffer
  ResponseData      - Pointer to response data buffer
  ResponseDataSize  - Pointer to response data buffer size
  InterfaceType     - BMC Interface type.

Returns:

  EFI_INVALID_PARAMETER - One of the input values is bad
  EFI_DEVICE_ERROR      - IPMI command failed
  EFI_BUFFER_TOO_SMALL  - Response buffer is too small
  EFI_UNSUPPORTED       - Command is not supported by BMC
  EFI_SUCCESS           - Command completed successfully

--*/
;

EFI_STATUS
EFIAPI
IpmiGetBmcStatus (
  IN IPMI_TRANSPORT   *This,
  OUT BMC_STATUS      *BmcStatus,
  OUT SM_COM_ADDRESS  *ComAddress
  )

/*++

Routine Description:

  Updates the BMC status and returns the Com Address

Arguments:

  This        - Pointer to IPMI protocol instance
  BmcStatus   - BMC status
  ComAddress  - Com Address

Returns:

  EFI_SUCCESS - Success

--*/
;

#endif
