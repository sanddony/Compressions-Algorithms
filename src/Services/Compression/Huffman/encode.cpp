#include "encode.h"

void Encode(files files) {
  eight_bytes sym_count;
  node **full_nodes_list;
  node **nodes_list = GetFrequencyOfBytes(files, &sym_count, &full_nodes_list);
  node *root = BuildTree(nodes_list, sym_count);
  SetCodeForSymb(root, 0, -1, 0);
  SerializationOfTheTree(files, root, sym_count);
  WriteEncodeFile(files, root, full_nodes_list);
}

node **GetFrequencyOfBytes(files files, eight_bytes *sym_count,
                           node ***full_nodes_list) {

  int file_size = DefineFileSize(files);

  AllocateMemForMassOn256Elem(full_nodes_list);

  CountingRepetitionsOfBytes(file_size, files, full_nodes_list);

  node **res_list = GetMassWithMeaningBytes(full_nodes_list, sym_count);

  return BubleSort(res_list, *sym_count, pred_ascending_by_weight);
}

// left should be less or equal right
node *UniteTwoNodes(node *left, node *right) {
  node *res = (node*)malloc(sizeof(node));
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
    nodes_list[0] = UniteTwoNodes(nodes_list[0], nodes_list[1]);
    nodes_list[1] = &empty_node;
    BubleSort(nodes_list, sym_count, pred_ascending_by_weight); //
  }
  node *root = nodes_list[0];
  return root;
}

void SetCodeForSymb(node *in_node, eight_bytes code, char len, byte_c add_code) {
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
      fwrite(&(*in_node).symb, sizeof(byte_c), 1, files_out);
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

code GetSymbCode(node **node_list, byte_c *symb) {
  code res = {0, 0};
  if (node_list[*symb]->right_leaf && node_list[*symb]->left_leaf) {
    PrintNode(node_list[*symb]);
    ErrorHandler(2, "");
  }
  res.code = node_list[*symb]->code;
  res.code_len = node_list[*symb]->code_len;
  return res;
}

int WriteEncodeFile(files files, node *root, node **full_nodes_list) {
  fseek(files._in, 0, SEEK_SET);
  byte_c byte_from_file;
  code code_from_file = {0, 0};
  code fitted_bits = {0, 0};
  code not_fitted_bits = {0, 0};
  code buff = {0, 0};
  byte_c end_file = 0;
  do {

    buff = not_fitted_bits;
    not_fitted_bits = ZeroingCode();

    while (buff.code_len < BUFFSIZE && !end_file) // !=
    {
      byte_from_file = fgetc(files._in);
      end_file = feof(files._in);
      if (!end_file) {
        ReadAndParseNewByte(&code_from_file, full_nodes_list, byte_from_file, &buff,
                         &fitted_bits, &not_fitted_bits);
      }
      AddingCode(&buff, &fitted_bits);
      fitted_bits = ZeroingCode();
    }

    fwrite(&buff.code, sizeof(eight_bytes), 1, files._out);

  } while (!end_file);
}

void ReadAndParseNewByte(code* code_from_file, node **full_nodes_list, byte_c byte_from_file, code* buff,
                         code* fitted_bits, code* not_fitted_bits) {
  (*code_from_file) = GetSymbCode(full_nodes_list, &byte_from_file);
  byte_c free_space_in_buffer = BUFFSIZE - (*buff).code_len;
  if ((*code_from_file).code_len <= free_space_in_buffer) {
    (*fitted_bits).code = (*code_from_file).code
                       << (free_space_in_buffer - (*code_from_file).code_len);
    (*fitted_bits).code_len = (*code_from_file).code_len;
  } else {
    (*fitted_bits).code =
        (*code_from_file).code >> ((*code_from_file).code_len - free_space_in_buffer);
    (*fitted_bits).code_len = free_space_in_buffer;

    (*not_fitted_bits).code_len = (*code_from_file).code_len - free_space_in_buffer;
    (*not_fitted_bits).code = (*code_from_file).code
                           << (BUFFSIZE - (*not_fitted_bits).code_len);
  }
}

int DefineFileSize(files files){
  fseek(files._in, 0, SEEK_END);
  int file_size = ftell(files._in);
  fseek(files._in, 0, SEEK_SET);
  if (file_size == 0) {
    ErrorHandler(3, "");
  }
  return file_size;
}

// Allocate mem for mass, which contain 256 elements. Exactly that much,
// because program encode bytes and BYTE_MAX_SIZE=256 (0x11111111)-uc
void AllocateMemForMassOn256Elem(node ***full_nodes_list){
  (*full_nodes_list) = (node**)malloc(sizeof(node *) * BYTE_MAX_SIZE);
  for (int i = 0; i < BYTE_MAX_SIZE; i++) {
    (*full_nodes_list)[i] = (node*)malloc(sizeof(node));
    (*full_nodes_list)[i]->symb = i;
    (*full_nodes_list)[i]->is_root = 1;
  }
}

void CountingRepetitionsOfBytes(int file_size, files files, node ***full_nodes_list){
  byte_c tmp[1];
  for (int i = 0; i < file_size; i++) {
    fread(tmp, sizeof(byte_c), 1, files._in);
    for (int k = 0; k < BYTE_MAX_SIZE; k++) {
      if ((*full_nodes_list)[k]->symb == tmp[0]) {
        (*full_nodes_list)[k]->weight++;
      }
    }
  }
}

node **GetMassWithMeaningBytes(node ***full_nodes_list, eight_bytes *sym_count){
  (*sym_count) = 0;
  int res_list_indx[BYTE_MAX_SIZE];
  for (int i = 0; i < BYTE_MAX_SIZE; i++) {
    if ((*full_nodes_list)[i]->weight != 0) {
      res_list_indx[(*sym_count)++] = i;
    }
  }

  node **res_list = (node**)malloc(sizeof(node *) * (*sym_count));
  for (int i = 0; i < (*sym_count); i++) {
    res_list[i] = (*full_nodes_list)[res_list_indx[i]];
  }
  return res_list;
}

int pred_ascending_by_weight(node* first, node* second){
  return first->weight > second->weight;
}
