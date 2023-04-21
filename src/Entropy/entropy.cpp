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
    for (auto symb: Ensemble)
    {
        double tmp_sum = 0;
        while (it->first[0] == symb.first)
        {
            tmp_sum += (it->second == 0) ? 0 : (it->second * logBy(it->second));
            it++;
        }
        result += symb.second * tmp_sum;
    }
    
    return -result;
}


vector<vector<byte>>& Message::getMsgData(){
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
    // setlocale(LC_ALL, "");                                                                            
    // Message<wchar_t> msg("блять");
    // cout << 'a';
    // wcout.imbue(locale("rus_rus.866"));
    // wcout << msg.getMsgData()[0];
    // wcout << msg.getMsgData()[1];
    // wcout << msg.getMsgData()[2];
    byBits bb;
    byDits bd;
    byNats bn;

    Message msg = "аа_а_б";
    cout << msg.getMsgData() << endl;
    cout << msg.Ensemble << endl ;
    cout << msg.bigramEnsemble << endl;
    cout << "Энтропия первого порядка: " << msg.calculateFirstOrderEntropy(bb) << endl;
    cout << "Абсолютная энтропия: " << msg.calculateUnconditionalEntropy(bb) << endl;
    cout << "Максимальная энтропия: " << msg.calculateMaximumEntropy(bb) << endl;
    cout << "Количество информации в сообщении: " << msg.calculateAmountOfOwnInformationy(bb) << endl;
    cout << "Пережгруженность сообщения: " << msg.calculatefUnderloadedAlphabet(bb) << endl;
    // cout << d << endl;
    // setUnicodeMessage
    // cout << (int)x[0] << (int)x[1] << (int)x[2] << endl;

    // string u = "blyat";
    // cout << (int)u[0] << ' ' << (int)u[1] << ' ' << (int)u[2] << endl;

    //ааааааааббб
    // cout << msg.calculateUnconditionalEntropy(bd) << endl;
    // cout << msg.Ensemble << endl;
    // cout << msg.bigramEnsemble << endl;
    // setlocale(LC_ALL,". 1251");
    // wchar_t t = 'е';
    // cout << t << endl;
    // cout << msg << endl;
    // cout << msg.Ensemble[static_cast<byte>(3)] << endl;
    // cout << msg.getMsgData() << endl;
    // byte tmp{0b10100000};
    // cout << ((tmp & U_0080_07FF_BYTE) == U_0080_07FF_BYTE)  << endl;



}