#include "consoleIO.h"

#ifdef HUFF_LIB
int main(int argc, char **argv) {

  files files = {NULL, NULL, 0, 2, 0};

  ParseParams(argc, argv, &files);
  switch (files.algorithm) {
  case Huffmans:
    Huff(files);
    break;
  default:
    Usage();
    exit(1);
    break;
  }
}
#endif // HUFF_LIB

FILE *GetFile(char *path) {
    FILE *file;
    if (!strcmp(path, "-")) {
        file = stdin;
    } else {
        file = fopen(path, "rb");
    }
    return file;
}

#ifdef HUFF_LIB
void ErrorHandler(int errnum, char *prm, ...) {
  va_list ptr;
  va_start(ptr, prm);
  if (errnum < 0 || errnum >= ERRLIST_SIZE) {
    fprintf(stderr, "Unknown error: %d", errnum);
  } else {
    fprintf(stderr, "%s ", (char *)errlist[errnum]);
  }
  while (*prm) {
    if (*prm == 'd') {
      fprintf(stderr, "%d ", va_arg(ptr, int));
    } else if (*prm == 'f') {
      fprintf(stderr, "%f ", va_arg(ptr, double));
    } else if (*prm == 's') {
      fprintf(stderr, "%s ", va_arg(ptr, char *));
    }
    prm++;
  }
  fprintf(stderr, "\n");
  exit(0);
}

void ParseParams(int argc, char **argv, files *files) {
    int flag;
    char *input_file = NULL;
    while ((flag = getopt_long(argc, argv, "a:edo:i:v", long_options, NULL)) !=
           -1) {
        switch (flag) {
        case 'a':
            (*files).algorithm = ParseAlgorithm(optarg);
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
                ErrorHandler(0, "s", input_file);
            }
            break;
        case 'o':
            (*files)._out = fopen(optarg, "wb");
            if (!(*files)._out) {
                ErrorHandler(0, "s", optarg);
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
        sprintf(output_file, "%s.compress", input_file);
        (*files)._out = fopen(output_file, "wb");
        if (!(*files)._out) {
            ErrorHandler(0, "s", output_file);
        }
    } else if ((*files).mode == 2 || !(*files)._in) {
        Usage();
        exit(1);
    }
}

int ParseAlgorithm(char *alg_str) {
    int res = 0;
    if (!strcmp(alg_str, "huff")) {
        res = 0;
    } else {
        ErrorHandler(1, "s", alg_str);
    }
    return res;
}
#endif

