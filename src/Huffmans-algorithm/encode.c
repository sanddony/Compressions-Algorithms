#include "encode.h"

// TO-DO return err_code
int Encode(files files) {
  int sym_count;
  node **nodes_list = GetFrequencyOfBytes(files, &sym_count);
  node *root = BuildTree(nodes_list, sym_count);
  // if(files.visualization) TraverseAndPrintThreeWrapper(root, PRINT_MIDDLE, 0);
  SetCodeForSymb(root, 0, -1, 0);
  if(files.visualization) PrintTree(root);
  // if(files.visualization) TraverseAndPrintThreeWrapper(root, PRINT_MIDDLE, 0);
  SerializationOfTheTree(files, root);
  WriteEncodeFile(files, root);
}

node **SortRoots(node **nodes_list, int list_size) {
  // Handle NULL case: node can be NULL
  // If symb == NULL, get to the end
  // Sort by Weight
  node *tmp;
  bool noSwap;
  for (int i = list_size - 1; i >= 0; i--) {
    noSwap = 1;
    for (int j = 0; j < i; j++) {
      if (nodes_list[j]->weight > nodes_list[j + 1]->weight) {
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

node **GetFrequencyOfBytes(files files, int *sym_count) {

  // Define file_size by move the file cursor to the end of the file
  fseek(files._in, 0, SEEK_END);
  int file_size = ftell(files._in);
  fseek(files._in, 0, SEEK_SET);
  //

  if(file_size == 0){
    fprintf(stderr, "Empty file!\n");
    exit(0);
  }

  // Allocate mem for mass, which contain 255 elements. Exactly that much,
  // because program encode bytes and BYTE_MAX_SIZE=255 (0x11111111)-uc
  node **nodes_list = malloc(sizeof(node *) * BYTE_MAX_SIZE);
  for (int i = 0; i < BYTE_MAX_SIZE; i++) {
    nodes_list[i] = malloc(sizeof(node));
    nodes_list[i]->symb = i;
    nodes_list[i]->is_root = 1;
  }
  //

  byte tmp[1];
  for (int i = 0; i < file_size; i++) {
    fread(tmp, sizeof(byte), 1, files._in);
    for (int k = 0; k < BYTE_MAX_SIZE; k++) {
      if (nodes_list[k]->symb == tmp[0]) {
        nodes_list[k]->weight++;
      }
    }
  }

  (*sym_count) = 0;
  int res_list_indx[BYTE_MAX_SIZE];
  for (int i = 0; i < BYTE_MAX_SIZE; i++) {
    if (nodes_list[i]->weight != 0) {
      res_list_indx[(*sym_count)++] = i;
    }
  }

  node **res_list = malloc(sizeof(node *) * (*sym_count));
  for (int i = 0; i < (*sym_count); i++) {
    res_list[i] = malloc(sizeof(node));
    res_list[i]->is_root = 1;
    res_list[i]->symb = nodes_list[res_list_indx[i]]->symb;
    res_list[i]->weight = nodes_list[res_list_indx[i]]->weight;
  }

  // free mass for 256 bits combination
  for (int i = 0; i < BYTE_MAX_SIZE; i++) {
    free(nodes_list[i]);
  }
  free(nodes_list);
  //
  return SortRoots(res_list, *sym_count);
}

// left should be less or equal right
node *UniteTwoNodes(node *left, node *right) {  
  node *res = malloc(sizeof(node));
  res->left_leaf = left;
  left->is_root = 0;

  res->right_leaf = right;
  right->is_root = 0;
  // printf("left->weight %d\n",left->weight);
  // printf("right->weight %d ",right->weight);
  // F_32(right->weight);
  res->weight = left->weight + right->weight;
  // printf("res->weight %d\n",res->weight);
  res->is_root = 1;

  res->code = 0;
  res->symb = 0;
  return res;
}

node *BuildTree(node **nodes_list, int sym_count) {
  node empty_node = {NULL, NULL, 0, 0, 0, 0, 0xFFFFFFFFFFFFFFFF};
  // while root > 1 in list (or second element not a empty_node)
  // Take first two elements and build root with them in leafs
  // Switch first element on the new generated on before step, then
  // switch second element on the empty_node

  while (nodes_list[1]->is_root) {
    nodes_list[0] = UniteTwoNodes(nodes_list[0], nodes_list[1]);
    // free(nodes_list[1]); CHECK WHY IT RAISE SEGA
    nodes_list[1] = &empty_node;
    SortRoots(nodes_list, sym_count);
  }

  node *root = nodes_list[0];
  free(nodes_list);

  return root;
}

// node* GetMin(node* first, node* second){
//   return ((first->weight >= second->weight) || second==NULL) ? first : second;
// }

// node *BuildTree(node **nodes_list, int sym_count) {
//   node empty_node = {NULL, NULL, 0, 0, 0, 0, 0xFFFFFFFFFFFFFFFF};

//   node** res_mass = malloc(sizeof(node*) * sym_count);
//   for (int i = 0; i < sym_count; i++)
//   {
//     res_mass[i] = &empty_node;
//   }
//   res_mass[0]->weight = 0;
//   int res_idx = 0;

//   free(nodes_list);
//   PrintNodeList(res_mass,sym_count);
//   return res_mass[res_idx];
// }

void SetCodeForSymb(node *in_node, eight_bytes code, char len, byte add_code) {
  if (in_node) {
    len += 1;
    code <<= 1;
    code |= add_code;
    (*in_node).code = code;
    (*in_node).code_len = len;
    SetCodeForSymb((*in_node).left_leaf, code, len, 0);
    SetCodeForSymb((*in_node).right_leaf, code, len, 1);
  }
}

void CountSymbInThree(node *in_node, eight_bytes *count) {
  if (in_node) {
    if (!(*in_node).left_leaf && !(*in_node).right_leaf)
      *count += 1;
    CountSymbInThree((*in_node).left_leaf, count);
    CountSymbInThree((*in_node).right_leaf, count);
  }
}

void WriteNodeInFile(node *in_node, FILE *files_out) {
  if (in_node) {
    if (!(*in_node).left_leaf && !(*in_node).right_leaf) {
      fwrite(&(*in_node).symb, sizeof(byte), 1, files_out);
      fwrite(&(*in_node).code, sizeof(eight_bytes), 1, files_out);
      fwrite(&(*in_node).code_len, sizeof(char), 1, files_out);
    }
    WriteNodeInFile((*in_node).left_leaf, files_out);
    WriteNodeInFile((*in_node).right_leaf, files_out);
  }
}

int SerializationOfTheTree(files files, node *root) {
  eight_bytes count = 0;
  // TO-DO meaning huff byte
  CountSymbInThree(root, &count);
  fwrite(&(root->weight), sizeof(eight_bytes), 1, files._out);
  fwrite(&count, sizeof(eight_bytes), 1, files._out);
  WriteNodeInFile(root, files._out);
}

void GetSymbCode(node *in_node, byte *symb, code *desired) {
  if (in_node) {
    if ((*in_node).symb == *symb){
      (*desired).code = (*in_node).code;
      (*desired).code_len = (*in_node).code_len;
    }
    GetSymbCode((*in_node).left_leaf, symb, desired);
    GetSymbCode((*in_node).right_leaf, symb, desired);
  }
}

int WriteEncodeFile(files files, node *root){
  fseek(files._in, 0, SEEK_SET);
  byte byte_from_file;
  code code_from_file = {0,0};
  code fitted_bits = {0,0};
  code not_fitted_bits = {0,0};
  code buff = {0,0};
  byte end_file = 0;
  int i = 0; // debug
  do
  {
    // i++;
    // if(i==10) exit(1);
    buff.code = not_fitted_bits.code;
    buff.code_len = not_fitted_bits.code_len;
    not_fitted_bits.code_len = 0;
    not_fitted_bits.code = 0;
    while (buff.code_len < BUFFSIZE && !end_file) // !=
    {
      byte_from_file = fgetc(files._in);
      end_file = feof(files._in);
      if(!end_file) {
        // printf("symb: ");
        // F(byte_from_file);
        // printf("code: ");
        // F_32_code(code_from_file.code, BUFFSIZE - code_from_file.code_len, BUFFSIZE);
        GetSymbCode(root, &byte_from_file, &code_from_file);

        byte free_space_in_buffer = BUFFSIZE - buff.code_len;
        if(code_from_file.code_len <= free_space_in_buffer) {
          fitted_bits.code = code_from_file.code << (free_space_in_buffer- code_from_file.code_len);
          fitted_bits.code_len = code_from_file.code_len;
        } else {
          fitted_bits.code = code_from_file.code >> (code_from_file.code_len-free_space_in_buffer);
          fitted_bits.code_len = free_space_in_buffer;

          not_fitted_bits.code_len = code_from_file.code_len-free_space_in_buffer;
          not_fitted_bits.code = code_from_file.code << (BUFFSIZE - not_fitted_bits.code_len);
        }
      }
      buff.code |= fitted_bits.code;
      buff.code_len += fitted_bits.code_len;
      fitted_bits.code_len = 0;
      fitted_bits.code = 0;

    }
    // printf("buff: ");
    // F_32(buff.code);
    // printf("\n");
    fwrite(&buff.code, sizeof(eight_bytes), 1, files._out);
    
  } while (!end_file);
  
}
