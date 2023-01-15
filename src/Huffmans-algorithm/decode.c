#include "decode.h"

void Decode(files files) {
  eight_bytes count_nodes;
  eight_bytes count_symb = 0;
  node **nodes_list = RestoreTree(files, &count_nodes, &count_symb);
  if (count_symb == 0) ErrorHandler(3, "");
  WriteDecodeFile(files, nodes_list, count_nodes, &count_symb);
}

node **RestoreTree(files files, eight_bytes *count_nodes,
                   eight_bytes *count_symb) {
  fread(count_symb, sizeof(eight_bytes), 1, files._in);
  fread(count_nodes, sizeof(eight_bytes), 1, files._in);

  node **nodes_list = malloc((*count_nodes) * sizeof(node *));
  for (int i = 0; i < (*count_nodes); i++) {
    nodes_list[i] = malloc(sizeof(node));
    fread(&(nodes_list[i]->symb), sizeof(byte), 1, files._in);
    fread(&(nodes_list[i]->code), sizeof(eight_bytes), 1, files._in);
    fread(&(nodes_list[i]->code_len), sizeof(byte), 1, files._in);
  }
  return BubleSort(nodes_list, (*count_nodes), pred_ascending_by_code_len);
}

//  WriteDecodeFile scheme
//  +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+
//  | 1 | | 1 | | 0 | | 0 | | 0 | | 0 | | 1 | | 0 | | 1 |  bytes from file
//  +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+
//  +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+
//  | 1 | | 0 | | 1 | | * | | * | | * | | * | | * | | * | buff |len = 3|
//  +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+
//  +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+
//  | * | | * | | * | | * | | * | | * | | 0 | | 1 | | 1 | not fitted bits in the prev step |len = 3|
//  +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+
//                    +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+
//                    | 1 | | 1 | | 0 | | 0 | | 0 | | 0 | | * | | * | | * | fitted bits from bytes which got from file |len = 5|
//                    +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+
//  +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+
//  | 1 | | 0 | | 1 | | 1 | | 1 | | 0 | | 0 | | 0 | | 0 | buff |len = 8|
//  +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+
//  DecodeBuff
//                                            +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+
//                                            | 0 | | 0 | | * | | * | | * | | * | | * | | * | | * | buff |len = 2|
//                                            +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+
//                    +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+
//                    | * | | * | | * | | * | | * | | * | | 0 | | 1 | | 1 | not fitted bits in the prev step |len = 3|
//                    +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+
//                                            +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+
//                                            | 0 | | 0 | | 0 | | 1 | | 1 | | * | | * | | * | | * | buff |len = 5|
//                                            +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+
//  DecodeBuff
//  +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+
//  | 1 | | 1 | | * | | * | | * | | * | | * | | * | | * | buff |len = 2| (in general <=5 and >=0)
//  +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+

int WriteDecodeFile(files files, node **node_list, eight_bytes count_nodes,
                    eight_bytes *count_symb) {

  int encode_part_size = DefineEncodedPartSize(files);
  code not_fitted_bits = {0, 0};
  code fitted_bits = {0, 0};
  code buff = {0, 0};
  eight_bytes byte_from_file = 0;
  for (int i = 0; i < encode_part_size; ++i) {
    fread(&byte_from_file, sizeof(eight_bytes), 1, files._in);

    fitted_bits = SetFittedBits(buff, byte_from_file);

    not_fitted_bits = SetNotFittedBits(fitted_bits, byte_from_file);

    AddingCode(&buff, &fitted_bits);

    if (buff.code_len != BUFFSIZE) ErrorHandler(6, "d", buff.code_len);


    DecodeBuff(files, node_list, count_nodes, count_symb, &buff);

    not_fitted_bits.code <<=
        (BUFFSIZE - buff.code_len - not_fitted_bits.code_len);

    AddingCode(&buff, &not_fitted_bits);

    DecodeBuff(files, node_list, count_nodes, count_symb, &buff);

    not_fitted_bits = ZeroingCode();
    fitted_bits = ZeroingCode();

    if (buff.code_len == BUFFSIZE) ErrorHandler(5, "");
  }

  if(buff.code_len != 0 && buff.code != 0) ErrorHandler(4, "");
}

void DecodeBuff(files files, node **node_list, eight_bytes count_nodes,
                eight_bytes *count_symb, code* buff) {
  node *symb_code = NULL;

  while ((symb_code = GetSymbByCode((*buff), node_list, count_nodes)) &&
         (*buff).code_len > node_list[0]->code_len) {

    (*buff).code <<= symb_code->code_len;
    (*buff).code_len -= symb_code->code_len;

    if ((*buff).code_len >= 0 && *count_symb > 0 && symb_code) {
      fwrite(&symb_code->symb, sizeof(byte), 1, files._out);
      (*count_symb)--;
    }
  }
}

int DefineEncodedPartSize(files files){
  int current_pos = ftell(files._in);
  fseek(files._in, 0, SEEK_END);
  int end_file = ftell(files._in);
  fseek(files._in, current_pos, SEEK_SET);
  int encode_part_size = (end_file - current_pos) / 8;
  if(encode_part_size==0) ErrorHandler(3, "");
  return encode_part_size;
}

void ZeroingFiittedAndNotFittedBitsCode(code* not_fitted_bits, code* fitted_bits){
    (*not_fitted_bits).code = 0;
    (*not_fitted_bits).code_len = 0;
    (*fitted_bits).code = 0;
    (*fitted_bits).code_len = 0;
}

code SetFittedBits(code buff, eight_bytes byte_from_file){
  code fitted_bits;
  fitted_bits.code_len = BUFFSIZE - buff.code_len;
  fitted_bits.code = byte_from_file >> buff.code_len;
  return fitted_bits;
}

code SetNotFittedBits(code fitted_bits, eight_bytes byte_from_file){
  code not_fitted_bits;
  not_fitted_bits.code_len = BUFFSIZE - fitted_bits.code_len;
    if (not_fitted_bits.code_len != 0) {
      not_fitted_bits.code = byte_from_file << fitted_bits.code_len;
      not_fitted_bits.code >>= fitted_bits.code_len;
    } else {
      not_fitted_bits.code = 0;
    }
  return not_fitted_bits;
}

node *GetSymbByCode(code buff, node **node_list, eight_bytes count_nodes) {
  node *res = NULL;

  if (buff.code_len <= node_list[0]->code_len)
    return NULL;

  for (int i = 0; i < count_nodes; i++) {
    eight_bytes tmp = buff.code >> (BUFFSIZE - node_list[i]->code_len);

    if (tmp == node_list[i]->code && buff.code_len >= node_list[i]->code_len) {
      res = node_list[i];
      return res;
    }
  }
  return res;
}

int pred_ascending_by_code_len(node* first, node* second){
  return first->code_len > second->code_len;
}