#ifndef GALOISFIELD_H
#define GALOISFIELD_H
#include <bitset>
#include <iostream>


#define watch(x)  std::cout << (x) << " <-- " << #x <<  std::endl;

class GaloisField
{
public:
    GaloisField()=default;

    template <int N, int M>
    std::bitset<N> findReminder(std::bitset<N> dividend,
                                std::bitset<M> _generatingPolynomial){

        if(getIndexOfLastBit<N>(dividend) < getIndexOfLastBit<M>(_generatingPolynomial))
            return dividend;
        watch(getIndexOfLastBit<N>(dividend));
        watch(getIndexOfLastBit<M>(_generatingPolynomial));
        watch(dividend);
        watch(_generatingPolynomial);
        std::bitset<N> generatingPolynomial(_generatingPolynomial.to_string());
        alignmentGeneratingPolynomial<N>(dividend, generatingPolynomial);

        watch(dividend);
        watch(generatingPolynomial);

        while(getIndexOfLastBit<N>(dividend) >= getIndexOfLastBit<N>(generatingPolynomial) &&
               dividend != 0){
            dividend ^= generatingPolynomial;
            watch(dividend);
            alignmentGeneratingPolynomial<N>(dividend, generatingPolynomial);
            watch(generatingPolynomial);
        }
        watch(dividend);
        watch(generatingPolynomial);
        return dividend;
    }

    template <int N>
    int getIndexOfLastBit(const std::bitset<N>& value){
        static std::bitset<N> mask;
        int result = N;
        mask = 1 << (N-1);

        while(value != 0 && (mask & value) == 0){
            mask >>= 1;
            result--;
        }

        return result-1;
    }

    template<int N>
    void alignmentGeneratingPolynomial(const std::bitset<N>& dividend,
                                       std::bitset<N>& generatingPolynomial){
        int idxOfDividendLastBit = getIndexOfLastBit<N>(dividend);
        int idxOfPolynomLastBit = getIndexOfLastBit<N>(generatingPolynomial);

        int shift = idxOfDividendLastBit - idxOfPolynomLastBit;
        if(shift > 0)generatingPolynomial <<= shift;
        if(shift < 0) generatingPolynomial >>= -shift;
    }
};

#endif // GALOISFIELD_H
