
#include <stdlib.h>

#include "bool.h"

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
