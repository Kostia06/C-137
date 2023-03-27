#!/bin/bash

CC="clang"
NAME="Lang"
C_FLAGS=""
SRC=$(find ../src/ -type f -name "*.c")

$CC $C_FLAGS $SRC -o $NAME

EXECUTE="./$NAME ../test/main.c137"

$EXECUTE
