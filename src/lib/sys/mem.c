#include "mem.h"
#include "string.h"
#include <stdint.h>

#define MEM_BEGIN 0x3000
#define MEM_END 0x103000 // 0x7000
#define MEM_LENGTH (MEM_END - MEM_BEGIN)
#define CHUNK_NUM 1024

struct chunk {
        size_t size;
        uint8_t *ptr;
};

// chunks are inserted starting from the last index
// because this list works like a stack.
struct chunk_list {
        struct chunk chunks[CHUNK_NUM];
        size_t count;
};

struct chunk_list allocated_chunks = { 0 };

struct chunk_list free_chunks = {
        .chunks = { (struct chunk){
                .size = MEM_LENGTH,
                .ptr = (uint8_t *)MEM_BEGIN,
        } },
        .count = 1,
};

struct chunk_list tmp_chunks = { 0 };

static size_t read_chunk_size(struct chunk chunk)
{
        const uint8_t *b = chunk.ptr;
        return (size_t)((*(b - 4) << 24) | (*(b - 3) << 16) | (*(b - 2) << 8) |
                        *(b - 1));
}

static void write_chunk_size(struct chunk chunk, const size_t size)
{
        const uint8_t b3 = (uint8_t)(size & 0xFF000000);
        const uint8_t b2 = (uint8_t)(size & 0x00FF0000);
        const uint8_t b1 = (uint8_t)(size & 0x0000FF00);
        const uint8_t b0 = (uint8_t)(size & 0x000000FF);
        uint8_t *b = chunk.ptr;
        *(b - 1) = b0;
        *(b - 2) = b1;
        *(b - 3) = b2;
        *(b - 4) = b3;
}

static void chunk_list_insert(struct chunk_list *list, struct chunk chunk)
{
        if (list->count >= CHUNK_NUM)
                return;

        list->chunks[list->count].ptr = chunk.ptr;
        list->chunks[list->count].size = chunk.size;

        for (size_t i = list->count;
             i > 0 && list->chunks[i].ptr < list->chunks[i - 1].ptr; --i) {
                const struct chunk tmp = list->chunks[i];
                list->chunks[i] = list->chunks[i - 1];
                list->chunks[i - 1] = tmp;
        }

        list->count++;
}

static void chunk_list_remove(struct chunk_list *list, size_t index)
{
        if (index >= list->count)
                return;

        for (size_t i = index; i < list->count - 1; ++i)
                list->chunks[i] = list->chunks[i + 1];

        list->count--;
}

static int chunk_list_find(const struct chunk_list *list, const uint8_t *ptr)
{
        for (size_t i = 0; i < list->count; ++i)
                if (ptr == list->chunks[i].ptr)
                        return (int)i;

        return -1;
}

// finds and merges contiguous free chunks
// * not supposed to be used with allocated_list!
static void chunk_list_merge(struct chunk_list *dst,
                             const struct chunk_list *src)
{
        dst->count = 0;

        for (size_t i = 0; i < src->count; ++i) {
                const struct chunk chunk = src->chunks[i];

                if (dst->count == 0) {
                        chunk_list_insert(dst, chunk);
                        continue;
                }

                struct chunk *top_chunk = &dst->chunks[dst->count - 1];

                if (top_chunk->ptr + top_chunk->size == chunk.ptr) {
                        top_chunk->size += chunk.size;
                } else {
                        chunk_list_insert(dst, chunk);
                }
        }
}

static size_t chunk_list_dump(struct terminal *term, struct chunk_list *list)
{
        char buf[16];
        size_t total_size = 0;

        for (size_t i = 0; i < list->count; ++i) {
                const struct chunk chunk = list->chunks[i];
                term_putstr(term, "    [");
                term_putstr(term, itos((int)i, buf, NULL));
                term_putstr(term, "]   PTR: ");
                term_putstr(term, itos((int)(uintptr_t)chunk.ptr, buf, NULL));
                term_putstr(term, "   SIZE: ");
                term_putstr(term, itos((int)chunk.size, buf, NULL));
                term_putstr(term, "\n");
                total_size += chunk.size;
        }
        term_putstr(term, "  TOTAL SIZE: ");
        term_putstr(term, itos((int)total_size, buf, NULL));
        term_putstr(term, "\n");

        return total_size;
}

void *malloc(size_t size)
{
        if (size == 0)
                return NULL;

        chunk_list_merge(&tmp_chunks, &free_chunks);
        free_chunks = tmp_chunks;

        // find a free chunk, reserve it, and split it up.
        for (size_t i = 0; i < free_chunks.count; ++i) {
                const struct chunk chunk = free_chunks.chunks[i];
                if (chunk.size >= size) {
                        chunk_list_remove(&free_chunks, i);

                        chunk_list_insert(&allocated_chunks,
                                          (struct chunk){
                                                  .size = size,
                                                  .ptr = chunk.ptr,
                                          });

                        const size_t tail_size = chunk.size - size;

                        if (tail_size > 0)
                                chunk_list_insert(
                                        &free_chunks,
                                        (struct chunk){
                                                .size = tail_size,
                                                .ptr = chunk.ptr + size,
                                        });

                        return chunk.ptr;
                }
        }

        return NULL;
}

void *calloc(const size_t num, const size_t size)
{
        const size_t real_size = num * size;
        void *ptr = malloc(real_size);
        memset(ptr, 0, real_size);
        return ptr;
}

void free(void *ptr)
{
        if (ptr == NULL)
                return;

        const int i = chunk_list_find(&allocated_chunks, ptr);
        if (i < 0)
                return;

        chunk_list_insert(&free_chunks, allocated_chunks.chunks[i]);
        chunk_list_remove(&allocated_chunks, (size_t)i);
}

void *realloc(void *ptr, size_t size)
{
        if (ptr == NULL)
                return NULL;

        const int i = chunk_list_find(&allocated_chunks, ptr);
        if (i < 0)
                return NULL;

        void *p = malloc(size);
        memcpy(p, ptr, allocated_chunks.chunks[i].size);
        free(ptr);
        return p;
}

size_t mem_list_dump(struct terminal *term, const int which)
{
        switch (which) {
        case 0:
                return chunk_list_dump(term, &allocated_chunks);
        case 1:
                return chunk_list_dump(term, &free_chunks);
        default:
                return 0;
        }
}
