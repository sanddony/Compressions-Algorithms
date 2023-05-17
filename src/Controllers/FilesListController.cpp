#include "FilesListController.h"
#include <iostream>

QHash<int, QByteArray> FilesListController::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[FilesListController::FilePathRole] = "filePath";
    roles[FilesListController::FileSizeRole] = "fileSize";

    return roles;
}

int FilesListController::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return static_cast<int>(_filesList .size());
}

QVariant FilesListController::data(const QModelIndex &index, int role) const
{
    std::cout << "ASDASDASDASD" << std::endl;
    if(!index.isValid() || index.row() > rowCount(index)){
        return {};
    }

    FileController* file = _filesList.at(index.row());

    switch (role) {
    case FilesListController::FilePathRole:
        return QVariant::fromValue(file->filePath());
    case FilesListController::FileSizeRole:
        return QVariant::fromValue(file->fileSize());
    default:
        return true;
    }
}

bool FilesListController::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!hasIndex(index.row(), index.column(), index.parent()) || !value.isValid())
        return false;

    FileController* file = _filesList[index.row()];
    if (role == FilesListController::FilePathRole) file->setFilePath(value.toString());
    else if (role == FilesListController::FileSizeRole) file->setFileSize(value.toDouble());
    else return false;

    emit dataChanged(index, index, { role } );

    return true ;
}

void FilesListController::registerMe(const std::string &moduleName)
{
    qmlRegisterType<FilesListController>(moduleName.c_str(), 1, 0, "FilesListController");
}

QList<FileController *> FilesListController::filesList() const
{
    return _filesList;
}

void FilesListController::setEnsemble(const QList<FileController *> newFilesList)
{
    _filesList.clear();
    emit beginResetModel();
    emit endResetModel();

    insertElements(0, newFilesList);

    emit dataChanged(createIndex(0,0), createIndex(newFilesList.size(),0));
}

void FilesListController::insertElement(int position, FileController *element)
{
    beginInsertRows(QModelIndex(),rowCount(),rowCount());
    _filesList.push_back(element);
    endInsertRows();
}

void FilesListController::insertElements(int position, QList<FileController *> elements)
{
    for (FileController* elem: elements) {
        insertElement(position++, elem);
    }
}
