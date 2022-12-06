#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <getopt.h>


#include "header.h"

#define BYTE_MAX_SIZE 255

int main(int argc, char** argv) {

    files files = {NULL, NULL, 2};
    
    int err_code = ParseParams(argc,argv,&files);
    if (!err_code) {
        if(files.mode==ENCODE) {
          err_code = Encode(files);
        } else {

        }
    }
    return err_code;
    
}

int Encode(files files){
  node** node_list = GetFrequencyOfBytes(files);
  node* root = BuildTree(node_list);
}

node** SortRoots(node** node_list){
  node* tmp;
  bool noSwap;
  // Handle NULL case: node can be NULL
  for (int i = BYTE_MAX_SIZE - 1; i >= 0; i--) {
        noSwap = 1;
        for (int j = 0; j < i; j++)
        {
            if (node_list[j]->weight > node_list[j + 1]->weight)
            {
                tmp = node_list[j];
                node_list[j] = node_list[j + 1];
                node_list[j + 1] = tmp;
                noSwap = 0;
            }
        }
        if (noSwap == 1)
            break;
  }
  return node_list;
}

void F(byte n)
{
    for (int i = 0; i < 8; i++)
    {
        printf("%d", ((128 & n)>0));
        n<<=1;
    }
}

node** GetFrequencyOfBytes(files files) {
  //Define file_size by move the file cursor to the end of the file
  fseek(files._in, 0, SEEK_END);
  int file_size = ftell(files._in);
  fseek(files._in, 0, SEEK_SET);
  //

  // Allocate mem for mass, which contain 255 elements. Exactly that much, because program encode bytes and BYTE_MAX_SIZE=255 (0x11111111)-uc 
  node** nodes_list = malloc(sizeof(node*)*BYTE_MAX_SIZE); 
  for (int i = 0; i < BYTE_MAX_SIZE; i++)
  {
    nodes_list[i] = malloc(sizeof(node));
    nodes_list[i]->symb = i;
    nodes_list[i]->is_root = 1;
  }
  //

  byte tmp[1];
  for (int i = 0; i < file_size; i++)
  {
    fread(tmp, sizeof(byte), 1, files._in);
    for (int k = 0; k < BYTE_MAX_SIZE; k++)
    {
      if(nodes_list[k]->symb==tmp[0]){
        nodes_list[k]->weight++;
      }
    }
  }

  // for (int i = 0; i < BYTE_MAX_SIZE; i++)
  // {
  //   if(nodes_list[i]->weight!=0){
  //     printf("%d : ",nodes_list[i]->weight);
  //     F(nodes_list[i]->symb);
  //     printf("\n");
  //   }
  // }

  return SortRoots(nodes_list);
}

node* BuildTree(node** nodes_list) {
  node empty_node = {NULL, NULL, 0, NULL, NULL, 0};
  // while root > 1 in list (or second element not a empty_node)
  // Take first two elements and build root with them in leafs
  // Switch first element on the new generated on before step, then 
  // switch second element on the empty_node
  
}

int SerializationOfTheTree(files files, node* root) {

}

int WriteEncodeFile(files files, node* root) {

}

FILE *GetFile(char *path) {
  FILE *file;
  if (!strcmp(path, "-")) {
    file = stdin;
  } else {
    file = fopen(path, "rb+");
  }
  return file;
}

int ParseParams(int argc, char** argv, files* files){
  int err_code = 0;
  int flag;
    while ((flag = getopt_long(argc, argv, "edo:i:", long_options, NULL)) !=
         -1) {
    switch (flag) {
      case 'e':
        (*files).mode = ENCODE;
        break;
      case 'd':
        (*files).mode = DECODE;
        break;
      case 'i':
        (*files)._in = GetFile(optarg);
        if(!(*files)._in){
          fprintf(stderr,"No files: %s\n",optarg);
          err_code = 1;
        }
        break;
      case 'o':
        (*files)._out = fopen(optarg,"wb+");
        break;
      default:
        break;
    }
  }
  if(!(*files)._in || !(*files)._out || (*files).mode==2){
      fprintf(stderr,"Usage: haffman [-ed] [-i file_input] [-o file_output]");
      err_code = 1;
  }
  return err_code;
}