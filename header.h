//
// Created by user on 12/5/22.
//

#ifndef HUFFMAN_HEADER_H
#define HUFFMAN_HEADER_H


#define INF (int)1e9
#define BUF_SIZE (int)1e4
#define uc unsigned char


typedef struct node node;
struct node {
    node* to1;
    node* to0;
    int sum;
    bool is_leaf;
    uc symb;
    char* code;
};
void char_to_bin (uc c, char* bin_code);
void bin_to_char (const char* bin, uc* c);
int print_bin (char* bin, FILE* fout, FILE* log);
node* build_tree (node* mas, int symb_sum);
void get_code (node* cur_node, char cur_code[], int cur_pos);
void make_dir (node* cur_node, char* cur_dir, int* cur_size);
void zip (FILE* fin, FILE* fout, FILE* log);
node* build_unzip_tree (const char* bin, int* cur_pos,
                        node* cur_node, int* free_node_pos,
                        node* nodes);
void decode (FILE* fout, char* bin, int* cur_pos,
             node* root, int end_pos, int last_byte,
             FILE* log, FILE* fin, uc* zip_text);
void unzip (FILE* fin, FILE* fout, FILE* log);
void CheckAllocMemErr(void* pointer);



#endif //HUFFMAN_HEADER_H
