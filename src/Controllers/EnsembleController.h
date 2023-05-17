#ifndef ENSEMBLECONTROLLER_H
#define ENSEMBLECONTROLLER_H

#include <QAbstractListModel>
#include <QQmlEngine>
#include <vector>


class EnsembleController: public QAbstractListModel {
    Q_OBJECT

public:
    EnsembleController(std::vector<std::pair<QString, qreal>>);

    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex& parent = {}) const override;
    QVariant data(const QModelIndex& index = {}, int role = Qt::DisplayRole) const override;

    static void registerMe(const std::string& moduleName);

    std::vector<std::pair<QString, qreal> > ensemble() const;
    void setEnsemble(const std::vector<std::pair<QString, qreal>> newEnsemble);

    void insertElement(int position, std::pair<QString, qreal>);
    void insertElements(int position, std::vector<std::pair<QString, qreal>> elements);



private:
    std::vector<std::pair<QString, qreal>> m_ensemble;

    enum Ensemble{
        SymbolRole = Qt::UserRole + 1,
        FrequrncyRole
    };
};



#endif // ENSEMBLECONTROLLER_H
