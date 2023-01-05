#ifndef HUFFMAN_DECODE_H
#define HUFFMAN_DECODE_H


#include "../header.h"
#include "huff.h"




//decode
int Decode(files files);
node** RestoreTree(files files, byte* count_nodes, size_t* count_bytes) ;
int WriteDecodeFile(files files, node** node_list, byte count_nodes, size_t* count_bytes);
node **SortNodes(node **nodes_list, int list_size);
node *GetSymbByCode(code buff, node **node_list, byte count_nodes);
//

void DebugWriteDecodeFile(byte byte_from_file,code not_fitted_bits, 
                        code fitted_bits, code buff, node* symb_code);


#endif // HUFFMAN_DECODE_H