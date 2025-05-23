/** @file
  This is the driver that publishes the SMM Control Protocol.

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Guid/EventGroup.h>
#include <Library/PmcLib.h>
#include <Library/GpioLib.h>
#include <IndustryStandard/Pci30.h>
#include <Register/PchRegsLpc.h>
#include <Register/SpiRegs.h>
#include <Register/PmcRegs.h>
#include "SmmControlDriver.h"

STATIC SMM_CONTROL_PRIVATE_DATA mSmmControl;
GLOBAL_REMOVE_IF_UNREFERENCED UINT16                          mABase;

/**
  Fixup internal data pointers so that the services can be called in virtual mode.

  @param[in] Event                The event registered.
  @param[in] Context              Event context.

**/
VOID
EFIAPI
SmmControlVirtualAddressChangeEvent (
  IN EFI_EVENT                  Event,
  IN VOID                       *Context
  )
{
  gRT->ConvertPointer (0, (VOID *) &(mSmmControl.SmmControl.Trigger));
  gRT->ConvertPointer (0, (VOID *) &(mSmmControl.SmmControl.Clear));
}

/**
  <b>SmmControl DXE RUNTIME Module Entry Point</b>\n
  - <b>Introduction</b>\n
    The SmmControl module is a DXE RUNTIME driver that provides a standard way
    for other drivers to trigger software SMIs.

  - @pre
    - PCH Power Management I/O space base address has already been programmed.
      If SmmControl Runtime DXE driver is run before Status Code Runtime Protocol
      is installed and there is the need to use Status code in the driver, it will
      be necessary to add EFI_STATUS_CODE_RUNTIME_PROTOCOL_GUID to the dependency file.
    - EFI_SMM_BASE2_PROTOCOL
      - Documented in the System Management Mode Core Interface Specification.

  - @result
    The SmmControl driver produces the EFI_SMM_CONTROL_PROTOCOL documented in
    System Management Mode Core Interface Specification.

  @param[in] ImageHandle          Handle for the image of this driver
  @param[in] SystemTable          Pointer to the EFI System Table

  @retval EFI_STATUS              Results of the installation of the SMM Control Protocol
**/
EFI_STATUS
EFIAPI
SmmControlDriverEntryInit (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS  Status;
  EFI_EVENT   Event;

  DEBUG ((DEBUG_INFO, "SmmControlDriverEntryInit() Start\n"));

  //
  // Get the Power Management I/O space base address. We assume that
  // this base address has already been programmed if this driver is
  // being run.
  //
  mABase = PmcGetAcpiBase ();

  Status = EFI_SUCCESS;
  if (mABase != 0) {
    //
    // Install the instance of the protocol
    //
    mSmmControl.Signature                       = SMM_CONTROL_PRIVATE_DATA_SIGNATURE;
    mSmmControl.Handle                          = ImageHandle;

    mSmmControl.SmmControl.Trigger              = Activate;
    mSmmControl.SmmControl.Clear                = Deactivate;
    mSmmControl.SmmControl.MinimumTriggerPeriod = 0;

    //
    // Install our protocol interfaces on the device's handle
    //
    Status = gBS->InstallMultipleProtocolInterfaces (
                    &mSmmControl.Handle,
                    &gEfiSmmControl2ProtocolGuid,
                    &mSmmControl.SmmControl,
                    NULL
                    );
  } else {
    Status = EFI_DEVICE_ERROR;
    return Status;
  }

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  SmmControlVirtualAddressChangeEvent,
                  NULL,
                  &gEfiEventVirtualAddressChangeGuid,
                  &Event
                  );

  DEBUG ((DEBUG_INFO, "SmmControlDriverEntryInit() End\n"));

  return Status;
}

/**
  Trigger the software SMI

  @param[in] Command              The value to be set on the software SMI command port
  @param[in] Data                 The value to be set on the software SMI data port

  @retval EFI_SUCCESS             Function completes successfully
**/
EFI_STATUS
EFIAPI
SmmTrigger (
  IN UINT8   Command,
  IN UINT8   Data
  )
{
  UINT32  OutputData;
  UINT32  OutputPort;

  //
  // Enable the APMC SMI
  //
  OutputPort  = mABase + R_ACPI_IO_SMI_EN;
  OutputData  = IoRead32 ((UINTN) OutputPort);
  OutputData |= (B_ACPI_IO_SMI_EN_APMC | B_ACPI_IO_SMI_EN_GBL_SMI);
  DEBUG (
    (DEBUG_VERBOSE,
     "The SMI Control Port at address %x will be written to %x.\n",
     OutputPort,
     OutputData)
    );
  IoWrite32 (
    (UINTN) OutputPort,
    (UINT32) (OutputData)
    );

  OutputPort  = R_PCH_IO_APM_STS;
  OutputData  = Data;

  //
  // Write data to APM DATA PORT
  //
  IoWrite8 (
    (UINTN) OutputPort,
    (UINT8) (OutputData)
    );
  OutputPort  = R_PCH_IO_APM_CNT;
  OutputData  = Command;

  //
  // Generate the APMC SMI
  //
  IoWrite8 (
    (UINTN) OutputPort,
    (UINT8) (OutputData)
    );

  return EFI_SUCCESS;
}

