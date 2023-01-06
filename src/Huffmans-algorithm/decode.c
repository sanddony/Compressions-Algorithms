#include "decode.h"

int Decode(files files) {
  byte count_nodes;
  size_t count_bytes; // TO-DO Handle equal zero case(instant exit, file empty)
  node **nodes_list = RestoreTree(files, &count_nodes, &count_bytes);
  PrintNodeList(nodes_list, count_nodes);
  WriteDecodeFile(files, nodes_list, count_nodes, &count_bytes);
}

node **RestoreTree(files files, byte *count_nodes, size_t* count_bytes) {
  char ptr[100];
  fread(count_bytes, sizeof(size_t),1 ,files._in);

  *count_nodes = fgetc(files._in);
  node **nodes_list = malloc((*count_nodes) * sizeof(node *));
  for (int i = 0; i < (*count_nodes); i++) {
    nodes_list[i] = malloc(sizeof(node));
    nodes_list[i]->symb = fgetc(files._in);
    nodes_list[i]->code = fgetc(files._in);
    nodes_list[i]->code_len = fgetc(files._in);
  }
  return SortNodes(nodes_list, (*count_nodes));
}
  
//  =====================================================
//  FIRST CASE
//  +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ 
//  | 1 | | 0 | | 1 | | * | | * | | * | | * | | * | | * | buff |len = 3|
//  +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+
//  +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ 
//  | * | | * | | * | | * | | * | | 1 | | 0 | | 1 | | 1 | not fitted bits in the prev step |len = 4|
//  +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+
//                                                  +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ 
//  bytes from file                                 | 1 | | 1 | | 0 | | 0 | | 0 | | 0 | | 1 | | 0 | | 1 | 
//                                                  +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+    
//  +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ 
//  | * | | * | | * | | * | | * | | * | | * | | * | | 1 | fitted bits from bytes which got from file
//  +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+       
//  |fitted bits len = 8 - buff len - not fitted bits in the prev step len = 8 - 3 - 4 = 1|
//  the another bits will write into not_fitted_bits
// 
//  SECOND CASE
//  +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ 
//  | * | | * | | * | | * | | * | | * | | * | | * | | * | bytes from file (don't get it)  
//  +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+    
//  +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ 
//  | * | | * | | * | | * | | * | | * | | * | | * | | * | fitted bits from bytes which got from file
//  +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+       
//  +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ 
//  | 1 | | 0 | | 1 | | * | | * | | * | | * | | * | | * | buff |len = 3|
//  +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+
//        +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ 
//        | * | | * | | 1 | | 0 | | 1 | | 1 | | 0 | | 1 | | 1 | not fitted bits in the prev step |len = 6|
//        +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+
//  +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ 
//  | 1 | | 0 | | 1 | | 1 | | 0 | | 1 | | 1 | | 0 | | 1 | fitted bits from bytes which got from file
//  +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+
//  |fitted bits len = 8 - buff len - not fitted bits in the prev step len = 8 - 3 - 6 = -1|      
//        +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ 
//        | * | | * | | * | | * | | * | | * | | * | | * | | 1 | not fitted bits |len = abs(fitted bits len)|
//        +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+
//  =====================================================
int WriteDecodeFile(files files, node **node_list, byte count_nodes, size_t* count_bytes) {
  //  Define size 
  int current_pos = ftell(files._in);
  fseek(files._in, 0, SEEK_END);
  int end_file = ftell(files._in);
  fseek(files._in, current_pos, SEEK_SET);
  int encode_part_size = end_file - current_pos;
  //  of the encoded bytes


  node *symb_code = NULL;
  code not_fitted_bits = {0, 0};
  code fitted_bits = {0, 0};
  code buff = {0, 0};
  byte byte_from_file = 0;
  for (int i = 0; i < encode_part_size; ++i) {
    
    //  Handle situation when remaining in the buffer bits and not fitted bits in previous step
    //  more or equal 8, it means we need to handle this bits and no need to get new byte from file
    if ((buff.code_len + not_fitted_bits.code_len) <= 8) {
      byte_from_file = fgetc(files._in);
    } else {
      i--;
    }
    //


    fitted_bits.code =
        byte_from_file >> (buff.code_len + not_fitted_bits.code_len);
    fitted_bits.code_len = 8 - (buff.code_len + not_fitted_bits.code_len);
    
    buff.code_len =
        buff.code_len + not_fitted_bits.code_len + fitted_bits.code_len;

    if(fitted_bits.code_len >= 0 ) { // FIRST CASE

      not_fitted_bits.code <<= fitted_bits.code_len; // old 
      buff.code |= not_fitted_bits.code;
      
      not_fitted_bits.code = byte_from_file << (fitted_bits.code_len);
      not_fitted_bits.code >>= (fitted_bits.code_len);
      not_fitted_bits.code_len = (8 - fitted_bits.code_len);

    } else { // SECOND CASE

      fitted_bits.code = not_fitted_bits.code ; // old 
      fitted_bits.code >>= abs(fitted_bits.code_len); // old 

      not_fitted_bits.code <<= 8 - abs(fitted_bits.code_len);
      not_fitted_bits.code >>= 8 - abs(fitted_bits.code_len);
      not_fitted_bits.code_len = abs(fitted_bits.code_len);

      fitted_bits.code_len = 8 - not_fitted_bits.code_len;
    }

    buff.code |= fitted_bits.code;


    if (buff.code_len != 8) {
      fprintf(stderr,"Error! Buff code len more then 8! |buff.code_len = %d|\n", buff.code_len);
      exit(1);
    }


    while ((symb_code = GetSymbByCode(buff, node_list, count_nodes))) {

      buff.code <<= symb_code->code_len;
      buff.code_len -= symb_code->code_len;

      if (buff.code_len >= 0 && *count_bytes > 0){
        fwrite(&symb_code->symb, sizeof(byte), 1, files._out);
        (*count_bytes)--;

      }

      DebugWriteDecodeFile(byte_from_file, not_fitted_bits, fitted_bits, buff,
                           symb_code);

    }
    buff.code_len = buff.code_len < 0 ? 0 : buff.code_len;
  }
}


