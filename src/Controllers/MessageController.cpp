#include "MessageController.h"


MessageController::MessageController():
    _model(new MessageModel()),
    m_ensemble(new EnsembleController({})),
    m_bigramEnsemble(new EnsembleController({}))
{

}

// TO-DO fix recall this prop(model already setted, but this
// method reset it threir own values)
void MessageController::setProperties(QString message)
{
    setMessage(message);
    setBigramEnsemble(_model->bigramEnsemble());
    setEnsemble(_model->ensemble());
    setUnconditionalEntropy(_model->unconditionalEntropy());
    setMaximumEntropy(_model->maximumEntropy());
    setFirstOrderEntropy(_model->firstOrderEntropy());
}

void MessageController::setMessage(QString value)
{
    _model->setMessage(value.toStdString());
    emit messageChanged();
}

void MessageController::setBigramEnsemble(std::vector<std::pair<std::string, double>> value)
{
    m_bigramEnsemble->setEnsemble(Converter::ToVectorOfPairQStringQreal(value));
    emit ensembleChanged();
}

void MessageController::registerMe(const string &moduleName)
{
    qmlRegisterType<MessageController>(moduleName.c_str(), 1, 0, "MessageController");
}

void MessageController::setEnsemble(std::vector<std::pair<std::string, double>> value)
{
    m_ensemble->setEnsemble(Converter::ToVectorOfPairQStringQreal(value));
    emit bigramEnsembleChanged();
}


void MessageController::setUnconditionalEntropy(double value)
{
    _model->setUnconditionalEntropy(static_cast<qreal>(value));
    emit unconditionalEntropyChanged();
}

void MessageController::setMaximumEntropy(double value)
{
    _model->setMaximumEntropy(static_cast<qreal>(value));
    emit maximumEntropyChanged();
}


void MessageController::setFirstOrderEntropy(double value)
{
    _model->setFirstOrderEntropy(static_cast<qreal>(value));
    emit firstOrderEntropyChanged();
}


QString MessageController::message() const
{
    return QString::fromStdString(_model->message());
}

qreal MessageController::unconditionalEntropy() const
{
    return static_cast<qreal>(_model->unconditionalEntropy());
}

qreal MessageController::maximumEntropy() const
{
    return static_cast<qreal>(_model->maximumEntropy());
}

qreal MessageController::firstOrderEntropy() const
{
    return static_cast<qreal>(_model->firstOrderEntropy());
}

EnsembleController *MessageController::ensemble() const
{
    return m_ensemble;
}

EnsembleController *MessageController::bigramEnsemble() const
{
    return m_bigramEnsemble;
}

