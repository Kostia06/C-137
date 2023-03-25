# Configuration files in C-137

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
        -- compile / run 
        _compiler_mode = "compile",
        _input_file = "../test/main.3f",
    }

```
    
Then in the same directory run 

    ./C-137

It will automatically search for config.lua file in the directory