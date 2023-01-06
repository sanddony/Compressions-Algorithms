#include "huff.h"
#include "decode.h"
#include "encode.h"
// TO-DO: Decide where need single pointer, where double. Maybe we need allocate
// mem for mass and then work with this pointers.

int Huff(files files) {
  int err_code = 0;
  if (files.mode == ENCODE) {
    err_code = Encode(files);
  } else {
    err_code = Decode(files);
  }
  return err_code;
}

void F(byte n) {
  for (int i = 0; i < 8; i++) {
    printf("%d", ((128 & n) > 0));
    n <<= 1;
  }
  printf("\n");
}


void F_32(size_t n)  {
  for (int i = 0; i < 64; i++) {
    printf("%d", ((0x8000000000000000 & n) > 0));
    n <<= 1;
  }
  printf("\n");
}

void PrintNodeList(node **nodes_list, int sym_count) {
  for (int i = 0; i < sym_count; i++) {
    if (nodes_list[i])
      PrintNode(nodes_list[i]);
    else
      printf("NULL at %d position\n", i);
  }
}

void PrintNode(node *input_node) {
  if (input_node) {
    printf("\n=======================\n");
      printf("symb_bytes:");
    F((*input_node).symb);
    printf("symb: (%c) \n", (*input_node).symb);
    printf("code:");
    F((*input_node).code);
    for (int i = 0; i < 5 + (8 - (*input_node).code_len); i++)
      printf(" ");
    for (int i = 0; i < (*input_node).code_len; i++)
      printf("^");
    printf("\n");
    printf("is_root: (%d) | ", (*input_node).is_root);
    printf("weight: %d | ", (*input_node).weight);
    printf("left_leaf address: %p | ", (*input_node).left_leaf);
    printf("right_leaf address: %p | ", (*input_node).right_leaf);
    printf("self address: %p | ", input_node);
    printf("\n=======================\n");
  } else
    printf("NULL pointer\n ");
}

void TraverseAndPrintThree(node *in_node) {
  if (in_node) {
    if ((*in_node).symb) {
      printf("symb: (%c) weight: %d\n", (*in_node).symb, (*in_node).weight);
      printf("symb:");
      F((*in_node).symb);
      printf("code:");
      F((*in_node).code);
      for (int i = 0; i < 5 + (8 - (*in_node).code_len); i++)
        printf(" ");
      for (int i = 0; i < (*in_node).code_len; i++)
        printf("^");
      printf("\n");
      printf("len of code: %d\n", (*in_node).code_len);
    }
    TraverseAndPrintThree((*in_node).left_leaf);
    TraverseAndPrintThree((*in_node).right_leaf);
  }
}