#ifndef SRC_ENTROPY
#define SRC_ENTROPY

#include <iostream>
#include <vector>
#include <list>
#include <array>
#include <map>
#include <cmath>
#include <cstddef>
#include <bitset>

using namespace std;

struct amountBy{
    virtual double operator()(double) = 0;
};

struct byBits: amountBy{
    double operator()(double) override;
};

struct byDits: amountBy{
    double operator()(double) override;
};

struct byNats: amountBy{
    double operator()(double) override;
};

enum encode{
    ANSII,
    UTF_8
};


class Message{
protected:
    vector<vector<byte>> _message;
    int _messageSize = 0;
public:
    Message(const vector<byte>& msg);
    Message(const string& msg);
    Message(const char[]);

    vector<vector<byte>>& getMsgData();

    friend ostream& operator<<(ostream &os, const Message& msg);
    friend istream& operator>>(istream &in, Message& msg);
    
    map<vector<byte>, double> Ensemble;
    map<array<vector<byte>,2>,double> bigramEnsemble;
    void definebigramEnsemble();
    void defineEnsemble();
    int size();

    //абсолютная энтропия
    double calculateUnconditionalEntropy(amountBy&);
    //максимальная энтропия
    double calculateMaximumEntropy(amountBy&);
    //собственное количество информации в конкретном символе
    double calculateAmountOfOwnInformationy(const vector<byte>&, amountBy&);
    //собственное количество информации в сообщении
    double calculateAmountOfOwnInformationy(amountBy&);
    //пережгруженность сообщения
    double calculatefUnderloadedAlphabet(amountBy&);
    //энтропия первого порядка
    double calculateFirstOrderEntropy(amountBy&);
    
    ~Message(){} // дописать
};   

ostream& operator<<(ostream& os, const byte b);

ostream& operator<<(ostream &os, const vector<byte>& msg);

ostream& operator<<(ostream &os, const vector<vector<byte>>& msg);

ostream& operator<<(ostream &os, const pair<vector<byte>,double>& symb);

ostream& operator<<(ostream &os, const map<vector<byte>,double>& ensemble);

ostream& operator<<(ostream &os, const map<array<vector<byte>,2>,double>& ensemble);

ostream& operator<<(ostream &os, const Message& msg);


#endif // SRC_ENTROPY