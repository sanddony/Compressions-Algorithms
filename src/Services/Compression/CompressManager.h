#ifndef COMPRESSMANAGER_H
#define COMPRESSMANAGER_H

// #include <bits/types/FILE.h>
#include <string>
#include <vector>
#include <map>

#include "Compress.h"
#include "Huffman.h"


//TO-DO add singelton pattern
class CompressManager
{
public:
    CompressManager()=default;
    static FILE *getFile(std::string filePath);
    static std::string generateStandartOutputFile(std::string inputFilePath);
    static files prepareAlgorithmData(std::string inputFilePath,
                                      std::string outputFilePath,
                                      std::string mode,
                                      std::string alg);
    static files prepareAlgorithmData(std::string inputFilePath,
                                      std::string mode,
                                      std::string alg);
    void compressFile(files);
private:
    std::map<const std::string,Compress*> _algorithmsDict;
    std::vector<Compress*> _algorithms = {new Huffman()};
};

#endif // COMPRESSMANAGER_H
