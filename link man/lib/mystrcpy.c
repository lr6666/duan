#include <assert.h>

char * mystrcpy(char * dest, const char *src)
{
    char *temp = dest;
    assert(src);

    while((*dest++ = *src++) != '\0');
    
    return temp;
}

