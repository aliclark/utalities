
redo-ifchange ../src/wc.c
clang -D TAB_SEPARATOR -I../include -g -Wall -Wextra -pedantic -c -o $3 ../src/wc.c
