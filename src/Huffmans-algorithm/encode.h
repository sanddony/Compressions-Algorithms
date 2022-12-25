#ifndef HUFFMAN_ENCODE_H
#define HUFFMAN_ENCODE_H

#include "../header.h"
#include "huff.h"


//encode
int Encode(files files);
node** SortRoots(node** nodes_list, int list_size);
node** GetFrequencyOfBytes(files files, int* sym_count);
node* BuildTree(node** nodes_list, int sym_count);

void SetCodeForBytes(node* root);
void Traverse(node* in_node);

int SerializationOfTheTree(files files, node* root);
int WriteEncodeFile(files files, node* root);

//

#endif  // HUFFMAN_ENCODE_H