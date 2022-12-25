## Set default configuration
CC = gcc
FLAGS = -Wall -Werror -Wextra -g
OS:=$(shell uname -s)
CCOV = -fprofile-arcs -ftest-coverage
LIB_FILE = libs21_string.a
HUFFMANS_DIR = ./Huffmans-algorithm
BUILD_DIR = ../build/

# ifeq ($(OS), Darwin)
# else
# 	CHECK_LIB += -lrt -lpthread -lm -lsubunit
# endif

## Make array of all object files for library


all: huff.o
	$(CC) huff.o decode.o encode.o main.c -o $(BUILD_DIR)/compress
	rm -rf *.o

huff.o:
	$(CC) -c $(HUFFMANS_DIR)/decode.c $(HUFFMANS_DIR)/encode.c $(HUFFMANS_DIR)/huff.c 

clean:
	rm -f *.o
	rm -f $(BUILD_DIR)/compress

clean_o:
	rm -rf *.o

rebuild: clean all
