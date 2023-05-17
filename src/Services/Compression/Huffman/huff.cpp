#include "huff.h"
#include "decode.h"
#include "encode.h"
#include <stdarg.h>

void Huff(files files) {
  files.mode == ENCODE ? Encode(files) : Decode(files);
}

code ZeroingCode() {
  code res = {0, 0};
  return res;
}

void AddingCode(code* result_code, code* added_code){
    (*result_code).code_len += (*added_code).code_len;
    (*result_code).code |= (*added_code).code;
}

// O(n^2)
node **BubleSort(node **nodes_list, int list_size, ruleCompare rule) {
  node *tmp;
  bool noSwap;
  for (int i = list_size - 1; i >= 0; i--) {
    noSwap = 1;
    for (int j = 0; j < i; j++) {
      if(rule(nodes_list[j], nodes_list[j+1])) {
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