/**
  Clear the SMI status


  @retval EFI_SUCCESS             The function completes successfully
  @retval EFI_DEVICE_ERROR        Something error occurred
**/
EFI_STATUS
EFIAPI
SmmClear (
  VOID
  )
{
  EFI_STATUS  Status;
  UINT32      OutputData;
  UINT32      OutputPort;

  Status = EFI_SUCCESS;

  //
  // Clear the Power Button Override Status Bit, it gates EOS from being set.
  //
  OutputPort  = mABase + R_ACPI_IO_PM1_STS;
  OutputData  = B_ACPI_IO_PM1_STS_PRBTNOR;
  DEBUG (
    (DEBUG_VERBOSE,
     "The PM1 Status Port at address %x will be written to %x.\n",
     OutputPort,
     OutputData)
    );
  IoWrite16 (
    (UINTN) OutputPort,
    (UINT16) (OutputData)
    );

  //
  // Clear the APM SMI Status Bit
  //
  OutputPort  = mABase + R_ACPI_IO_SMI_STS;
  OutputData  = B_ACPI_IO_SMI_STS_APM;
  DEBUG (
    (DEBUG_VERBOSE,
     "The SMI Status Port at address %x will be written to %x.\n",
     OutputPort,
     OutputData)
    );
  IoWrite32 (
    (UINTN) OutputPort,
    (UINT32) (OutputData)
    );

  //
  // Set the EOS Bit
  //
  OutputPort  = mABase + R_ACPI_IO_SMI_EN;
  OutputData  = IoRead32 ((UINTN) OutputPort);
  OutputData |= B_ACPI_IO_SMI_EN_EOS;
  DEBUG (
    (DEBUG_VERBOSE,
     "The SMI Control Port at address %x will be written to %x.\n",
     OutputPort,
     OutputData)
    );
  IoWrite32 (
    (UINTN) OutputPort,
    (UINT32) (OutputData)
    );

  //
  // There is no need to read EOS back and check if it is set.
  // This can lead to a reading of zero if an SMI occurs right after the SMI_EN port read
  // but before the data is returned to the CPU.
  // SMM Dispatcher should make sure that EOS is set after all SMI sources are processed.
  //
  return Status;
}

/**
  This routine generates an SMI

  @param[in] This                       The EFI SMM Control protocol instance
  @param[in, out] CommandPort           The buffer contains data to the command port
  @param[in, out] DataPort              The buffer contains data to the data port
  @param[in] Periodic                   Periodic or not
  @param[in] ActivationInterval         Interval of periodic SMI

  @retval EFI Status                    Describing the result of the operation
  @retval EFI_INVALID_PARAMETER         Some parameter value passed is not supported
**/
EFI_STATUS
EFIAPI
Activate (
  IN CONST EFI_SMM_CONTROL2_PROTOCOL                    * This,
  IN OUT  UINT8                                         *CommandPort       OPTIONAL,
  IN OUT  UINT8                                         *DataPort          OPTIONAL,
  IN      BOOLEAN                                       Periodic           OPTIONAL,
  IN      UINTN                                         ActivationInterval OPTIONAL
  )
{
  EFI_STATUS  Status;
  UINT8       Command;
  UINT8       Data;

  if (Periodic) {
    DEBUG ((DEBUG_WARN, "Invalid parameter\n"));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Extract the values from CommandPort and DataPort
  //
  if (CommandPort == NULL) {
    Command = 0xFF;
  } else {
    Command = *CommandPort;
  }

  if (DataPort == NULL) {
    Data = 0x00;
  } else {
    Data = *DataPort;
  }

  //
  // Clear any pending the APM SMI
  //
  Status = SmmClear ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return SmmTrigger (Command, Data);
}

/**
  This routine clears an SMI

  @param[in] This                 The EFI SMM Control protocol instance
  @param[in] Periodic             Periodic or not

  @retval EFI Status              Describing the result of the operation
  @retval EFI_INVALID_PARAMETER   Some parameter value passed is not supported
**/
EFI_STATUS
EFIAPI
Deactivate (
  IN CONST EFI_SMM_CONTROL2_PROTOCOL       *This,
  IN  BOOLEAN                              Periodic OPTIONAL
  )
{
  if (Periodic) {
    return EFI_INVALID_PARAMETER;
  }

  return SmmClear ();
}
