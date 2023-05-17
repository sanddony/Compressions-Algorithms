#include "GaloisField.h"



int main(){
    std::string inputPolynomStr;
    std::string generatingPolynomialStr;
    while(true)
    {
        std::cin >> inputPolynomStr;
        std::cin >> generatingPolynomialStr;

        GaloisField g;

        auto x = g.findReminder<10,10>(std::bitset<10>(inputPolynomStr),std::bitset<10>(generatingPolynomialStr));
        //    int x = g.getIndexOfLastBit<4>(std::bitset<4>("1011"));
        watch(x);
    }
}
