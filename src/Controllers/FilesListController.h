#ifndef FILESLISTCONTROLLER_H
#define FILESLISTCONTROLLER_H

#include"FileController.h"
#include <QObject>
#include <QAbstractListModel>
#include <QQmlEngine>

class FilesListController: public QAbstractListModel{

    Q_OBJECT

public:
    FilesListController() = default;
    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex& parent = {}) const override;
    QVariant data(const QModelIndex& index = {}, int role = Qt::DisplayRole) const override;


    static void registerMe(const std::string& moduleName);

    QList<FileController*> filesList() const;
    void setEnsemble(const QList<FileController*> newFilesList);

    void insertElement(int position, FileController* element);
    void insertElements(int position, QList<FileController*> elements);

public slots:
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

private:
    QList<FileController*> _filesList;

    enum FilesList{
        FilePathRole = Qt::UserRole + 1,
        FileSizeRole
    };
};

#endif // FILESLISTCONTROLLER_H
