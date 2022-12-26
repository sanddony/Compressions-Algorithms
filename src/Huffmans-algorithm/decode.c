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
    byte shift = 0;
    byte roller = 0;
    byte roller_len = 8;
    byte in_byte;
    while (!feof(files._in))
    {
        if(roller_len>=8){
            in_byte = fgetc(files._in);
            printf("in_byte ");
            F(in_byte);
            roller_len = 0;
            shift = 8;
        }
        roller <<= shift;
        printf("roller ");
        F(roller);
        roller |= (in_byte>>(8-shift));
        printf("roller ");
        F(roller);

        node* node = GetSymbByCode(roller, node_list, count_nodes);
        PrintNode(node);
        fwrite(&node->symb, sizeof(byte), 1, files._out);
        shift = node->code_len;
        roller_len+=node->code_len;
    }
    
}

//TO-DO union with another GetSymbCode
node* GetSymbByCode(byte code, node** node_list, byte count_nodes) {
    node* res = NULL;
    for (int i = 0; i < count_nodes; i++)
    {
        byte tmp = code;
        tmp >>= (8 - node_list[i]->code_len);
        // PrintNode(node_list[i]);
        // printf("tmp=");
        // F(tmp);
        // F((tmp & node_list[i]->code));
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