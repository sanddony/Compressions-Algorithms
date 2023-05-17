#include "MessageModel.h"


void MessageModel::setMaximumEntropy(double newMaximumEntropy)
{
    _maximumEntropy = newMaximumEntropy;
}


void MessageModel::setFirstOrderEntropy(double newFirstOrderEntropy)
{
    _firstOrderEntropy = newFirstOrderEntropy;
}

void MessageModel::setEnsemble(const std::vector<std::pair<std::string, double>> &newEnsemble)
{
    _ensemble = newEnsemble;
}

void MessageModel::setBigramEnsemble(const std::vector<std::pair<std::string, double> > &newBigramEnsemble)
{
    _bigramEnsemble = newBigramEnsemble;

}

void MessageModel::setMessage(const std::string &newMessage)
{
    _message = newMessage;
    Message msg = Message(_message);

    setEnsemble(Converter::ToVectorOfPairStringDouble(msg.Ensemble));
    setBigramEnsemble(Converter::ToVectorOfPairStringDouble(msg.bigramEnsemble));

    setUnconditionalEntropy(msg.calculateUnconditionalEntropy(*amount));
    setMaximumEntropy(msg.calculateMaximumEntropy(*amount));
    setFirstOrderEntropy(msg.calculateFirstOrderEntropy(*amount));
}


void MessageModel::setUnconditionalEntropy(double newUnconditionalEntropy)
{
    _unconditionalEntropy = newUnconditionalEntropy;
}

double MessageModel::maximumEntropy() const
{
    return _maximumEntropy;
}

double MessageModel::firstOrderEntropy() const
{
    return _firstOrderEntropy;
}

std::string MessageModel::message() const
{
    return _message;
}

double MessageModel::unconditionalEntropy() const
{
    return _unconditionalEntropy;
}

std::vector<std::pair<std::string, double>> MessageModel::ensemble() const
{
    return _ensemble;
}

std::vector<std::pair<std::string, double> > MessageModel::bigramEnsemble() const
{
    return _bigramEnsemble;
}

