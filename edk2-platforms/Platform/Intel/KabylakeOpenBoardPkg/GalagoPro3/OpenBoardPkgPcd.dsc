## @file
#  PCD configuration build description file for the GalagoPro3 board.
#
# Copyright (c) 2019 - 2022, Intel Corporation. All rights reserved.<BR>
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

################################################################################
#
# Pcd Section - list of all PCD Entries used by this board.
#
################################################################################

[PcdsFixedAtBuild.common]
  ######################################
  # Key Boot Stage and FSP configuration
  ######################################
  #
  # Please select the Boot Stage here.
  # Stage 1 - enable debug (system deadloop after debug init)
  # Stage 2 - mem init (system deadloop after mem init)
  # Stage 3 - boot to shell only
  # Stage 4 - boot to OS
  # Stage 5 - boot to OS with security boot enabled
  # Stage 6 - boot with advanced features enabled
  #
  gMinPlatformPkgTokenSpaceGuid.PcdBootStage|4

  #
  # 0: FSP Wrapper is running in Dispatch mode.
  # 1: FSP Wrapper is running in API mode.
  #
  gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection|1

  #
  # FALSE: The board is not a FSP wrapper (FSP binary not used)
  # TRUE:  The board is a FSP wrapper (FSP binary is used)
  #
  gMinPlatformPkgTokenSpaceGuid.PcdFspWrapperBootMode|TRUE

  #
  # FALSE: The PEI Main included in FvPreMemory is used to dispatch all PEIMs
  #        (both inside FSP and outside FSP).
  #        Pros:
  #          * PEI Main is re-built from source and is always the latest version
  #          * Platform code can link any desired LibraryClass to PEI Main
  #            (Ex: Custom DebugLib instance, SerialPortLib, etc.)
  #        Cons:
  #          * The PEI Main being used to execute FSP PEIMs is not the PEI Main
  #            that the FSP PEIMs were tested with, adding risk of breakage.
  #          * Two copies of PEI Main will exist in the final binary,
  #            #1 in FSP-M, #2 in FvPreMemory. The copy in FSP-M is never
  #            executed, wasting space.
  #
  # <b>TRUE</b>:  The PEI Main included in FSP is used to dispatch all PEIMs
  #        (both inside FSP and outside FSP). PEI Main will not be included in
  #        FvPreMemory. This is the default and is the recommended choice.
  #
  gMinPlatformPkgTokenSpaceGuid.PcdFspDispatchModeUseFspPeiMain|TRUE

  #
  # FSP Base address PCD will be updated in FDF basing on flash map.
  #
  gIntelFsp2WrapperTokenSpaceGuid.PcdFsptBaseAddress|0
  gIntelFsp2WrapperTokenSpaceGuid.PcdFspmBaseAddress|0

  gIntelFsp2PkgTokenSpaceGuid.PcdTemporaryRamBase|0xFEF00000
  gIntelFsp2PkgTokenSpaceGuid.PcdTemporaryRamSize|0x00040000
  gSiPkgTokenSpaceGuid.PcdTemporaryRamBase|0xFEF80000
  gSiPkgTokenSpaceGuid.PcdTemporaryRamSize|0x00040000
  gSiPkgTokenSpaceGuid.PcdTsegSize|0x1000000

!if gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection == 1
  #
  # FSP API mode does not share stack with the boot loader,
  # so FSP needs more temporary memory for FSP heap + stack size.
  #
  gIntelFsp2PkgTokenSpaceGuid.PcdFspTemporaryRamSize|0x26000

  #
  # FSP API mode does not need to enlarge the boot loader stack size
  # since the stacks are separate.
  #
  gSiPkgTokenSpaceGuid.PcdPeiTemporaryRamStackSize|0x20000
!else
  #
  # In FSP Dispatch mode boot loader stack size must be large
  # enough for executing both boot loader and FSP.
  #
  gSiPkgTokenSpaceGuid.PcdPeiTemporaryRamStackSize|0x40000
!endif

!if (gMinPlatformPkgTokenSpaceGuid.PcdFspWrapperBootMode == FALSE) || (gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection == 1)
  gSiPkgTokenSpaceGuid.PcdSiPciExpressBaseAddress|gEfiMdePkgTokenSpaceGuid.PcdPciExpressBaseAddress
  gSiPkgTokenSpaceGuid.PcdSiPciExpressRegionLength|gMinPlatformPkgTokenSpaceGuid.PcdPciExpressRegionLength
!else
  #
  # FSP Dispatch mode requires more platform memory as boot loader and FSP sharing the same
  # platform memory.
  #
  gSiPkgTokenSpaceGuid.PcdPeiMinMemorySize|0x5500000
