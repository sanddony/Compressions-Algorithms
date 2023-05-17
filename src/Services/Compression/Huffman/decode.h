#ifndef HUFFMAN_DECODE_H
#define HUFFMAN_DECODE_H

#include "huff.h"


//decode
void Decode(files files);
node** RestoreTree(files files, eight_bytes* count_nodes, eight_bytes* count_symb) ;
int WriteDecodeFile(files files, node** node_list, eight_bytes count_nodes, eight_bytes* count_symb);
int pred_ascending_by_code_len(node* first, node* second);
node *GetSymbByCode(code buff, node **node_list, eight_bytes count_nodes);
void DecodeBuff(files files, node **node_list, eight_bytes count_nodes,
                eight_bytes *count_symb, code* buff);
//

int DefineEncodedPartSize(files files);
code SetFittedBits(code buff, eight_bytes byte_from_file);
code SetNotFittedBits(code fitted_bits, eight_bytes byte_from_file);

#endif // HUFFMAN_DECODE_H
