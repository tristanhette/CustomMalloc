#include "recycler.h"

#include <stddef.h>
#include <stdlib.h>

struct recycler *recycler_create(size_t block_size, size_t total_size)
{
    if (block_size % sizeof(size_t) != 0 || block_size == 0 || total_size == 0
        || total_size % block_size != 0)
        return NULL;
    struct recycler *new = malloc(sizeof(struct recycler));
    if (!new)
        return NULL;
    new->block_size = block_size;
    new->capacity = total_size / block_size;
    struct free_list *ch = malloc(total_size * sizeof(struct free_list));
    if (!ch)
    {
        free(new);
        return NULL;
    }
    new->chunk = ch;
    struct free_list *tmp = new->chunk;
    new->free = tmp;
    struct free_list *b = new->free;
    for (size_t i = 0; i < new->capacity - 1; i++)
    {
        b->next = b + block_size;
        b = b->next;
    }
    b = NULL;
    return new;
}

void recycler_destroy(struct recycler *r)
{
    if (!r)
        return;
    free(r->chunk);
    free(r);
}

void *recycler_allocate(struct recycler *r)
{
    if (!r || !r->free)
        return NULL;
    struct free_list *l = r->free;
    r->free = l->next;
    return l;
}

void recycler_free(struct recycler *r, void *block)
{
    if (!r || !block)
        return;
    struct free_list *b = block;
    b->next = r->free;
    r->free = b;
}