!endif

[PcdsFeatureFlag.common]
  ######################################
  # Edk2 Configuration
  ######################################
  gEfiMdeModulePkgTokenSpaceGuid.PcdPeiCoreImageLoaderSearchTeSectionFirst|FALSE
  gUefiCpuPkgTokenSpaceGuid.PcdCpuSmmEnableBspElection|FALSE
  gUefiCpuPkgTokenSpaceGuid.PcdCpuSmmProfileEnable|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdInstallAcpiSdtProtocol|TRUE

  ######################################
  # Silicon Configuration
  ######################################
  # Build switches
  gSiPkgTokenSpaceGuid.PcdOptimizeCompilerEnable|TRUE

  # CPU
  gSiPkgTokenSpaceGuid.PcdBiosGuardEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdSourceDebugEnable|FALSE
  gSiPkgTokenSpaceGuid.PcdTxtEnable|FALSE

  # SA
  gSiPkgTokenSpaceGuid.PcdIgdEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdPegEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdSgEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdSaDmiEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdSkycamEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdGmmEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdSaOcEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdVtdEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdPeiDisplayEnable|TRUE

  # ME
  gSiPkgTokenSpaceGuid.PcdAmtEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdAtaEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdPttEnable|FALSE
  gSiPkgTokenSpaceGuid.PcdJhiEnable|TRUE

  # Others
  gSiPkgTokenSpaceGuid.PcdAcpiEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdBdatEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdBootGuardEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdCpuPowerOnConfigEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdEvLoaderEnable|FALSE
  gSiPkgTokenSpaceGuid.PcdIntegratedTouchEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdOcWdtEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdOverclockEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdPpmEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdS3Enable|TRUE
  gSiPkgTokenSpaceGuid.PcdSerialGpioEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdSiCatalogDebugEnable|FALSE
  gSiPkgTokenSpaceGuid.PcdSiCsmEnable|FALSE
  gSiPkgTokenSpaceGuid.PcdSmbiosEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdSmmVariableEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdSoftwareGuardEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdSsaFlagEnable|FALSE
  gSiPkgTokenSpaceGuid.PcdTraceHubEnable|TRUE

  ######################################
  # Platform Configuration
  ######################################
  #
  # MinPlatform common include for required feature PCD
  # These PCD must be set before the core include files, CoreCommonLib,
  # CorePeiLib, and CoreDxeLib.
  # Optional MinPlatformPkg features should be enabled after this
  #
  !include MinPlatformPkg/Include/Dsc/MinPlatformFeaturesPcd.dsc.inc

  #
  # Commonly used MinPlatform feature configuration logic that maps functionity to stage
  #
  !include BoardModulePkg/Include/Dsc/CommonStageConfig.dsc.inc

  ######################################
  # Board Configuration
  ######################################
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdMultiBoardSupport|TRUE
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdTbtEnable|FALSE

[PcdsFixedAtBuild.common]
  ######################################
  # Edk2 Configuration
  ######################################
!if $(TARGET) == RELEASE
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x0
  gEfiMdePkgTokenSpaceGuid.PcdReportStatusCodePropertyMask|0x3
!else
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x2F
  gEfiMdePkgTokenSpaceGuid.PcdReportStatusCodePropertyMask|0x07
!endif
  gEfiMdePkgTokenSpaceGuid.PcdPciExpressBaseAddress|0xE0000000
  gIntelSiliconPkgTokenSpaceGuid.PcdAcpiBaseAddress|0x1800
!if gMinPlatformPkgTokenSpaceGuid.PcdPerformanceEnable == TRUE
  gEfiMdePkgTokenSpaceGuid.PcdPerformanceLibraryPropertyMask|0x1
!endif

  gEfiMdeModulePkgTokenSpaceGuid.PcdAriSupport|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdBrowserFieldTextColor|0x01
  gEfiMdeModulePkgTokenSpaceGuid.PcdBrowserSubtitleTextColor|0x0
  gEfiMdeModulePkgTokenSpaceGuid.PcdHwErrStorageSize|0x00000800
  gEfiMdeModulePkgTokenSpaceGuid.PcdLoadModuleAtFixAddressEnable|$(TOP_MEMORY_ADDRESS)
  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxHardwareErrorVariableSize|0x400
!if gMinPlatformPkgTokenSpaceGuid.PcdPerformanceEnable == TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxPeiPerformanceLogEntries|140
!endif
  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxVariableSize|0x5000
  gEfiMdeModulePkgTokenSpaceGuid.PcdReclaimVariableSpaceAtEndOfDxe|TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdSrIovSupport|FALSE
