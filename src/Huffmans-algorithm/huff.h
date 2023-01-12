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
#define eight_bytes unsigned long long

#define BYTE_MAX_SIZE 256
#define BUFFSIZE 64

#define PRINT_MIDDLE 256


typedef struct code_t{
    eight_bytes code;
    char code_len;
}code;

typedef struct node_t node;
struct node_t {
    node* left_leaf;
    node* right_leaf;
    int is_root;
    byte symb;
    eight_bytes code;
    char code_len;
    eight_bytes weight;
};


//Debugging functions
void F(byte n);
void F_32(size_t n);
void F_32_code(eight_bytes n,int start_dig, int stop_dig);

void PrintNodeList(node** nodes_list, int sym_count);
void PrintNode(node* input_node);
void TraverseAndPrintThree(node *in_node, int x, int y, int k);
void TraverseAndPrintThreeWrapper(node* root, int init_x, int init_y);
int GetMiddle(int x);
void PrintTree(node* in_node);
void PrintLeaf(node* in_node,int x, int y);
void PrintLeftBranch(int x, int y, int k);
void PrintRightBranch(int x, int y, int k);
void F_ncurses(byte n,int x, int y);
void F_ncurses_32(eight_bytes n, int x, int y);
int Huff(files files);


#endif //HUFFMAN_HEADER_H
