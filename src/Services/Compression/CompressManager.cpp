#include "CompressManager.h"

FILE *CompressManager::getFile(std::string filePath)
{
    return fopen(filePath.c_str(), "rb");
}

std::string CompressManager::generateStandartOutputFile(std::string inputFilePath) {
    int pointIdx = inputFilePath.find(".");
    std::string result = inputFilePath.insert(pointIdx-1,"_output");
    fclose(fopen(result.c_str(), "wb"));
    return result;
}

files CompressManager::prepareAlgorithmData(std::string inputFilePath,
                                            std::string outputFilePath,
                                            std::string mode,
                                            std::string alg){
    files result;
    result._in = getFile(inputFilePath);
    result._in = getFile(inputFilePath);
    result.algorithm = Huffmans;
    result.mode = mode == "Encode" ? ENCODE : DECODE;
    return result;
}
files CompressManager::prepareAlgorithmData(std::string inputFilePath,
                                            std::string mode,
                                            std::string alg) {
    return prepareAlgorithmData(inputFilePath,
                                generateStandartOutputFile(inputFilePath),
                                mode, alg);
}

void CompressManager::compressFile(files target){
    (*_algorithms[target.algorithm])(target);
}