!if $(TARGET) == DEBUG
  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialUseHardwareFlowControl|FALSE
!endif
  gEfiMdeModulePkgTokenSpaceGuid.PcdStatusCodeUseMemory|FALSE
!if $(TARGET) == RELEASE
  gEfiMdeModulePkgTokenSpaceGuid.PcdStatusCodeUseSerial|FALSE
!else
  gEfiMdeModulePkgTokenSpaceGuid.PcdStatusCodeUseSerial|TRUE
!endif

  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoBarEnableMask|0x80
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPciBarRegisterOffset|0x40
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPciBusNumber|0x0
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPciDeviceNumber|0x1F
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPciEnableRegisterOffset|0x44
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPciFunctionNumber|0x2
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPortBaseAddress|0x1800
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPortBaseAddressMask|0xFFFC
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiPm1TmrOffset|0x08

  # Specifies timeout value in microseconds for the BSP to detect all APs for the first time.
  gUefiCpuPkgTokenSpaceGuid.PcdCpuApInitTimeOutInMicroSeconds|1000
  gUefiCpuPkgTokenSpaceGuid.PcdCpuSmmApSyncTimeout|10000
  gUefiCpuPkgTokenSpaceGuid.PcdCpuSmmStackSize|0x20000
!if (gMinPlatformPkgTokenSpaceGuid.PcdFspWrapperBootMode == FALSE) || (gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection == 1)
  #
  # In non-FSP build (EDK2 build) or FSP API mode below PCD are FixedAtBuild
  # (They will be DynamicEx in FSP Dispatch mode)
  #
  ## Specifies max supported number of Logical Processors.
  # @Prompt Configure max supported number of Logical Processors
  gUefiCpuPkgTokenSpaceGuid.PcdCpuMaxLogicalProcessorNumber|12

  ## Specifies the size of the microcode Region.
  # @Prompt Microcode Region size.
  gUefiCpuPkgTokenSpaceGuid.PcdCpuMicrocodePatchRegionSize|0

  ## Specifies the AP wait loop state during POST phase.
  #  The value is defined as below.
  #  1: Place AP in the Hlt-Loop state.
  #  2: Place AP in the Mwait-Loop state.
  #  3: Place AP in the Run-Loop state.
  # @Prompt The AP wait loop state.
  gUefiCpuPkgTokenSpaceGuid.PcdCpuApLoopMode|2
!endif

  ######################################
  # Silicon Configuration
  ######################################

  # Refer to HstiFeatureBit.h for bit definitions
  gSiPkgTokenSpaceGuid.PcdHstiIhvFeature1|0xF2
  gSiPkgTokenSpaceGuid.PcdHstiIhvFeature2|0x07

  #
  # Set the location of the DUTY_CYCLE field in the P_CNT register
  # and indicate the width of the clock duty cycle to OS power management
  #
  gMinPlatformPkgTokenSpaceGuid.PcdFadtDutyOffset|0x1
  gMinPlatformPkgTokenSpaceGuid.PcdFadtDutyWidth|0x3

  ######################################
  # Platform Configuration
  ######################################
  gMinPlatformPkgTokenSpaceGuid.PcdMaxCpuSocketCount|1
  gMinPlatformPkgTokenSpaceGuid.PcdMaxCpuCoreCount|8
  gMinPlatformPkgTokenSpaceGuid.PcdMaxCpuThreadCount|2
  gMinPlatformPkgTokenSpaceGuid.PcdPciExpressRegionLength|0x10000000
  gIntelFsp2WrapperTokenSpaceGuid.PcdPeiMinMemSize|0x3800000

  #
  # The PCDs are used to control the Windows SMM Security Mitigations Table - Protection Flags
  #
  # BIT0: If set, expresses that for all synchronous SMM entries,SMM will validate that input and output buffers lie entirely within the expected fixed memory regions.
  # BIT1: If set, expresses that for all synchronous SMM entries, SMM will validate that input and output pointers embedded within the fixed communication buffer only refer to address ranges \
  #       that lie entirely within the expected fixed memory regions.
  # BIT2: Firmware setting this bit is an indication that it will not allow reconfiguration of system resources via non-architectural mechanisms.
  # BIT3-31: Reserved
  #
  gMinPlatformPkgTokenSpaceGuid.PcdWsmtProtectionFlags|0x07

!if $(TARGET) == RELEASE
  gMinPlatformPkgTokenSpaceGuid.PcdPlatformEfiReservedMemorySize|0x402
