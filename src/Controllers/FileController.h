#ifndef FILECONTROLLER_H
#define FILECONTROLLER_H

#include <QObject>
#include <QQmlEngine>

class FileController: public QObject{
    Q_OBJECT
    Q_PROPERTY(QString filePath READ filePath WRITE setFilePath NOTIFY filePathChanged)
    Q_PROPERTY(qreal fileSize READ fileSize WRITE setFileSize NOTIFY fileSizeChanged)

public:
    FileController()=default;
    static void registerMe(const std::string& moduleName);

    QString filePath();
    void setFilePath(QString);

    qreal fileSize();
    void setFileSize(qreal);

private:
    QString _filePath;
    qreal _fileSize;

signals:
    void filePathChanged();
    void fileSizeChanged();
};

#endif // FILECONTROLLER_H
