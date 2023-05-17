#ifndef HUFFMAN_H
#define HUFFMAN_H


#include "Compress.h"
#include "Huffman/huff.h"



class Huffman: public Compress{
public:
    Huffman();
    void EncodeFile(files input) override;
    void DecodeFile(files input) override;
    void operator()(files) override;
};

#endif // HUFFMAN_H
