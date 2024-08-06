COMPILER = clang++
FLAGS =  -std=c++17 -O0 
SRC = src
TARGET = main

ARGS = test/main.c137

FILES = $(wildcard $(SRC)/*.cpp)
#OBJECTS = $(FILES:.cpp=.o)

all: build run clean

$(TARGET):
	@echo "Compiling..."
	@$(COMPILER) $(FLAGS) $(FILES) -o $@ $^

build: $(TARGET)

run:
	@echo "Running..."
	@./$(TARGET) $(ARGS)

clean:
	@echo "Cleaning..."
	@rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean	
