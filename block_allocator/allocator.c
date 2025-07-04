#include "allocator.h"

#include <stddef.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

struct blk_allocator *blka_new(void)
{
    struct blk_allocator *new = malloc(sizeof(struct blk_allocator));
    if (!new)
        return NULL;
    new->meta = NULL;
    return new;
}

void blka_delete(struct blk_allocator *blka)
{
    if (!blka)
        return;
    while (blka->meta)
    {
        blka_pop(blka);
    }
    free(blka);
}

struct blk_meta *blka_alloc(struct blk_allocator *blka, size_t size)
{
    size_t page_size = sysconf(_SC_PAGESIZE);
    int i = 0;
    while (size > page_size)
    {
        i++;
        page_size *= i;
    }
    struct blk_meta *m = mmap(NULL, page_size, PROT_READ | PROT_WRITE,
                              MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (m == MAP_FAILED)
        return NULL;
    m->next = blka->meta;
    m->size = page_size - sizeof(struct blk_meta *) - sizeof(size_t);
    blka->meta = m;
    return m;
}
void blka_free(struct blk_meta *blk)
{
    if (!blk)
        return;
    munmap(blk, sizeof(struct blk_meta) + blk->size);
}
void blka_pop(struct blk_allocator *blka)
{
    if (!blka || !blka->meta)
        return;
    struct blk_meta *m = blka->meta;
    blka->meta = blka->meta->next;
    blka_free(m);
}