void DebugWriteDecodeFile(byte byte_from_file, code not_fitted_bits,
                          code fitted_bits, code buff, node *symb_code) {
  printf("\n=============================\n");
  printf("DebugWriteDecodeFile\n");
  printf("byte_from_file\n");
  F(byte_from_file);
  printf("fitted_bits |len = %d|\n", fitted_bits.code_len);
  F(fitted_bits.code);
  for (int i = 0; i < 8-fitted_bits.code_len; i++)
    printf(" ");
  for (int i = 0; i < fitted_bits.code_len; i++)
    printf("^");
  printf("\n");
  printf("not_fitted_bits |len = %d|\n", not_fitted_bits.code_len);
  F(not_fitted_bits.code);
  for (int i = 0; i < 8-not_fitted_bits.code_len; i++)
    printf(" ");
  for (int i = 0; i < not_fitted_bits.code_len; i++)
    printf("^");
  printf("\n");
  printf("buff |len = %d|\n", buff.code_len);
  F(buff.code);
  for (int i = 0; i < buff.code_len; i++)
    printf("^");
  printf("\n");
  printf("NODE:\n");
  PrintNode(symb_code);
  printf("\n=============================\n");
}

// TO-DO union with another GetSymbCode
node *GetSymbByCode(code buff, node **node_list, byte count_nodes) {
  node *res = NULL;
  if (buff.code_len <= 0)
    return NULL;
  for (int i = 0; i < count_nodes; i++) {
    byte tmp = buff.code;
    tmp >>= (8 - node_list[i]->code_len);
    if (tmp == node_list[i]->code && buff.code_len >= node_list[i]->code_len) {
      res = node_list[i];
    }
  }
  return res;
}

// TO-DO union with another sort
node **SortNodes(node **nodes_list, int list_size) {
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