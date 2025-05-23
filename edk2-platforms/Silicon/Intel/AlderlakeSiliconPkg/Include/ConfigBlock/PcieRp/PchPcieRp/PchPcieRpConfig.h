/** @file
  PCH Pcie root port policy

   Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
   SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _PCH_PCIERP_CONFIG_H_
#define _PCH_PCIERP_CONFIG_H_

#include <PchLimits.h>
#include <PcieConfig.h>
#include <ConfigBlock.h>





/*
 Making any setup structure change after code frozen
 will need to maintain backward compatibility, bump up
 structure revision and update below history table\n

<b>Revision 1</b>:  - Initial version.
*/
#define PCIE_RP_DXE_CONFIG_REVISION 1

extern EFI_GUID gPchPcieConfigGuid;
extern EFI_GUID gPcieRpPreMemConfigGuid;

#pragma pack (push,1)

typedef enum {
  PchPcieOverrideDisabled             = 0,
  PchPcieL1L2Override                 = 0x01,
  PchPcieL1SubstatesOverride          = 0x02,
  PchPcieL1L2AndL1SubstatesOverride   = 0x03,
  PchPcieLtrOverride                  = 0x04
} PCH_PCIE_OVERRIDE_CONFIG;

///
/// The values before AutoConfig match the setting of PCI Express Base Specification 1.1, please be careful for adding new feature
///
typedef enum {
  PchPcieAspmDisabled,
  PchPcieAspmL0s,
  PchPcieAspmL1,
  PchPcieAspmL0sL1,
  PchPcieAspmAutoConfig,
  PchPcieAspmMax
} PCH_PCIE_ASPM_CONTROL;

/**
  Refer to PCH EDS for the PCH implementation values corresponding
  to below PCI-E spec defined ranges
**/
typedef enum {
  PchPcieL1SubstatesDisabled,
  PchPcieL1SubstatesL1_1,
  PchPcieL1SubstatesL1_1_2,
  PchPcieL1SubstatesMax
} PCH_PCIE_L1SUBSTATES_CONTROL;

enum PCH_PCIE_MAX_PAYLOAD {
  PchPcieMaxPayload128 = 0,
  PchPcieMaxPayload256,
  PchPcieMaxPayloadMax
};


/**
  PCH_PCIE_CLOCK describes PCIe source clock generated by PCH.
**/
typedef struct {
  UINT8   Usage;             ///< Purpose of given clock (see PCH_PCIE_CLOCK_USAGE). Default: Unused, 0xFF
  UINT8   ClkReq;            ///< ClkSrc - ClkReq mapping. Default: 1:1 mapping with Clock numbers
  UINT8   RsvdBytes[2];      ///< Reserved byte
  UINT32  ClkReqGpioPinMux;  /// Muxed GPIO details. Refer GPIO_*_MUXING_SRC_CLKREQ_x*
} PCH_PCIE_CLOCK;


/**
  The PCH_PCI_EXPRESS_ROOT_PORT_CONFIG describe the feature and capability of each PCH PCIe root port.
**/
typedef struct {
  PCIE_ROOT_PORT_COMMON_CONFIG  PcieRpCommonConfig; ///an instance of Pcie Common Config
  UINT8  ExtSync;              ///< Indicate whether the extended synch is enabled. <b>0: Disable</b>; 1: Enable.
  //
  // Error handlings
  //
  UINT8  SystemErrorEnable;    ///< Indicate whether the System Error is enabled. <b>0: Disable</b>; 1: Enable.
  /**
    The Multiple VC (MVC) supports hardware to avoid HoQ block for latency sensitive TC.
    Currently it is only applicable to Root Ports with 2pX4 port configuration with 2 VCs,or
    DMI port configuration with 3 VCs. For Root Ports 2pX4 configuration, two RPs (RP0,
    RP2) shall support two PCIe VCs (VC0 & VC1) and the other RPs (RP1, RP3) shall be
    disabled.
    <b>0: Disable</b>; 1: Enable
  **/
  UINT8  MvcEnabled;
  /**
    Virtual Pin Port is industry standard introduced to PCIe Hot Plug support in systems
    when GPIO pins expansion is needed. It is server specific feature.
    <b>0x00: Default</b>; 0xFF: Disabled
  **/
  UINT8   VppPort;
  UINT8   VppAddress;                               ///< PCIe Hot Plug VPP SMBus Address. Default is zero.
  UINT8   RsvdBytes0[3];                            ///< Reserved bytes
} PCH_PCIE_ROOT_PORT_CONFIG;

/**
  The PCH_PCIE_CONFIG block describes the expected configuration of the PCH PCI Express controllers
  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>:
  - Moved EnablePort8xhDecode policy to PCIE_COMMON_CONFIG
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Config Block Header
  ///
  /// These members describe the configuration of each PCH PCIe root port.
  ///
  PCIE_COMMON_CONFIG                PcieCommonConfig;
  PCH_PCIE_ROOT_PORT_CONFIG         RootPort[PCH_MAX_PCIE_ROOT_PORTS];
  PCH_PCIE_ROOT_PORT_CONFIG         DmiPort;

  ///
  /// <b>(Test)</b> The Index of PCIe Port that is selected for Port8xh Decode (0 Based)
  ///
  UINT8                             PchPciePort8xhDecodePortIndex;
  UINT8                             DmiPowerReduction;
  UINT8                             RsvdBytes0[2];
} PCH_PCIE_CONFIG;

/**
  The PCH_PCIE_RP_PREMEM_CONFIG block describes early configuration of the PCH PCI Express controllers
  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                                ///< Config Block Header
  /**
    Root Port enabling mask.
    Bit0 presents RP1, Bit1 presents RP2, and so on.
    0: Disable; <b>1: Enable</b>.
  **/
  UINT32                RpEnabledMask;
  /// Configuration of PCIe source clocks
  ///
  PCH_PCIE_CLOCK        PcieClock[PCH_MAX_PCIE_CLOCKS];

  /**
    Per Controller Bifurcation Configuration
    <b>0: Disabled</b>; 1: 4x1; 2: 1x2_2x1; 3: 2x2; 4: 1x4; 5: 4x2; 6: 1x4_2x2; 7: 2x2_1x4; 8: 2x4; 9: 1x8 (see: PCIE_BIFURCATION_CONFIG)
  **/
  UINT8                 Bifurcation[PCH_MAX_PCIE_CONTROLLERS];
  UINT8                 Rsvd4[(4 - PCH_MAX_PCIE_CONTROLLERS % 4) % 4];
} PCH_PCIE_RP_PREMEM_CONFIG;

/**
  The PCIE_RP_DXE_CONFIG block describes the expected configuration of the PCH PCI Express controllers in DXE phase

  <b>Revision 1</b>:
  - Init version
**/
typedef struct {
  CONFIG_BLOCK_HEADER      Header;                     ///< Config Block Header

  /**
    PCIe device override table
    The PCIe device table is being used to override PCIe device ASPM settings.
    And it's only used in DXE phase.
    Please refer to PCIE_DEVICE_OVERRIDE structure for the table.
    Last entry VendorId must be 0.
  **/
  PCIE_DEVICE_OVERRIDE     *PcieDeviceOverrideTablePtr;
} PCIE_RP_DXE_CONFIG;

#pragma pack (pop)

#endif // _PCH_PCIERP_CONFIG_H_
