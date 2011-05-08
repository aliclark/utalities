
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>

#include "bool.h"
#include "io.h"

static const char* input_flags_found = "true";
static const char* input_files_error = "Unknown error";

/* Doesn't include the trailing \n . Must be free'd manually */
char* read_line (void)
{
    size_t i = 0;
    size_t len = 8;
    char* rv = (char*) malloc(len);
    int tmp;

    while (true)
    {
        if (i == len)
        {
            len *= 2;
            rv = (char*) realloc(rv, len);
        }

        tmp = getchar();

        if ((tmp == EOF) && (i == 0))
        {
            free(rv);
            return NULL;
        }

        if ((tmp == '\n') || (tmp == EOF))
        {
            rv[i] = '\0';
            return rv;
        }

        rv[i++] = tmp;
    }
}

void input_files (int argc, char** argv, void (*func)(char*, FILE*, void*), void* data)
{
    bool isstdin;
    FILE* strm;
    int i;
    int myerrno;

    if (optind == argc)
    {
        assert(stdin != NULL);
        func(NULL, stdin, data);
    }
    else
    {
        for (i = optind; i < argc; ++i)
        {
            isstdin = strcmp(argv[i], "-") == 0;
            assert(!isstdin || (stdin != NULL));
            errno = 0;
            strm = isstdin ? stdin : fopen(argv[i], "r");
            myerrno = errno;

            if (strm == NULL)
            {
                (void) fputs(argv[0], stderr);
                (void) fputs(": ",    stderr);
                (void) fputs(argv[i], stderr);
                (void) fputs(": ",    stderr);

                if (myerrno == 0)
                {
                    (void) fputs(input_files_error, stderr);
                }
                else
                {
                    (void) fputs(strerror(myerrno), stderr);
                }
                (void) putc('\n', stderr);
                return;
            }

            func(argv[i], strm, data);

            if (isstdin)
            {
                (void) fclose(strm);
            }
        }
    }
}

/* fixme: ind should not be affected by ':' chars in optstring */
int input_flags (int argc, char* const *argv, const char* optstring, const char ** flags)
{
    const char* pos;
    int arg;
    int ind;

    while ((arg = getopt(argc, argv, optstring)) != -1)
    {
        if (arg == '?')
        {
            return 1;
        }
        if (arg == ':')
        {
            return 2;
        }

        pos = strchr(optstring, arg);
        ind = pos - optstring;

        if (pos[1] == ':')
        {
            flags[ind] = optarg;
        }
        else
        {
            flags[ind] = input_flags_found;
        }
    }

    return 0;
}
