# Source Files in 3F

There are several directories can be found in the source directory. A primary C code and include header may be found in each folder. Apart than utils/include.h, which has a lot of valuable information, most headers are worthless to readers.

Read the code in this sequence as you go through it.

1. [main/main.c](../src/main/main.c) <-- starting point of the program
2. [config/include.h](../src/config/include.h) <-- configuration checks for Lua
3. [config/main.c](../src/compile/main.c) <-- configuration functions
4. [utils/include.h](../src/utils/include.h) <-- important methods
5. [lexer/main.c](../src/lexer/main.c) <-- lexer that gets tokens
6. [parser/node/main.c](../src/parser/node/main.c) <-- translates tokens into nodes
7. [parser/block/main.c](../src/parser/block/main.c) <-- builds node blocks out of nodes with a parser action
8. [compile/main.c](../src/compile/main.c) <-- translates blocks into C code and checks for errors

