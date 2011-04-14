
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

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
    bool immediate = false;
    int arg;

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

    input_files(cat, argc, argv, NULL);

    return EXIT_SUCCESS;
}
