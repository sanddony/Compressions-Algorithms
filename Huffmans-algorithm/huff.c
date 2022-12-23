#include "huff.h"

#define BYTE_MAX_SIZE 255


//TO-DO: Decide where need single pointer, where double. Maybe we need allocate mem for mass and
//then work with this pointers. 
// 

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
  int sym_count;
  node** nodes_list = GetFrequencyOfBytes(files, &sym_count);
  // PrintNodeList(nodes_list,sym_count);
  node* root = BuildTree(nodes_list, sym_count);
}

//TO-DO don't alloc mem
node* CopyNode(node input_node){
  node* res = malloc(sizeof(node));
  (*res).code = input_node.code;
  (*res).is_root = input_node.is_root;
  (*res).left_leaf = input_node.left_leaf;
  (*res).right_leaf = input_node.right_leaf;
  (*res).symb = input_node.symb;
  (*res).weight = input_node.weight;
  return res;
}

node* SortRoots(node* nodes_list, int list_size){
  // Handle NULL case: node can be NULL
  // If symb == NULL, get to the end
  // Sort by Weight
  node tmp;
  bool noSwap;
  for (int i = list_size - 1; i >= 0; i--)
  {
    noSwap = 1;
    for (int j = 0; j < i; j++)
    {
        if (nodes_list[j].weight > nodes_list[j+1].weight)
        {
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

  // Allocate FILE *GetFile(char *path) {
  FILE *file;
  if (!strcmp(path, "-")) {
    file = stdin;
  } else {
    file = fopen(path, "rb+");
  }
  return file;
}mem for mass, which contain 255 elements. Exactly that much, because program encode bytes and BYTE_MAX_SIZE=255 (0x11111111)-uc
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
  

  //
  for (int i = 0; i < BYTE_MAX_SIZE; i++)
  {
    free(nodes_list[i]);
  }
  free(nodes_list);
  //

  return SortRoots(res_list, *sym_count);
}

// first should be less then second, cause it'll go to the left leaf 
node* UniteTwoNodes(node* first, node* second){
  node* res = malloc(sizeof(node));
  res->left_leaf = first;
  first->is_root = 0;

  res->right_leaf = second;
  second->is_root = 0;

  res->weight = first->weight + second->weight;
  res->is_root = 1;

  res->code = 0;
  res->symb = '\\';
  return res;
}

node* BuildTree(node** nodes_list, int sym_count) {
  node empty_node = {NULL, NULL, 0, 0, 0, 0x7FFFFFFF};
  // while root > 1 in list (or second element not a empty_node)
  // Take first two elements and build root with them in leafs
  // Switch first element on the new generated on before step, then 
  // switch second element on the empty_node  
  printf("size %d\n", sizeof(node));

  PrintNodeList(nodes_list,sym_count);
  printf("\n\n\n");
  while(nodes_list[1]->is_root) {
    nodes_list[0] = UniteTwoNodes(nodes_list[0],nodes_list[1]);
    nodes_list[1] = &empty_node;
    SortRoots(nodes_list, sym_count);
    PrintNodeList(nodes_list,sym_count);
    break;
  }

  printf("\n\n\n");
  PrintNode(nodes_list[0]);
  PrintNode(nodes_list[1]);
  PrintNode(nodes_list[2]);
  // PrintNode(nodes_list[3]);
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

  

  // for (int i = 0; i < BYTE_MAX_SIZE; i++)
  // {
  //   if(nodes_list[i]->weight!=0){
  //     printf("%d : ",nodes_list[i]->weight);
  //     F(nodes_list[i]->symb);
  //     printf("\n");
  //   }
  // }


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
    PrintNode(nodes_list[i]);
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
