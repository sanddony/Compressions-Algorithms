#ifndef DEBUG_H
#define DEBUG_H

#include "huff.h"

void F(byte_c n);
void F_32(size_t n);
void F_32_code(eight_bytes n,int start_dig, int stop_dig);
int PrintNeededNodeTree(node* in_node, byte_c symb);


#endif // DEBUG_H
