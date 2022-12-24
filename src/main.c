#include "header.h"
#include "./Huffmans-algorithm/huff.h"

int main(int argc, char** argv) {

    files files = {NULL, NULL, -1, -1};
    
    int err_code = ParseParams(argc,argv,&files);
    if (!err_code) {
        switch (files.algorithm)
        {
        case Huffmans:
            err_code = Huff(files);
            break;
        }
    } else {
        Usage();
    }
    return err_code;
    
}

int ParseParams(int argc, char** argv, files* files){
    int err_code = 0;
    int flag;
    while ((flag = getopt_long(argc, argv, "a:edo:i:", long_options, NULL)) !=
           -1) {
        switch (flag) {
            case 'a':
                ParseAlgorithm(optarg, &err_code);
                (*files).algorithm = Huffmans;
                break;
            case 'e':
                (*files).mode = ENCODE;
                break;
            case 'd':
                (*files).mode = DECODE;
                break;
            case 'i':
                (*files)._in = GetFile(optarg);
                if(!(*files)._in){
                    fprintf(stderr,"No files: %s\n",optarg);
                    Usage();
                    err_code = 1;
                }
                break;
            case 'o':
                (*files)._out = fopen(optarg,"wb+");
                break;
            default:
                break;
        }
    }
    if(!(*files)._in || !(*files)._out || (*files).mode==2){
        Usage();
        err_code = 1;
    }
    return err_code;
}

int ParseAlgorithm(char* alg_str, int * err_code){
    int res;
    if(strcmp(alg_str, "huff") == 0){
        res = 1;
       }
       else{
        fprintf(stderr,"No available algoritms with this name: %s\n",optarg);
        *err_code = 1;
       }
    return res;
}

FILE *GetFile(char *path) {
  FILE *file;
  if (!strcmp(path, "-")) {
    file = stdin;
  } else {
    file = fopen(path, "rb+");
  }
  return file;
}

//TO-DO modify to error handler (smth like strerror)
void Usage(){
    fprintf(stderr,"Usage: haffman [-a(algorithm) huff] [-e(encode)|-d(decode)] [-i(input) file_input] [-o(output) file_output]\n");
}
