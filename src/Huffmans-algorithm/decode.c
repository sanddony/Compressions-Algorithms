#include "decode.h"
 
int Decode(files files) {
    byte count_nodes;
    node** nodes_list = RestoreTree(files, &count_nodes);
    WriteDecodeFile(files, nodes_list, count_nodes);
}


node** RestoreTree(files files, byte* count_nodes) {
    *count_nodes = fgetc(files._in);
    printf("%d",(*count_nodes));
    node** nodes_list = malloc((*count_nodes)*sizeof(node*));
    for (int i = 0; i < (*count_nodes); i++)
    {
        nodes_list[i] = malloc(sizeof(node));
        nodes_list[i]->symb = fgetc(files._in);
        nodes_list[i]->code = fgetc(files._in);
        nodes_list[i]->code_len = fgetc(files._in);
    }
    // PrintNodeList(nodes_list, (*count_nodes));
    return SortNodes(nodes_list, (*count_nodes));
}

int WriteDecodeFile(files files, node** node_list, byte count_nodes) {
    byte roller = fgetc(files._in);
    byte in_byte;
    byte shift = 0;
    byte shift_prev = 0;
    byte flag = 0;
    node* node = NULL;
    while (!feof(files._in))
    {

        if(shift>0 && !flag){
            in_byte = fgetc(files._in);
            printf("\nGet new byte!\n");  
            shift%=8;  
            flag = 1;
        }
        if(shift>8){
            flag = 0;
            shift%=8;
        }
        printf("shift: %d\n", shift);
        printf("roller ");
        F(roller);
        
        byte tmp = in_byte<<shift_prev;
        if(node)tmp >>=(8-(node->code_len))+shift_prev;
        else tmp >>=(8-shift)+shift_prev;
        printf("tmp ");
        F(tmp);
        if(node)roller <<= (node->code_len);
        else roller <<= shift;
        roller|=tmp;
        printf("roller ");
        F(roller);
        node = GetSymbByCode(roller, node_list, count_nodes);
        PrintNode(node);
        fwrite(&node->symb, sizeof(byte), 1, files._out);
        shift_prev = shift;
        shift += node->code_len;
    }
    
}

//TO-DO union with another GetSymbCode
node* GetSymbByCode(byte code, node** node_list, byte count_nodes) {
    node* res = NULL;
    for (int i = 0; i < count_nodes; i++)
    {
        byte tmp = code;
        tmp >>= (8 - node_list[i]->code_len);
        if(tmp == node_list[i]->code) {
            res = node_list[i];
        }
    }
    return res;
}

//TO-DO union with another sort
node **SortNodes(node **nodes_list, int list_size) {
  // Handle NULL case: node can be NULL
  // If symb == NULL, get to the end
  // Sort by Weight
  node *tmp;
  bool noSwap;
  for (int i = list_size - 1; i >= 0; i--) {
    noSwap = 1;
    for (int j = 0; j < i; j++) {
      if (nodes_list[j]->code_len > nodes_list[j + 1]->code_len) {
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