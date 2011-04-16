
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>

#include "bool.h"
#include "io.h"

#ifdef TAB_SEPARATOR
#define SEPARATOR "\t"
#else
#define SEPARATOR " "
#endif

#define FLAG_LINE      0
#define FLAG_WORD      1
#define FLAG_CHARACTER 2
#define FLAG_BYTE      3

typedef struct printvals
{
    size_t byte;
    size_t character;
    size_t word;
    size_t line;
} printvals;

static const char* wc_flag_set = "true";
static printvals pvals_total = { 0, 0, 0, 0 };

/* todo: m != c */
static void calcwc (FILE* stream, printvals* pvals)
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

    pvals_total.line      += l;
    pvals_total.word      += w;
    pvals_total.character += m;
    pvals_total.byte      += c;
}

static void printwc1 (const char* filepath, size_t* cols)
{
    if (filepath == NULL)
    {
        (void) printf("%d\n", cols[0]);
    }
    else
    {
        (void) printf("%d" SEPARATOR "%s\n", cols[0], filepath);
    }
}

static void printwc2 (const char* filepath, size_t* cols)
{
    if (filepath == NULL)
    {
        (void) printf("%d" SEPARATOR "%d\n", cols[0], cols[1]);
    }
    else
    {
        (void) printf("%d" SEPARATOR "%d" SEPARATOR "%s\n", cols[0], cols[1], filepath);
    }
}

static void printwc3 (const char* filepath, size_t* cols)
{
    if (filepath == NULL)
    {
        (void) printf("%d" SEPARATOR "%d" SEPARATOR "%d\n", cols[0], cols[1], cols[2]);
    }
    else
    {
        (void) printf("%d" SEPARATOR "%d" SEPARATOR "%d" SEPARATOR "%s\n", cols[0], cols[1], cols[2], filepath);
    }
}

static void printwc (const char* filepath, const char** flags, printvals* pvals)
{
    int numdo = 0;
    size_t cols[3];
    void (*printwcfuncs[])(const char*, size_t*) = { NULL, printwc1, printwc2, printwc3 };

    if (flags[FLAG_LINE] != NULL)
    {
        cols[numdo++] = pvals->line;
    }
    if (flags[FLAG_WORD] != NULL)
    {
        cols[numdo++] = pvals->word;
    }
    if (flags[FLAG_CHARACTER] != NULL)
    {
        cols[numdo++] = pvals->character;
    }
    if (flags[FLAG_BYTE] != NULL)
    {
        cols[numdo++] = pvals->byte;
    }

    printwcfuncs[numdo](filepath, cols);
}

static void wc (char* filename, FILE* strm, void* data)
{
    const char** flags = (const char**) data;
    printvals pvals;

    calcwc(strm, &pvals);
    printwc(filename, flags, &pvals);
}

int main (int argc, char** argv)
{
    const char* flags[4] = { NULL };
    const char* flagstr = "lwmc";

    (void) input_flags(argc, argv, flagstr, flags);

    if (flags[FLAG_CHARACTER] != NULL)
    {
        flags[FLAG_BYTE] = NULL;
    }

    if ((flags[FLAG_LINE]      == NULL) &&
        (flags[FLAG_WORD]      == NULL) &&
        (flags[FLAG_CHARACTER] == NULL) &&
        (flags[FLAG_BYTE]      == NULL))
    {
        flags[FLAG_LINE] = wc_flag_set;
        flags[FLAG_WORD] = wc_flag_set;
        flags[FLAG_BYTE] = wc_flag_set;
    }

    input_files(argc, argv, wc, flags);

    if ((optind + 1) < argc)
    {
        printwc("total", flags, &pvals_total);
    }

    return EXIT_SUCCESS;
}
