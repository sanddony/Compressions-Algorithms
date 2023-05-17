#include "Huffman.h"

Huffman::Huffman(){
//    _algorithmName = std::string("Huffman's algorithm");
}

void Huffman::EncodeFile(files input)
{
    input.algorithm = 0;
    Huff(input);
}

void Huffman::DecodeFile(files input)
{
    input.algorithm = 0;
    Huff(input);
}

void Huffman::operator()(files input)
{
    input.algorithm = 0;
    Huff(input);
}
