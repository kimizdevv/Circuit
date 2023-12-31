// https://wiki.osdev.org/Bare_Bones#Booting_the_Operating_System

//.set ALIGN,     1<<0
//.set MEMINFO,   1<<1
//.set FLAGS,     ALIGN | MEMINFO
//.set MAGIC,     0x1BADB002
//.set CHECKSUM,  -(MAGIC + FLAGS)
//
//.section .multiboot
//.align 4
//        .long MAGIC
//        .long FLAGS
//        .long CHECKSUM

// https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html#Header-tags

#define ASM_FILE
#include "multiboot2.h"

#define STACK_SIZE 0x4000

.section .multiboot
.align 4
.header_start:
    .long MULTIBOOT2_HEADER_MAGIC       // magic
    .long MULTIBOOT_ARCHITECTURE_I386   // architecture
    .long .header_end - .header_start   // header_length
    .long 0x100000000 - (0xe85250d6 + 0 + (.header_end - .header_start)) // checksum

    // framebuffer tag
    .framebuffer_tag_start:
        .short MULTIBOOT_HEADER_TAG_FRAMEBUFFER
        .short 0        // flags
        .long .framebuffer_tag_end - .framebuffer_tag_start
        .long 1280      // width
        .long 720       // height
        .long 32        // depth
    .framebuffer_tag_end:

    .align 8

    // required end tag
    .end_tag_start:
        .short MULTIBOOT_HEADER_TAG_END
        .short 0        // flags
        .long .end_tag_end - .end_tag_start     // size
    .end_tag_end:
.header_end:

.section .bss
.align 16
        stack_btm:
                .skip STACK_SIZE
        stack_top:

.section .text
.global _start
.type _start, @function
_start:
        mov $stack_top, %esp
        pushl %ebx      // multiboot header address
        pushl %eax      // magic bootloader
        call kernel_main

        cli
1:      hlt
        jmp 1b

.size _start, . - _start

