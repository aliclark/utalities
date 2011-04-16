
redo-ifchange ../src/$1.c

if [ "$CC" = "" ]; then
  CC=clang
fi

if [ "$DEBUG" = "" ]; then
  OPTIMISE=-O2
else
  OPTIMISE=-g
fi

$CC -I../include $OPTIMISE -Wall -Wextra -pedantic -c -o $3 ../src/$1.c
