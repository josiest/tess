# Compiler
CC = g++

# Compiler flags
CFLAGS = -g -Wall

# include all headers from the include folder
INC = -I include

tests:
	$(CC) $(CFLAGS) test/test.cpp src/hex.cpp $(INC) -o bin/test
