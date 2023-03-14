#!/bin/bash

CC="clang"
NAME="Lang"
C_FLAGS=""
SRC=$(find ../src/ -type f -name "*.c")
FRAMEWORK_FLAGS="-I /opt/homebrew/Cellar/ -I /opt/homebrew/Cellar/llvm/15.0.7_1/include -I /opt/homebrew/include/lua5.4/"
LIBRARY_FLAGS="/opt/homebrew/lib/liblua.a"

$CC $C_FLAGS $SRC -o $NAME $FRAMEWORK_FLAGS $LIBRARY_FLAGS

EXECUTE="./$NAME"

$EXECUTE