!else
  gMinPlatformPkgTokenSpaceGuid.PcdPlatformEfiReservedMemorySize|0x188B
!endif

  gMinPlatformPkgTokenSpaceGuid.PcdPlatformEfiRtDataMemorySize|0x4b
!if $(TARGET) == RELEASE
  gMinPlatformPkgTokenSpaceGuid.PcdPlatformEfiRtCodeMemorySize|0x70
!else
  gMinPlatformPkgTokenSpaceGuid.PcdPlatformEfiRtCodeMemorySize|0xE0
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdBootStage == 1
  gMinPlatformPkgTokenSpaceGuid.PcdTestPointIbvPlatformFeature|{0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdBootStage == 2
  gMinPlatformPkgTokenSpaceGuid.PcdTestPointIbvPlatformFeature|{0x03, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdBootStage == 3
  gMinPlatformPkgTokenSpaceGuid.PcdTestPointIbvPlatformFeature|{0x03, 0x07, 0x03, 0x05, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdBootStage == 4
  gMinPlatformPkgTokenSpaceGuid.PcdTestPointIbvPlatformFeature|{0x03, 0x07, 0x03, 0x05, 0x1F, 0x00, 0x0F, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdBootStage == 5
  gMinPlatformPkgTokenSpaceGuid.PcdTestPointIbvPlatformFeature|{0x03, 0x0F, 0x07, 0x1F, 0x1F, 0x0F, 0x0F, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdBootStage >= 6
  gMinPlatformPkgTokenSpaceGuid.PcdTestPointIbvPlatformFeature|{0x03, 0x0F, 0x07, 0x1F, 0x1F, 0x0F, 0x0F, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
!endif

  ######################################
  # Board Configuration
  ######################################
  gBoardModulePkgTokenSpaceGuid.PcdPs2KbMsEnable|1
  gBoardModulePkgTokenSpaceGuid.PcdSuperIoPciIsaBridgeDevice|{0x00, 0x00, 0x1F, 0x00}
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdGttMmAddress|0xDF000000

  ## Enable usage the HDMI DDC channel as a debug port - Causes the BIOS debug log
  #  to be written to the HDMI DDC channel.
  #  The value is defined as below.
  #  FALSE: Do NOT use the HDMI DDC channel as a debug port
  #  TRUE:  Use the HDMI DDC channel as a debug port
  # @Prompt Enable usage the HDMI DDC channel as a debug port
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdI2cHdmiDebugPortEnable|FALSE

  ## Enable usage the HDMI DDC channel as a serial terminal - Enables usage of the
  #  HDMI DDC channel to display BIOS Setup, UEFI Shell, etc. using a terminal
  #  emulator. Useful for cases where video is not operating correctly.
  #
  #  The value is defined as below.
  #  FALSE: Do NOT use the HDMI DDC channel as a debug port
  #  TRUE:  Use the HDMI DDC channel as a debug port
  # @Prompt Enable usage the HDMI DDC channel as a debug port
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdI2cHdmiDebugPortSerialTerminalEnable|FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdSerialTerminalEnable|gKabylakeOpenBoardPkgTokenSpaceGuid.PcdI2cHdmiDebugPortSerialTerminalEnable

  ## Indicates the type of terminal to use.
  #  If PcdI2cHdmiDebugPortSerialTerminalEnable is TRUE, this PCD will be used
  #  to determine which terminal protocol to use.
  #  0 - PCANSI
  #  1 - VT100
  #  2 - VT100+
  #  3 - UTF8
  #  4 - TTYTERM
  # @Prompt Default Terminal Type.
  # @ValidRange 0x80000001 | 0 - 4
  gEfiMdePkgTokenSpaceGuid.PcdDefaultTerminalType|3

  ## Specifies the DDC I2C channel to claim as the HDMI debug port
  #  The value is defined as below.
  #  2: DDC channel B
  #  3: DDC channel C
  #  4: DDC channel D
  # @Prompt DDC I2C channel to claim as the HDMI debug port
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdI2cHdmiDebugPortDdcI2cChannel|0x03

[PcdsFixedAtBuild.IA32]
  ######################################
  # Edk2 Configuration
  ######################################
  gEfiMdeModulePkgTokenSpaceGuid.PcdVpdBaseAddress|0x0
  gIntelFsp2PkgTokenSpaceGuid.PcdGlobalDataPointerAddress|0xFED00148

  ######################################
  # Platform Configuration
  ######################################
  gMinPlatformPkgTokenSpaceGuid.PcdPeiPhaseStackTop|0xA0000

[PcdsFixedAtBuild.X64]
  ######################################
  # Edk2 Configuration
  ######################################

  # Default platform supported RFC 4646 languages: (American) English
  gEfiMdePkgTokenSpaceGuid.PcdUefiVariableDefaultPlatformLangCodes|"en-US"

[PcdsPatchableInModule.common]
  ######################################
  # Edk2 Configuration
  ######################################
  gEfiMdeModulePkgTokenSpaceGuid.PcdSmbiosVersion|0x0208
  gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x80000046

  ######################################
  # Silicon Configuration
  ######################################
!if $(TARGET) == DEBUG
  gSiPkgTokenSpaceGuid.PcdSerialIoUartDebugEnable|1
!endif

[PcdsDynamicDefault]
  ######################################
  # Edk2 Configuration
  ######################################
  gEfiMdeModulePkgTokenSpaceGuid.PcdAtaSmartEnable|TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdConInConnectOnDemand|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutColumn|0x0
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutRow|0x0
  gEfiMdeModulePkgTokenSpaceGuid.PcdS3BootScriptTablePrivateDataPtr|0x0

  #
  #  Set video to native resolution as Windows 8 WHCK requirement.
  #
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|0x0
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|0x0

  gEfiSecurityPkgTokenSpaceGuid.PcdTcg2HashAlgorithmBitmap|0
  gEfiSecurityPkgTokenSpaceGuid.PcdTpm2HashMask|0x0000001F
  gEfiSecurityPkgTokenSpaceGuid.PcdTpmInitializationPolicy|1
  gEfiSecurityPkgTokenSpaceGuid.PcdTpmInstanceGuid|{0x5a, 0xf2, 0x6b, 0x28, 0xc3, 0xc2, 0x8c, 0x40, 0xb3, 0xb4, 0x25, 0xe6, 0x75, 0x8b, 0x73, 0x17}

  #
  # FSP Base address PCD will be updated in FDF basing on flash map.
  #
  gIntelFsp2WrapperTokenSpaceGuid.PcdFspsBaseAddress|0
  # Platform will pre-allocate UPD buffer and pass it to FspWrapper
  # Those dummy address will be patched before FspWrapper executing
  gIntelFsp2WrapperTokenSpaceGuid.PcdFspmUpdDataAddress|0xFFFFFFFF
  gIntelFsp2WrapperTokenSpaceGuid.PcdFspsUpdDataAddress|0xFFFFFFFF

  ######################################
  # Board Configuration
  ######################################

  # Thunderbolt Configuration
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdDTbtAcDcSwitch|0x0
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdDTbtAcpiGpeSignature|0
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdDTbtAcpiGpeSignaturePorting|0
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdDTbtAspm|0x0
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdDTbtCioPlugEventGpioPad|0x02010011
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdDTbtControllerEn|0x1
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdDTbtControllerType|0x1
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdDTbtForcepowerGpioPad|13
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdDTbtGpioAccessType|0x2
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdDTbtGpioLevel|0x1
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdDTbtHotNotify|0x1
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdDTbtHotSMI|0x1
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdDTbtPcieExtraBusRsvd|56
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdDTbtPcieMemAddrRngMax|26
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdDTbtPcieMemRsvd|100
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdDTbtPciePMemAddrRngMax|28
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdDTbtPciePMemRsvd|100
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdDTbtPcieRpNumber|0x1
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdDTbtSecurityMode|0x1
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdDTbtSetClkReq|0x1
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdDTbtWakeupSupport|0x0
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdDTbtWin10Support|0x0
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdExpander|0x0
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdPchPcieRootPortHpe|0x00000001
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdRtd3Tbt|0x1
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdRtd3TbtClkReq|0x1
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdRtd3TbtClkReqDelay|0x0
  gKabylakeOpenBoardPkgTokenSpaceGuid.PcdRtd3TbtOffDelay|5000

[PcdsDynamicHii.X64.DEFAULT]
  ######################################
  # Edk2 Configuration
  ######################################
  gEfiMdePkgTokenSpaceGuid.PcdHardwareErrorRecordLevel|L"HwErrRecSupport"|gEfiGlobalVariableGuid|0x0|1 # Variable: L"HwErrRecSupport"
!if gMinPlatformPkgTokenSpaceGuid.PcdPerformanceEnable == TRUE
  gEfiMdePkgTokenSpaceGuid.PcdPlatformBootTimeOut|L"Timeout"|gEfiGlobalVariableGuid|0x0|1 # Variable: L"Timeout"
!else
  gEfiMdePkgTokenSpaceGuid.PcdPlatformBootTimeOut|L"Timeout"|gEfiGlobalVariableGuid|0x0|5 # Variable: L"Timeout"
!endif
