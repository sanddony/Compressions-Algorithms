#include "decode.h"

int Decode(files files) {
  eight_bytes count_nodes;
  eight_bytes count_symb = 0; 
  node **nodes_list = RestoreTree(files, &count_nodes, &count_symb);
  if(count_symb == 0){
    fprintf(stderr, "Empty file!\n");
    exit(0);
  }
  WriteDecodeFile(files, nodes_list, count_nodes, &count_symb);
}

node **RestoreTree(files files, eight_bytes* count_nodes, eight_bytes* count_symb) {
  fread(count_symb, sizeof(eight_bytes),1 ,files._in);
  fread(count_nodes, sizeof(eight_bytes),1 ,files._in);

  node **nodes_list = malloc((*count_nodes) * sizeof(node *));
  for (int i = 0; i < (*count_nodes); i++) {
    nodes_list[i] = malloc(sizeof(node));
    fread(&(nodes_list[i]->symb), sizeof(byte),1 ,files._in);
    fread(&(nodes_list[i]->code), sizeof(eight_bytes),1 ,files._in);
    fread(&(nodes_list[i]->code_len), sizeof(byte),1 ,files._in);
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

//int WriteDecodeFile(files files, node **node_list, eight_bytes count_nodes, eight_bytes* count_symb) {
//  //  Define size
//  int current_pos = ftell(files._in);
//  fseek(files._in, 0, SEEK_END);
//  int end_file = ftell(files._in);
//  fseek(files._in, current_pos, SEEK_SET);
//  int encode_part_size = (end_file - current_pos)/8;
//  //  of the encoded part (in bytes)
//
//  node *symb_code = NULL;
//  code not_fitted_bits = {0, 0};
//  code fitted_bits = {0, 0};
//  code buff = {0, 0};
//  eight_bytes byte_from_file = 0;
//  for (int i = 0; i < encode_part_size; ++i) {
//
//    //  Handle situation when remaining in the buffer bits and not fitted bits in previous step
//    //  more or equal 8, it means we need to handle this bits and no need to get new byte from file
//    if ((buff.code_len + not_fitted_bits.code_len) <= BUFFSIZE) {
//      fread(&byte_from_file, sizeof(eight_bytes),1 ,files._in);
//    } else {
//      i--;
//    }
//    // printf("file            %.2d: ",i);
//    // F_32(byte_from_file);
//
//    fitted_bits.code_len = BUFFSIZE - (buff.code_len + not_fitted_bits.code_len);
//    if(fitted_bits.code_len != 0){
//      fitted_bits.code =
//              byte_from_file >> (buff.code_len + not_fitted_bits.code_len);
//    } else {
//      fitted_bits.code = 0;
//      fitted_bits.code_len = 0;
//    }
//
//
//    // printf("fitted  |len = %.2d|: ", fitted_bits.code_len);
//    // F_32(fitted_bits.code);
//    // printf("not fit |len = %.2d|: ", not_fitted_bits.code_len);
//    // F_32(not_fitted_bits.code);
//
//    buff.code_len =
//        buff.code_len + not_fitted_bits.code_len + fitted_bits.code_len;
//
//    if(fitted_bits.code_len >= 0 ) { // FIRST CASE
//      // printf("FIRST CASE\n");
//      not_fitted_bits.code <<= fitted_bits.code_len; // old
//      buff.code |= not_fitted_bits.code;
//
//      if(fitted_bits.code_len != BUFFSIZE){
//        not_fitted_bits.code = byte_from_file << (fitted_bits.code_len);
//        not_fitted_bits.code >>= (fitted_bits.code_len);
//        not_fitted_bits.code_len = (BUFFSIZE - fitted_bits.code_len);
//      } else {
//        not_fitted_bits.code = 0;
//        not_fitted_bits.code_len = 0;
//      }
//
//    } else { // SECOND CASE
//      // printf("SECOND CASE\n");
//      fitted_bits.code = not_fitted_bits.code ; // old
//      fitted_bits.code >>= abs(fitted_bits.code_len); // old
//
//      not_fitted_bits.code <<= BUFFSIZE - abs(fitted_bits.code_len);
//      not_fitted_bits.code >>= BUFFSIZE - abs(fitted_bits.code_len);
//      not_fitted_bits.code_len = abs(fitted_bits.code_len);
//
//      fitted_bits.code_len = BUFFSIZE - not_fitted_bits.code_len; // ?
//    }
//    // printf("current\n");
//    // printf("not fit |len = %.2d|: ", not_fitted_bits.code_len);
//    // F_32(not_fitted_bits.code);
//
//    buff.code |= fitted_bits.code;
//
//
//    if (buff.code_len != BUFFSIZE) {
//      fprintf(stderr,"Error! Buff code len not equal 64! |buff.code_len = %.2d|\n", buff.code_len);
//      exit(1);
//    }
//    // printf("buff in |len = %.2d|: ", buff.code_len);
//    // F_32(buff.code);
//    // printf("\n=========count_symb %.3d===============\n",*count_symb);
//    while ((symb_code = GetSymbByCode(buff, node_list, count_nodes))) {
//      buff.code <<= symb_code->code_len;
//      buff.code_len -= symb_code->code_len;
//
//      if (buff.code_len >= 0 && *count_symb > 0){
//        fwrite(&symb_code->symb, sizeof(byte), 1, files._out);
//        (*count_symb)--;
//        // printf("%c",symb_code->symb);
//        // F(symb_code->symb);
//      }
//
//    }
//    // printf("\n===========================\n");
//    // printf("buff out|len = %.2d|: ", buff.code_len );
//    // F_32(buff.code);
//    // printf("\n\n\n");
//    buff.code_len = buff.code_len < 0 ? 0 : buff.code_len;
//
//    if(buff.code_len > 0 && i == encode_part_size-1 && (*count_symb)!=0){
//      i--;
//    }
//  }
//
//}

int WriteDecodeFile(files files, node **node_list, eight_bytes count_nodes, eight_bytes* count_symb) {

    //  Define size
    int current_pos = ftell(files._in);
    fseek(files._in, 0, SEEK_END);
    int end_file = ftell(files._in);
    fseek(files._in, current_pos, SEEK_SET);
    int encode_part_size = (end_file - current_pos)/8;
    //  of the encoded part (in bytes)

    node *symb_code = NULL;
    code not_fitted_bits = {0, 0};
    code fitted_bits = {0, 0};
    code buff = {0, 0};
    eight_bytes byte_from_file = 0;


    for (int i = 0; i < encode_part_size; ++i) {

        fread(&byte_from_file, sizeof(eight_bytes),1 ,files._in);


        if(files.visualization) printf("byte_from_file |064|: ");
        if(files.visualization) F_32(byte_from_file);

        if(files.visualization) printf("buffer |%.3d|:         ",buff.code_len);
        if(files.visualization) F_32_code(buff.code, 0, buff.code_len);

        fitted_bits.code_len = BUFFSIZE - buff.code_len;
        fitted_bits.code = byte_from_file >> buff.code_len;
 
        if(files.visualization) printf("fitted |%.3d|:         ",fitted_bits.code_len);
        if(files.visualization) F_32_code(fitted_bits.code, BUFFSIZE - fitted_bits.code_len, 64);

        not_fitted_bits.code_len = BUFFSIZE - fitted_bits.code_len;
        if(not_fitted_bits.code_len != 0){
            not_fitted_bits.code = byte_from_file << fitted_bits.code_len;
            not_fitted_bits.code >>= fitted_bits.code_len;
        } else{
            not_fitted_bits.code = 0;
        }


        if(files.visualization) printf("not fitted |%.3d|:     ",not_fitted_bits.code_len);
        if(files.visualization) F_32_code(not_fitted_bits.code, BUFFSIZE - not_fitted_bits.code_len, 64);

        buff.code_len+=fitted_bits.code_len;
        buff.code |= fitted_bits.code;

        if(files.visualization) printf("buffer |%.3d|:         ",buff.code_len);
        if(files.visualization) F_32_code(buff.code, 0, buff.code_len);

        if (buff.code_len != BUFFSIZE) {
            fprintf(stderr,"Error! Buff code len not equal 64! |buff.code_len = %.2d|\n", buff.code_len);
            exit(1);
        }

        if(files.visualization) printf("\n====================================\n");
        

        while ((symb_code = GetSymbByCode(buff, node_list, count_nodes)) &&
                buff.code_len > node_list[0]->code_len) {

            buff.code <<= symb_code->code_len;
            buff.code_len -= symb_code->code_len;

            if (buff.code_len >= 0 && *count_symb > 0){
                fwrite(&symb_code->symb, sizeof(byte), 1, files._out);
                (*count_symb)--;
                if(files.visualization) printf("%c",symb_code->symb);
            }
        }
        if(files.visualization) printf("\n====================================\n");

        if(files.visualization) printf("buffer |%.3d|:         ",buff.code_len);
        if(files.visualization) F_32_code(buff.code, 0, buff.code_len);

        not_fitted_bits.code <<= (BUFFSIZE - buff.code_len - not_fitted_bits.code_len);
        
        buff.code |= not_fitted_bits.code;
        buff.code_len+=not_fitted_bits.code_len;

        if(files.visualization) printf("not fitted |%.3d|:     ",not_fitted_bits.code_len);
        if(files.visualization) F_32_code(not_fitted_bits.code, 0, not_fitted_bits.code_len);

        if(files.visualization) printf("buffer |%.3d|:         ",buff.code_len);
        if(files.visualization) F_32_code(buff.code, 0, buff.code_len);

        if(files.visualization) printf("\n====================================\n");

        while ((symb_code = GetSymbByCode(buff, node_list, count_nodes)) &&
                buff.code_len > 0) {
            buff.code <<= symb_code->code_len;
            buff.code_len -= symb_code->code_len;

            if (buff.code_len >= 0 && *count_symb > 0){
                fwrite(&symb_code->symb, sizeof(byte), 1, files._out);
                (*count_symb)--;
                if(files.visualization) printf("%c",symb_code->symb);
            }
        }
        if(files.visualization) printf("\n===============i=%.2d==================\n",i);
        
        if(files.visualization) printf("buffer |%.3d|:         ",buff.code_len);
        if(files.visualization) F_32_code(buff.code, 0, buff.code_len);

        if(files.visualization) printf("\n\n\n");

        not_fitted_bits.code = 0;
        not_fitted_bits.code_len = 0;
        fitted_bits.code = 0;
        fitted_bits.code_len = 0;

        if (buff.code_len == BUFFSIZE) {
            fprintf(stderr,"Error! Can't decode file!\n");
            exit(1);
        }
      // if(i == 6)exit(1);
    }
}

node *GetSymbByCode(code buff, node **node_list, eight_bytes count_nodes) {
  node *res = NULL;

  if (buff.code_len <= 0) // <= node_list[0]->code_len
    return NULL;

  for (int i = 0; i < count_nodes; i++) {
    eight_bytes tmp = buff.code >> (BUFFSIZE - node_list[i]->code_len);

    if (tmp == node_list[i]->code && buff.code_len >= node_list[i]->code_len) {
      res = node_list[i];
    }
  }
  return res;
}

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

// ============DEBUG============
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


// int WriteDecodeFile(files files, node **node_list, eight_bytes count_nodes, eight_bytes* count_symb) {
//   //  Define size 
//   int current_pos = ftell(files._in);
//   fseek(files._in, 0, SEEK_END);
//   int end_file = ftell(files._in);
//   fseek(files._in, current_pos, SEEK_SET);
//   int encode_part_size = (end_file - current_pos)/8;
//   //  of the encoded part (in bytes)

//   node *symb_code = NULL;
//   code not_fitted_bits = {0, 0};
//   code fitted_bits = {0, 0};
//   code buff = {0, 0};
//   eight_bytes byte_from_file = 0;
//   for (int i = 0; i < encode_part_size; ++i) {
    
//     //  Handle situation when remaining in the buffer bits and not fitted bits in previous step
//     //  more or equal BUFFSIZE, it means we need to handle this bits and no need to get new byte from file
//     if ((buff.code_len + not_fitted_bits.code_len) <= BUFFSIZE) {
//       fread(&byte_from_file, sizeof(eight_bytes),1 ,files._in);
//     } else {
//       i--;
//     }
//     //

//   }
// }


//1000000000000000000000000000000000000000000000000000000000000000
//1111010010110010001101111101111100110110110000110111100100010010
// 1111111110111011011101111111111111110110111010110111111111110011