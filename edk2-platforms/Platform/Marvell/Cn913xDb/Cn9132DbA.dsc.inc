## @file
#  Component description file for the CN9132 Development Board (variant A)
#
#  Copyright (c) 2019 Marvell International Ltd.<BR>
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

################################################################################
#
# Pcd Section - list of all EDK II PCD Entries defined by this Platform
#
################################################################################
[PcdsFixedAtBuild.common]
  # CP115 count
  gMarvellSiliconTokenSpaceGuid.PcdMaxCpCount|3

  # MPP
  gMarvellSiliconTokenSpaceGuid.PcdMppChipCount|4

  # CP115 #2 MPP
  gMarvellSiliconTokenSpaceGuid.PcdChip3MppReverseFlag|FALSE
  gMarvellSiliconTokenSpaceGuid.PcdChip3MppBaseAddress|0xF6440000
  gMarvellSiliconTokenSpaceGuid.PcdChip3MppPinCount|64
  gMarvellSiliconTokenSpaceGuid.PcdChip3MppSel0|{ 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 }
  gMarvellSiliconTokenSpaceGuid.PcdChip3MppSel1|{ 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 }
  gMarvellSiliconTokenSpaceGuid.PcdChip3MppSel2|{ 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x9, 0x9, 0x0 }
  gMarvellSiliconTokenSpaceGuid.PcdChip3MppSel3|{ 0x0, 0x0, 0x8, 0x0, 0x8, 0x0, 0x0, 0x2, 0x2, 0x0 }
  gMarvellSiliconTokenSpaceGuid.PcdChip3MppSel4|{ 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 }
  gMarvellSiliconTokenSpaceGuid.PcdChip3MppSel5|{ 0x0, 0x0, 0x0, 0x0, 0xA, 0xB, 0xE, 0xE, 0xE, 0xE }
  gMarvellSiliconTokenSpaceGuid.PcdChip3MppSel6|{ 0xE, 0xE, 0xE, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 }

  # ComPhy
  gMarvellSiliconTokenSpaceGuid.PcdComPhyDevices|{ 0x1, 0x1, 0x1 }
  # ComPhy1
  # 0: PCIE0         5 Gbps
  # 1: PCIE0         5 Gbps
  # 2: SATA0         5 Gbps
  # 3: USB3_HOST1    5 Gbps
  # 4: SFI           10.31 Gbps
  # 5: PCIE2         5 Gbps
  gMarvellSiliconTokenSpaceGuid.PcdChip2ComPhyTypes|{ $(CP_PCIE0), $(CP_PCIE0), $(CP_SATA0), $(CP_USB3_HOST1), $(CP_SFI), $(CP_PCIE2)}
  gMarvellSiliconTokenSpaceGuid.PcdChip2ComPhySpeeds|{ $(CP_5G), $(CP_5G), $(CP_5G), $(CP_5G), $(CP_10_3125G), $(CP_5G) }

  # UtmiPhy
  gMarvellSiliconTokenSpaceGuid.PcdUtmiControllersEnabled|{ 0x1, 0x1, 0x0, 0x1, 0x1, 0x1 }
  gMarvellSiliconTokenSpaceGuid.PcdUtmiPortType|{ $(UTMI_USB_HOST0), $(UTMI_USB_HOST1), $(UTMI_USB_HOST0), $(UTMI_USB_HOST1), $(UTMI_USB_HOST0), $(UTMI_USB_HOST1) }

  # MDIO
  gMarvellSiliconTokenSpaceGuid.PcdMdioControllersEnabled|{ 0x1, 0x0 }

  # PHY
  gMarvellSiliconTokenSpaceGuid.PcdPhy2MdioController|{ 0x0, 0x0 }
  gMarvellSiliconTokenSpaceGuid.PcdPhyDeviceIds|{ 0x0, 0x0 }
  gMarvellSiliconTokenSpaceGuid.PcdPhySmiAddresses|{ 0x0, 0x1 }
  gMarvellSiliconTokenSpaceGuid.PcdPhyStartupAutoneg|FALSE

  # NET
  gMarvellSiliconTokenSpaceGuid.PcdPp2GopIndexes|{ 0x0, 0x2, 0x3, 0x0, 0x0 }
  gMarvellSiliconTokenSpaceGuid.PcdPp2InterfaceAlwaysUp|{ 0x0, 0x0, 0x0, 0x0, 0x0 }
  gMarvellSiliconTokenSpaceGuid.PcdPp2InterfaceSpeed|{ $(PHY_SPEED_10000), $(PHY_SPEED_1000), $(PHY_SPEED_1000), $(PHY_SPEED_10000), $(PHY_SPEED_10000) }
  gMarvellSiliconTokenSpaceGuid.PcdPp2PhyConnectionTypes|{ $(PHY_SFI), $(PHY_RGMII), $(PHY_RGMII), $(PHY_SFI), $(PHY_SFI) }
  gMarvellSiliconTokenSpaceGuid.PcdPp2PhyIndexes|{ 0xFF, 0x0, 0x1, 0xFF, 0xFF }
  gMarvellSiliconTokenSpaceGuid.PcdPp2Port2Controller|{ 0x0, 0x0, 0x0, 0x1, 0x2 }
  gMarvellSiliconTokenSpaceGuid.PcdPp2PortIds|{ 0x0, 0x1, 0x2, 0x0, 0x0 }
  gMarvellSiliconTokenSpaceGuid.PcdPp2Controllers|{ 0x1, 0x1, 0x1 }

  # NonDiscoverableDevices
  gMarvellSiliconTokenSpaceGuid.PcdPciEXhci|{ 0x1, 0x1, 0x0, 0x1, 0x1, 0x1 }
  gMarvellSiliconTokenSpaceGuid.PcdPciEAhci|{ 0x1, 0x1, 0x1 }
  gMarvellSiliconTokenSpaceGuid.PcdPciESdhci|{ 0x1, 0x1, 0x0, 0x1 }
