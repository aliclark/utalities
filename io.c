
#include <stdlib.h>
#include <stdio.h>

#include "bool.h"
#include "io.h"

/* Doesn't include the trailing \n . Must be free'd manually */
char* read_line ()
{
    size_t i = 0;
    size_t len = 8;
    char* rv = malloc(len);
    int tmp;

    while (true)
    {
        if (i == len)
        {
            len *= 2;
            rv = realloc(rv, len);
        }

        tmp = getchar();

        if ((tmp == EOF) && (i == 0))
        {
            free(rv);
            return NULL;
        }

        if ((tmp == '\n') || (tmp == EOF))
        {
            rv[i] = '\0';
            return rv;
        }

        rv[i++] = tmp;
    }
}
