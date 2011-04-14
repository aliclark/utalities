
#include <stdlib.h>
#include <stdio.h>

#include "bool.h"
#include "util.h"
#include "io.h"

#define FLAG_UNBUFFERED 0

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

        (void) putchar(tmp);
    }
}

int main (int argc, char** argv)
{
    const char* flags[1] = { NULL };
    const char* flagstr = "u";

    (void) input_flags(argc, argv, flagstr, flags);

    if (flags[FLAG_UNBUFFERED] != NULL)
    {
        setbuf(stdout, NULL);
    }

    input_files(argc, argv, cat, NULL);

    return EXIT_SUCCESS;
}
