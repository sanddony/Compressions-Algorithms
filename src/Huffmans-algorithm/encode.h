#ifndef HUFFMAN_ENCODE_H
#define HUFFMAN_ENCODE_H

#include "../header.h"
#include "huff.h"


//encode
int Encode(files files);
node** SortRoots(node** nodes_list, int list_size);
node** GetFrequencyOfBytes(files files, int* sym_count);
node* BuildTree(node** nodes_list, int sym_count);

void SetCodeForSymb(node* in_node, byte code, byte len, byte add_code);
void CountSymbInThree(node* in_node, byte* size);

int SerializationOfTheTree(files files, node* root);
void CountSymbInThree(node* in_node, byte *size);
void GetSymbCode(node* in_node, byte* symb, node** out_node);
void WriteNodeInFile(node* in_node, FILE* files_out);
int WriteEncodeFile(files files, node* root);

//

#endif  // HUFFMAN_ENCODE_H