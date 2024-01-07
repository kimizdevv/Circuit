#include "entry.h"
#include "terminal.h"
#include "shell.h"

#define PUTSTR(S) term_putstr(term, S)

#include "../boot/multiboot2.h"
#include "../lib/sys/string.h"
#include <stdint.h>

struct framebuffer fb;

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wcast-align"
#pragma clang diagnostic ignored "-Wsign-conversion"
#endif
static void get_framebuffer_from_tag(struct multiboot_tag *tag,
                                     struct framebuffer *framebuffer)
{
        struct multiboot_tag_framebuffer *tagfb =
                (struct multiboot_tag_framebuffer *)tag;
        framebuffer->bufadr = (uintptr_t)tagfb->common.framebuffer_addr;
        framebuffer->width = tagfb->common.framebuffer_width;
        framebuffer->height = tagfb->common.framebuffer_height;
        framebuffer->bpp = tagfb->common.framebuffer_bpp;
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif

extern void kernel_main(unsigned long magic, unsigned long addr)
{
        if (magic != MULTIBOOT2_BOOTLOADER_MAGIC)
                return;

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wcast-align"
#pragma clang diagnostic ignored "-Wsign-conversion"
#endif

        for (struct multiboot_tag *tag = (struct multiboot_tag *)(addr + 8);
             tag->type != MULTIBOOT_TAG_TYPE_END;
             tag = (struct multiboot_tag *)((multiboot_uint8_t *)tag +
                                            ((tag->size + 7) & ~7))) {
                switch (tag->type) {
                case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
                        get_framebuffer_from_tag(tag, &fb);
                        break;
                default:
                        break;
                }
        }

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

        struct terminal term_o = term_init(&fb);
        struct terminal *term = &term_o;

        PUTSTR("Initiating shell...\n");

        struct shell shell = shell_init(term);

        PUTSTR("Success.\n\n");
        PUTSTR("Circuit Operating System, Version ");
        PUTSTR(CIRCUIT_SYSTEM_VERSION);
        PUTSTR("\n\n");

        shell_process_command(&shell, "WELCOME");

        for (;;)
                shell_await_command(&shell);
}
