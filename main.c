





int ParseParams(int argc, char** argv, files* files){
    int err_code = 0;
    int flag;
    while ((flag = getopt_long(argc, argv, "edo:i:", long_options, NULL)) !=
           -1) {
        switch (flag) {
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
        fprintf(stderr,"Usage: haffman [-ed] [-i file_input] [-o file_output]");
        err_code = 1;
    }
    return err_code;
}
