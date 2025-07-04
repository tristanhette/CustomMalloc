#include <stddef.h>

void *beware_overflow(void *ptr, size_t nmemb, size_t size)
{
    size_t res;
    char *a = ptr;
    if (__builtin_mul_overflow(nmemb, size, &res))
        return NULL;
    a = a + res;
    return a;
}
