
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

typedef struct printvals
{
    size_t byte;
    size_t character;
    size_t word;
    size_t line;
} printvals;

/* todo: m != c */
static void wc (FILE* stream, printvals* pvals)
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

    m = c;

    pvals->line      = l;
    pvals->word      = w;
    pvals->character = m;
    pvals->byte      = c;
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

static void printwc (printopts* popts, const char* filepath, printvals* pvals)
{
    int numdo = 0;
    size_t cols[3];
    void (*printwcfuncs[])(const char*, size_t*) = { NULL, printwc1, printwc2, printwc3 };

    if (popts->line)
    {
        cols[numdo++] = pvals->line;
    }
    if (popts->word)
    {
        cols[numdo++] = pvals->word;
    }
    if (popts->character)
    {
        cols[numdo++] = pvals->character;
    }
    if (popts->byte)
    {
        cols[numdo++] = pvals->byte;
    }

    printwcfuncs[numdo](filepath, cols);
}

static void getprintopts (int argc, char* const *argv, printopts* popts)
{
    int arg;
    bool usedefault = true;

    popts->line      = false;
    popts->word      = false;
    popts->character = false;
    popts->byte      = false;

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
    printvals pvals_total;
    printvals pvals;
    printopts popts;
    char* filename;
    bool usestdin = false;
    FILE* strm;
    int i;

    getprintopts(argc, argv, &popts);

    pvals_total.line      = 0;
    pvals_total.word      = 0;
    pvals_total.character = 0;
    pvals_total.byte      = 0;

    if (optind == argc)
    {
        wc(stdin, &pvals);
        printwc(&popts, NULL, &pvals);
    }
    else
    {
        for (i = optind; i < argc; ++i)
        {
            filename = argv[i];

            usestdin = (filename[0] == '-') && (filename[1] == '\0');
            strm = usestdin ? stdin : fopen(filename, "r");

            wc(strm, &pvals);

            if (!usestdin)
            {
                fclose(strm);
            }

            pvals_total.line      += pvals.line;
            pvals_total.word      += pvals.word;
            pvals_total.character += pvals.character;
            pvals_total.byte      += pvals.byte;

            printwc(&popts, filename, &pvals);
        }

        if ((argc - optind) > 1)
        {
            printwc(&popts, "total", &pvals_total);
        }
    }

    return EXIT_SUCCESS;
}
