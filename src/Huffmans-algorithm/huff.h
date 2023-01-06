#ifndef HUFFMAN_HEADER_H
#define HUFFMAN_HEADER_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h> 
#include <math.h>
#include <string.h>
#include "../header.h"


#define byte unsigned char
#define BYTE_MAX_SIZE 255
#define MIDDLE 64
#define tc_move_cursor(X, Y) printf("\033[%d;%dH", Y, X);


typedef struct node node;
struct node {
    node* left_leaf;
    node* right_leaf;
    int is_root;
    byte symb;
    byte code;
    int code_len;  
    size_t weight;
};

typedef struct code{
    byte code;
    int code_len;
}code;


//Debugging functions
void F(byte n);
void F_32(size_t n);

void PrintNodeList(node** nodes_list, int sym_count);
void PrintNode(node* input_node);
void TraverseAndPrintThree(node *in_node, int x, int y);

int Huff(files files);

#endif //HUFFMAN_HEADER_H
