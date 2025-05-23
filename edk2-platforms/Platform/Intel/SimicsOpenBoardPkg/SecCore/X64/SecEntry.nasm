; @file
; Copyright (c) 2006 - 2019 Intel Corporation. All rights reserved. <BR>
;
; SPDX-License-Identifier: BSD-2-Clause-Patent
;

#include <Base.h>

DEFAULT REL
SECTION .text

extern ASM_PFX(SecCoreStartupWithStack)

;
; SecCore Entry Point
;
; Processor is in flat protected mode
;
; @param[in]  RAX   Initial value of the EAX register (BIST: Built-in Self Test)
; @param[in]  DI    'BP': boot-strap processor, or 'AP': application processor
; @param[in]  RBP   Pointer to the start of the Boot Firmware Volume
;
; @return     None  This routine does not return
;
global ASM_PFX(_ModuleEntryPoint)
ASM_PFX(_ModuleEntryPoint):
    ;
    ; Fill the temporary RAM with the initial stack value.
    ; The loop below will seed the heap as well, but that's harmless.
    ;
    mov     rax, (FixedPcdGet32 (PcdInitValueInTempStack) << 32) | FixedPcdGet32 (PcdInitValueInTempStack)
                                                                ; qword to store
    mov     rdi, FixedPcdGet32 (PcdSimicsSecPeiTempRamBase)     ; base address,
                                                                ;   relative to
                                                                ;   ES
    mov     rcx, FixedPcdGet32 (PcdSimicsSecPeiTempRamSize) / 8 ; qword count
    cld                                                         ; store from base
                                                                ;   up
    rep stosq

    ;
    ; Load temporary RAM stack based on PCDs
    ;
    %define SEC_TOP_OF_STACK (FixedPcdGet32 (PcdSimicsSecPeiTempRamBase) + \
                          FixedPcdGet32 (PcdSimicsSecPeiTempRamSize))
    mov     rsp, SEC_TOP_OF_STACK
    nop

    ;
    ; Setup parameters and call SecCoreStartupWithStack
    ;   rcx: BootFirmwareVolumePtr
    ;   rdx: TopOfCurrentStack
    ;
    mov     rcx, rbp
    mov     rdx, rsp
    sub     rsp, 0x20
    call    ASM_PFX(SecCoreStartupWithStack)

