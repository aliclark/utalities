
#include <stdlib.h>
#include <stdio.h>

int main (int argc, char** argv)
{
    int i = 1;

    if (i < argc)
    {
        fputs(argv[i++], stdout);
    }

    for (; i < argc; ++i)
    {
        putchar(' ');
        fputs(argv[i], stdout);
    }
    putchar('\n');

    return EXIT_SUCCESS;
}
