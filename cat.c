
#include <stdlib.h>
#include <stdio.h>

#include "bool.h"
#include "util.h"
#include "io.h"

static void cat (char* filename, FILE* strm, void* data)
{
    int tmp;

    UNUSED(filename);
    UNUSED(data);

    while (true)
    {
        tmp = getc(strm);

        if (tmp == EOF)
        {
            return;
        }

        putchar(tmp);
    }
}

int main (int argc, char** argv)
{
    const char* flags[1] = { NULL };
    const char* flagstr = "u";

    (void) input_flags(argc, argv, flagstr, flags);

    if (flags[0] != NULL)
    {
        setvbuf(stdout, NULL, _IONBF, 0);
    }

    input_files(argc, argv, cat, NULL);

    return EXIT_SUCCESS;
}
