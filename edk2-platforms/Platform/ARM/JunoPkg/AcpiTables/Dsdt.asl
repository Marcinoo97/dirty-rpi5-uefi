/** @file
  Differentiated System Description Table Fields (DSDT)

  Copyright (c) 2014-2021, ARM Ltd. All rights reserved.<BR>
    SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "ArmPlatform.h"

DefinitionBlock("DsdtTable.aml", "DSDT", 2, "ARMLTD", "ARM-JUNO", EFI_ACPI_ARM_OEM_REVISION) {
  Scope(_SB) {
    //
    // A57x2-A53x4 Processor declaration
    //
    Method (_OSC, 4, Serialized)  { // _OSC: Operating System Capabilities
      CreateDWordField (Arg3, 0x00, STS0)
      CreateDWordField (Arg3, 0x04, CAP0)
      If ((Arg0 == ToUUID ("0811b06e-4a27-44f9-8d60-3cbbc22e7b48") /* Platform-wide Capabilities */)) {
        If (!(Arg1 == One)) {
          STS0 &= ~0x1F
          STS0 |= 0x0A
        } Else {
          If ((CAP0 & 0x100)) {
            CAP0 &= ~0x100 /* No support for OS Initiated LPI */
            STS0 &= ~0x1F
            STS0 |= 0x12
          }
        }
      } Else {
        STS0 &= ~0x1F
        STS0 |= 0x06
      }
      Return (Arg3)
    }
    Device (CLU0) { // Cluster0 state
      Name(_HID, "ACPI0010")
      Name(_UID, 1)
      Name (_LPI, Package() {
        0, // Version
        0, // Level Index
        1, // Count
        Package() { // Power Gating state for Cluster
          2500, // Min residency (uS)
          1150, // Wake latency (uS)
          1, // Flags
          1, // Arch Context Flags
          100, //Residency Counter Frequency
          0, // No Parent State
          0x01000000, // Integer Entry method
          ResourceTemplate() { // Null Residency Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          ResourceTemplate() { // Null Usage Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          "CluPwrDn"
        },
      })
      Name(PLPI, Package() {
        0, // Version
        0, // Level Index
        2, // Count
        Package() { // WFI for CPU
          1, // Min residency (uS)
          1, // Wake latency (uS)
          1, // Flags
          0, // Arch Context Flags
          100, //Residency Counter Frequency
          0, // No parent state
          ResourceTemplate () {
            // Register Entry method
            Register (FFixedHW,
              0x20,               // Bit Width
              0x00,               // Bit Offset
              0xFFFFFFFF,         // Address
              0x03,               // Access Size
              )
          },
          ResourceTemplate() { // Null Residency Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          ResourceTemplate() { // Null Usage Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          "WFI",
        },
        Package() { // Power Gating state for CPU
          150, // Min residency (uS)
          350, // Wake latency (uS)
          1, // Flags
          1, // Arch Context Flags
          100, //Residency Counter Frequency
          1, // Parent node can be in any state
          ResourceTemplate () {
            // Register Entry method
            Register (FFixedHW,
              0x20,               // Bit Width
              0x00,               // Bit Offset
              0x00010000,         // Address
              0x03,               // Access Size
              )
          },
          ResourceTemplate() { // Null Residency Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          ResourceTemplate() { // Null Usage Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          "CorePwrDn"
        },
      })
      Device(CPU0) { // A57-0: Cluster 0, Cpu 0
        Name(_HID, "ACPI0007")
        Name(_UID, 4)
        Method (_LPI, 0, NotSerialized) {
          return(PLPI)
        }
      }
      Device(CPU1) { // A57-1: Cluster 0, Cpu 1
        Name(_HID, "ACPI0007")
        Name(_UID, 5)
        Method (_LPI, 0, NotSerialized) {
          return(PLPI)
        }
      }
    }
    Device (CLU1) { // Cluster1 state
      Name(_HID, "ACPI0010")
      Name(_UID, 2)
      Name (_LPI, Package() {
        0, // Version
        0, // Level Index
        1, // Count
        Package() { // Power Gating state for Cluster
          2500, // Min residency (uS)
          1150, // Wake latency (uS)
          1, // Flags
          1, // Arch Context Flags
          100, //Residency Counter Frequency
          0, // No Parent State
          0x01000000, // Integer Entry method
          ResourceTemplate() { // Null Residency Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          ResourceTemplate() { // Null Usage Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          "CluPwrDn"
        },
      })
      Name(PLPI, Package() {
        0, // Version
        0, // Level Index
        2, // Count
        Package() { // WFI for CPU
          1, // Min residency (uS)
          1, // Wake latency (uS)
          1, // Flags
          0, // Arch Context Flags
          100, //Residency Counter Frequency
          0, // No parent state
          ResourceTemplate () {
            // Register Entry method
            Register (FFixedHW,
              0x20,               // Bit Width
              0x00,               // Bit Offset
              0xFFFFFFFF,         // Address
              0x03,               // Access Size
              )
          },
          ResourceTemplate() { // Null Residency Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          ResourceTemplate() { // Null Usage Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          "WFI",
        },
        Package() { // Power Gating state for CPU
          150, // Min residency (uS)
          350, // Wake latency (uS)
          1, // Flags
          1, // Arch Context Flags
          100, //Residency Counter Frequency
          1, // Parent node can be in any state
          ResourceTemplate () {
            // Register Entry method
            Register (FFixedHW,
              0x20,               // Bit Width
              0x00,               // Bit Offset
              0x00010000,         // Address
              0x03,               // Access Size
              )
          },
          ResourceTemplate() { // Null Residency Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          ResourceTemplate() { // Null Usage Counter
            Register (SystemMemory, 0, 0, 0, 0)
          },
          "CorePwrDn"
        },
      })
      Device(CPU2) { // A53-0: Cluster 1, Cpu 0
        Name(_HID, "ACPI0007")
        Name(_UID, 0)
        Method (_LPI, 0, NotSerialized) {
          return(PLPI)
        }
      }
      Device(CPU3) { // A53-1: Cluster 1, Cpu 1
        Name(_HID, "ACPI0007")
        Name(_UID, 1)
        Method (_LPI, 0, NotSerialized) {
          return(PLPI)
        }
      }
      Device(CPU4) { // A53-2: Cluster 1, Cpu 2
        Name(_HID, "ACPI0007")
        Name(_UID, 2)
        Method (_LPI, 0, NotSerialized) {
          return(PLPI)
        }
      }
      Device(CPU5) { // A53-3: Cluster 1, Cpu 3
        Name(_HID, "ACPI0007")
        Name(_UID, 3)
        Method (_LPI, 0, NotSerialized) {
          return(PLPI)
        }
      }
    }

    //
    // LAN9118 Ethernet
    //
    Device(ETH0) {
      Name(_HID, "ARMH9118")
      Name(_UID, Zero)
      Name(_CRS, ResourceTemplate() {
              Memory32Fixed(ReadWrite, 0x18000000, 0x1000)
              Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { 192 }
      })
      Name(_DSD, Package() {
                   ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
                       Package() {
                               Package(2) {"phy-mode", "mii"},
                               Package(2) {"reg-io-width", 4 },
                               Package(2) {"smsc,irq-active-high",1},
                               Package(2) {"smsc,irq-push-pull",1}
                      }
      }) // _DSD()
    }

    // UART PL011
    Device(COM0) {
      Name(_HID, "ARMH0011")
      Name(_UID, Zero)
      Name(_CRS, ResourceTemplate() {
        Memory32Fixed(ReadWrite, 0x7FF80000, 0x1000)
        Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { 115 }
      })
    }

    //
    // USB EHCI Host Controller
    //
    Device(USB0){
        Name(_HID, "ARMH0D20")
        Name(_CID, "PNP0D20")
        Name(_UID, 2)
        Name(_CCA, 0) //EHCI on this platform is not coherent!

        Method(_CRS, 0x0, Serialized){
            Name(RBUF, ResourceTemplate(){
                Memory32Fixed(ReadWrite, 0x7FFC0000, 0x10000)
                Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) {149}  // INT ID=149 GIC IRQ ID=117 for Juno SoC USB EHCI Controller
            })
            Return(RBUF)
        }

        //
        // Root Hub
        //
        Device(RHUB){
            Name(_ADR, 0x00000000)  // Address of Root Hub should be 0 as per ACPI 5.0 spec

            //
            // Ports connected to Root Hub
            //
            Device(HUB1){
                Name(_ADR, 0x00000001)
                Name(_UPC, Package(){
                    0x00,       // Port is NOT connectable
                    0xFF,       // Don't care
                    0x00000000, // Reserved 0 must be zero
                    0x00000000  // Reserved 1 must be zero
                })

                Device(PRT1){
                    Name(_ADR, 0x00000001)
                    Name(_UPC, Package(){
                        0xFF,        // Port is connectable
                        0x00,        // Port connector is A
                        0x00000000,
                        0x00000000
                    })
                    Name(_PLD, Package(){
                        Buffer(0x10){
                            0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0x31, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                        }
                    })
                } // USB0_RHUB_HUB1_PRT1
                Device(PRT2){
                    Name(_ADR, 0x00000002)
                    Name(_UPC, Package(){
                        0xFF,        // Port is connectable
                        0x00,        // Port connector is A
                        0x00000000,
                        0x00000000
                    })
                    Name(_PLD, Package(){
                        Buffer(0x10){
                            0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0x31, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                        }
                    })
                } // USB0_RHUB_HUB1_PRT2

                Device(PRT3){
                    Name(_ADR, 0x00000003)
                    Name(_UPC, Package(){
                        0xFF,        // Port is connectable
                        0x00,        // Port connector is A
                        0x00000000,
                        0x00000000
                    })
                    Name(_PLD, Package(){
                        Buffer(0x10){
                            0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0x31, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                        }
                    })
                } // USB0_RHUB_HUB1_PRT3

                Device(PRT4){
                    Name(_ADR, 0x00000004)
                    Name(_UPC, Package(){
                        0xFF,        // Port is connectable
                        0x00,        // Port connector is A
                        0x00000000,
                        0x00000000
                    })
                    Name(_PLD, Package(){
                        Buffer(0x10){
                            0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0x31, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                        }
                    })
                } // USB0_RHUB_HUB1_PRT4
            } // USB0_RHUB_HUB1
        } // USB0_RHUB
    } // USB0
  } // Scope(_SB)
}
