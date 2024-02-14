#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc != 5) {
    fprintf(stderr, "Wrong number of arguments! Need only 4 arguments: Encode "
                    "time, encoded files path, decode time and decoded files path\n");
    exit(1);
  }
  FILE* encode_file = fopen(argv[2],"rb");
  fseek(encode_file, 0, SEEK_SET);
  fseek(encode_file, 0, SEEK_END);
  double encode_file_size = (double)ftell(encode_file);

  FILE* decode_file = fopen(argv[4],"rb");
  fseek(decode_file, 0, SEEK_SET);
  fseek(decode_file, 0, SEEK_END);
  double decode_file_size = (double)ftell(decode_file);
  printf("\n+-----------------+-----------------+-----------------+\n");
  printf("| encode time     | decode time     | ratio           | \n");
  printf("+-----------------+-----------------+-----------------+\n");

  printf("|%.15f|", atof(argv[1]));
  // printf("Encode time: %s\n", argv[1]);

  printf("%.15f|", atof(argv[3]));
  // printf("Decode time: %s\n", argv[3]);
  printf("%.15f|\n", decode_file_size/encode_file_size);

  printf("+-----------------+-----------------+-----------------+\n");
//  +-----------------+-----------------+-----------------+
//  | encode time     | decode time     | ratio           | 
//  +-----------------+-----------------+-----------------+

}