#!/bin/bash

CC="clang"
NAME="Lang"
C_FLAGS=""
SRC=$(find ../src/ -type f -name "*.c")

LUA_INCLUDE_DIR="-I /opt/homebrew/include/lua5.4/"
LUA_LIBRARIES="/opt/homebrew/lib/liblua.a"

LLVM_INCLUDE_DIR="-L/opt/homebrew/opt/llvm/lib"
LLVM_LIBRARIES="-I/opt/homebrew/opt/llvm/include -l LLVM"

$CC $C_FLAGS $SRC -o $NAME $LUA_INCLUDE_DIR $LUA_LIBRARIES $LLVM_INCLUDE_DIR $LLVM_LIBRARIES

EXECUTE="./$NAME"

$EXECUTE