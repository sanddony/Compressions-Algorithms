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
#define PRINT_MIDDLE 256

typedef struct node node;
struct node {
    node* left_leaf;
    node* right_leaf;
    int is_root;
    byte symb;
    int code;
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
void TraverseAndPrintThree(node *in_node, int x, int y, int k);
void TraverseAndPrintThreeWrapper(node* root, int init_x, int init_y);
int GetMiddle(int x);
void PrintLeaf(node* in_node,int x, int y);
void PrintLeftBranch(int x, int y, int k);
void PrintRightBranch(int x, int y, int k);
void F_ncurses(byte n,int x, int y);
int Huff(files files);


#endif //HUFFMAN_HEADER_H
