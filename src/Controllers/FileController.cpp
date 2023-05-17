#include "FileController.h"

void FileController::registerMe(const std::string &moduleName)
{
    qmlRegisterType<FileController>(moduleName.c_str(), 1, 0, "FileController");
}

QString FileController::filePath()
{
    return _filePath;
}

void FileController::setFilePath(QString value)
{
    _filePath = value;
}

qreal FileController::fileSize()
{
    return _fileSize;
}

void FileController::setFileSize(qreal value)
{
    _fileSize = value;
}
