#include "EnsembleController.h"
#include <QDebug>
#include <iostream>

EnsembleController::EnsembleController(std::vector<std::pair<QString, qreal>> ensemble):
    m_ensemble(ensemble)
{
}

QHash<int, QByteArray> EnsembleController::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[EnsembleController::SymbolRole] = "symbol";
    roles[EnsembleController::FrequrncyRole] = "frequency";

    return roles;
}

int EnsembleController::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return static_cast<int>(m_ensemble.size());
}

QVariant EnsembleController::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || index.row() > rowCount(index)){
        return {};
    }

    const std::pair<QString, qreal> symbEntity = m_ensemble.at(index.row());

    switch (role) {
        case EnsembleController::SymbolRole:
            return QVariant::fromValue(symbEntity.first);
        case EnsembleController::FrequrncyRole:
            return QVariant::fromValue(symbEntity.second);
        default:
            return true;
    }

}

void EnsembleController::registerMe(const std::string &moduleName)
{
    qmlRegisterType<EnsembleController>(moduleName.c_str(), 1, 0, "EnsembleModel");
}

std::vector<std::pair<QString, qreal>> EnsembleController::ensemble() const
{
    return m_ensemble;
}

void EnsembleController::setEnsemble(const std::vector<std::pair<QString, qreal>> newEnsemble)
{

    m_ensemble.clear();
    beginResetModel();
    endResetModel();

    insertElements(0, newEnsemble);

    emit dataChanged(createIndex(0,0), createIndex(m_ensemble.size(),0));
}

void EnsembleController::insertElement(int position, std::pair<QString, qreal> value)
{
    beginInsertRows(QModelIndex(),rowCount(),rowCount());
    m_ensemble.push_back(value);
    endInsertRows();
}

void EnsembleController::insertElements(int position, std::vector<std::pair<QString, qreal>> elements)
{
    for (std::pair<QString, qreal> elem: elements) {
        insertElement(position++, elem);
    }
}


