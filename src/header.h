#ifndef COMPRESSIONS_ALGORITHMS_HEADER_H
#define COMPRESSIONS_ALGORITHMS_HEADER_H

#include <stdio.h>
#include <getopt.h>
#include <string.h>


static struct option long_options[] = {{"algorithm", required_argument, NULL, 'a'},
                                       {"encode", no_argument, NULL, 'e'},
                                       {"decode", no_argument, NULL, 'd'},
                                       {"output", required_argument, NULL, 'o'},
                                       {"input", required_argument, NULL, 'i'},
                                       {"visualization", no_argument, NULL, 'v'},
                                       {NULL, 0, NULL, 0}};

enum Modes{ENCODE,DECODE};

enum Algorithms{Huffmans};

typedef struct files {
   FILE* _in;
   FILE* _out;

   int algorithm;
   int mode;
   int visualization;
}files;


int ParseParams(int argc, char** argv, files* files);
int ParseAlgorithm(char* alg_str, int * err_code);
FILE *GetFile(char *path);
void Usage();

#endif //COMPRESSIONS_ALGORITHMS_HEADER_H
