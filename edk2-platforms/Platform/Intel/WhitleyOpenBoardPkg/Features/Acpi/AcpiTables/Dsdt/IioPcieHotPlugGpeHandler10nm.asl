/** @file

  @copyright
  Copyright 2019 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

  //
  // Delay introduced as initial delay after entering ACPI hotplug method
  //
  Sleep (200)
  Store (0x01, IO80)
  Sleep (10)
  Store (0,Local1)

  // PC00 Port 1A PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
 If( LEqual(\_SB.PC01.BR1A.PMEP,1) ) {
    Store(\_SB.PC01.BR1A.PMEH(1), Local0)
  } else {
    Store (\_SB.PC01.BR1A.HPEH(1), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(1, Local1)
    Notify(\_SB.PC01.BR1A, Local0)
  }

  // PC00 Port 1B PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC01.BR1B.PMEP,1) ) {
    Store(\_SB.PC01.BR1B.PMEH(2), Local0)
  } else {
    Store (\_SB.PC01.BR1B.HPEH(2), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(2, Local1)
    Notify(\_SB.PC01.BR1B, Local0)
  }

  // PC00 Port 1C PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC01.BR1C.PMEP,1) ) {
    Store(\_SB.PC01.BR1C.PMEH(3), Local0)
  } else {
    Store (\_SB.PC01.BR1C.HPEH(3), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(3, Local1)
    Notify(\_SB.PC01.BR1C, Local0)
  }

  // PC00 Port 1D PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC01.BR1D.PMEP,1) ) {
    Store(\_SB.PC01.BR1D.PMEH(4), Local0)
  } else {
    Store (\_SB.PC01.BR1D.HPEH(4), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(4, Local1)
    Notify(\_SB.PC01.BR1D, Local0)
  }


 // PC01 Port 2A PCI-Ex Hot Plug
  If( LEqual(\_SB.PC02.BR2A.PMEP,1) ) {
    Store(\_SB.PC02.BR2A.PMEH(5), Local0)
  } else {
    Store (\_SB.PC02.BR2A.HPEH(5), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(5, Local1)
    Notify(\_SB.PC02.BR2A, Local0)
  }

  // PC01 Port 2B PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC02.BR2B.PMEP,1) ) {
    Store(\_SB.PC02.BR2B.PMEH(6), Local0)
  } else {
    Store (\_SB.PC02.BR2B.HPEH(6), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(6, Local1)
    Notify(\_SB.PC02.BR2B, Local0)
  }

  // PC01 Port 2C PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC02.BR2C.PMEP,1) ) {
    Store(\_SB.PC02.BR2C.PMEH(7), Local0)
  } else {
    Store (\_SB.PC02.BR2C.HPEH(7), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(7, Local1)
    Notify(\_SB.PC02.BR2C, Local0)
  }

  // PC01 Port 2D PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC02.BR2D.PMEP,1) ) {
    Store(\_SB.PC02.BR2D.PMEH(8), Local0)
  } else {
    Store (\_SB.PC02.BR2D.HPEH(8), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(8, Local1)
    Notify(\_SB.PC02.BR2D, Local0)
  }

  // PC01 Port 3A PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC03.BR3A.PMEP,1) ) {
    Store(\_SB.PC03.BR3A.PMEH(9), Local0)
  } else {
    Store (\_SB.PC03.BR3A.HPEH(9), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(9, Local1)
    Notify(\_SB.PC03.BR3A, Local0)
  }

  // PC01 Port 3B PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC03.BR3B.PMEP,1) ) {
    Store(\_SB.PC03.BR3B.PMEH(10), Local0)
  } else {
    Store (\_SB.PC03.BR3B.HPEH(10), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(10, Local1)
    Notify(\_SB.PC03.BR3B, Local0)
  }

  // PC01 Port 3C PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC03.BR3C.PMEP,1) ) {
    Store(\_SB.PC03.BR3C.PMEH(11), Local0)
  } else {
    Store (\_SB.PC03.BR3C.HPEH(11), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(11, Local1)
    Notify(\_SB.PC03.BR3C, Local0)
  }

  // PC01 Port 3D PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC03.BR3D.PMEP,1) ) {
    Store(\_SB.PC03.BR3D.PMEH(12), Local0)
  } else {
    Store (\_SB.PC03.BR3D.HPEH(12), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(12, Local1)
    Notify(\_SB.PC03.BR3D, Local0)
  }

  // PC02 Port 4A PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC04.BR4A.PMEP,1) ) {
    Store(\_SB.PC04.BR4A.PMEH(13), Local0)
  } else {
    Store (\_SB.PC04.BR4A.HPEH(13), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(13, Local1)
    Notify(\_SB.PC04.BR4A, Local0)
  }

  // PC02 Port 4B PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC04.BR4B.PMEP,1) ) {
    Store(\_SB.PC04.BR4B.PMEH(14), Local0)
  } else {
    Store (\_SB.PC04.BR4B.HPEH(14), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(14, Local1)
    Notify(\_SB.PC04.BR4B, Local0)
  }

  // PC02 Port 4C PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC04.BR4C.PMEP,1) ) {
    Store(\_SB.PC04.BR4C.PMEH(15), Local0)
  } else {
    Store (\_SB.PC04.BR4C.HPEH(15), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(15, Local1)
    Notify(\_SB.PC04.BR4C, Local0)
  }

  // PC02 Port 4D PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC04.BR4D.PMEP,1) ) {
    Store(\_SB.PC04.BR4D.PMEH(16), Local0)
  } else {
    Store (\_SB.PC04.BR4D.HPEH(16), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(16, Local1)
    Notify(\_SB.PC04.BR4D, Local0)
  }

  // PC02 Port 5A PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC05.BR5A.PMEP,1) ) {
    Store(\_SB.PC05.BR5A.PMEH(17), Local0)
  } else {
    Store (\_SB.PC05.BR5A.HPEH(17), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(17, Local1)
    Notify(\_SB.PC05.BR5A, Local0)
  }

  // PC02 Port 5B PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC05.BR5B.PMEP,1) ) {
    Store(\_SB.PC05.BR5B.PMEH(18), Local0)
  } else {
    Store (\_SB.PC05.BR5B.HPEH(18), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(18, Local1)
    Notify(\_SB.PC05.BR5B, Local0)
  }

  // PC02 Port 5C PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC05.BR5C.PMEP,1) ) {
    Store(\_SB.PC05.BR5C.PMEH(19), Local0)
  } else {
    Store (\_SB.PC05.BR5C.HPEH(19), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(19, Local1)
    Notify(\_SB.PC05.BR5C, Local0)
  }

  // PC02 Port 5D PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC05.BR5D.PMEP,1) ) {
    Store(\_SB.PC05.BR5D.PMEH(20), Local0)
  } else {
    Store (\_SB.PC05.BR5D.HPEH(20), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(20, Local1)
    Notify(\_SB.PC05.BR5D, Local0)
  }

#if MAX_SOCKET > 1


 // PC06 Port 1A PCI-Ex Hot Plug
 // If PM_PME event clear INTs and AttnBtnPressed
 If( LEqual(\_SB.PC07.QR1A.PMEP,1) ) {
    Store(\_SB.PC07.QR1A.PMEH(1), Local0)
  } else {
    Store (\_SB.PC07.QR1A.HPEH(1), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(1, Local1)
    Notify(\_SB.PC07.QR1A, Local0)
  }

  // PC06 Port 1B PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC07.QR1B.PMEP,1) ) {
    Store(\_SB.PC07.QR1B.PMEH(2), Local0)
  } else {
    Store (\_SB.PC07.QR1B.HPEH(2), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(2, Local1)
    Notify(\_SB.PC07.QR1B, Local0)
  }

  // PC06 Port 1C PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC07.QR1C.PMEP,1) ) {
    Store(\_SB.PC07.QR1C.PMEH(3), Local0)
  } else {
    Store (\_SB.PC07.QR1C.HPEH(3), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(3, Local1)
    Notify(\_SB.PC07.QR1C, Local0)
  }

  // PC06 Port 1D PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC07.QR1D.PMEP,1) ) {
    Store(\_SB.PC07.QR1D.PMEH(4), Local0)
  } else {
    Store (\_SB.PC07.QR1D.HPEH(4), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(4, Local1)
    Notify(\_SB.PC07.QR1D, Local0)
  }

  // PC04 Port 2A PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC08.QR2A.PMEP,1) ) {
    Store(\_SB.PC08.QR2A.PMEH(5), Local0)
  } else {
    Store (\_SB.PC08.QR2A.HPEH(5), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(5, Local1)
    Notify(\_SB.PC08.QR2A, Local0)
  }

  // PC04 Port 2B PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC08.QR2B.PMEP,1) ) {
    Store(\_SB.PC08.QR2B.PMEH(6), Local0)
  } else {
    Store (\_SB.PC08.QR2B.HPEH(6), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(6, Local1)
    Notify(\_SB.PC08.QR2B, Local0)
  }

  // PC04 Port 2C PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC08.QR2C.PMEP,1) ) {
    Store(\_SB.PC08.QR2C.PMEH(7), Local0)
  } else {
    Store (\_SB.PC08.QR2C.HPEH(7), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(7, Local1)
    Notify(\_SB.PC08.QR2C, Local0)
  }

  // PC04 Port 2D PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC08.QR2D.PMEP,1) ) {
    Store(\_SB.PC08.QR2D.PMEH(8), Local0)
  } else {
    Store (\_SB.PC08.QR2D.HPEH(8), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(8, Local1)
    Notify(\_SB.PC08.QR2D, Local0)
  }

  // PC04 Port 3A PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC09.QR3A.PMEP,1) ) {
    Store(\_SB.PC09.QR3A.PMEH(9), Local0)
  } else {
    Store (\_SB.PC09.QR3A.HPEH(9), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(9, Local1)
    Notify(\_SB.PC09.QR3A, Local0)
  }

  // PC04 Port 3B PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC09.QR3B.PMEP,1) ) {
    Store(\_SB.PC09.QR3B.PMEH(10), Local0)
  } else {
    Store (\_SB.PC09.QR3B.HPEH(10), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(10, Local1)
    Notify(\_SB.PC09.QR3B, Local0)
  }

  // PC04 Port 3C PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC09.QR3C.PMEP,1) ) {
    Store(\_SB.PC09.QR3C.PMEH(11), Local0)
  } else {
    Store (\_SB.PC09.QR3C.HPEH(11), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(11, Local1)
    Notify(\_SB.PC09.QR3C, Local0)
  }

  // PC04 Port 3D PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC09.QR3D.PMEP,1) ) {
    Store(\_SB.PC09.QR3D.PMEH(12), Local0)
  } else {
    Store (\_SB.PC09.QR3D.HPEH(12), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(12, Local1)
    Notify(\_SB.PC09.QR3D, Local0)
  }

  // PC10 Port 4A PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC10.QR4A.PMEP,1) ) {
    Store(\_SB.PC10.QR4A.PMEH(13), Local0)
  } else {
    Store (\_SB.PC10.QR4A.HPEH(13), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(13, Local1)
    Notify(\_SB.PC10.QR4A, Local0)
  }

  // PC10 Port 4B PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC10.QR4B.PMEP,1) ) {
    Store(\_SB.PC10.QR4B.PMEH(14), Local0)
  } else {
    Store (\_SB.PC10.QR4B.HPEH(14), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(14, Local1)
    Notify(\_SB.PC10.QR4B, Local0)
  }

  // PC10 Port 4C PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC10.QR4C.PMEP,1) ) {
    Store(\_SB.PC10.QR4C.PMEH(15), Local0)
  } else {
    Store (\_SB.PC10.QR4C.HPEH(15), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(15, Local1)
    Notify(\_SB.PC10.QR4C, Local0)
  }

  // PC10 Port 4D PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC10.QR4D.PMEP,1) ) {
    Store(\_SB.PC10.QR4D.PMEH(16), Local0)
  } else {
    Store (\_SB.PC10.QR4D.HPEH(16), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(16, Local1)
    Notify(\_SB.PC10.QR4D, Local0)
  }

  // PC10 Port 5A PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC11.QR5A.PMEP,1) ) {
    Store(\_SB.PC11.QR5A.PMEH(17), Local0)
  } else {
    Store (\_SB.PC11.QR5A.HPEH(17), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(17, Local1)
    Notify(\_SB.PC11.QR5A, Local0)
  }

  // PC10 Port 5B PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC11.QR5B.PMEP,1) ) {
    Store(\_SB.PC11.QR5B.PMEH(18), Local0)
  } else {
    Store (\_SB.PC11.QR5B.HPEH(18), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(18, Local1)
    Notify(\_SB.PC11.QR5B, Local0)
  }

  // PC10 Port 5C PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC11.QR5C.PMEP,1) ) {
    Store(\_SB.PC11.QR5C.PMEH(19), Local0)
  } else {
    Store (\_SB.PC11.QR5C.HPEH(19), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(19, Local1)
    Notify(\_SB.PC11.QR5C, Local0)
  }

  // PC10 Port 5D PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC11.QR5D.PMEP,1) ) {
    Store(\_SB.PC11.QR5D.PMEH(20), Local0)
  } else {
    Store (\_SB.PC11.QR5D.HPEH(20), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(20, Local1)
    Notify(\_SB.PC11.QR5D, Local0)
  }
#endif
#if MAX_SOCKET > 2

 // PC06 Port 1A PCI-Ex Hot Plug
 // If PM_PME event clear INTs and AttnBtnPressed
 If( LEqual(\_SB.PC13.RR1A.PMEP,1) ) {
    Store(\_SB.PC13.RR1A.PMEH(1), Local0)
  } else {
    Store (\_SB.PC13.RR1A.HPEH(1), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(1, Local1)
    Notify(\_SB.PC13.RR1A, Local0)
  }

  // PC06 Port 1B PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC13.RR1B.PMEP,1) ) {
    Store(\_SB.PC13.RR1B.PMEH(2), Local0)
  } else {
    Store (\_SB.PC13.RR1B.HPEH(2), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(2, Local1)
    Notify(\_SB.PC13.RR1B, Local0)
  }

  // PC06 Port 1C PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC13.RR1C.PMEP,1) ) {
    Store(\_SB.PC13.RR1C.PMEH(3), Local0)
  } else {
    Store (\_SB.PC13.RR1C.HPEH(3), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(3, Local1)
    Notify(\_SB.PC13.RR1C, Local0)
  }

  // PC06 Port 1D PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC13.RR1D.PMEP,1) ) {
    Store(\_SB.PC13.RR1D.PMEH(4), Local0)
  } else {
    Store (\_SB.PC13.RR1D.HPEH(4), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(4, Local1)
    Notify(\_SB.PC13.RR1D, Local0)
  }

  // PC07 Port 2A PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC14.RR2A.PMEP,1) ) {
    Store(\_SB.PC14.RR2A.PMEH(5), Local0)
  } else {
    Store (\_SB.PC14.RR2A.HPEH(5), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(5, Local1)
    Notify(\_SB.PC14.RR2A, Local0)
  }

  // PC07 Port 2B PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC14.RR2B.PMEP,1) ) {
    Store(\_SB.PC14.RR2B.PMEH(6), Local0)
  } else {
    Store (\_SB.PC14.RR2B.HPEH(6), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(6, Local1)
    Notify(\_SB.PC14.RR2B, Local0)
  }

  // PC07 Port 2C PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC14.RR2C.PMEP,1) ) {
    Store(\_SB.PC14.RR2C.PMEH(7), Local0)
  } else {
    Store (\_SB.PC14.RR2C.HPEH(7), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(7, Local1)
    Notify(\_SB.PC14.RR2C, Local0)
  }

  // PC07 Port 2D PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC14.RR2D.PMEP,1) ) {
    Store(\_SB.PC14.RR2D.PMEH(8), Local0)
  } else {
    Store (\_SB.PC14.RR2D.HPEH(8), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(8, Local1)
    Notify(\_SB.PC14.RR2D, Local0)
  }

  // PC07 Port 3A PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC15.RR3A.PMEP,1) ) {
    Store(\_SB.PC15.RR3A.PMEH(9), Local0)
  } else {
    Store (\_SB.PC15.RR3A.HPEH(9), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(9, Local1)
    Notify(\_SB.PC15.RR3A, Local0)
  }

  // PC07 Port 3B PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC15.RR3B.PMEP,1) ) {
    Store(\_SB.PC15.RR3B.PMEH(10), Local0)
  } else {
    Store (\_SB.PC15.RR3B.HPEH(10), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(10, Local1)
    Notify(\_SB.PC15.RR3B, Local0)
  }

  // PC07 Port 3C PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC15.RR3C.PMEP,1) ) {
    Store(\_SB.PC15.RR3C.PMEH(11), Local0)
  } else {
    Store (\_SB.PC15.RR3C.HPEH(11), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(11, Local1)
    Notify(\_SB.PC15.RR3C, Local0)
  }

  // PC07 Port 3D PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC15.RR3D.PMEP,1) ) {
    Store(\_SB.PC15.RR3D.PMEH(12), Local0)
  } else {
    Store (\_SB.PC15.RR3D.HPEH(12), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(12, Local1)
    Notify(\_SB.PC15.RR3D, Local0)
  }

  // PC08 Port 4A PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC16.RR4A.PMEP,1) ) {
    Store(\_SB.PC16.RR4A.PMEH(13), Local0)
  } else {
    Store (\_SB.PC16.RR4A.HPEH(13), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(13, Local1)
    Notify(\_SB.PC16.RR4A, Local0)
  }

  // PC08 Port 4B PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC16.RR4B.PMEP,1) ) {
    Store(\_SB.PC16.RR4B.PMEH(14), Local0)
  } else {
    Store (\_SB.PC16.RR4B.HPEH(14), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(14, Local1)
    Notify(\_SB.PC16.RR4B, Local0)
  }

  // PC08 Port 4C PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC16.RR4C.PMEP,1) ) {
    Store(\_SB.PC16.RR4C.PMEH(15), Local0)
  } else {
    Store (\_SB.PC16.RR4C.HPEH(15), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(15, Local1)
    Notify(\_SB.PC16.RR4C, Local0)
  }

  // PC08 Port 4D PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC16.RR4D.PMEP,1) ) {
    Store(\_SB.PC16.RR4D.PMEH(16), Local0)
  } else {
    Store (\_SB.PC16.RR4D.HPEH(16), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(16, Local1)
    Notify(\_SB.PC16.RR4D, Local0)
  }

  // PC08 Port 5A PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC17.RR5A.PMEP,1) ) {
    Store(\_SB.PC17.RR5A.PMEH(17), Local0)
  } else {
    Store (\_SB.PC17.RR5A.HPEH(17), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(17, Local1)
    Notify(\_SB.PC17.RR5A, Local0)
  }

  // PC08 Port 5B PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC17.RR5B.PMEP,1) ) {
    Store(\_SB.PC17.RR5B.PMEH(18), Local0)
  } else {
    Store (\_SB.PC17.RR5B.HPEH(18), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(18, Local1)
    Notify(\_SB.PC17.RR5B, Local0)
  }

  // PC08 Port 5C PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC17.RR5C.PMEP,1) ) {
    Store(\_SB.PC17.RR5C.PMEH(19), Local0)
  } else {
    Store (\_SB.PC17.RR5C.HPEH(19), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(19, Local1)
    Notify(\_SB.PC17.RR5C, Local0)
  }

  // PC08 Port 5D PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC17.RR5D.PMEP,1) ) {
    Store(\_SB.PC17.RR5D.PMEH(20), Local0)
  } else {
    Store (\_SB.PC17.RR5D.HPEH(20), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(20, Local1)
    Notify(\_SB.PC17.RR5D, Local0)
  }
#endif
#if MAX_SOCKET > 3


  // PC09 Port 1A PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC19.SR1A.PMEP,1) ) {
    Store(\_SB.PC19.SR1A.PMEH(1), Local0)
  } else {
    Store (\_SB.PC19.SR1A.HPEH(1), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(1, Local1)
    Notify(\_SB.PC19.SR1A, Local0)
  }

  // PC09 Port 1B PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC19.SR1B.PMEP,1) ) {
    Store(\_SB.PC19.SR1B.PMEH(2), Local0)
  } else {
    Store (\_SB.PC19.SR1B.HPEH(2), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(2, Local1)
    Notify(\_SB.PC19.SR1B, Local0)
  }

  // PC09 Port 1C PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC19.SR1C.PMEP,1) ) {
    Store(\_SB.PC19.SR1C.PMEH(3), Local0)
  } else {
    Store (\_SB.PC19.SR1C.HPEH(3), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(3, Local1)
    Notify(\_SB.PC19.SR1C, Local0)
  }

  // PC09 Port 1D PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC19.SR1D.PMEP,1) ) {
    Store(\_SB.PC19.SR1D.PMEH(4), Local0)
  } else {
    Store (\_SB.PC19.SR1D.HPEH(4), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(4, Local1)
    Notify(\_SB.PC19.SR1D, Local0)
  }

  // PC10 Port 2A PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC20.SR2A.PMEP,1) ) {
    Store(\_SB.PC20.SR2A.PMEH(5), Local0)
  } else {
    Store (\_SB.PC20.SR2A.HPEH(5), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(5, Local1)
    Notify(\_SB.PC20.SR2A, Local0)
  }

  // PC10 Port 2B PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC20.SR2B.PMEP,1) ) {
    Store(\_SB.PC20.SR2B.PMEH(6), Local0)
  } else {
    Store (\_SB.PC20.SR2B.HPEH(6), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(6, Local1)
    Notify(\_SB.PC20.SR2B, Local0)
  }

  // PC10 Port 2C PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC20.SR2C.PMEP,1) ) {
    Store(\_SB.PC20.SR2C.PMEH(7), Local0)
  } else {
    Store (\_SB.PC20.SR2C.HPEH(7), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(7, Local1)
    Notify(\_SB.PC20.SR2C, Local0)
  }

  // PC10 Port 2D PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC20.SR2D.PMEP,1) ) {
    Store(\_SB.PC20.SR2D.PMEH(8), Local0)
  } else {
    Store (\_SB.PC20.SR2D.HPEH(8), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(8, Local1)
    Notify(\_SB.PC20.SR2D, Local0)
  }

  // PC10 Port 3A PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC21.SR3A.PMEP,1) ) {
    Store(\_SB.PC21.SR3A.PMEH(9), Local0)
  } else {
    Store (\_SB.PC21.SR3A.HPEH(9), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(9, Local1)
    Notify(\_SB.PC21.SR3A, Local0)
  }

  // PC10 Port 3B PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC21.SR3B.PMEP,1) ) {
    Store(\_SB.PC21.SR3B.PMEH(10), Local0)
  } else {
    Store (\_SB.PC21.SR3B.HPEH(10), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(10, Local1)
    Notify(\_SB.PC21.SR3B, Local0)
  }

  // PC10 Port 3C PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC21.SR3C.PMEP,1) ) {
    Store(\_SB.PC21.SR3C.PMEH(11), Local0)
  } else {
    Store (\_SB.PC21.SR3C.HPEH(11), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(11, Local1)
    Notify(\_SB.PC21.SR3C, Local0)
  }

  // PC10 Port 3D PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC21.SR3D.PMEP,1) ) {
    Store(\_SB.PC21.SR3D.PMEH(12), Local0)
  } else {
    Store (\_SB.PC21.SR3D.HPEH(12), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(12, Local1)
    Notify(\_SB.PC21.SR3D, Local0)
  }

  // PC11 Port 4A PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC22.SR4A.PMEP,1) ) {
    Store(\_SB.PC22.SR4A.PMEH(13), Local0)
  } else {
    Store (\_SB.PC22.SR4A.HPEH(13), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(13, Local1)
    Notify(\_SB.PC22.SR4A, Local0)
  }

  // PC11 Port 4B PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC22.SR4B.PMEP,1) ) {
    Store(\_SB.PC22.SR4B.PMEH(14), Local0)
  } else {
    Store (\_SB.PC22.SR4B.HPEH(14), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(14, Local1)
    Notify(\_SB.PC22.SR4B, Local0)
  }

  // PC11 Port 4C PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC22.SR4C.PMEP,1) ) {
    Store(\_SB.PC22.SR4C.PMEH(15), Local0)
  } else {
    Store (\_SB.PC22.SR4C.HPEH(15), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(15, Local1)
    Notify(\_SB.PC22.SR4C, Local0)
  }

  // PC11 Port 4D PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC22.SR4D.PMEP,1) ) {
    Store(\_SB.PC22.SR4D.PMEH(16), Local0)
  } else {
    Store (\_SB.PC22.SR4D.HPEH(16), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(16, Local1)
    Notify(\_SB.PC22.SR4D, Local0)
  }

  // PC11 Port 5A PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC23.SR5A.PMEP,1) ) {
    Store(\_SB.PC23.SR5A.PMEH(17), Local0)
  } else {
    Store (\_SB.PC23.SR5A.HPEH(17), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(17, Local1)
    Notify(\_SB.PC23.SR5A, Local0)
  }

  // PC11 Port 5B PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC23.SR5B.PMEP,1) ) {
    Store(\_SB.PC23.SR5B.PMEH(18), Local0)
  } else {
    Store (\_SB.PC23.SR5B.HPEH(18), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(18, Local1)
    Notify(\_SB.PC23.SR5B, Local0)
  }

  // PC11 Port 5C PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC23.SR5C.PMEP,1) ) {
    Store(\_SB.PC23.SR5C.PMEH(19), Local0)
  } else {
    Store (\_SB.PC23.SR5C.HPEH(19), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(19, Local1)
    Notify(\_SB.PC23.SR5C, Local0)
  }

  // PC11 Port 5D PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PC23.SR5D.PMEP,1) ) {
    Store(\_SB.PC23.SR5D.PMEH(20), Local0)
  } else {
    Store (\_SB.PC23.SR5D.HPEH(20), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(20, Local1)
    Notify(\_SB.PC23.SR5D, Local0)
  }
#endif

  //If a hotplug event was serviced check if this was generated by PM_PME
  If (Lnot (LEqual(Local0, 0))) {
    //Clear the status bit 16 of PMEStatus
    //Clear the PME Pending bit 17 of PMEStatus
    If( LEqual(Local1, 1)) {
      Store(1, \_SB.PC01.BR1A.PMES)
      Store(1, \_SB.PC01.BR1A.PMEP)
    }
    If( LEqual(Local1, 2)) {
      Store(1, \_SB.PC01.BR1B.PMES)
      Store(1, \_SB.PC01.BR1B.PMEP)
    }
    If( LEqual(Local1, 3)) {
      Store(1, \_SB.PC01.BR1C.PMES)
      Store(1, \_SB.PC01.BR1C.PMEP)
    }
    If( LEqual(Local1, 4)) {
      Store(1, \_SB.PC01.BR1D.PMES)
      Store(1, \_SB.PC01.BR1D.PMEP)
    }

    If( LEqual(Local1, 5)) {
      Store(1, \_SB.PC02.BR2A.PMES)
      Store(1, \_SB.PC02.BR2A.PMEP)
    }
    If( LEqual(Local1, 6)) {
      Store(1, \_SB.PC02.BR2B.PMES)
      Store(1, \_SB.PC02.BR2B.PMEP)
    }
    If( LEqual(Local1, 7)) {
      Store(1, \_SB.PC02.BR2C.PMES)
      Store(1, \_SB.PC02.BR2C.PMEP)
    }
    If( LEqual(Local1, 8)) {
      Store(1, \_SB.PC02.BR2D.PMES)
      Store(1, \_SB.PC02.BR2D.PMEP)
    }
    If( LEqual(Local1, 9)) {
      Store(1, \_SB.PC03.BR3A.PMES)
      Store(1, \_SB.PC03.BR3A.PMEP)
    }
    If( LEqual(Local1, 10)) {
      Store(1, \_SB.PC03.BR3B.PMES)
      Store(1, \_SB.PC03.BR3B.PMEP)
    }
    If( LEqual(Local1, 11)) {
      Store(1, \_SB.PC03.BR3C.PMES)
      Store(1, \_SB.PC03.BR3C.PMEP)
    }
    If( LEqual(Local1, 12)) {
      Store(1, \_SB.PC03.BR3D.PMES)
      Store(1, \_SB.PC03.BR3D.PMEP)
    }
    If( LEqual(Local1, 13)) {
      Store(1, \_SB.PC04.BR4A.PMES)
      Store(1, \_SB.PC04.BR4A.PMEP)
    }
    If( LEqual(Local1, 14)) {
      Store(1, \_SB.PC04.BR4B.PMES)
      Store(1, \_SB.PC04.BR4B.PMEP)
    }
    If( LEqual(Local1, 15)) {
      Store(1, \_SB.PC04.BR4C.PMES)
      Store(1, \_SB.PC04.BR4C.PMEP)
    }
    If( LEqual(Local1, 16)) {
      Store(1, \_SB.PC04.BR4D.PMES)
      Store(1, \_SB.PC04.BR4D.PMEP)
    }
    If( LEqual(Local1, 17)) {
      Store(1, \_SB.PC05.BR5A.PMES)
      Store(1, \_SB.PC05.BR5A.PMEP)
    }
    If( LEqual(Local1, 18)) {
      Store(1, \_SB.PC05.BR5B.PMES)
      Store(1, \_SB.PC05.BR5B.PMEP)
    }
    If( LEqual(Local1, 19)) {
      Store(1, \_SB.PC05.BR5C.PMES)
      Store(1, \_SB.PC05.BR5C.PMEP)
    }
    If( LEqual(Local1, 20)) {
      Store(1, \_SB.PC05.BR5D.PMES)
      Store(1, \_SB.PC05.BR5D.PMEP)
    }

#if MAX_SOCKET > 1
    If( LEqual(Local1, 1)) {
      Store(1, \_SB.PC07.QR1A.PMES)
      Store(1, \_SB.PC07.QR1A.PMEP)
    }
    If( LEqual(Local1, 2)) {
      Store(1, \_SB.PC07.QR1B.PMES)
      Store(1, \_SB.PC07.QR1B.PMEP)
    }
    If( LEqual(Local1, 3)) {
      Store(1, \_SB.PC07.QR1C.PMES)
      Store(1, \_SB.PC07.QR1C.PMEP)
    }
    If( LEqual(Local1, 4)) {
      Store(1, \_SB.PC07.QR1D.PMES)
      Store(1, \_SB.PC07.QR1D.PMEP)
    }
    If( LEqual(Local1, 5)) {
      Store(1, \_SB.PC08.QR2A.PMES)
      Store(1, \_SB.PC08.QR2A.PMEP)
    }
    If( LEqual(Local1, 6)) {
      Store(1, \_SB.PC08.QR2B.PMES)
      Store(1, \_SB.PC08.QR2B.PMEP)
    }
    If( LEqual(Local1, 7)) {
      Store(1, \_SB.PC08.QR2C.PMES)
      Store(1, \_SB.PC08.QR2C.PMEP)
    }
    If( LEqual(Local1, 8)) {
      Store(1, \_SB.PC08.QR2D.PMES)
      Store(1, \_SB.PC08.QR2D.PMEP)
    }
    If( LEqual(Local1, 9)) {
      Store(1, \_SB.PC09.QR3A.PMES)
      Store(1, \_SB.PC09.QR3A.PMEP)
    }
    If( LEqual(Local1, 10)) {
      Store(1, \_SB.PC09.QR3B.PMES)
      Store(1, \_SB.PC09.QR3B.PMEP)
    }
    If( LEqual(Local1, 11)) {
      Store(1, \_SB.PC09.QR3C.PMES)
      Store(1, \_SB.PC09.QR3C.PMEP)
    }
    If( LEqual(Local1, 12)) {
      Store(1, \_SB.PC09.QR3D.PMES)
      Store(1, \_SB.PC09.QR3D.PMEP)
    }
    If( LEqual(Local1, 13)) {
      Store(1, \_SB.PC10.QR4A.PMES)
      Store(1, \_SB.PC10.QR4A.PMEP)
    }
    If( LEqual(Local1, 14)) {
      Store(1, \_SB.PC10.QR4B.PMES)
      Store(1, \_SB.PC10.QR4B.PMEP)
    }
    If( LEqual(Local1, 15)) {
      Store(1, \_SB.PC10.QR4C.PMES)
      Store(1, \_SB.PC10.QR4C.PMEP)
    }
    If( LEqual(Local1, 16)) {
      Store(1, \_SB.PC10.QR4D.PMES)
      Store(1, \_SB.PC10.QR4D.PMEP)
    }
    If( LEqual(Local1, 17)) {
      Store(1, \_SB.PC11.QR5A.PMES)
      Store(1, \_SB.PC11.QR5A.PMEP)
    }
    If( LEqual(Local1, 18)) {
      Store(1, \_SB.PC11.QR5B.PMES)
      Store(1, \_SB.PC11.QR5B.PMEP)
    }
    If( LEqual(Local1, 19)) {
      Store(1, \_SB.PC11.QR5C.PMES)
      Store(1, \_SB.PC11.QR5C.PMEP)
    }
    If( LEqual(Local1, 20)) {
      Store(1, \_SB.PC11.QR5D.PMES)
      Store(1, \_SB.PC11.QR5D.PMEP)
    }
#endif
#if MAX_SOCKET > 2

    If( LEqual(Local1, 1)) {
      Store(1, \_SB.PC13.RR1A.PMES)
      Store(1, \_SB.PC13.RR1A.PMEP)
    }
    If( LEqual(Local1, 2)) {
      Store(1, \_SB.PC13.RR1B.PMES)
      Store(1, \_SB.PC13.RR1B.PMEP)
    }
    If( LEqual(Local1, 3)) {
      Store(1, \_SB.PC13.RR1C.PMES)
      Store(1, \_SB.PC13.RR1C.PMEP)
    }
    If( LEqual(Local1, 4)) {
      Store(1, \_SB.PC13.RR1D.PMES)
      Store(1, \_SB.PC13.RR1D.PMEP)
    }
    If( LEqual(Local1, 5)) {
      Store(1, \_SB.PC14.RR2A.PMES)
      Store(1, \_SB.PC14.RR2A.PMEP)
    }
    If( LEqual(Local1, 6)) {
      Store(1, \_SB.PC14.RR2B.PMES)
      Store(1, \_SB.PC14.RR2B.PMEP)
    }
    If( LEqual(Local1, 7)) {
      Store(1, \_SB.PC14.RR2C.PMES)
      Store(1, \_SB.PC14.RR2C.PMEP)
    }
    If( LEqual(Local1, 8)) {
      Store(1, \_SB.PC14.RR2D.PMES)
      Store(1, \_SB.PC14.RR2D.PMEP)
    }
    If( LEqual(Local1, 9)) {
      Store(1, \_SB.PC15.RR3A.PMES)
      Store(1, \_SB.PC15.RR3A.PMEP)
    }
    If( LEqual(Local1, 10)) {
      Store(1, \_SB.PC15.RR3B.PMES)
      Store(1, \_SB.PC15.RR3B.PMEP)
    }
    If( LEqual(Local1, 11)) {
      Store(1, \_SB.PC15.RR3C.PMES)
      Store(1, \_SB.PC15.RR3C.PMEP)
    }
    If( LEqual(Local1, 12)) {
      Store(1, \_SB.PC15.RR3D.PMES)
      Store(1, \_SB.PC15.RR3D.PMEP)
    }
    If( LEqual(Local1, 13)) {
      Store(1, \_SB.PC16.RR4A.PMES)
      Store(1, \_SB.PC16.RR4A.PMEP)
    }
    If( LEqual(Local1, 14)) {
      Store(1, \_SB.PC16.RR4B.PMES)
      Store(1, \_SB.PC16.RR4B.PMEP)
    }
    If( LEqual(Local1, 15)) {
      Store(1, \_SB.PC16.RR4C.PMES)
      Store(1, \_SB.PC16.RR4C.PMEP)
    }
    If( LEqual(Local1, 16)) {
      Store(1, \_SB.PC16.RR4D.PMES)
      Store(1, \_SB.PC16.RR4D.PMEP)
    }
    If( LEqual(Local1, 17)) {
      Store(1, \_SB.PC17.RR5A.PMES)
      Store(1, \_SB.PC17.RR5A.PMEP)
    }
    If( LEqual(Local1, 18)) {
      Store(1, \_SB.PC17.RR5B.PMES)
      Store(1, \_SB.PC17.RR5B.PMEP)
    }
    If( LEqual(Local1, 19)) {
      Store(1, \_SB.PC17.RR5C.PMES)
      Store(1, \_SB.PC17.RR5C.PMEP)
    }
    If( LEqual(Local1, 20)) {
      Store(1, \_SB.PC17.RR5D.PMES)
      Store(1, \_SB.PC17.RR5D.PMEP)
    }
#endif
#if MAX_SOCKET > 3

    If( LEqual(Local1, 1)) {
      Store(1, \_SB.PC19.SR1A.PMES)
      Store(1, \_SB.PC19.SR1A.PMEP)
    }
    If( LEqual(Local1, 2)) {
      Store(1, \_SB.PC19.SR1B.PMES)
      Store(1, \_SB.PC19.SR1B.PMEP)
    }
    If( LEqual(Local1, 3)) {
      Store(1, \_SB.PC19.SR1C.PMES)
      Store(1, \_SB.PC19.SR1C.PMEP)
    }
    If( LEqual(Local1, 4)) {
      Store(1, \_SB.PC19.SR1D.PMES)
      Store(1, \_SB.PC19.SR1D.PMEP)
    }
    If( LEqual(Local1, 5)) {
      Store(1, \_SB.PC20.SR2A.PMES)
      Store(1, \_SB.PC20.SR2A.PMEP)
    }
    If( LEqual(Local1, 6)) {
      Store(1, \_SB.PC20.SR2B.PMES)
      Store(1, \_SB.PC20.SR2B.PMEP)
    }
    If( LEqual(Local1, 7)) {
      Store(1, \_SB.PC20.SR2C.PMES)
      Store(1, \_SB.PC20.SR2C.PMEP)
    }
    If( LEqual(Local1, 8)) {
      Store(1, \_SB.PC20.SR2D.PMES)
      Store(1, \_SB.PC20.SR2D.PMEP)
    }
    If( LEqual(Local1, 9)) {
      Store(1, \_SB.PC21.SR3A.PMES)
      Store(1, \_SB.PC21.SR3A.PMEP)
    }
    If( LEqual(Local1, 10)) {
      Store(1, \_SB.PC21.SR3B.PMES)
      Store(1, \_SB.PC21.SR3B.PMEP)
    }
    If( LEqual(Local1, 11)) {
      Store(1, \_SB.PC21.SR3C.PMES)
      Store(1, \_SB.PC21.SR3C.PMEP)
    }
    If( LEqual(Local1, 12)) {
      Store(1, \_SB.PC21.SR3D.PMES)
      Store(1, \_SB.PC21.SR3D.PMEP)
    }
    If( LEqual(Local1, 13)) {
      Store(1, \_SB.PC22.SR4A.PMES)
      Store(1, \_SB.PC22.SR4A.PMEP)
    }
    If( LEqual(Local1, 14)) {
      Store(1, \_SB.PC22.SR4B.PMES)
      Store(1, \_SB.PC22.SR4B.PMEP)
    }
    If( LEqual(Local1, 15)) {
      Store(1, \_SB.PC22.SR4C.PMES)
      Store(1, \_SB.PC22.SR4C.PMEP)
    }
    If( LEqual(Local1, 16)) {
      Store(1, \_SB.PC22.SR4D.PMES)
      Store(1, \_SB.PC22.SR4D.PMEP)
    }
    If( LEqual(Local1, 17)) {
      Store(1, \_SB.PC23.SR5A.PMES)
      Store(1, \_SB.PC23.SR5A.PMEP)
    }
    If( LEqual(Local1, 18)) {
      Store(1, \_SB.PC23.SR5B.PMES)
      Store(1, \_SB.PC23.SR5B.PMEP)
    }
    If( LEqual(Local1, 19)) {
      Store(1, \_SB.PC23.SR5C.PMES)
      Store(1, \_SB.PC23.SR5C.PMEP)
    }
    If( LEqual(Local1, 20)) {
      Store(1, \_SB.PC23.SR5D.PMES)
      Store(1, \_SB.PC23.SR5D.PMEP)
    }
#endif
    Store(0x01,PEES)               //Clear bit 9 of Status
    Store(0x00,PMEE)               //Clear bit 9 of GPE0_EN
  }
