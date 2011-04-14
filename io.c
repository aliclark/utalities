
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "bool.h"
#include "io.h"

/* Doesn't include the trailing \n . Must be free'd manually */
char* read_line (void)
{
    size_t i = 0;
    size_t len = 8;
    char* rv = (char*) malloc(len);
    int tmp;

    while (true)
    {
        if (i == len)
        {
            len *= 2;
            rv = (char*) realloc(rv, len);
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

void input_files (void (*func)(char*, FILE*, void*), int argc, char** argv, void* data)
{
    bool isstdin;
    FILE* strm;
    int i;

    if (optind == argc)
    {
        func(NULL, stdin, data);
    }
    else
    {
        for (i = optind; i < argc; ++i)
        {
            isstdin = strcmp(argv[i], "-") == 0;
            strm = isstdin ? stdin : fopen(argv[i], "r");

            func(argv[i], strm, data);

            if (isstdin)
            {
                fclose(strm);
            }
        }
    }
}
