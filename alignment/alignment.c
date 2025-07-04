#include "alignment.h"

#include <stddef.h>

size_t align(size_t size)
{
    size_t res;
    if (size % sizeof(long double) == 0)
        return size;
    size_t remainder = size / sizeof(long double);
    if (__builtin_mul_overflow(remainder + 1, sizeof(long double), &res))
        return 0;
    return (remainder + 1) * sizeof(long double);
}
