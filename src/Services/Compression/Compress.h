#ifndef COMPRESS_H
#define COMPRESS_H

#include <string>


enum Modes{ENCODE,DECODE};

enum Algorithms{Huffmans};

//TO-DO Remove int params

typedef struct files {
    FILE* _in;
    FILE* _out;

    int algorithm;
    int mode;
    int visualization;
}files;


class Compress
{
public:
    virtual void EncodeFile(files) = 0;
    virtual void DecodeFile(files) = 0;
    virtual void operator()(files) = 0;

protected:
    const std::string _algorithmName;
};

#endif // COMPRESS_H
