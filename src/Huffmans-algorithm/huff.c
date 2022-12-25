#include "huff.h"
#include "decode.h"
#include "encode.h"
//TO-DO: Decide where need single pointer, where double. Maybe we need allocate mem for mass and
//then work with this pointers. 


int Huff(files files){
  int err_code = 0;
  if(files.mode==ENCODE) {
    err_code = Encode(files);
  } else {
    err_code = Decode(files);
  }
}


void F(byte n) {
    for (int i = 0; i < 8; i++)
    {
        printf("%d", ((128 & n)>0));
        n<<=1;
    }
    printf("\n");
}

void PrintNodeList(node** nodes_list, int sym_count) {
  for (int i = 0; i < sym_count; i++)
  {
    if(nodes_list[i])PrintNode(nodes_list[i]);
    else printf("NULL at %d position\n",i);
  }
}

void PrintNode(node* input_node) {
    printf("\n=======================\n");
    F((*input_node).symb);
    printf("symb: (%c) | ", (*input_node).symb);
    printf("code: (%c) | ", (*input_node).code);
    printf("is_root: (%d) | ", (*input_node).is_root);
    printf("weight: %d | ", (*input_node).weight);
    printf("left_leaf address: %p | ", (*input_node).left_leaf);
    printf("right_leaf address: %p | ", (*input_node).right_leaf);
    printf("self address: %p | ",  input_node);
    printf("\n=======================\n");
}
