#ifndef HUFFMAN_HEADER_H
#define HUFFMAN_HEADER_H

#include <stdbool.h>
#include <unistd.h> 
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "../Compress.h"



#define byte_c unsigned char
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
    byte_c symb;
    eight_bytes code;
    char code_len;
    eight_bytes weight;
};



void PrintNodeList(node** nodes_list, int sym_count);
void PrintNode(node* input_node);
void Huff(files files);


// utils for decode and encode
code ZeroingCode();
void AddingCode(code* result_code, code* added_code);

typedef int (*ruleCompare)(node*, node*);
node **BubleSort(node **nodes_list, int list_size, ruleCompare rule);


#define ERRLIST_SIZE 10

static const char *const errlist[ERRLIST_SIZE] = {
        [0] = "No such file or directory:",
        [1] = "No available algoritms with this name:",
        [2] = "GetSymbCode Error! Get byte, which haven't a code!",
        [3] = "Empty file!",
        [4] = "Error! Not all code was decoded!",
        [5] = "Error! Can't decode file!",
        [6] = "Error! Buff code len not equal 64! |buff.code_len =",
};

void ErrorHandler(int errnum, char* prm, ...);
FILE *GetFile(char *path);




#endif //HUFFMAN_HEADER_H
