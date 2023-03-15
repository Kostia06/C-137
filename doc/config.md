# Configuration files in 3F

To use config file first you need to install lua on your operating system
### Mac
    brew install lua
### Linux 
    sudo apt-get install lua5.3
### Windows
&emsp;&emsp;download the binary from [Lua Site]( https://www.lua.org/download.html)

Create config.lua file

inside the file create tables
```lua
    --configuration for running code
    config = {
        _c_flags = "",
        -- compile / run /save
        _compiler_mode = "save",
        _output_file = "../test/output.c",
        _input_file = "../test/main.3f",
    }
    --configuration for setting name for keywords
    keywords = {
        _function = "fn",
        _variable = "var",
        _constant = "const",
        _if = "if",
        _else_if = "elif",
        _while = "while",
        _for = "for",
        _return = "return",
        _break = "break",
        _continue = "continue",
        _integer = "int",
        _double = "double",
        _float = "float",
        _void = "void",
        _character = "char"
    }

```
    
Then in the same directory run 

    ./3F

It will automatically search for config.lua file in the directory