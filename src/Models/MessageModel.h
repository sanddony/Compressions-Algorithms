#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H

//#include <vector>
//#include <map>
//#include <string>
#include "../Services/Services.h"
#include "../Utility/Converter.h"

class MessageModel{

public:
    void calculatePropertiers();

    std::string message() const;
    void setMessage(const std::string &newMessage);

    double unconditionalEntropy() const;
    void setUnconditionalEntropy(double newUnconditionalEntropy);

    double maximumEntropy() const;
    void setMaximumEntropy(double newMaximumEntropy);

    double firstOrderEntropy() const;
    void setFirstOrderEntropy(double newFirstOrderEntropy);


    std::vector<std::pair<std::string, double> > ensemble() const;
    void setEnsemble(const std::vector<std::pair<std::string, double> > &newEnsemble);


    std::vector<std::pair<std::string, double> > bigramEnsemble() const;
    void setBigramEnsemble(const std::vector<std::pair<std::string, double> > &newBigramEnsemble);

private:

    std::string _message = "";
    double _unconditionalEntropy = 0;
    double _maximumEntropy = 0;
    double _firstOrderEntropy = 0;
    std::vector<std::pair<std::string, double>> _ensemble = {};
    std::vector<std::pair<std::string, double>> _bigramEnsemble = {};

    amountBy* amount = new byBits();
};
#endif // MESSAGEMODEL_H
