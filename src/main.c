#include "./Huffmans-algorithm/huff.h"
#include "header.h"

int main(int argc, char **argv) {

  files files = {NULL, NULL, 0, 2, 0};

  int err_code = ParseParams(argc, argv, &files);
  if (!err_code) {
    switch (files.algorithm) {
    case Huffmans:
      err_code = Huff(files);
      break;
    default:
      Usage();
      break;
    }
  } else {
    Usage();
  }
  return err_code;
}

int ParseParams(int argc, char **argv, files *files) {
  int err_code = 0;
  int flag;
  char* input_file = NULL;
  while ((flag = getopt_long(argc, argv, "a:edo:i:v", long_options, NULL)) !=
         -1) {
    switch (flag) {
    case 'a':
      (*files).algorithm = ParseAlgorithm(optarg, &err_code);
      ;
      break;
    case 'e':
      (*files).mode = ENCODE;
      break;
    case 'd':
      (*files).mode = DECODE;
      break;
    case 'i':
      input_file = optarg;
      (*files)._in = GetFile(input_file);
      if (!(*files)._in) {
        fprintf(stderr, "Can't open file with this name: %s\n", input_file);
        err_code = 1;
      }
      break;
    case 'o':
      (*files)._out = fopen(optarg, "wb");
      if (!(*files)._out) {
        fprintf(stderr, "Can't write in file with this name: %s\n", optarg);
        err_code = 1;
      }
      break;
    case 'v':
      (*files).visualization = 1;
      break;
    default:
      break;
    }
  }
  if (!(*files)._out) {
    char output_file[100];
    sprintf(output_file, "%s.compress",input_file);
    (*files)._out = fopen(output_file, "wb");
    if (!(*files)._out) {
      fprintf(stderr, "Can't write in file with this name: %s\n", output_file);
      err_code = 1;
    }
  } else if ((*files).mode == 2 || !(*files)._in) {
    err_code = 1;
  }
  return err_code;
}

int ParseAlgorithm(char *alg_str, int *err_code) {
  int res = 0;
  if (!strcmp(alg_str, "huff")) {
    res = 0;
  } else {
    fprintf(stderr, "No available algoritms with this name: %s\n", optarg);
    *err_code = 1;
  }
  return res;
}

FILE *GetFile(char *path) {
  FILE *file;
  if (!strcmp(path, "-")) {
    file = stdin;
  } else {
    file = fopen(path, "rb");
  }
  return file;
}

// TO-DO modify to error handler (smth like strerror)
void Usage() {
  fprintf(stderr,
          "Usage: compress [-a(algorithm) huff] [-e(encode)|-d(decode)] "
          "[-i(input) file_input] [-o(output) file_output]\n");
}
