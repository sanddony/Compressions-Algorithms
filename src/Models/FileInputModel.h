#ifndef FILEINPUTMODEL_H
#define FILEINPUTMODEL_H


#include "Compression/CompressManager.h"
#include <string>
#include <vector>

class FileInputModel
{
public:
    FileInputModel();

    std::string outputFileDir() const;
    void setOutputFileDir(const std::string &newOutputFileDir);

    std::string currentAlgorithm() const;
    void setCurrentAlgorithm(const std::string &newCurrentAlgorithm);

    std::string compressMode() const;
    void setCompressMode(const std::string &newCompressMode);

    std::vector<std::string> inputFilesPath() const;
    void setInputFilesPath(const std::vector<std::string> &newInputFilesPath);

    void compressFiles();

private:
    std::vector<std::string> _inputFilesPath;
    std::string _outputFileDir;
    std::string _currentAlgorithm;
    std::string _compressMode;
    CompressManager* _compressManager;

};

#endif // FILEINPUTMODEL_H
