#ifndef ARCHI_H
#define ARCHI_H

#include <stddef.h>

struct block
{
    struct block *next;
};

struct bucket
{
    size_t block_size;
    struct block *free_list;
    struct bucket *next_bucket;
};

#endif /* ! ARCHI_H */
