#include "beware_overflow.h"
#include <stddef.h>

int main(void)
{
    uint8_t a = 255;
    uint8_t b = 3;
    uint8_t c = a + b;
    printf("%u + %u = %u\n", a, b, c)
    beware_overflow(ptr,nmemb,size);
    return 0;
}
