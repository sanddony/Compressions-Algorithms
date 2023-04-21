#include "entropy.h"

// ostream& operator<<(ostream& os, const byte b) {
//     return os << "["<< bitset<8>(to_integer<int>(b)) << "]";
// }

ostream& operator<<(ostream& os, const byte b) {
    return os << (char)b;
}


// istream& operator>>(istream &in, vector<byte>& msg){ // didn't work
//     while (!in.eof())//0000110110
//     {
//         msg.push_back(static_cast<byte>(in.get()));
//     }
//     return in;
// }

ostream& operator<<(ostream &os, const vector<byte>& msg){
    for (int i = 0; i < msg.size(); i++)
    {
        os  << msg[i];
    }
    return os;
}


ostream& operator<<(ostream &os, const vector<vector<byte>>& msg){
    for (int i = 0; i < msg.size(); i++)
    {
        os  << msg[i] << " ";
    }
    return os;
}


ostream& operator<<(ostream &os, const pair<array<vector<byte>,2>,double>& symb){
    return os << "[" << symb.first[0] << " " << symb.first[1] << "]=" << symb.second << endl;
}


ostream& operator<<(ostream &os, const map<array<vector<byte>,2>,double>& ensemble) {
    for(auto elem: ensemble) {
        os << elem;
    }
    return os;
}

ostream& operator<<(ostream& os, const pair<vector<byte>, double>& p) {
    return os << p.first << "= " << p.second  << endl;
}


ostream& operator<<(ostream &os, const map<vector<byte>,double>& ensemble){
    for(auto elem: ensemble) {
        os << elem;
    }
    return os;
}


ostream& operator<<(ostream &os, const Message& msg){
    return os << msg._message << endl;
}

