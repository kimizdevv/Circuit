#include <stddef.h>
#include "../../kernel/terminal.h"

void *malloc(size_t size);

void *calloc(size_t num, size_t size);

void free(void *ptr);

void *realloc(void *ptr, size_t size);

size_t mem_list_dump(struct terminal *term, int which);
