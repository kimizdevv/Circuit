; https://wiki.osdev.org/Bare_Bones#Booting_the_Operating_System

align_bnd:      equ 1<<0
meminfo:        equ 1<<1
flags:          equ align_bnd | meminfo
magic:          equ 0x1BADB002
checksum:       equ -(magic + flags)

segment .multiboot
align 4
        dd magic
        dd flags
        dd checksum

segment .bss
align 16
        stack_btm:
                resb 16384
        stack_top:

segment .text
extern kernel_main

global _start
_start:
        mov esp, stack_top
        call kernel_main

        cli
.h:     hlt
        jmp .h

