#ifndef HUFFMAN_ENCODE_H
#define HUFFMAN_ENCODE_H

#include "../header.h"
#include "huff.h"

#define BUFFSIZE 64


//encode
void Encode(files files);
int pred_ascending_by_weight(node* first, node* second);
node** GetFrequencyOfBytes(files files, eight_bytes* sym_count, node*** full_nodes_list);
node* BuildTree(node** nodes_list, eight_bytes sym_count);

void SetCodeForSymb(node *in_node, eight_bytes code, char len, byte add_code);

int SerializationOfTheTree(files files, node *root, eight_bytes sym_count); 
node *BuildTree(node **nodes_list, eight_bytes sym_count);
code GetSymbCode(node **node_list, byte *symb);
void WriteNodeInFile(node* in_node, FILE* files_out);
int WriteEncodeFile(files files, node* root, node** full_nodes_list);
void ReadAndParseNewByte(code* code_from_file, node **full_nodes_list, byte byte_from_file, code* buff,
                         code* fitted_bits, code* not_fitted_bits);

//
void AllocateMemForMassOn256Elem(node ***full_nodes_list);
void CountingRepetitionsOfBytes(int file_size, files files, node ***full_nodes_list);
node **GetMassWithMeaningBytes(node ***full_nodes_list, eight_bytes *sym_count);
int DefineFileSize(files files);


#endif  // HUFFMAN_ENCODE_H