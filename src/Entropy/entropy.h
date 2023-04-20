#ifndef SRC_ENTROPY
#define SRC_ENTROPY

#include <iostream>
#include <vector>
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

template <typename T=byte>
class Message{
protected:
    vector<T> _message;
    int _messageSize = 0;
public:
    Message(const vector<T>& msg);
    Message(const string& msg);

    vector<T>& getMsgData();

    friend ostream& operator<<(ostream &os, const Message& msg);
    friend istream& operator>>(istream &in, Message& msg);
    
    map<T, double> Ensemble;
    map<array<T,2>,double> bigramEnsemble;
    void definebigramEnsemble();
    void defineEnsemble();
    int size();

    //абсолютная энтропия
    double calculateUnconditionalEntropy(amountBy&);
    //максимальная энтропия
    double calculateMaximumEntropy(amountBy&);
    //собственное количество информации в конкретном символе
    double calculateAmountOfOwnInformationy(const T&, amountBy&);
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

ostream& operator<<(ostream &os, const pair<byte,double>& symb);

ostream& operator<<(ostream &os, const map<byte,double>& ensemble);

ostream& operator<<(ostream &os, const map<array<byte,2>,double>& ensemble);

template <typename T>
ostream& operator<<(ostream &os, const Message<T>& msg);


#endif // SRC_ENTROPY