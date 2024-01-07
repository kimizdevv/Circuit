#define KDEBUG_MODE 1

#include "entry.h"
#include "terminal.h"
#include "shell.h"
#include "../lib/sys/mem.h"

#define PUTSTR(S) term_putstr(term, S)
#define PUTVER()                                      \
        PUTSTR("Circuit Operating System, Version "); \
        PUTSTR(CIRCUIT_SYSTEM_VERSION);

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
        framebuffer->pitch = tagfb->common.framebuffer_pitch;
        framebuffer->width = tagfb->common.framebuffer_width;
        framebuffer->height = tagfb->common.framebuffer_height;
        framebuffer->bpp = tagfb->common.framebuffer_bpp;
        framebuffer->type = tagfb->common.framebuffer_type;
        framebuffer->ncol = tagfb->framebuffer_palette_num_colors;
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif

static int debug_boot_mode(struct terminal *term)
{
        PUTSTR("The system was scheduled to boot into debug mode. Continue? (Y/n): ");

        char response = 0;
        while (response != 'y' && response != 'n' && response != '\n')
                shell_await_input(&response);

        term_putchr(term, (char)(response == '\n' ? 'Y' : response));
        PUTSTR("\n");

        switch (response) {
        case 'y':
        case '\n':
                break;
        default:
                return -1;
        }

        PUTSTR("> Success\n\n");

        PUTVER();
        PUTSTR("\n\n");

        PUTSTR("PERFORMING A MEMORY TEST\n\n");

        char sbuf[16] = { 0 };

        const size_t from = 1;
        const size_t to = 17;
        const size_t size = to - from;

        PUTSTR(" --  ALLOCATION BEGINS [");
        PUTSTR(itos(size, sbuf, NULL));
        PUTSTR(" CHUNKS(S)]\n\n");

        for (size_t i = from; i < to; ++i) {
                const size_t b = i;

                void *p = malloc(b);

                PUTSTR("  ====  ALLOCATED ");
                PUTSTR(itos((int)b, sbuf, NULL));
                PUTSTR(" BYTES...   PTR: ");
                PUTSTR(itos((int)(uintptr_t)p, sbuf, NULL));

                if (i % 4 != 0) {
                        free(p);
                        PUTSTR(" (DEALLOCATED)");
                }

                term_newline(term);
        }

        PUTSTR("\n --  RESULTS\n\n");

        PUTSTR("ALLOCATED CHUNKS:\n");
        const size_t tot_a = mem_list_dump(term, 0);
        PUTSTR("\nFREE CHUNKS:\n");
        const size_t tot_f = mem_list_dump(term, 1);
        PUTSTR("\n");

        const size_t total = tot_a + tot_f;
        if (total != 0x100000) {
                PUTSTR("TEST FAILED: SIZE MISMATCH BY ");
                PUTSTR(itos((int)(total - 0x100000), sbuf, NULL));
                PUTSTR(" BYTES\n");
        } else
                PUTSTR("TEST SUCCEEDED\n");

        PUTSTR("END OF TEST: PRESS ANY KEY TO REBOOT...");

        shell_await_input(NULL);

        for (;;)
                ;

        return 0;
}

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

        if (fb.bufadr == 0)
                return;

        PUTSTR("Terminal is ready\n");

        if (KDEBUG_MODE) {
                PUTSTR("> Entering debug mode...\n");
                const int user_declined = debug_boot_mode(term);
                if (user_declined != -1)
                        return;

                PUTSTR("> Failure: Operation cancelled by the user\n");
                PUTSTR("> Entering user mode...\n");
        }

        PUTSTR("> Success\n\n");
        PUTSTR("> Initiating shell...\n");

        struct shell shell = shell_init(term);

        PUTSTR("> Success\n\n");

        char buf[24] = { 0 };

        PUTSTR("VIDEO (");
        PUTSTR(itos((int)fb.type, buf, NULL));
        PUTSTR("): ");
        PUTSTR(itos((int)fb.width, buf, NULL));
        PUTSTR("x");
        PUTSTR(itos((int)fb.height, buf, NULL));
        PUTSTR("x");
        PUTSTR(itos((int)fb.bpp, buf, NULL));
        PUTSTR(", ADR (TRIM): ");
        PUTSTR(itos((int)fb.bufadr, buf, NULL));
        PUTSTR(", PALETTE: ");
        PUTSTR(itos((int)fb.ncol, buf, NULL));
        PUTSTR("\n\n");

        PUTVER();
        PUTSTR("\n\n");

        shell_process_command(&shell, "WELCOME");

        for (;;)
                shell_await_command(&shell);
}
