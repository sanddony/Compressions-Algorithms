#include "entropy.h"

template <typename T>
Message<T>::Message(const vector<T>& msg): _message(msg), _messageSize(_message.size()) {
    defineEnsemble();
    definebigramEnsemble();
};

template <typename T>
Message<T>::Message(const string& msg): _messageSize(msg.size()){
    // _message = *(new vector<byte>);
    for (int i = 0; i < _messageSize; i++)
    {
       _message.push_back(T(msg[i]));
       cout << T(msg[i]) << ":" << msg[i] << endl;
    }
    defineEnsemble();
    definebigramEnsemble();
}

template <typename T>
int Message<T>::size(){
    return _messageSize;
}

template <typename T>
void Message<T>::defineEnsemble(){
    for (int i = 0; i < _messageSize; i++)
    {
        Ensemble[_message[i]]++;
    }

    for (map<byte,double>::iterator it = Ensemble.begin(); it != Ensemble.end(); it++)
    {
        it->second /= _messageSize;
    }
}

template <typename T>
void Message<T>::definebigramEnsemble(){
    for (int i = 0; i < _messageSize-1; i++)
    {
        bigramEnsemble[array<byte,2>{_message[i],_message[i+1]}]++;
    }

    for (map<array<byte,2>,double>::iterator it = bigramEnsemble.begin(); it != bigramEnsemble.end(); it++)
    {
        it->second /= (_messageSize-1);
    }
    
    for (auto firstElem: Ensemble)
    {
        for (auto secondElem: Ensemble)
        {
            bigramEnsemble[array<byte,2>{firstElem.first,secondElem.first}];
        }
    }
    
    
    
    
}

template <typename T>
double Message<T>::calculateUnconditionalEntropy(amountBy& logBy){
    //H(X)=-sum(1->m)P(x_i)*log_2 P(x_i)
    double result = 0;
    for (map<byte,double>::iterator it = Ensemble.begin(); it != Ensemble.end(); it++){
        result+=(it->second * logBy(it->second));
    }
    return -result;
}

template <typename T>
double Message<T>::calculateMaximumEntropy(amountBy& logBy){
    //H_max = log_2 m
    return logBy(Ensemble.size());
}

template <typename T>
double Message<T>::calculatefUnderloadedAlphabet(amountBy& logBy){
    // H_max - H(x)
    return calculateMaximumEntropy(logBy) - calculateUnconditionalEntropy(logBy);
}

template <typename T>
double Message<T>::calculateAmountOfOwnInformationy(const T& symb, amountBy& logBy){
    return -logBy(Ensemble[symb]);
}

template <typename T>
double Message<T>::calculateAmountOfOwnInformationy(amountBy& logBy){
    double result = 0;
    for (auto symb: Ensemble)
    {
        result += calculateAmountOfOwnInformationy(symb.first, logBy);
    }
    return result;
}

template <typename T>
double Message<T>::calculateFirstOrderEntropy(amountBy& logBy){
    double result = 0;
    map<array<byte,2>,double>::iterator it = bigramEnsemble.begin();
    for (auto symb: Ensemble)
    {
        double tmp_sum = 0;
        while (it->first[0] == symb.first)
        {
            tmp_sum += it->second * logBy(it->second);
            it++;
        }
        result -= symb.second * tmp_sum;
    }
    
    return result;
}

template <typename T>
vector<T>& Message<T>::getMsgData(){
    return this->_message;
}


int main(){
    //1/2 = 3/6
    //1/3 = 2/6
    //1/6
    //
    // vector<byte> x;
    // x.push_back(byte{0b11110001});
    // x.push_back(byte{0b11110001});
    // x.push_back(byte{0b11110100});
    // x.push_back(byte{0b11110001});
    // x.push_back(byte{0b11110100});
    // x.push_back(byte{0b11110111});
    // // cout << x;                                                                            
    Message<string> msg("123");
    cout << msg;
    // byBits bb;
    // byDits bd;
    // byNats bn;
    //ааааааааббб
    // cout << msg.calculateUnconditionalEntropy(bd) << endl;
    // cout << msg.Ensemble << endl;
    // cout << msg.bigramEnsemble << endl;
    setlocale(LC_ALL,". 1251");
    string t = "р";
    cout << t << endl;
    // cout << msg << endl;
    // cout << msg.Ensemble[static_cast<byte>(3)] << endl;
    // cout << msg.getMsgData() << endl;



}