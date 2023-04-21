#include "entropy.h"

ostream& operator<<(ostream& os, const wchar_t b) {
    // return os << "["<< b << "]";
    return os << "["<< b << "]";
}

ostream& operator<<(ostream& os, const byte b) {
    return os << "["<< std::bitset<8>(std::to_integer<int>(b)) << "]";
}

istream& operator>>(istream &in, vector<byte>& msg){ // didn't work
    while (!in.eof())//0000110110
    {
        msg.push_back(static_cast<byte>(in.get()));
    }
    return in;
}


template <typename T>
ostream& operator<<(ostream &os, const vector<T>& msg){
    for (int i = 0; i < msg.size(); i++)
    {
        os  << msg[i] << " " << endl;
    }
    return os;
}

template <typename T>
ostream& operator<<(ostream &os, const pair<T,double>& symb){
    return os << "[" << symb.first << "]=" << symb.second << endl;
}

template <typename T>
ostream& operator<<(ostream &os, const pair<array<T,2>,double>& symb){
    return os << "[" << symb.first[0] << " " << symb.first[1] << "]=" << symb.second << endl;
}

template <typename T>
ostream& operator<<(ostream &os, const map<array<T,2>,double>& ensemble) {
    for(auto elem: ensemble) {
        os << elem;
    }
    return os;
}

template <typename T>
ostream& operator<<(ostream &os, const map<T,double>& ensemble){
    for(auto elem: ensemble) {
        os << elem;
    }
    return os;
}

template <typename T>
ostream& operator<<(ostream &os, const Message<T>& msg){
    return os << msg._message << endl;
}


ostream& operator<<(ostream &os, const Message<wchar_t>& msg){
    return os << msg._message << endl;
}