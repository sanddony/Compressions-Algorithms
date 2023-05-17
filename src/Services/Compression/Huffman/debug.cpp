#include "debug.h"

void F(byte_c n) {
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

// Numbeing starts from left bit
void F_32_code(eight_bytes n, int start_dig, int stop_dig)  {
    // if(stop_dig < start_dig){
    //   fprintf(stderr,"F_32_code_wrapper: stop_dig < start_dig(%d < %d)\n", stop_dig, start_dig);
    //   return;
    // }
    for (int i = 0; i < 64; i++) {
        if(i >= start_dig && i < stop_dig) printf("\033[1;31m%d\033[0m", ((0x8000000000000000 & n) > 0));
        else printf("%d", ((0x8000000000000000 & n) > 0));
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
        printf("num: %d\n",(*input_node).symb);
        printf("symb: (%c) \n", (*input_node).symb);
        printf("symb:");
        F((*input_node).symb);
        printf("code:");
        F_32_code((*input_node).code,BUFFSIZE - (*input_node).code_len ,BUFFSIZE);
        printf("code_len: (%d) | ", (*input_node).code_len);
        printf("is_root: (%d) | ", (*input_node).is_root);
        printf("weight: %lld | ", (*input_node).weight);
        printf("left_leaf address: %p | ", (*input_node).left_leaf);
        printf("right_leaf address: %p | ", (*input_node).right_leaf);
        printf("self address: %p | ", input_node);
        printf("\n=======================\n");
    } else
        printf("NULL pointer\n ");
}

void PrintTree(node* in_node){
    if(in_node){
        PrintNode(in_node);
        PrintTree(in_node->left_leaf);
        PrintTree(in_node->right_leaf);
    }
}

int PrintNeededNodeTree(node* in_node, byte_c symb){
    if(in_node){
        if((*in_node).symb == symb && !(*in_node).left_leaf && !(*in_node).right_leaf){
            PrintNode(in_node);
            return 1;
        }
        PrintNeededNodeTree(in_node->left_leaf, symb);
        PrintNeededNodeTree(in_node->right_leaf, symb);


    }
}
