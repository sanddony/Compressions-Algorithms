#include "entropy.h"

istream& operator>>(istream &in, vector<byte>& msg){ // didn't work
    while (!in.eof())//0000110110
    {
        msg.push_back(static_cast<byte>(in.get()));
    }
    return in;
}

ostream& operator<<(ostream& os, const byte b) {
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

ostream& operator<<(ostream &os, const pair<array<byte,2>,double>& symb){
    return os << "[" << symb.first[0] << " " << symb.first[1] << "]=" << symb.second << endl;
}

ostream& operator<<(ostream &os, const map<array<byte,2>,double>& ensemble) {
    for(auto elem: ensemble) {
        os << elem;
    }
    return os;
}

ostream& operator<<(ostream &os, const map<byte,double>& ensemble){
    for(auto elem: ensemble) {
        os << elem;
    }
    return os;
}

template <typename T>
ostream& operator<<(ostream &os, const Message<T>& msg){
    return os << msg._message << endl;
}


