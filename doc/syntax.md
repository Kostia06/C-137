# Syntax of 3F

With 3F, syntax is very flexible. For instance, if you're a Python coder, you may use the tab key to space out your code.

```C
    fn int main()
    ....printf("Hello world") // dots represent space
```
The spacing simply has to be larger than a previous code; it need not be a tab.

```C
    fn int main()
    .printf("Hello world")
```

Or if you use Javascript, C++ or C where code is spaced out using brackets or semicolons you can do this

```C
    fn int main(){
        printf("Hello world"); printf("Goodbye world")
    }
```

In this example, there are 3 different ways to space out code

```C
    fn int main(){ 
        var age 17
        // space
        if(age == 17) 
            printf("Hello world 17 year old")
        // single line
        elif(age == 18);printf("Hello world 18 year old") 
        // brackets
        elif(1){ 
            printf("Hello world")
        } 
    }
```

If you don't like the keywords in the 3F you can change their names in 2 ways.
1. In the configuration you can change the keywords names [link to configuration documentation](config.md)

2. Use the macro called `#replace` and follow with a new name and a keyword(s) you want to replace

The fastest way to do it both for compiler and as a user is to do the 1st step.
