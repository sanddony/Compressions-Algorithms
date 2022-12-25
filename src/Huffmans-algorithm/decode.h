#ifndef HUFFMAN_DECODE_H
#define HUFFMAN_DECODE_H


#include "../header.h"
#include "huff.h"


//decode
int Decode(files files);
node* RestoreTree(files files);
int WriteDecodeFile(files files, node* root);
//


#endif // HUFFMAN_DECODE_H