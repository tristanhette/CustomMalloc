#include <stddef.h>

void *page_begin(void *ptr, size_t page_size)
{
    char *b = ptr;
    char *a = NULL;
    while (a <= b)
    {
        a = a + page_size;
    }
    return a - page_size;
}
