#include "encode.h"

// TO-DO return err_code
int Encode(files files) {
  int sym_count;
  node **nodes_list = GetFrequencyOfBytes(files, &sym_count);
  PrintNodeList(nodes_list, sym_count);
  node *root = BuildTree(nodes_list, sym_count);
  // PrintNode(root); // debug
  SetCodeForSymb(root, 0, -1, 0);
  TraverseAndPrintThree(root); // debug
  SerializationOfTheTree(files, root);
  // node* wanted = NULL;
  // byte l = 'a';
  // F(l);
  // GetSymbCode(root, &l, &wanted);
  // PrintNode(wanted);
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
  res->symb = 0;
  return res;
}

node *BuildTree(node **nodes_list, int sym_count) {
  node empty_node = {NULL, NULL, 0, 0, 0, 0, 0x7FFFFFFF};
  // while root > 1 in list (or second element not a empty_node)
  // Take first two elements and build root with them in leafs
  // Switch first element on the new generated on before step, then
  // switch second element on the empty_node

  while (nodes_list[1]->is_root) {
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

  node *root = nodes_list[0];
  free(nodes_list);

  return root;
}

void SetCodeForSymb(node *in_node, byte code, byte len, byte add_code) {
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

void CountSymbInThree(node *in_node, byte *size) {
  if (in_node) {
    // TO-DO assert for len > 8
    if (!(*in_node).left_leaf && !(*in_node).right_leaf)
      *size += 1;
    CountSymbInThree((*in_node).left_leaf, size);
    CountSymbInThree((*in_node).right_leaf, size);
  }
}

void WriteNodeInFile(node *in_node, FILE *files_out) {
  if (in_node) {
    if (!(*in_node).left_leaf && !(*in_node).right_leaf) {
      fwrite(&(*in_node).symb, sizeof(byte), 1, files_out);
      fwrite(&(*in_node).code, sizeof(byte), 1, files_out);
      fwrite(&(*in_node).code_len, sizeof(byte), 1, files_out);
    }
    WriteNodeInFile((*in_node).left_leaf, files_out);
    WriteNodeInFile((*in_node).right_leaf, files_out);
  }
}

int SerializationOfTheTree(files files, node *root) {
  // count of the nodes 1 bytes
  //
  // code (1 byte)
  // code_len (1 byte)
  // symb (1 byte)

  byte count = 0;
  CountSymbInThree(root, &count);
  printf("%d\n", count);
  fwrite(&root->weight, sizeof(size_t), 1, files._out);
  fwrite(&count, sizeof(byte), 1, files._out);
  WriteNodeInFile(root, files._out);
}

void GetSymbCode(node *in_node, byte *symb, node **out_node) {
  if (in_node) {
    if ((*in_node).symb == *symb)
      (*out_node) = in_node;
    GetSymbCode((*in_node).left_leaf, symb, out_node);
    GetSymbCode((*in_node).right_leaf, symb, out_node);
  }
}

int WriteEncodeFile(files files, node *root) {
  node *desired = NULL;
  byte output_byte = 0;
  byte len_output_byte = 8;
  byte input_byte;
  fseek(files._in, 0, SEEK_SET);
  while (!feof(files._in) && len_output_byte > 0) {
    input_byte = fgetc(files._in);
    GetSymbCode(root, &input_byte, &desired);
    if (!desired) {
      fprintf(stderr,
              "Erorr: Code for symb not found, three built incorrectly!");
      exit(1);
    }
    PrintNode(desired);
    if (len_output_byte >= (*desired).code_len) {
      printf("\nfirst case\n");
      len_output_byte -= (*desired).code_len;
      printf("len_output_byte: %d \n", len_output_byte);
      printf("Write output_byte\n");
      printf("Shift on %d: ", (*desired).code_len);
      output_byte <<= (*desired).code_len;
      F(output_byte);
      printf("Adding : ");
      F((*desired).code);
      output_byte |= (*desired).code;
      printf("Added : ");
      F(output_byte);
      if (len_output_byte == 0) {
        fwrite(&output_byte, sizeof(byte), 1, files._out);
        printf("output_byte wrote!\n\n");
        output_byte = 0;
        len_output_byte = 8;
      }
    } else {
      printf("\nsecond case\n");
      printf("len_output_byte: %d \n", len_output_byte);
      printf("Write output_byte\n");
      printf("Shift on %d: ", len_output_byte);
      output_byte <<= len_output_byte;
      F(output_byte);
      byte tmp = (*desired).code >> (*desired).code_len - len_output_byte;
      printf("Adding: ");
      F(tmp);
      output_byte |= tmp;
      printf("Added: ");
      F(output_byte);
      fwrite(&output_byte, sizeof(byte), 1, files._out);
      printf("output_byte wrote!\n\n");
      //======================
      printf("len_output_byte: %d \n", len_output_byte);
      len_output_byte = (*desired).code_len - len_output_byte;
      output_byte = 0;
      tmp = (*desired).code << (8 - len_output_byte);
      tmp >>= 8 - len_output_byte;
      printf("Write output_byte\n");
      printf("Adding : ");
      F(tmp);
      output_byte |= tmp;
      printf("Added : ");
      F(output_byte);
      len_output_byte = 8 - len_output_byte;
    }
  }
  printf("len_output_byte: %d \n", len_output_byte);
  if (len_output_byte != 0) {
    output_byte <<= len_output_byte;
    fwrite(&output_byte, sizeof(byte), 1, files._out);
    printf("output_byte wrote!\n\n");
  }
}

// int WriteEncodeFile(files files, node *root) {
//   fseek(files._in, 0, SEEK_SET);
//   node *desired = NULL;
//   code output_byte = {0,0};
//   code buff = {0,0};
//   while (!feof(files._in)) {
//     // if (!desired) {
//     //   fprintf(stderr,
//     //           "Erorr: Code for symb not found, three built incorrectly!");
//     //   exit(1);
//     // }
//     output_byte = buff;
//     while (output_byte.code_len != 8)
//     {
//       byte input_byte = fgetc(files._in);
//       if(feof(files._in)) exit(1); //
//       GetSymbCode(root, &input_byte, &desired);
//       buff.code = desired->code;
//       buff.code_len = desired->code_len;
//       if(output_byte.code_len + buff.code_len <= 8) {
//         buff.code <<= (8 - output_byte.code_len - buff.code_len);
//         output_byte.code_len += buff.code_len;
//       } else {
//         code not_fitted_bits = {0,0};
//         not_fitted_bits.code_len = output_byte.code_len + buff.code_len - 8;
//         not_fitted_bits.code = buff.code << (8-not_fitted_bits.code_len);
//         buff.code <<= (8-output_byte.code_len);
//         buff.code >>= (8-output_byte.code_len);
//         output_byte.code_len += (8-output_byte.code_len);
//       }
//       output_byte.code |= buff.code;
//     }
//     if(!feof(files._in)){
//       // PrintNode(desired);
//       fwrite(&output_byte, sizeof(byte), 1, files._out);
//       // printf("||||||||||||||||||||||||");
//       output_byte.code_len = 0;
//     }
//   }
// }
