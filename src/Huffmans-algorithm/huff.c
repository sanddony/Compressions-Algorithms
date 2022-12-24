#include "huff.h"

//TO-DO: Decide where need single pointer, where double. Maybe we need allocate mem for mass and
//then work with this pointers. 


#ifdef DEBUGG
int main(int argc, char** argv) {

    
    
}
#endif // DEBUGG

int Huff(files files){
  int err_code = 0;
  if(files.mode==ENCODE) {
    err_code = Encode(files);
  } else {
    err_code = Decode(files);
  }
}

//TO-DO return err_code
int Encode(files files){
  int sym_count;
  // node** nodes_list = GetFrequencyOfBytes(files, &sym_count);
  // PrintNodeList(nodes_list,sym_count);
  // node* root = BuildTree(nodes_list, sym_count); 
}

int Decode(files files){
    
}

