
#include <stdlib.h>
#include <stdio.h>

int main (int argc, char** argv)
{
    int i = 1;

    if (i < argc)
    {
        (void) fputs(argv[i++], stdout);
    }

    for (; i < argc; ++i)
    {
        (void) putchar(' ');
        (void) fputs(argv[i], stdout);
    }
    (void) putchar('\n');

    return EXIT_SUCCESS;
}
