
# what if there are other headers?

redo-ifchange ../src/$1.c
clang -I../include -g -Wall -Wextra -pedantic -c -o $3 ../src/$1.c
