## Set default configuration
CC = gcc
FLAGS = -Wall -Werror -Wextra -g
OS:=$(shell uname -s)
CCOV = -fprofile-arcs -ftest-coverage
LIB_FILE = libs21_string.a
Huffmans_dir = ./Huffmans-algorithm

# ifeq ($(OS), Darwin)
# else
# 	CHECK_LIB += -lrt -lpthread -lm -lsubunit
# endif

## Make array of all object files for library


all: huff.o
	$(CC) huff.o main.c -o compress
	rm -rf *.o

huff.o:
	$(CC) -c $(Huffmans_dir)/decode.c $(Huffmans_dir)/encode.c $(Huffmans_dir)/huff.c 

clean:
	rm -f *.o
	rm -f $(Huffmans_dir)/compress

clean_o:
	rm -rf *.o

rebuild: clean all
