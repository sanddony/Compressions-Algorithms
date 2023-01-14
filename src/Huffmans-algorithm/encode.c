#include "encode.h"

// TO-DO return err_code
int Encode(files files) {
  eight_bytes sym_count;
  node **nodes_list = GetFrequencyOfBytes(files, &sym_count);
  node *root = BuildTree(nodes_list, sym_count);  
  SetCodeForSymb(root, 0, -1, 0);
  SerializationOfTheTree(files, root, sym_count);
  WriteEncodeFile(files, root);
}

// Sort by Weight
node **SortRoots(node **nodes_list, int list_size) {
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

node **GetFrequencyOfBytes(files files, eight_bytes *sym_count) {

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
  res->weight = left->weight + right->weight;
  res->is_root = 1;

  res->code = 0;
  res->code_len = 0;
  res->symb = 0;
  return res;
}

node *BuildTree(node **nodes_list, eight_bytes sym_count) {
  node empty_node = {NULL, NULL, 0, 0, 0, 0, 0xFFFFFFFFFFFFFFFF};
  // while root > 1 in list (or second element not a empty_node)
  // Take first two elements and build root with them in leafs
  // Switch first element on the new generated on before step, then
  // switch second element on the empty_node
  while (nodes_list[1]->is_root) {
    //GetTwoMin 
    nodes_list[0] = UniteTwoNodes(nodes_list[0], nodes_list[1]);
    // free(nodes_list[1]); CHECK WHY IT RAISE SEGA
    nodes_list[1] = &empty_node;
    SortRoots(nodes_list, sym_count); //
  }
  node *root = nodes_list[0];
  free(nodes_list);
  return root;
}


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

int SerializationOfTheTree(files files, node *root, eight_bytes sym_count) {
  // TO-DO meaning huff byte
  fwrite(&(root->weight), sizeof(eight_bytes), 1, files._out);
  fwrite(&sym_count, sizeof(eight_bytes), 1, files._out);
  WriteNodeInFile(root, files._out);
}

void GetSymbCode(node *in_node, byte *symb, code *desired) {
  if (in_node) {
    if ((*in_node).symb == *symb && !(*in_node).left_leaf && !(*in_node).right_leaf){
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
    buff.code = not_fitted_bits.code;
    buff.code_len = not_fitted_bits.code_len;
    not_fitted_bits.code_len = 0;
    not_fitted_bits.code = 0;
    while (buff.code_len < BUFFSIZE && !end_file) // !=
    {
      byte_from_file = fgetc(files._in);
      end_file = feof(files._in);
      if(!end_file) {
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
    fwrite(&buff.code, sizeof(eight_bytes), 1, files._out);
    
  } while (!end_file);
  
}

// node *BuildTree(node **nodes_list, eight_bytes sym_count){
//   node empty_node = {NULL, NULL, 0, 0, 0, 0, 0xFFFFFFFFFFFFFFFF};
//   node** res_mass = malloc(sizeof(node*) * sym_count);
//   node** p1 = &nodes_list[0];
//   node** p2 = &res_mass[0];
//   node** insert = &res_mass[0];
//   int i = 0;
//   *insert = UniteTwoNodes(*p1, *(p1+1));
//   insert++;
//   p1+=2;
//   while (p1 != &nodes_list[sym_count-1] && insert != p2)
//   {
//     if((p1 + 1) < (p2 + 1)){
//       *insert = UniteTwoNodes(*p1, *(p1+1));
//       p1+=2;
//     } else {
//       *insert = UniteTwoNodes(*p1, *p2);
//       p1+=2;
//       p2++;
//     }
//     insert++;
//   }
// }