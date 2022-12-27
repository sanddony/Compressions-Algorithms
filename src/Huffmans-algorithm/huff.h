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
    byte code;
    byte code_len;  
    int weight;
};




//Debugging functions
void F(byte n);
void PrintNodeList(node** nodes_list, int sym_count);
void PrintNode(node* input_node);
void TraverseAndPrintThree(node* in_node);

int Huff(files files);

#endif //HUFFMAN_HEADER_H

/*int WriteDecodeFile(files files, node** node_list, byte count_nodes) {
    byte shift = 0;
    byte roller = 0;
    byte roller_len = 8;
    byte in_byte;
    while (!feof(files._in))
    {
        if(roller_len<=8 && roller_len>0){
            in_byte = fgetc(files._in);
            printf("in_byte ");
            F(in_byte);
            roller_len = 0;
            shift = 1;
        }
        printf("roller ");
        F(roller);
        roller <<= shift;
        printf("roller ");
        F(roller);
        roller |= (in_byte>>(8-shift));
        printf("roller ");
        F(roller);

        node* node = GetSymbByCode(roller, node_list, count_nodes);
        PrintNode(node);
        fwrite(&node->symb, sizeof(byte), 1, files._out);
        shift = node->code_len;
        roller_len+=node->code_len;
    }
    
}*/