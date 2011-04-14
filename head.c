
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "util.h"
#include "bool.h"
#include "io.h"

#define OPTION_N 0

static int files_seen = 0;
static bool multiple_mode = false;

static void head (char* filename, FILE* strm, void* data)
{
    int tmp;
    int n = *((int*) data);
    int i = 0;

    UNUSED(filename);

    if (multiple_mode)
    {
        if (files_seen++ != 0)
        {
            putchar('\n');
        }

        fputs("==> ",   stdout);
        fputs(filename, stdout);
        fputs(" <==\n", stdout);
    }

    while (true)
    {
        tmp = getc(strm);

        if (tmp == EOF)
        {
            return;
        }

        putchar(tmp);

        if (tmp == '\n')
        {
            ++i;

            if (i == n)
            {
                return;
            }
        }
    }
}

int main (int argc, char** argv)
{
    const char* flags[1] = { NULL };
    const char* flagstr = "n:";
    int n = 10;

    (void) input_flags(argc, argv, flagstr, flags);

    if (flags[OPTION_N] != NULL)
    {
        n = atoi(flags[OPTION_N]);

        if (n <= 0)
        {
            n = 1;
        }
    }

    if ((optind + 1) < argc)
    {
        multiple_mode = true;
    }

    input_files(argc, argv, head, &n);

    return EXIT_SUCCESS;
}
