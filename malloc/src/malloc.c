#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include "archi.h"

struct bucket *bucket_list = NULL;

static int all_free(struct bucket *buck)
{
    struct block *free_l = buck->free_list;
    size_t page_size = sysconf(_SC_PAGESIZE);
    size_t nb_block = page_size / buck->block_size;
    size_t i = 0;
    if (buck->block_size >= page_size)
        return 1;
    while (i < nb_block)
    {
        if (!free_l->next)
            return 0;
        free_l = free_l->next;
        i++;
    }
    return 1;
}

struct bucket *find_bucket_by_pointer(void *ptr)
{
    size_t page_size = sysconf(_SC_PAGESIZE);
    struct bucket *curr_buck = bucket_list;
    while (curr_buck != NULL)
    {
        void *curr_buckk = curr_buck;
        char *curr_buckkk = curr_buckk;
        char *bucket_start = curr_buckkk + sizeof(struct bucket) + 8;
        char *bucket_end = curr_buckkk + sizeof(struct bucket) + page_size + 8;
        char *p = ptr;
        if (p >= bucket_start && p <= bucket_end)
        {
            return curr_buck;
        }
        curr_buck = curr_buck->next_bucket;
    }
    return NULL;
}

static size_t align(size_t size)
{
    size_t res;
    if (size % sizeof(long double) == 0)
        return size;
    size_t remainder = size / sizeof(long double);
    if (__builtin_mul_overflow(remainder + 1, sizeof(long double), &res))
        return 0;
    return (remainder + 1) * sizeof(long double);
}

struct bucket *find_buck(size_t block_size)
{
    struct bucket *curr_buck = bucket_list;
    struct bucket *buck = NULL;
    while (curr_buck != NULL)
    {
        if (curr_buck->block_size == block_size)
        {
            if (!curr_buck->free_list)
                buck = curr_buck;
            else
                return curr_buck;
        }
        curr_buck = curr_buck->next_bucket;
    }
    return buck;
}

static size_t page_si(size_t page_size, size_t block_size)
{
    while (page_size < block_size)
        page_size *= 2;
    return page_size;
}

struct bucket *init_buck(size_t block_size)
{
    size_t page_size = sysconf(_SC_PAGESIZE);
    size_t nb_block = 1;
    if (block_size <= page_size)
        nb_block = page_size / block_size;
    else
        page_size = page_si(page_size, block_size);
    struct bucket *new_buck =
        mmap(NULL, sizeof(struct bucket) + page_size + 8,
             PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (new_buck == MAP_FAILED)
        return NULL;
    new_buck->block_size = block_size;
    new_buck->free_list = NULL;
    new_buck->next_bucket = bucket_list;
    bucket_list = new_buck;
    void *new_buckk = new_buck;
    char *new_buckkk = new_buckk;
    char *memory_start = new_buckkk + sizeof(struct bucket) + 8;
    if (nb_block > 1)
    {
        for (size_t i = 0; i < nb_block + 1; ++i)
        {
            void *memory_generic = memory_start + i * block_size;
            struct block *new_block_free = memory_generic;
            new_block_free->next = new_buck->free_list;
            new_buck->free_list = new_block_free;
        }
    }
    else
    {
        void *memory_generic = memory_start;
        struct block *new_block_free = memory_generic;
        new_block_free->next = new_buck->free_list;
        new_buck->free_list = new_block_free;
    }
    return new_buck;
}

struct bucket *get_buck(size_t block_size)
{
    struct bucket *found_buck = find_buck(block_size);
    if (found_buck != NULL)
        return found_buck;
    return init_buck(block_size);
}

void *block_allocator(struct bucket *buck)
{
    if (buck->free_list == NULL)
    {
        struct bucket *bucket = init_buck(buck->block_size);
        void *ptr = bucket->free_list;
        bucket->free_list = bucket->free_list->next;
        return ptr;
    }
    void *ptr = buck->free_list;
    buck->free_list = buck->free_list->next;
    return ptr;
}

__attribute__((visibility("default"))) void *malloc(size_t size)
{
    size = align(size);
    if (size == 0)
        return NULL;
    size_t block_size = 1;
    while (block_size < size)
        block_size *= 2;
    struct bucket *buck = get_buck(block_size);
    if (!buck)
        return NULL;
    return block_allocator(buck);
}

__attribute__((visibility("default"))) void free(void *ptr)
{
    if (!ptr)
        return;
    struct bucket *buck = find_bucket_by_pointer(ptr);
    struct block *block = ptr;
    block->next = buck->free_list;
    buck->free_list = block;
    size_t page_size = sysconf(_SC_PAGESIZE);
    if (all_free(buck))
    {
        while (page_size < buck->block_size)
        {
            page_size *= 2;
        }
        if (bucket_list == buck)
            bucket_list = buck->next_bucket;
        else
        {
            struct bucket *prev = bucket_list;
            while (prev && prev->next_bucket != buck)
            {
                prev = prev->next_bucket;
            }
            if (prev)
            {
                prev->next_bucket = buck->next_bucket;
            }
        }
        munmap(buck, sizeof(struct bucket) + page_size + 8);
    }
}

__attribute__((visibility("default"))) void *calloc(size_t nmemb, size_t size)
{
    size_t total_size;
    if (__builtin_mul_overflow(nmemb, size, &total_size))
        return NULL;
    size = nmemb * size;
    size = align(size);
    if (size == 0)
        return NULL;
    size_t block_size = 1;
    while (block_size < size)
        block_size *= 2;
    if (__builtin_mul_overflow(nmemb, block_size, &total_size))
        return NULL;
    void *ptr = malloc(block_size);
    if (!ptr)
        return NULL;
    memset(ptr, 0, block_size);
    return ptr;
}

__attribute__((visibility("default"))) void *realloc(void *ptr, size_t size)
{
    if (!ptr)
        return malloc(size);
    if (size == 0)
    {
        free(ptr);
        return NULL;
    }
    size = align(size);
    size_t block_size = 1;
    while (block_size < size)
    {
        block_size *= 2;
    }
    struct bucket *buck = find_bucket_by_pointer(ptr);
    if (size <= buck->block_size)
        return ptr;
    else
    {
        void *new_ptr = malloc(block_size);
        if (!new_ptr)
            return NULL;
        memcpy(new_ptr, ptr, buck->block_size);
        free(ptr);
        return new_ptr;
    }
}
