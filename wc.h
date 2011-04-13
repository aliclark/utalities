
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
