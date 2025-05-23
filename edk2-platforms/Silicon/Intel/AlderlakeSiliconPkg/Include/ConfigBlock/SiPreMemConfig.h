/** @file
  Si Config Block PreMem

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _SI_PREMEM_CONFIG_H_
#define _SI_PREMEM_CONFIG_H_

#define SI_PREMEM_CONFIG_REVISION  1

extern EFI_GUID gSiPreMemConfigGuid;

typedef enum {
  ProbeTypeDisabled    = 0x00,
  ProbeTypeDciOob      = 0x02,
  ProbeType2WireDciOob = 0x06,
  ProbeTypeManual      = 0x07,
  ProbeTypeMax
} PLATFORM_DEBUG_CONSENT_PROBE_TYPE;

#pragma pack (push,1)
/**
  The Silicon PreMem Policy allows the platform code to publish a set of configuration
  information that the RC drivers will use to configure the silicon hardware.

  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;  ///< Offset 0 - 27 Config Block Header
  /**
    Platform Debug Consent
    As a main switch to enable platform debug capability and relevant settings with specified probe type.
    Manual: Do not use Platform Debug Consent to override other debug-relevant policies, but the user must set each debug option manually, aimed at advanced users.

    PDC-dependent policies are listed:
    DciPreMemConfig->DciEn
    CpuTraceHubConfig->EnableMode
    CpuTraceHubConfig->CpuTraceHubMemReg0Size
    CpuTraceHubConfig->CpuTraceHubMemReg1Size
    PchTraceHubPreMemConfig->EnableMode
    PchTraceHubPreMemConfig->MemReg0Size
    PchTraceHubPreMemConfig->MemReg1Size

    Note: DCI OOB (aka BSSB) uses CCA probe.
    Refer to definition of PLATFORM_DEBUG_CONSENT_PROBE_TYPE
    <b>0:Disabled</b>; 2:DCI OOB (All + TraceHubs); 6:2-wire DCI OOB (Low power); 7:Manual
  **/
  UINT32    PlatformDebugConsent  :  4;       /// offset 28 - 31
  UINT32    RsvdBits              : 28;
  /**
    This is used to skip override boot mode during firmware update boot mode.
    When set to TRUE and boot mode is BOOT_ON_FLASH_UPDATE,
    skip setting boot mode to BOOT_WITH_FULL_CONFIGURATION in PEI memory init.
    <b>0: FALSE</b>, 1: TRUE
  **/
  UINT8     SkipOverrideBootModeWhenFwUpdate; /// offset 32
  UINT8     RsvdBytes[3];                     /// offset 33 - 35
} SI_PREMEM_CONFIG;

#pragma pack (pop)
#endif // _SI_PREMEM_CONFIG_H_
