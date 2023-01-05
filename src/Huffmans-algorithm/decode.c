#include "decode.h"

int Decode(files files) {
  byte count_nodes;
  size_t count_bytes;
  node **nodes_list = RestoreTree(files, &count_nodes, &count_bytes);
  PrintNodeList(nodes_list, count_nodes);
  WriteDecodeFile(files, nodes_list, count_nodes, &count_bytes);
}

node **RestoreTree(files files, byte *count_nodes, size_t* count_bytes) {
  *count_bytes = fgetc(files._in);
  for (int i = 0; i < 7; i++)
  {
    fgetc(files._in);
  }
  
  *count_nodes = fgetc(files._in);
  node **nodes_list = malloc((*count_nodes) * sizeof(node *));
  for (int i = 0; i < (*count_nodes); i++) {
    nodes_list[i] = malloc(sizeof(node));
    nodes_list[i]->symb = fgetc(files._in);
    nodes_list[i]->code = fgetc(files._in);
    nodes_list[i]->code_len = fgetc(files._in);
  }
  // PrintNodeList(nodes_list, (*count_nodes));
  return SortNodes(nodes_list, (*count_nodes));
}

// int WriteDecodeFile(files files, node** node_list, byte count_nodes) {
//     // Define size of the encoded bytes
//     int current_pos = ftell(files._in);
//     fseek(files._in, 0, SEEK_END);
//     int end_file = ftell(files._in);
//     fseek(files._in, current_pos, SEEK_SET);
//     int encode_part_size = end_file - current_pos;
//     //    // ________ | ________
// //    // |NEED TO HANDLE| 1. Last byte
// //    // =====================================================
// //    // 1. Get byte from file | sum_of_the_code_len = 8;
// //    // Get first sum_of_the_code_len bytes from received byte
// //    // 2. While get node move to the left and print symb (TRUE)
// //    //      2.1 Get node
// //    //      2.2 Write received symb to the file,
// //    //          if got the NULL: break
// //    //      2.3 Move to the left on the code_len and
// sum_of_the_code_len+=code_len
// //    // =====================================================
// //
// // ______ __
// // >>= 8-code_len; <<= 8-code_len
// // >>= code_len; <<= code_len
//     node* symb_code = NULL;
//     code not_fitted_bits = {0,0};
//     code fitted_bits = {0,0};
//     code buff = {0,0};
//     byte byte_from_file = 0;
//     for (int i = 0; i < encode_part_size; ++i) {
//         byte byte_from_file = fgetc(files._in);

//         fitted_bits.code = byte_from_file >>
//         (buff.code_len+not_fitted_bits.code_len);

//         not_fitted_bits.code <<= buff.code_len-not_fitted_bits.code_len;

//         buff.code |= not_fitted_bits.code;

//         buff.code |= fitted_bits.code;

//         not_fitted_bits.code = byte_from_file <<
//         (buff.code_len+not_fitted_bits.code_len);

//         not_fitted_bits.code >>= (buff.code_len+not_fitted_bits.code_len);

//         not_fitted_bits.code_len = buff.code_len+not_fitted_bits.code_len;

//         buff.code_len = 8;

//         int k = 0;
//         while((symb_code = GetSymbByCode(buff.code, node_list, count_nodes))
//         &&
//               buff.code_len>=0){

//             buff.code_len-=symb_code->code_len;

//             if(buff.code_len>=0){
//                 fwrite(&symb_code->symb, sizeof(byte), 1, files._out);
//                 buff.code <<= symb_code->code_len;
//             }else {
//                 buff.code_len+=symb_code->code_len;
//                 break;
//             }

//         }
//     }
//     //
// }

