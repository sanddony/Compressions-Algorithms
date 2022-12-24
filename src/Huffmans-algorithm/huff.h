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
    char code;  
    int weight;
};

int Huff(files files);

//encode
int Encode(files files);
node** SortRoots(node** nodes_list, int list_size);
node** GetFrequencyOfBytes(files files, int* sym_count);
node* BuildTree(node** nodes_list, int sym_count);
int SerializationOfTheTree(files files, node* root);
int WriteEncodeFile(files files, node* root);
node* CopyNode(node input_node);
//

//decode
int Decode(files files);
node* RestoreTree(files files);
int WriteDecodeFile(files files, node* root);
//

//Debugging functions
void F(byte n);
void PrintNodeList(node** nodes_list, int sym_count);
void PrintNode(node* input_node);

#endif //HUFFMAN_HEADER_H
