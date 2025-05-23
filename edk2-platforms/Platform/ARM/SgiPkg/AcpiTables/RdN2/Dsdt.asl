/** @file
*  Differentiated System Description Table Fields (DSDT)
*
*  Copyright (c) 2020-2021, Arm Ltd. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include "SgiPlatform.h"
#include "SgiAcpiHeader.h"

DefinitionBlock ("DsdtTable.aml", "DSDT", 2, "ARMLTD", "ARMSGI",
                 EFI_ACPI_ARM_OEM_REVISION) {
  Scope (_SB) {
    /* _OSC: Operating System Capabilities */
    Method (_OSC, 4, Serialized) {
      CreateDWordField (Arg3, 0x00, STS0)
      CreateDWordField (Arg3, 0x04, CAP0)

      /* Platform-wide Capabilities */
      If (LEqual (Arg0, ToUUID("0811b06e-4a27-44f9-8d60-3cbbc22e7b48"))) {
        /* OSC rev 1 supported, for other version, return failure */
        If (LEqual (Arg1, One)) {
          And (STS0, Not (OSC_STS_MASK), STS0)

          If (And (CAP0, OSC_CAP_OS_INITIATED_LPI)) {
            /* OS initiated LPI not supported */
            And (CAP0, Not (OSC_CAP_OS_INITIATED_LPI), CAP0)
            Or (STS0, OSC_STS_CAPABILITY_MASKED, STS0)
          }

          If (And (CAP0, OSC_CAP_PLAT_COORDINATED_LPI)) {
            if (LEqual (FixedPcdGet32 (PcdOscLpiEnable), Zero)) {
              And (CAP0, Not (OSC_CAP_PLAT_COORDINATED_LPI), CAP0)
              Or (STS0, OSC_STS_CAPABILITY_MASKED, STS0)
            }
          }

          If (And (CAP0, OSC_CAP_CPPC_SUPPORT)) {
            /* CPPC revision 1 and below not supported */
            And (CAP0, Not (OSC_CAP_CPPC_SUPPORT), CAP0)
            Or (STS0, OSC_STS_CAPABILITY_MASKED, STS0)
          }

          If (And (CAP0, OSC_CAP_CPPC2_SUPPORT)) {
            if (LEqual (FixedPcdGet32 (PcdOscCppcEnable), Zero)) {
              And (CAP0, Not (OSC_CAP_CPPC2_SUPPORT), CAP0)
              Or (STS0, OSC_STS_CAPABILITY_MASKED, STS0)
            }
          }

        } Else {
          And (STS0, Not (OSC_STS_MASK), STS0)
          Or (STS0, Or (OSC_STS_FAILURE, OSC_STS_UNRECOGNIZED_REV), STS0)
        }
      } Else {
        And (STS0, Not (OSC_STS_MASK), STS0)
        Or (STS0, Or (OSC_STS_FAILURE, OSC_STS_UNRECOGNIZED_UUID), STS0)
      }

      Return (Arg3)
    }

    Name (PLPI, Package () {  /* LPI for Processor, support 2 LPI states */
      0,                      // Version
      0,                      // Level Index
      2,                      // Count
      Package () {            // WFI for CPU
        1,                    // Min residency (uS)
        1,                    // Wake latency (uS)
        1,                    // Flags
        0,                    // Arch Context lost Flags (no loss)
        0,                    // Residency Counter Frequency
        0,                    // No parent state
        ResourceTemplate () { // Register Entry method
          Register (FFixedHW,
            32,               // Bit Width
            0,                // Bit Offset
            0xFFFFFFFF,       // Address
            3,                // Access Size
          )
        },
        ResourceTemplate () { // Null Residency Counter
          Register (SystemMemory, 0, 0, 0, 0)
        },
        ResourceTemplate () { // Null Usage Counter
          Register (SystemMemory, 0, 0, 0, 0)
        },
        "LPI1-Core"
      },
      Package () {            // Power Gating state for CPU
        150,                  // Min residency (uS)
        350,                  // Wake latency (uS)
        1,                    // Flags
        1,                    // Arch Context lost Flags (Core context lost)
        0,                    // Residency Counter Frequency
        0,                    // No parent state
        ResourceTemplate () { // Register Entry method
          Register (FFixedHW,
            32,               // Bit Width
            0,                // Bit Offset
            0x40000002,       // Address (PwrLvl:core, StateTyp:PwrDn)
            3,                // Access Size
          )
        },
        ResourceTemplate () { // Null Residency Counter
          Register (SystemMemory, 0, 0, 0, 0)
        },
        ResourceTemplate () { // Null Usage Counter
          Register (SystemMemory, 0, 0, 0, 0)
        },
        "LPI3-Core"
      },
    })

    Device (CL00) {   // Cluster 0
      Name (_HID, "ACPI0010")
      Name (_UID, 0)

      Device (CP00) { // Neoverse N2 core 0
        Name (_HID, "ACPI0007")
        Name (_UID, 0)
        Name (_STA, 0xF)

        Name (_CPC, Package()
          CPPC_PACKAGE_INIT (0x06000500, 0x06000504, 20, 160, 160, 115, 115, 5)
        )

        Name (_PSD, Package () {
          Package ()
            PSD_INIT (0)
        })

        Method (_LPI, 0, NotSerialized) {
          Return (\_SB.PLPI)
        }
      }
    }

    Device (CL01) {   // Cluster 1
      Name (_HID, "ACPI0010")
      Name (_UID, 1)

      Device (CP01) { // Neoverse N2 core 1
        Name (_HID, "ACPI0007")
        Name (_UID, 1)
        Name (_STA, 0xF)

        Name (_CPC, Package()
          CPPC_PACKAGE_INIT (0x06000518, 0x0600051C, 20, 160, 160, 115, 115, 5)
        )

        Name (_PSD, Package () {
          Package ()
            PSD_INIT (1)
        })

        Method (_LPI, 0, NotSerialized) {
          Return (\_SB.PLPI)
        }
      }
    }

    Device (CL02) {   // Cluster 2
      Name (_HID, "ACPI0010")
      Name (_UID, 2)

      Device (CP02) { // Neoverse N2 core 2
        Name (_HID, "ACPI0007")
        Name (_UID, 2)
        Name (_STA, 0xF)

        Name (_CPC, Package()
          CPPC_PACKAGE_INIT (0x06000530, 0x06000534, 20, 160, 160, 115, 115, 5)
        )

        Name (_PSD, Package () {
          Package ()
            PSD_INIT (2)
        })

        Method (_LPI, 0, NotSerialized) {
          Return (\_SB.PLPI)
        }
      }
    }

    Device (CL03) {   // Cluster 3
      Name (_HID, "ACPI0010")
      Name (_UID, 3)

      Device (CP03) { // Neoverse N2 core 3
        Name (_HID, "ACPI0007")
        Name (_UID, 3)
        Name (_STA, 0xF)

        Name (_CPC, Package()
          CPPC_PACKAGE_INIT (0x06000548, 0x0600054C, 20, 160, 160, 115, 115, 5)
        )

        Name (_PSD, Package () {
          Package ()
            PSD_INIT (3)
        })

        Method (_LPI, 0, NotSerialized) {
          Return (\_SB.PLPI)
        }
      }
    }

    Device (CL04) {   // Cluster 4
      Name (_HID, "ACPI0010")
      Name (_UID, 4)

      Device (CP04) { // Neoverse N2 core 4
        Name (_HID, "ACPI0007")
        Name (_UID, 4)
        Name (_STA, 0xF)

        Name (_CPC, Package()
          CPPC_PACKAGE_INIT (0x06000560, 0x06000564, 20, 160, 160, 115, 115, 5)
        )

        Name (_PSD, Package () {
          Package ()
            PSD_INIT (4)
        })

        Method (_LPI, 0, NotSerialized) {
          Return (\_SB.PLPI)
        }
      }
    }

    Device (CL05) {   // Cluster 5
      Name (_HID, "ACPI0010")
      Name (_UID, 5)

      Device (CP05) { // Neoverse N2 core 5
        Name (_HID, "ACPI0007")
        Name (_UID, 5)
        Name (_STA, 0xF)

        Name (_CPC, Package()
          CPPC_PACKAGE_INIT (0x06000578, 0x0600057C, 20, 160, 160, 115, 115, 5)
        )

        Name (_PSD, Package () {
          Package ()
            PSD_INIT (5)
        })

        Method (_LPI, 0, NotSerialized) {
          Return (\_SB.PLPI)
        }
      }
    }

    Device (CL06) {   // Cluster 6
      Name (_HID, "ACPI0010")
      Name (_UID, 6)

      Device (CP06) { // Neoverse N2 core 6
        Name (_HID, "ACPI0007")
        Name (_UID, 6)
        Name (_STA, 0xF)

        Name (_CPC, Package()
          CPPC_PACKAGE_INIT (0x06000590, 0x06000594, 20, 160, 160, 115, 115, 5)
        )

        Name (_PSD, Package () {
          Package ()
            PSD_INIT (6)
        })

        Method (_LPI, 0, NotSerialized) {
          Return (\_SB.PLPI)
        }
      }
    }

    Device (CL07) {   // Cluster 7
      Name (_HID, "ACPI0010")
      Name (_UID, 7)

      Device (CP07) { // Neoverse N2 core 7
        Name (_HID, "ACPI0007")
        Name (_UID, 7)
        Name (_STA, 0xF)

        Name (_CPC, Package()
          CPPC_PACKAGE_INIT (0x060005A8, 0x060005AC, 20, 160, 160, 115, 115, 5)
        )

        Name (_PSD, Package () {
          Package ()
            PSD_INIT (7)
        })

        Method (_LPI, 0, NotSerialized) {
          Return (\_SB.PLPI)
        }
      }
    }

    Device (CL08) {   // Cluster 8
      Name (_HID, "ACPI0010")
      Name (_UID, 8)

      Device (CP08) { // Neoverse N2 core 8
        Name (_HID, "ACPI0007")
        Name (_UID, 8)
        Name (_STA, 0xF)

        Name (_CPC, Package()
          CPPC_PACKAGE_INIT (0x060005C0, 0x060005C4, 20, 160, 160, 115, 115, 5)
        )

        Name (_PSD, Package () {
          Package ()
            PSD_INIT (8)
        })

        Method (_LPI, 0, NotSerialized) {
          Return (\_SB.PLPI)
        }
      }
    }

    Device (CL09) {   // Cluster 9
      Name (_HID, "ACPI0010")
      Name (_UID, 9)

      Device (CP09) { // Neoverse N2 core 9
        Name (_HID, "ACPI0007")
        Name (_UID, 9)
        Name (_STA, 0xF)

        Name (_CPC, Package()
          CPPC_PACKAGE_INIT (0x060005D8, 0x060005DC, 20, 160, 160, 115, 115, 5)
        )

        Name (_PSD, Package () {
          Package ()
            PSD_INIT (9)
        })

        Method (_LPI, 0, NotSerialized) {
          Return (\_SB.PLPI)
        }
      }
    }

    Device (CL10) {   // Cluster 10
      Name (_HID, "ACPI0010")
      Name (_UID, 10)

      Device (CP10) { // Neoverse N2 core 10
        Name (_HID, "ACPI0007")
        Name (_UID, 10)
        Name (_STA, 0xF)

        Name (_CPC, Package()
          CPPC_PACKAGE_INIT (0x060005F0, 0x060005F4, 20, 160, 160, 115, 115, 5)
        )

        Name (_PSD, Package () {
          Package ()
            PSD_INIT (10)
        })

        Method (_LPI, 0, NotSerialized) {
          Return (\_SB.PLPI)
        }
      }
    }

    Device (CL11) {   // Cluster 11
      Name (_HID, "ACPI0010")
      Name (_UID, 11)

      Device (CP11) { // Neoverse N2 core 11
        Name (_HID, "ACPI0007")
        Name (_UID, 11)
        Name (_STA, 0xF)

        Name (_CPC, Package()
          CPPC_PACKAGE_INIT (0x06000608, 0x0600060C, 20, 160, 160, 115, 115, 5)
        )

        Name (_PSD, Package () {
          Package ()
            PSD_INIT (11)
        })

        Method (_LPI, 0, NotSerialized) {
          Return (\_SB.PLPI)
        }
      }
    }

    Device (CL12) {   // Cluster 12
      Name (_HID, "ACPI0010")
      Name (_UID, 12)

      Device (CP12) { // Neoverse N2 core 12
        Name (_HID, "ACPI0007")
        Name (_UID, 12)
        Name (_STA, 0xF)

        Name (_CPC, Package()
          CPPC_PACKAGE_INIT (0x06000620, 0x06000624, 20, 160, 160, 115, 115, 5)
        )

        Name (_PSD, Package () {
          Package ()
            PSD_INIT (12)
        })

        Method (_LPI, 0, NotSerialized) {
          Return (\_SB.PLPI)
        }
      }
    }

    Device (CL13) {   // Cluster 13
      Name (_HID, "ACPI0010")
      Name (_UID, 13)

      Device (CP13) { // Neoverse N2 core 13
        Name (_HID, "ACPI0007")
        Name (_UID, 13)
        Name (_STA, 0xF)

        Name (_CPC, Package()
          CPPC_PACKAGE_INIT (0x06000638, 0x0600063C, 20, 160, 160, 115, 115, 5)
        )

        Name (_PSD, Package () {
          Package ()
            PSD_INIT (13)
        })

        Method (_LPI, 0, NotSerialized) {
          Return (\_SB.PLPI)
        }
      }
    }

    Device (CL14) {   // Cluster 14
      Name (_HID, "ACPI0010")
      Name (_UID, 14)

      Device (CP14) { // Neoverse N2 core 14
        Name (_HID, "ACPI0007")
        Name (_UID, 14)
        Name (_STA, 0xF)

        Name (_CPC, Package()
          CPPC_PACKAGE_INIT (0x06000650, 0x06000654, 20, 160, 160, 115, 115, 5)
        )

        Name (_PSD, Package () {
          Package ()
            PSD_INIT (14)
        })

        Method (_LPI, 0, NotSerialized) {
          Return (\_SB.PLPI)
        }
      }
    }

    Device (CL15) {   // Cluster 15
      Name (_HID, "ACPI0010")
      Name (_UID, 15)

      Device (CP15) { // Neoverse N2 core 15
        Name (_HID, "ACPI0007")
        Name (_UID, 15)
        Name (_STA, 0xF)

        Name (_CPC, Package()
          CPPC_PACKAGE_INIT (0x06000668, 0x0600066C, 20, 160, 160, 115, 115, 5)
        )

        Name (_PSD, Package () {
          Package ()
            PSD_INIT (15)
        })

        Method (_LPI, 0, NotSerialized) {
          Return (\_SB.PLPI)
        }
      }
    }
  } // Scope(_SB)
}
