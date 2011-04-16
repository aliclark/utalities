
DEPS="$1.o"
redo-ifchange $DEPS

if [ "$CC" = "" ]; then
  CC=clang
fi

$CC -o $3 $DEPS
