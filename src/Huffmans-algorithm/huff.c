#include "huff.h"
#include "decode.h"
#include "encode.h"
#include <ncurses.h>
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
//\033[31m$ 033[0m/\033[32m



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
      printf("symb_bytes:");
    F((*input_node).symb);
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

void TraverseAndPrintThreeWrapper(node* root, int init_x, int init_y){
  initscr();
  noecho();
  raw();
  start_color();
  init_pair(1,COLOR_BLACK,COLOR_RED);
  init_pair(2,COLOR_WHITE, COLOR_BLUE);
  TraverseAndPrintThree(root, init_x, init_y, 256);
  getch();
  getch();
  getch();

  endwin();
}

void TraverseAndPrintThree(node *in_node, int x, int y, int k) {
  if (in_node) {

    PrintLeaf(in_node, x, y);
    getch();

    k/=2;
    
    if((*in_node).left_leaf){
      PrintLeftBranch(x, y, k);
      TraverseAndPrintThree((*in_node).left_leaf, x-k, y+4, k);

    }
    if((*in_node).right_leaf){
      PrintRightBranch(x, y, k);
      TraverseAndPrintThree((*in_node).right_leaf, x+k, y+4, k);
    }
    
  }
}

void PrintLeaf(node* in_node,int x, int y){
  if((*in_node).symb == '\n'){
      mvprintw(y, x, "\\n", (*in_node).weight);
      mvprintw(y+1, x, "%d", (*in_node).weight);
      mvprintw(y+2, x, "_");
      F_ncurses_32((*in_node).code,x,y+3);
      mvprintw(y+67, x, "_");
      mvprintw(y+68, x, "%d",(*in_node).code_len);
    } else if((*in_node).symb){
      mvprintw(y, x, "%c", (*in_node).symb, (*in_node).weight);
      mvprintw(y+1, x, "%d", (*in_node).weight);
      mvprintw(y+2, x, "_");
      F_ncurses_32((*in_node).code,x,y+3);
      mvprintw(y+67, x, "_");
      mvprintw(y+68, x, "%d",(*in_node).code_len);
    } else {
      mvprintw(y, x, "%d",(*in_node).weight);
    }
  if((*in_node).right_leaf || (*in_node).right_leaf){
    mvprintw(y+1, x, "|");
  }
}

void F_ncurses(byte n, int x, int y) {
  for (int i = 0; i < 8; i++) {
    mvprintw(y+i,x,"%d", ((128 & n) > 0));
    n <<= 1;
  }
}

void F_ncurses_32(eight_bytes n, int x, int y) {
  for (int i = 0; i < 64; i++) {
     mvprintw(y+i,x,"%d",  ((0x8000000000000000 & n) > 0));
    n <<= 1;
  }
}

void PrintLeftBranch(int x, int y, int k){
  //0
  for (int i = x; i > x-k; i--)
  {
    mvprintw(y+2, i, "_");
  }                     
  mvprintw(y+3, x-k-1, "|0|");
  
}

void PrintRightBranch(int x, int y, int k){
  //1
  for (int i = x; i < x+k; i++)
  {
    mvprintw(y+2, i, "_");
  }
  mvprintw(y+3, x+k-1, "|1|");
}

int GetMiddle(int x){
  int res = 0;
  if(x%2==0){
    res = x/2;
  } else {
    res = x/2+1;
  }
  return res;
}

void PrintTree(node* in_node){
  if(in_node){
    PrintNode(in_node);
    PrintTree(in_node->left_leaf);
    PrintTree(in_node->right_leaf);
  }
}