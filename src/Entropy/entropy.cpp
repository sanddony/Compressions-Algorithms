#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <cstddef>
#include <bitset>

using namespace std;

struct sortByUnicode{

};

struct sortByFrequency{

};


class Message{
private:
    vector<byte>& _message;
    int _messageSize;
public:
    Message(vector<byte>& msg): _message(msg), _messageSize(_message.size()){
        defineEnsemble();
    };
    ~Message(){};

    friend ostream& operator<<(ostream &os, const Message& msg);
    friend istream& operator>>(istream &in, Message& msg);

    map<byte, double> Ensemble;

    void defineEnsemble();

    vector<byte>& getMsgText();
    double calculateUnconditionalEntropy();
    int size();
};  



istream& operator>>(istream &in, vector<byte>& msg){ // didn't work
    while (!in.eof())//0000110110
    {
        msg.push_back(static_cast<byte>(in.get()));
    }
    return in;
}


std::ostream& operator<<(std::ostream& os, std::byte b) {
    return os << "["<< std::bitset<8>(std::to_integer<int>(b)) << "]";
}

ostream& operator<<(ostream &os, const vector<byte>& msg){
    for (int i = 0; i < msg.size(); i++)
    {
        os  << msg[i] << " " << endl;
    }
    return os;
}

ostream& operator<<(ostream &os, const pair<byte,double>& symb){
    return os << "[" << symb.first << "]=" << symb.second << endl;
}

ostream& operator<<(ostream &os, const map<byte,double>& ensemble){
    for(auto elem: ensemble) {
        os << elem;
    }
    return os;
}

ostream& operator<<(ostream &os, const Message& msg){
    return os << msg._message << endl;
}



int Message::size(){
    return _messageSize;
}

void Message::defineEnsemble(){
    for (int i = 0; i < _message.size(); i++)
    {
        Ensemble[_message[i]]++;
    }

    for (map<byte,double>::iterator it = Ensemble.begin(); it != Ensemble.end(); it++)
    {
        it->second /= _messageSize;
    }
}

double Message::calculateUnconditionalEntropy(){
    //-sum(1->m)P(x_i)*log_2 P(x_i)
    double result = 0;
    for (map<byte,double>::iterator it = Ensemble.begin(); it != Ensemble.end(); it++){
        result+=(it->second * log2(it->second));
    }
    return -result;
}

vector<byte>& Message::getMsgText(){
    return this->_message;
}


int main(){
    vector<byte> x;
    x.push_back(byte{0b11110001});
    x.push_back(byte{0b11110001});
    x.push_back(byte{0b11110100});
    x.push_back(byte{0b11110001});
    x.push_back(byte{0b11110100});
    x.push_back(byte{0b11110111});
    cout << x;
    Message msg(x);
    cout << msg.calculateUnconditionalEntropy() << endl;
    // cout << msg.Ensemble << endl;
    // cout << msg.Ensemble << endl;
    // cout << msg.Ensemble[static_cast<byte>(3)] << endl;
    // cout << msg.getMsgText() << endl;



}