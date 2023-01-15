#include "huff.h"
#include "decode.h"
#include "encode.h"

void Huff(files files) {
  if (files.mode == ENCODE) {
    Encode(files);
  } else {
    Decode(files);
  }
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

int PrintNeededNodeTree(node* in_node, byte symb){
  if(in_node){
    if((*in_node).symb == symb && !(*in_node).left_leaf && !(*in_node).right_leaf){
      PrintNode(in_node);
      return 1;
    }
    PrintNeededNodeTree(in_node->left_leaf, symb);
    PrintNeededNodeTree(in_node->right_leaf, symb);
    

  }
}

code ZeroingCode() {
  code res = {0, 0};
  return res;
}

void AddingCode(code* result_code, code* added_code){
    (*result_code).code_len += (*added_code).code_len;
    (*result_code).code |= (*added_code).code;
}

// O(n^2)
node **BubleSort(node **nodes_list, int list_size, ruleCompare rule) {
  node *tmp;
  bool noSwap;
  for (int i = list_size - 1; i >= 0; i--) {
    noSwap = 1;
    for (int j = 0; j < i; j++) {
      if(rule(nodes_list[j], nodes_list[j+1])) {
        tmp = nodes_list[j];
        nodes_list[j] = nodes_list[j + 1];
        nodes_list[j + 1] = tmp;
        noSwap = 0;
      }
    }
    if (noSwap == 1)
      break;
  }

  return nodes_list;
}


// #include <stdio.h>

// void lamuto_sort(int array[], int low, int high, int order);
// int partition(int array[], int low, int high, int order);
// void swap(int *a, int *b);

// void lamuto_sort(int array[], int low, int high, int order) {
//     if (low < high) {
//         int pivot = partition(array, low, high, order);
//         lamuto_sort(array, low, pivot - 1, order);
//         lamuto_sort(array, pivot + 1, high, order);
//     }
// }

// int partition(int array[], int low, int high, int order) {
//     int pivot = array[high];
//     int i = low - 1;
//     for (int j = low; j <= high - 1; j++) {
//         if ((order == 1 && array[j] <= pivot) || (order == -1 && array[j] >= pivot)) {
//             i++;
//             swap(&array[i], &array[j]);
//         }
//     }
//     swap(&array[i + 1], &array[high]);
//     return i + 1;
// }

// void swap(int *a, int *b) {
//     int temp = *a;
//     *a = *b;
//     *b = temp;
// }

// int main() {
//     int array[] = {5, 2, 9, 1, 5, 6};
//     int n = sizeof(array) / sizeof(array[0]);
//     printf("Original array: ");
//     for (int i = 0; i < n; i++) {
//         printf("%d ", array[i]);
//     }
//     int order = -1; // -1 for descending, 1 for ascending
//     lamuto_sort(array, 0, n - 1, order);
//     printf("\nSorted array: ");
//     for (int i = 0; i < n; i++) {
//         printf("%d ", array[i]);
//     }

//     order = 1; // -1 for descending, 1 for ascending
//     lamuto_sort(array, 0, n - 1, order);
//     printf("\nSorted array: ");
//     for (int i = 0; i < n; i++) {
//         printf("%d ", array[i]);
//     }
//     return 0;
// }