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

typedef struct node node;
struct node {
    node* left_leaf;
    node* right_leaf;
    int is_root;
    byte symb;
    // union symbol{
    //     byte symb;
    //     void* nothing;
    // };
    char code;  
    int weight;
};




//Debugging functions
void F(byte n);
void PrintNodeList(node** nodes_list, int sym_count);
void PrintNode(node* input_node);

int Huff(files files);

#endif //HUFFMAN_HEADER_H
