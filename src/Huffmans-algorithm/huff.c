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


void TraverseAndPrintThreeWrapper(node* root, int init_x, int init_y){
  initscr();
  noecho();
  start_color();
  init_pair(1,COLOR_BLACK,COLOR_RED);
  init_pair(2,COLOR_WHITE, COLOR_BLUE);
  int x_cur = 0;
  int y_cur = 0;
  TraverseAndPrintThree(root, init_x, init_y, &x_cur, &y_cur);
  getch();
  endwin();
}

// void TraverseAndPrintThree(node *in_node, int x, int y) {
//   if (in_node) {
//     if((*in_node).symb == '\n'){
//        mvprintw(y, x, "(\\n:%d)", (*in_node).weight);
//     } else if((*in_node).symb){
//       mvprintw(y, x, "(%c:%d)", (*in_node).symb, (*in_node).weight);
//     } else {
//       mvprintw(y, x, "(%d)",(*in_node).weight);
//     }
//     if((*in_node).left_leaf){
//       for (int i = x-1; i > x - (x*0.2); i--)
//       {
//          mvprintw(y+1,i,"_");
//       }
//       mvprintw(y+2,(x - (x*0.2))+2,"/");
//       getch();
//       TraverseAndPrintThree((*in_node).left_leaf, x - (x*0.2),y+3);
//       getch();
//     }
//     if((*in_node).right_leaf){
//       for (int i = x+7; i < x + (x*0.4); i++)
//       {
//         mvprintw(y+1,i,"_");
//       }
//       mvprintw(y+2,(x + (x*0.4))-2,"\\");
//       getch();
//       TraverseAndPrintThree((*in_node).right_leaf,x + (x*0.4),y+3);
//       getch();
//     }
//     getch(); 
//   }
// }

// void TraverseAndPrintThree(node *in_node, int x, int y, int k) {
//   if (in_node) {
//     if((*in_node).symb == '\n'){
//        mvprintw(y, x, "(\\n:%d)", (*in_node).weight);
//     } else if((*in_node).symb){
//       mvprintw(y, x, "(%c:%d)", (*in_node).symb, (*in_node).weight);
//     } else {
//       mvprintw(y, x, "(%d)",(*in_node).weight);
//     }
//     if((*in_node).left_leaf){
//       attron(COLOR_PAIR(1));
//       getch();
//       if(!(*in_node).left_leaf->left_leaf && !(*in_node).left_leaf->right_leaf){
//         mvprintw(y+1,x,"|");
//         TraverseAndPrintThree((*in_node).left_leaf, x, y+2, k);
//       } else{
//         for (int i = 1; i < 15; i++)
//         {
//           mvprintw(y+i,x,"|");
//         }
//         TraverseAndPrintThree((*in_node).left_leaf, x, (y + 11) + k, k+2);
//       }
//       attroff(COLOR_PAIR(2));
//       getch();
//     }
//     if((*in_node).right_leaf){
//       attron(COLOR_PAIR(2));
//       getch();
//       if(!(*in_node).right_leaf->left_leaf && !(*in_node).right_leaf->right_leaf) {
//         mvprintw(y,x+5,"-");
//         TraverseAndPrintThree((*in_node).right_leaf, x+6, y, k+2);
//       } else{
//         for (int i = 5; i < 15; i++)
//         {
//           mvprintw(y,x+i,"-");
//         }
//         TraverseAndPrintThree((*in_node).right_leaf, (x + 11) + k, y, k+2);
//       }
//       attroff(COLOR_PAIR(2));
//       getch();
//     }
//     getch();
//   }
// }


void TraverseAndPrintThree(node *in_node, int x, int y, int* x_cur, int* y_cur) {
  if (in_node) {

    if((*in_node).symb == '\n'){
       mvprintw(y, x, "(\\n:%d)", (*in_node).weight);
    } else if((*in_node).symb){
      mvprintw(y, x, "(%c:%d)", (*in_node).symb, (*in_node).weight);
    } else {
      mvprintw(y, x, "(%d)",(*in_node).weight);
    }

    getsyx(*y_cur, *x_cur);

    if((*in_node).left_leaf){
      getch();
      TraverseAndPrintThree((*in_node).left_leaf, x, *y_cur+10, x_cur, y_cur);
      getch();
      getch();
      getch();
    }
    if((*in_node).right_leaf){
      getch();
      TraverseAndPrintThree((*in_node).right_leaf, *x_cur+10, y, x_cur, y_cur);
      getch();
    }
    getch();
    
  }
}