#ifndef HUFFMAN_HEADER_H
#define HUFFMAN_HEADER_H
#include <stdio.h>
#define byte unsigned char


static struct option long_options[] = {{"encode", no_argument, NULL, 'e'},
                                       {"decode", no_argument, NULL, 'd'},
                                       {"output", required_argument, NULL, 'o'},
                                       {"input", required_argument, NULL, 'i'},
                                       {NULL, 0, NULL, 0}};

enum Modes{ENCODE,DECODE};

typedef struct files {
   FILE* _in;
   FILE* _out;
   int mode;
}files;

typedef struct node node;
struct node {
    node* left_leaf;
    node* right_leaf;
    int is_root;
    byte symb;
    char code;  
    int weight;
};

int ParseParams(int argc, char** argv, files* files);
FILE *GetFile(char *path);

// encode
int Encode(files files);
node** SortRoots(node** node_list);
node** GetFrequencyOfBytes(files files);
node* BuildTree(node** nodes_list);
int SerializationOfTheTree(files files, node* root);
// node* BinarySearch(node* root, char byte);
int WriteEncodeFile(files files, node* root);
//

// decode
node* RestoreTree(files files);
int WriteDecodeFile(files files, node* root);
//


#endif //HUFFMAN_HEADER_H
