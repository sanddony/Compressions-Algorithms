#ifndef HUFFMAN_DECODE_H
#define HUFFMAN_DECODE_H


#include "../header.h"
#include "huff.h"


typedef struct code{
    byte code;
    byte code_len;
}code;

//decode
int Decode(files files);
node** RestoreTree(files files, byte* count_nodes) ;
int WriteDecodeFile(files files, node** node_list, byte count_nodes);
node **SortNodes(node **nodes_list, int list_size);
node* GetSymbByCode(byte code, node** node_list, byte count_nodes);
//


#endif // HUFFMAN_DECODE_H