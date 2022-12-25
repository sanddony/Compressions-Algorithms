#include "encode.h"

//TO-DO return err_code
int Encode(files files){
  int sym_count;
  node** nodes_list = GetFrequencyOfBytes(files, &sym_count);
  node* root = BuildTree(nodes_list, sym_count);
  PrintNode(root);
  Traverse(root);
  // SetCodeForBytes(root);
}

node** SortRoots(node** nodes_list, int list_size){
  // Handle NULL case: node can be NULL
  // If symb == NULL, get to the end
  // Sort by Weight
  node* tmp;
  bool noSwap;
  for (int i = list_size - 1; i >= 0; i--)
  {
    noSwap = 1;
    for (int j = 0; j < i; j++)
    {
        if (nodes_list[j]->weight > nodes_list[j+1]->weight) {
            tmp = nodes_list[j];
            nodes_list[j] = nodes_list[j+1];
            nodes_list[j+1] = tmp;
            noSwap = 0;
        }
    }
    if (noSwap == 1)
        break;
  }
  
  return nodes_list;
}

node** GetFrequencyOfBytes(files files, int* sym_count) {
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
      if(nodes_list[k]->symb==tmp[0]) {
        nodes_list[k]->weight++;
      }
    }
  }

  (*sym_count) = 0;
  int res_list_indx[BYTE_MAX_SIZE];
  for (int i = 0; i < BYTE_MAX_SIZE; i++) {
    if(nodes_list[i]->weight!=0) {
      res_list_indx[(*sym_count)++] = i;
      }
  }
  
  node** res_list = malloc(sizeof(node*)*(*sym_count)); 
  for (int i = 0; i < (*sym_count); i++) {
    res_list[i] = malloc(sizeof(node));
    res_list[i]->is_root = 1;
    res_list[i]->symb = nodes_list[res_list_indx[i]]->symb;
    res_list[i]->weight = nodes_list[res_list_indx[i]]->weight;
  }

  // free mass for 256 bits combination
  for (int i = 0; i < BYTE_MAX_SIZE; i++)
  {
    free(nodes_list[i]);
  }
  free(nodes_list);
  //

  return SortRoots(res_list, *sym_count);
}

// left should be less than right
node* UniteTwoNodes(node* left, node* right){
  node* res = malloc(sizeof(node));
  res->left_leaf = left;
  left->is_root = 0;

  res->right_leaf = right;
  right->is_root = 0;

  res->weight = left->weight + right->weight;
  res->is_root = 1;

  res->code = 0;
  res->symb = 0;
  return res;
}

node* BuildTree(node** nodes_list, int sym_count) {
  node empty_node = {NULL, NULL, 0, 0, 0, 0x7FFFFFFF};
  // while root > 1 in list (or second element not a empty_node)
  // Take first two elements and build root with them in leafs
  // Switch first element on the new generated on before step, then 
  // switch second element on the empty_node  

  // printf("originaly\n");
  // PrintNodeList(nodes_list,sym_count);
  // printf("\n\n\n");
  while(nodes_list[1]->is_root) {
    // printf("Unite and insert\n");
    nodes_list[0] = UniteTwoNodes(nodes_list[0], nodes_list[1]);
    // free(nodes_list[1]); CHECK WHY IT RAISE SEGA
    nodes_list[1] = &empty_node;

    // PrintNodeList(nodes_list,sym_count);
    // printf("\n\n\n");

    // printf("Sorted\n");
    SortRoots(nodes_list, sym_count);

    // PrintNodeList(nodes_list,sym_count);
    // printf("\n\n\n");
  }

  node* root = nodes_list[0];
  free(nodes_list);
  
  return root;
}

void SetCodeForBytes(node* root){
  int code = 00000000;

}

void Traverse(node* in_node){
  if(in_node){
      if((*in_node).symb){
        F((*in_node).symb);
        printf(" (%c) %d\n",(*in_node).symb, (*in_node).weight);
      }
      Traverse((*in_node).left_leaf);
      Traverse((*in_node).right_leaf);
  }
  

}

int SerializationOfTheTree(files files, node* root) {

}

int WriteEncodeFile(files files, node* root) {

}

