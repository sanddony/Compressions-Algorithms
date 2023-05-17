#ifndef MESSAGECONTROLLER_H
#define MESSAGECONTROLLER_H

#include <QObject>
#include <QQmlEngine>
#include <vector>
#include "EnsembleController.h"
#include "../Models/MessageModel.h"
#include "../Utility/Converter.h"


class MessageController: public QObject{

   Q_OBJECT

   Q_PROPERTY(QString message
              READ message
              NOTIFY messageChanged)
   Q_PROPERTY(qreal unconditionalEntropy
                  READ unconditionalEntropy
                  NOTIFY unconditionalEntropyChanged)
   Q_PROPERTY(qreal maximumEntropy
                  READ maximumEntropy
                  NOTIFY maximumEntropyChanged)
   Q_PROPERTY(qreal firstOrderEntropy
                  READ firstOrderEntropy
                  NOTIFY firstOrderEntropyChanged)

   Q_PROPERTY(EnsembleController* ensemble
                  READ ensemble
                  NOTIFY ensembleChanged)
   Q_PROPERTY(EnsembleController* bigramEnsemble
                  READ bigramEnsemble
                  NOTIFY bigramEnsembleChanged)

public:
    MessageController();

    Q_INVOKABLE void setProperties(QString message);

    QString message() const;
    void setMessage(QString);

    qreal unconditionalEntropy() const;
    void setUnconditionalEntropy(double);

    qreal maximumEntropy() const;
    void setMaximumEntropy(double);

    qreal firstOrderEntropy() const;
    void setFirstOrderEntropy(double);

    EnsembleController* ensemble() const;
    void setEnsemble(std::vector<std::pair<std::string, double>>);
    EnsembleController* bigramEnsemble() const;
    void setBigramEnsemble(std::vector<std::pair<std::string, double>>);

    static void registerMe(const std::string &moduleName);

signals:

    void messageChanged();
    void unconditionalEntropyChanged();
    void maximumEntropyChanged();
    void firstOrderEntropyChanged();
    void ensembleChanged();
    void bigramEnsembleChanged();

private:

    EnsembleController* m_ensemble;
    EnsembleController* m_bigramEnsemble;
    MessageModel* _model;
};


#endif // MESSAGECONTROLLER_H