int WriteDecodeFile(files files, node **node_list, byte count_nodes, size_t* count_bytes) {
  // Define size of the encoded bytes
  int current_pos = ftell(files._in);
  fseek(files._in, 0, SEEK_END);
  int end_file = ftell(files._in);
  fseek(files._in, current_pos, SEEK_SET);
  int encode_part_size = end_file - current_pos;
  printf("encode_part_size %d\n",encode_part_size);
  printf("count_bytes %d\n",count_bytes);
  //    // ________ | ________
  //    // |NEED TO HANDLE| 1. Last byte
  //    // =====================================================
  //    // 1. Get byte from file | sum_of_the_code_len = 8;
  //    // Get first sum_of_the_code_len bytes from received byte
  //    // 2. While get node move to the left and print symb (TRUE)
  //    //      2.1 Get node
  //    //      2.2 Write received symb to the file,
  //    //          if got the NULL: break
  //    //      2.3 Move to the left on the code_len and
  //    sum_of_the_code_len+=code_len
  //    // =====================================================
  node *symb_code = NULL;
  code not_fitted_bits = {0, 0};
  code fitted_bits = {0, 0};
  code buff = {0, 0};
  byte byte_from_file = 0;
  for (int i = 0; i < encode_part_size; ++i) {
    if ((buff.code_len + not_fitted_bits.code_len) <= 8) {
      byte_from_file = fgetc(files._in);
    } else {
      i--;
    }

    // printf("До заполения буфера\n");
    DebugWriteDecodeFile(byte_from_file, not_fitted_bits, fitted_bits, buff,
                         symb_code);

    fitted_bits.code =
        byte_from_file >> (buff.code_len + not_fitted_bits.code_len);
    fitted_bits.code_len = 8 - (buff.code_len + not_fitted_bits.code_len);

    if(fitted_bits.code_len >= 0 ) {
      not_fitted_bits.code <<= fitted_bits.code_len; // old
      buff.code |= not_fitted_bits.code;
      buff.code |= fitted_bits.code;
      buff.code_len =
        buff.code_len + not_fitted_bits.code_len + fitted_bits.code_len;
      not_fitted_bits.code = byte_from_file << fitted_bits.code_len;
      not_fitted_bits.code >>= fitted_bits.code_len;
      not_fitted_bits.code_len = (8 - fitted_bits.code_len);
    } else {
      byte tmp = not_fitted_bits.code ; // old
      tmp >>= abs(fitted_bits.code_len); // old
      buff.code |= tmp;
      buff.code |= fitted_bits.code;
      buff.code_len =
        buff.code_len + not_fitted_bits.code_len + fitted_bits.code_len;
      not_fitted_bits.code <<= 8 - abs(fitted_bits.code_len);
      not_fitted_bits.code >>= 8 - abs(fitted_bits.code_len);
      not_fitted_bits.code_len = abs(fitted_bits.code_len);
    }
    fitted_bits.code_len = fitted_bits.code_len <= 0 ? 0 : fitted_bits.code_len;
    // printf("После заполения буфера\n");
    DebugWriteDecodeFile(byte_from_file, not_fitted_bits, fitted_bits, buff,
                         symb_code);

    if (buff.code_len != 8) {
      printf("!!!!!!!!!!!!! buff.code_len = %d !!!!!!!!!!!!!!\n",
             buff.code_len);
      exit(1);
    }


    int k = 0;
    while ((symb_code = GetSymbByCode(buff, node_list, count_nodes))) {

      buff.code <<= symb_code->code_len;
      buff.code_len -= symb_code->code_len;

      if (buff.code_len >= 0){
        fwrite(&symb_code->symb, sizeof(byte), 1, files._out);
        count_bytes--;
      }

      k++;
      // printf("while #%d\n", k);
      DebugWriteDecodeFile(byte_from_file, not_fitted_bits, fitted_bits, buff,
                           symb_code);
    }
    buff.code_len = buff.code_len < 0 ? 0 : buff.code_len;
  }
}

void DebugWriteDecodeFile(byte byte_from_file, code not_fitted_bits,
                          code fitted_bits, code buff, node *symb_code) {
  // printf("\n=============================\n");
  // printf("DebugWriteDecodeFile\n");
  // printf("byte_from_file\n");
  // F(byte_from_file);
  // printf("fitted_bits |len = %d|\n", fitted_bits.code_len);
  // F(fitted_bits.code);
  // for (int i = 0; i < 8-fitted_bits.code_len; i++)
  //   printf(" ");
  // for (int i = 0; i < fitted_bits.code_len; i++)
  //   printf("^");
  // printf("\n");
  // printf("not_fitted_bits |len = %d|\n", not_fitted_bits.code_len);
  // F(not_fitted_bits.code);
  // for (int i = 0; i < 8-not_fitted_bits.code_len; i++)
  //   printf(" ");
  // for (int i = 0; i < not_fitted_bits.code_len; i++)
  //   printf("^");
  // printf("\n");
  // printf("buff |len = %d|\n", buff.code_len);
  // F(buff.code);
  // for (int i = 0; i < buff.code_len; i++)
  //   printf("^");
  // printf("\n");
  // printf("NODE:\n");
  // PrintNode(symb_code);
  // printf("\n=============================\n");
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