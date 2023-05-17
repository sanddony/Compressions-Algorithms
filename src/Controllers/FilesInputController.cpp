#include "FilesInputController.h"
#include <iostream>

void FilesInputController::registerMe(const std::string &moduleName)
{
    qmlRegisterType<FilesInputController>(moduleName.c_str(), 1, 0, "FilesInputController");
}

void FilesInputController::compressFiles()
{
    _model->compressFiles();
}

QString FilesInputController::folderPath()
{
    return QString::fromStdString(_model->outputFileDir());
}

void FilesInputController::setFolderPath(QString newFolder)
{
    if(newFolder.toStdString() == _model->outputFileDir()){
        return;
    }
    _model->setOutputFileDir(newFolder.toStdString());
    std::cout << "setFolderPath " << newFolder.toStdString() << std::endl;
    emit folderPathChanged();
}

FilesListController *FilesInputController::filesList()
{

    return _filesList;
}

void FilesInputController::setFilesList(FilesListController * newFilesList)
{
    _filesList = newFilesList;
    _model->setInputFilesPath(Converter::toVectorOfString(newFilesList->filesList()));
    emit filesListChanged();
}

QString FilesInputController::currentAlgorithm()
{
    return QString::fromStdString(_model->currentAlgorithm());
}

void FilesInputController::setCurrentAlgorithm(QString alg)
{
    _model->setCurrentAlgorithm(alg.toStdString());
    emit currentAlgorithmChanged();
}

QString FilesInputController::compressMode()
{
    return QString::fromStdString(_model->compressMode());
}

void FilesInputController::setCompressMode(QString mode)
{
    _model->setCompressMode(mode.toStdString());
}
