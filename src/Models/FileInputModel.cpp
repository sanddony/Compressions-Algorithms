#include "FileInputModel.h"

FileInputModel::FileInputModel():_compressManager(new CompressManager)
{

}

std::string FileInputModel::outputFileDir() const
{
    return _outputFileDir;
}

void FileInputModel::setOutputFileDir(const std::string &newOutputFileDir)
{
    _outputFileDir = newOutputFileDir;
}

std::string FileInputModel::currentAlgorithm() const
{
    return _currentAlgorithm;
}

void FileInputModel::setCurrentAlgorithm(const std::string &newCurrentAlgorithm)
{
    _currentAlgorithm = newCurrentAlgorithm;
}

std::string FileInputModel::compressMode() const
{
    return _compressMode;
}

void FileInputModel::setCompressMode(const std::string &newCompressMode)
{
    _compressMode = newCompressMode;
}

std::vector<std::string> FileInputModel::inputFilesPath() const
{
    return _inputFilesPath;
}

void FileInputModel::setInputFilesPath(const std::vector<std::string> &newInputFilesPath)
{
    _inputFilesPath = newInputFilesPath;
}

void FileInputModel::compressFiles()
{
    for (auto& file : _inputFilesPath) {
        _compressManager->compressFile(CompressManager::prepareAlgorithmData(file,
                                                                             _compressMode,
                                                                             _currentAlgorithm));
    }
}
