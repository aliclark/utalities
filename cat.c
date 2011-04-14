
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "cat.h"

static void cat (FILE* strm)
{
    int tmp;

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
    bool immediate = false;
    bool isstdin;
    FILE* strm;
    int arg;
    int i;

    while ((arg = getopt(argc, argv, "u")) != -1)
    {
        switch (arg)
        {
        case 'u':
            immediate = true;
            break;
        }
    }

    if (immediate)
    {
        setvbuf(stdout, NULL, _IONBF, 0);
    }

    if (optind == argc)
    {
        cat(stdin);
    }
    else
    {
        for (i = optind; i < argc; ++i)
        {
            isstdin = strcmp(argv[i], "-") == 0;
            strm = isstdin ? stdin : fopen(argv[i], "r");

            cat(strm);

            if (isstdin)
            {
                fclose(strm);
            }
        }
    }

    return EXIT_SUCCESS;
}
