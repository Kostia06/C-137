#!/bin/bash

CC="clang"
NAME="Lang"
#- toml
C_FLAGS=" -I /opt/homebrew/Cellar/llvm/16.0.2/include/ -L /opt/homebrew/Cellar/llvm/16.0.2/lib/ -lLLVM"
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

time $EXECUTE $ARGS 
