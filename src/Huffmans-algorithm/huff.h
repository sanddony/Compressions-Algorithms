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
int PrintNeededNodeTree(node* in_node, byte symb);

void PrintNodeList(node** nodes_list, int sym_count);
void PrintNode(node* input_node);
void Huff(files files);


// utils for decode and encode
code ZeroingCode();
void AddingCode(code* result_code, code* added_code);

typedef int (*ruleCompare)(node*, node*);
node **BubleSort(node **nodes_list, int list_size, ruleCompare rule);


#endif //HUFFMAN_HEADER_H
