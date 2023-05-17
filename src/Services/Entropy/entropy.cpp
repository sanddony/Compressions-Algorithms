#include "entropy.h"

#define UTF_ADDITION_BYTE byte{0b10000000}

#define U_0080_07FF_BYTE byte{0b11000000} 
#define U_0800_FFFF_BYTE byte{0b11100000}
#define U_10000_10FFFF_BYTE byte{0b11110000}


vector<byte> stringToByteVector(const string& str){
    vector<byte> result;
    for (int i = 0; i < str.size(); i++)
    {
       result.push_back(byte(str[i]));
    }
    return result;
}

vector<vector<byte>> setUnicodeMessage(const vector<byte>& msg) {
    vector<vector<byte>> result;
    for (int i = 0; i < msg.size(); i++)
    {
        vector<byte> elem;
        if((msg[i] & U_0080_07FF_BYTE) == U_0080_07FF_BYTE) {

            for (int j = 0; j < 2; ++j, ++i)
            {
                elem.push_back(msg[i]);
            }
            i--;
        } else if((msg[i] & U_0800_FFFF_BYTE) == U_0800_FFFF_BYTE) {
            for (int j = 0; j < 3; ++j, ++i)
            {
                elem.push_back(msg[i]);
            }
            i--;
        } else if((msg[i] & U_10000_10FFFF_BYTE) == U_10000_10FFFF_BYTE) {
            for (int j = 0; j < 4; ++j, ++i)
            {
                elem.push_back(msg[i]); 
            }
            i--;
        } else{
            elem.push_back(msg[i]);
        }
            result.push_back(elem);
        elem.clear();
    }
    return result;
}

//except if don't use UTF_ADDITION_BYTE 
Message::Message(const vector<byte>& msg): _message(setUnicodeMessage(msg)), _messageSize(_message.size()){
    defineEnsemble();
    definebigramEnsemble();
};


Message::Message(const string& msg): Message(stringToByteVector(msg)){}

Message::Message(const char msg[]): Message(stringToByteVector(string(msg))){}


int Message::size(){
    return _messageSize;
}


void Message::defineEnsemble(){
    for (int i = 0; i < _messageSize; i++)
    {
        Ensemble[_message[i]]++;
    }

    for (auto it = Ensemble.begin(); it != Ensemble.end(); it++)
    {
        it->second /= _messageSize;
    }
}


void Message::definebigramEnsemble(){
    for (auto firstElem: Ensemble)
    {
        for (auto secondElem: Ensemble)
        {
            bigramEnsemble[array<vector<byte>,2>{firstElem.first,secondElem.first}];
        }
    }

    for (int i = 0; i < _messageSize-1; i++)
    {
        bigramEnsemble[array<vector<byte>,2>{_message[i],_message[i+1]}]++;
    }

    for (auto it = bigramEnsemble.begin(); it != bigramEnsemble.end(); it++)
    {
        it->second /= (_messageSize-1);
    }

}


double Message::calculateUnconditionalEntropy(amountBy& logBy){
    //H(X)=-sum(1->m)P(x_i)*log_2 P(x_i)
    double result = 0;
    for (auto it = Ensemble.begin(); it != Ensemble.end(); it++){
        result+=(it->second * logBy(it->second));
    }
    return -result;
}


double Message::calculateMaximumEntropy(amountBy& logBy){
    //H_max = log_2 m
    return logBy(Ensemble.size());
}


double Message::calculatefUnderloadedAlphabet(amountBy& logBy){
    // H_max - H(x)
    return calculateMaximumEntropy(logBy) - calculateUnconditionalEntropy(logBy);
}


double Message::calculateAmountOfOwnInformationy(const vector<byte>& symb, amountBy& logBy){
    return -logBy(Ensemble[symb]);
}


double Message::calculateAmountOfOwnInformationy(amountBy& logBy){
    double result = 0;
    for (auto symb: Ensemble)
    {
        result += calculateAmountOfOwnInformationy(symb.first, logBy);
    }
    return result;
}


double Message::calculateFirstOrderEntropy(amountBy& logBy){
    double result = 0;
    auto it = bigramEnsemble.begin();
    for (auto& symb: Ensemble)
    {
        double tmp_sum = 0;
        for (; it != bigramEnsemble.end() && it->first[0] == symb.first; ++it) {
            tmp_sum += (it->second == 0) ? 0 : (it->second * logBy(it->second));
        }
        result += symb.second * tmp_sum;
    }
    
    return -result == -0 ? 0 : -result;
}


vector<vector<byte>>& Message::getMsgData(){
    return this->_message;
}

//int main(){
//    Message m = Message("фоторгаф фотал фотографию");
//    byBits b = byBits();
//    std:: cout << m.calculateUnconditionalEntropy(b) << std::endl;

//    std:: cout << m.calculateMaximumEntropy(b) << std::endl;
//    vector<byte> v;
//    v.push_back(byte{32});
//    std:: cout << m.calculateAmountOfOwnInformationy(v,b) << std::endl;


//}
