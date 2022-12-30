#include "decode.h"
 
int Decode(files files) {
    byte count_nodes;
    node** nodes_list = RestoreTree(files, &count_nodes);
    WriteDecodeFile(files, nodes_list, count_nodes);
}

node** RestoreTree(files files, byte* count_nodes) {
    *count_nodes = fgetc(files._in);
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

//int WriteDecodeFile(files files, node** node_list, byte count_nodes) {

//    byte buff = fgetc(files._in);
//    byte next_iter_buf = 0;
//    byte buff_len_inv = 0; //?
//    node* node_;
//    while(!feof(files._in)) {
//        printf("BUFF ");
//        F(buff);
//        do {
//            node_ = GetSymbByCode(buff,node_list,count_nodes);
//            PrintNode(node_);
//            fwrite(&node_->symb, sizeof(byte), 1, files._out);
//            if(node_){
//                buff_len_inv+=node_->code_len;
//                buff <<= node_->code_len;
//                // buff >>= buff_len_inv;
//            }
//            printf("BUFF ");
//            F(buff);
//            // if(buff_len_inv>20)break;
//        } while(node_ && buff_len_inv <= 8);
//        byte buff_len = 8-buff_len_inv;
//        buff<<=(8-buff_len);
//        byte tmp = fgetc(files._in);
//        next_iter_buf = tmp << (8-buff_len);
//        tmp >>= buff_len;
//        buff |= tmp;
//        // break;
//    }
//}

int WriteDecodeFile(files files, node** node_list, byte count_nodes) {
    // Define size of the encoded bytes
    int current_pos = ftell(files._in);
    fseek(files._in, 0, SEEK_END);
    int end_file = ftell(files._in);
    fseek(files._in, current_pos, SEEK_SET);
    int encode_part_size = end_file - current_pos;
    printf("%d\n",encode_part_size);
    printf("%d\n",current_pos);
    //    // ________ | ________
//    // |NEED TO HANDLE| 1. Last byte
//    // =====================================================
//    // 1. Get byte from file | sum_of_the_code_len = 8;
//    // Get first sum_of_the_code_len bytes from received byte
//    // 2. While get node move to the left and print symb (TRUE)
//    //      2.1 Get node
//    //      2.2 Write received symb to the file,
//    //          if got the NULL: break
//    //      2.3 Move to the left on the code_len and sum_of_the_code_len+=code_len
//    // =====================================================
//
// ______ __
// >>= 8-code_len; <<= 8-code_len
// >>= code_len; <<= code_len
    byte sum_of_the_code_len = 8;
    node* right_byte_code;
    byte not_fitted_bits = 0;
    byte fitted_bits;
    byte buff;
    byte byte_from_file = 0;
    byte not_fitted_bits_len = 0;
    for (int i = 0; i < encode_part_size; ++i) {
        byte byte_from_file = fgetc(files._in);
         // Handle situation, when checked not all of the byte
        printf("not_fitted_bits_len : %d\n",not_fitted_bits_len);
        printf("buff: ");
        F(buff);
        printf("byte_from_file: ");
        F(byte_from_file);
        fitted_bits = byte_from_file >> abs((8-sum_of_the_code_len)+not_fitted_bits_len);
        printf("fitted_bits: ");
        F(fitted_bits);
        printf("shitf:%d\n",abs((8-sum_of_the_code_len)+not_fitted_bits_len));
        // 8-sum_of_the_code_len - длина оставшейся не декодированной части
        not_fitted_bits >>= (8-sum_of_the_code_len);
        printf("not_fitted_bits: ");
        F(not_fitted_bits);
        printf("buff: ");
        F(buff);
        buff |= not_fitted_bits;
        printf("buff: ");
        F(buff);
        buff |= fitted_bits;
        printf("buff: ");
        F(buff);
        not_fitted_bits = byte_from_file << sum_of_the_code_len;
        not_fitted_bits >>= sum_of_the_code_len;
        printf("not_fitted_bits: ");
        F(not_fitted_bits);
        sum_of_the_code_len = 0;
        not_fitted_bits_len = sum_of_the_code_len;
        while((right_byte_code = GetSymbByCode(buff, node_list, count_nodes)) &&
                sum_of_the_code_len<=8){
            PrintNode(right_byte_code);
            F(buff);
            printf("sum_of_the_code_len %d\n", sum_of_the_code_len);
            fwrite(&right_byte_code->symb, sizeof(byte), 1, files._out);
            buff <<= right_byte_code->code_len;
            buff >>= right_byte_code->code_len;
            sum_of_the_code_len+=right_byte_code->code_len;
            printf("sum_of_the_code_len %d\n", sum_of_the_code_len);
        }
    }
    //
}

//TO-DO union with another GetSymbCode

// code XXXxxxxx
//TO-DO change it at all
node* GetSymbByCode(byte code, node** node_list, byte count_nodes) {
    node* res = NULL;
    for (int i = 0; i < count_nodes; i++)
    {
        byte tmp = code;
        tmp >>= (8 - node_list[i]->code_len);
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