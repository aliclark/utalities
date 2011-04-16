
redo-ifchange ../src/wc.c

if [ "$CC" = "" ]; then
  CC=clang
fi

if [ "$DEBUG" = "" ]; then
  OPTIMISE=-O2
else
  OPTIMISE=-g
fi

$CC -D TAB_SEPARATOR -I../include $OPTIMISE -Wall -Wextra -pedantic -c -o $3 ../src/wc.c
