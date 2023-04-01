#!/bin/bash

CC="clang"
NAME="Lang"
C_FLAGS=""
SRC=$(find ../src/ -type f -name "*.c")
ARGS=""
for i in {1..10}
do
    if [ -n "${!i}" ]; then
        ARGS+=${!i}
        ARGS+=" "
    fi
done

$CC $C_FLAGS $SRC -o $NAME




EXECUTE="./$NAME"

$EXECUTE $ARGS 
