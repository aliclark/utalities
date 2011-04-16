
DEPS="io.o $1.o"
redo-ifchange $DEPS
clang -o $3 $DEPS
