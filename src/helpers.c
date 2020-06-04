#include "helpers.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Thought you might find this handy:
char* strdupb(const char* s, const char* blame)
{
    char* result = mallocb(strlen(s) + 1, blame);
    return strcpy(result, s);
}


// Maybe this too:
void* mallocb(size_t size, const char* blame)
{
    void* result = malloc(size);
    if (!result) {
        perror(blame);
        exit(1);
    }

    return result;
}
