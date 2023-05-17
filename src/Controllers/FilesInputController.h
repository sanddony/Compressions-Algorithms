#ifndef FILESINPUTCONTROLLER_H
#define FILESINPUTCONTROLLER_H

#include <QAbstractListModel>
#include <QQmlEngine>
#include "FilesListController.h"

#include "../Models/FileInputModel.h"
#include "../Utility/Converter.h"

class FilesInputController: public QObject{

    Q_OBJECT

    Q_PROPERTY(QString folderPath
               READ folderPath
               WRITE setFolderPath
               NOTIFY folderPathChanged)

    Q_PROPERTY(FilesListController* filesList
               READ filesList
               WRITE setFilesList
               NOTIFY filesListChanged)

    Q_PROPERTY(QString currentAlgorithm
                READ currentAlgorithm
                WRITE setCurrentAlgorithm
                NOTIFY currentAlgorithmChanged)

    Q_PROPERTY(QString compressMode
                READ compressMode
                WRITE setCompressMode
                NOTIFY compressModeChanged)



public:
    FilesInputController()=default;
    static void registerMe(const std::string& moduleName);

    Q_INVOKABLE void compressFiles();

    QString folderPath();
    FilesListController* filesList();
    QString currentAlgorithm();
    QString compressMode();

public slots:
    void setCompressMode(QString);
    void setFolderPath(QString);
    void setCurrentAlgorithm(QString);
    void setFilesList(FilesListController*);


private:
    FilesListController* _filesList;
    FileInputModel* _model;


signals:
    void folderPathChanged();
    void filesListChanged();
    void currentAlgorithmChanged();
    void compressModeChanged();


};

#endif // FILESINPUTCONTROLLER_H
