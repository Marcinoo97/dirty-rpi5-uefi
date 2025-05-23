/** @file
  Platform Hook Library instances

  @copyright
  Copyright (c) 2017 - 2021, Intel Corporation. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Base.h>
#include <Uefi.h>
#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/BoardAcpiEnableLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <PchAccess.h>
#include <Protocol/DynamicSiLibrarySmmProtocol.h>
#include <Library/SmmServicesTableLib.h>

EFI_STATUS
EFIAPI
BoardEnableAcpi (
  IN BOOLEAN  EnableSci
  )
{
  UINT32                           SmiEn;
  UINT16                           Pm1En;
  UINT16                           Pm1Cnt;
  UINT16                           PchPmBase;
  EFI_STATUS                       Status;
  DYNAMIC_SI_LIBARY_SMM_PROTOCOL   *DynamicSiLibrarySmmProtocol = NULL;

  Status = gSmst->SmmLocateProtocol (&gDynamicSiLibrarySmmProtocolGuid, NULL, (VOID**) &DynamicSiLibrarySmmProtocol);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  //
  // Init Power Management I/O Base aka ACPI Base
  //
  PchPmBase = DynamicSiLibrarySmmProtocol->PmcGetAcpiBase ();

  SmiEn = IoRead32 (PchPmBase + R_ACPI_IO_SMI_EN);

  //
  // Disable SW SMI Timer and legacy USB
  //
  SmiEn &= ~(B_ACPI_IO_SMI_EN_SWSMI_TMR | B_ACPI_IO_SMI_EN_LEGACY_USB | B_ACPI_IO_SMI_EN_LEGACY_USB2);

  //
  // And enable SMI on write to B_PCH_ACPI_PM1_CNT_SLP_EN when SLP_TYP is written
  //
  SmiEn |= B_ACPI_IO_SMI_EN_ON_SLP_EN ;
  IoWrite32 (PchPmBase + R_ACPI_IO_SMI_EN, SmiEn);

  //
  // Disable PM sources except power button
  //
  Pm1En   = B_ACPI_IO_PM1_EN_PWRBTN;
  IoWrite16 (PchPmBase + R_ACPI_IO_PM1_EN, Pm1En);

  //
  // Enable SCI
  //
  if (EnableSci) {
    Pm1Cnt = IoRead16 (PchPmBase + R_ACPI_IO_PM1_CNT);
    Pm1Cnt |= B_ACPI_IO_PM1_CNT_SCI_EN;
    IoWrite16 (PchPmBase + R_ACPI_IO_PM1_CNT, Pm1Cnt);
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
BoardDisableAcpi (
  IN BOOLEAN  DisableSci
  )
{
  UINT16                           Pm1Cnt;
  UINT16                           PchPmBase;
  EFI_STATUS                       Status;
  DYNAMIC_SI_LIBARY_SMM_PROTOCOL   *DynamicSiLibrarySmmProtocol = NULL;

  Status = gSmst->SmmLocateProtocol (&gDynamicSiLibrarySmmProtocolGuid, NULL, (VOID**) &DynamicSiLibrarySmmProtocol);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  //
  // Init Power Management I/O Base aka ACPI Base
  //
  PchPmBase = DynamicSiLibrarySmmProtocol->PmcGetAcpiBase ();


  //
  // Disable SCI
  //
  if (DisableSci) {
    Pm1Cnt = IoRead16 (PchPmBase + R_ACPI_IO_PM1_CNT);
    Pm1Cnt &= ~B_ACPI_IO_PM1_CNT_SCI_EN;
    IoWrite16 (PchPmBase + R_ACPI_IO_PM1_CNT, Pm1Cnt);
  }

  return EFI_SUCCESS;
}
