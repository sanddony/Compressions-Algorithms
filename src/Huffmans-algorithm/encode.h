#ifndef HUFFMAN_ENCODE_H
#define HUFFMAN_ENCODE_H

#include "../header.h"
#include "huff.h"

#define BUFFSIZE 64


//encode
int Encode(files files);
node** SortRoots(node** nodes_list, int list_size);
node** GetFrequencyOfBytes(files files, eight_bytes* sym_count);
node* BuildTree(node** nodes_list, eight_bytes sym_count);

void SetCodeForSymb(node *in_node, eight_bytes code, char len, byte add_code);

int SerializationOfTheTree(files files, node *root, eight_bytes sym_count); 
node *BuildTree(node **nodes_list, eight_bytes sym_count);
void GetSymbCode(node *in_node, byte *symb, code *desired) ;
void WriteNodeInFile(node* in_node, FILE* files_out);
int WriteEncodeFile(files files, node* root);

//

#endif  // HUFFMAN_ENCODE_H