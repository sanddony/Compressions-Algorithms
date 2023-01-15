#ifndef COMPRESSIONS_ALGORITHMS_HEADER_H
#define COMPRESSIONS_ALGORITHMS_HEADER_H

#include <stdio.h>
#include <getopt.h>
#include <string.h>


static struct option long_options[] = {{"algorithm", optional_argument, NULL, 'a'},
                                       {"encode", no_argument, NULL, 'e'},
                                       {"decode", no_argument, NULL, 'd'},
                                       {"output", optional_argument, NULL, 'o'},
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


void ParseParams(int argc, char** argv, files* files);
int ParseAlgorithm(char *alg_str);
FILE *GetFile(char *path);
void Usage();

#define ERRLIST_SIZE 10



static const char *const errlist[ERRLIST_SIZE] = {
    [0] = "No such file or directory:",
    [1] = "No available algoritms with this name:",
    [2] = "GetSymbCode Error! Get byte, which haven't a code!",
    [3] = "Empty file!",
    [4] = "Error! Not all code was decoded!",
    [5] = "Error! Can't decode file!",
    [6] = "Error! Buff code len not equal 64! |buff.code_len =",
};
void ErrorHandler(int errnum, char* prm, ...);

#endif //COMPRESSIONS_ALGORITHMS_HEADER_H
