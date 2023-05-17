#ifndef CONSOLEIO_H
#define CONSOLEIO_H

#include "huff.h"
#include <getopt.h>
#include <stdarg.h>


static struct option long_options[] = {{"algorithm", optional_argument, NULL, 'a'},
                                       {"encode", no_argument, NULL, 'e'},
                                       {"decode", no_argument, NULL, 'd'},
                                       {"output", optional_argument, NULL, 'o'},
                                       {"input", required_argument, NULL, 'i'},
                                       {"visualization", no_argument, NULL, 'v'},
                                       {NULL, 0, NULL, 0}};


void ParseParams(int argc, char** argv, files* files);
int ParseAlgorithm(char *alg_str);
void Usage();


#endif // CONSOLEIO_H
