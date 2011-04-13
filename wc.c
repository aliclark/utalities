
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>

#ifndef __cplusplus
typedef int bool;
#define false 0
#define true  1
#endif

typedef struct printopts
{
    bool byte;
    bool character;
    bool word;
    bool line;
} printopts;

/* todo: m != c */
static void wc (FILE* stream, size_t* lp, size_t* wp, size_t* mp, size_t* cp)
{
    size_t l = 0, w = 0, m = 0, c = 0;
    int ch;
    bool inword = false;

    while (true)
    {
        ch = fgetc(stream);

        if (ch == EOF)
        {
            if (inword)
            {
                ++w;
            }
            break;
        }

        if (ch == '\n')
        {
            ++l;
        }

        if (isspace(ch))
        {
            if (inword)
            {
                ++w;
                inword = false;
            }
        }
        else
        {
            inword = true;
        }

        ++c;
    }

    *lp = l;
    *wp = w;
    *mp = c;
    *cp = c;
}

static void printwc1 (const char* filepath, size_t* cols)
{
    if (filepath == NULL)
    {
        printf("%d\n", cols[0]);
    }
    else
    {
        printf("%d %s\n", cols[0], filepath);
    }
}

static void printwc2 (const char* filepath, size_t* cols)
{
    if (filepath == NULL)
    {
        printf("%d %d\n", cols[0], cols[1]);
    }
    else
    {
        printf("%d %d %s\n", cols[0], cols[1], filepath);
    }
}

static void printwc3 (const char* filepath, size_t* cols)
{
    if (filepath == NULL)
    {
        printf("%d %d %d\n", cols[0], cols[1], cols[2]);
    }
    else
    {
        printf("%d %d %d %s\n", cols[0], cols[1], cols[2], filepath);
    }
}

static void printwc (printopts* popts, const char* filepath, size_t l, size_t w, size_t m, size_t c)
{
    int numdo = 0;
    size_t cols[3];
    void (*printwcfuncs[])(const char*, size_t*) = { NULL, printwc1, printwc2, printwc3 };

    if (popts->line)
    {
        cols[numdo++] = l;
    }
    if (popts->word)
    {
        cols[numdo++] = w;
    }
    if (popts->character)
    {
        cols[numdo++] = m;
    }
    if (popts->byte)
    {
        cols[numdo++] = c;
    }

    printwcfuncs[numdo](filepath, cols);
}

static void getprintopts (int argc, char* const *argv, printopts* popts)
{
    int arg;
    bool usedefault = true;

    popts->line = false;
    popts->word = false;
    popts->character = false;
    popts->byte = false;

    while ((arg = getopt(argc, argv, "lwmc")) != -1)
    {
        usedefault = false;

        switch (arg)
        {
        case 'l':
            popts->line = true;
            break;
        case 'w':
            popts->word = true;
            break;
        case 'm':
            popts->character = true;
            break;
        case 'c':
            popts->byte = true;
            break;
        }
    }

    if (usedefault)
    {
        popts->line = true;
        popts->word = true;
        popts->byte = true;
    }
    else if (popts->character)
    {
        popts->byte = false;
    }
}

int main (int argc, char** argv)
{
    size_t totall = 0, totalw = 0, totalm = 0, totalc = 0;
    size_t l, w, m, c;
    size_t i;
    char* filename;
    printopts popts;
    bool usestdin = false;
    FILE* strm;

    getprintopts(argc, argv, &popts);

    if (optind == argc)
    {
        wc(stdin, &l, &w, &m, &c);
        printwc(&popts, NULL, l, w, m, c);
    }
    else
    {
        for (i = optind; i < argc; ++i)
        {
            filename = argv[i];

            usestdin = (filename[0] == '-') && (filename[1] == '\0');
            strm = usestdin ? stdin : fopen(filename, "r");

            wc(strm, &l, &w, &m, &c);

            if (!usestdin)
            {
                fclose(strm);
            }

            totall += l;
            totalw += w;
            totalm += m;
            totalc += c;

            printwc(&popts, filename, l, w, m, c);
        }

        if ((argc - optind) > 1)
        {
            printwc(&popts, "total", totall, totalw, totalm, totalc);
        }
    }

    return EXIT_SUCCESS;
}
