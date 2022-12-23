#ifndef HUFFMAN_HEADER_H
#define HUFFMAN_HEADER_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <getopt.h>

#define byte unsigned char


static struct option long_options[] = {{"encode", no_argument, NULL, 'e'},
                                       {"decode", no_argument, NULL, 'd'},
                                       {"output", required_argument, NULL, 'o'},
                                       {"input", required_argument, NULL, 'i'},
                                       {NULL, 0, NULL, 0}};

enum Modes{ENCODE,DECODE};

typedef struct files {
   FILE* _in;
   FILE* _out;
   int mode;
}files;

typedef struct node node;
struct node {
    node* left_leaf;
    node* right_leaf;
    int is_root;
    byte symb;
    char code;  
    int weight;
};

int ParseParams(int argc, char** argv, files* files);
FILE *GetFile(char *path);


//Debugging functions
void F(byte n);
void PrintNodeList(node** nodes_list, int sym_count);
void PrintNode(node* input_node);

#endif //HUFFMAN_HEADER_H
