#include "entropy.h"


double byBits::operator()(double x){
    return log2(x);
}

double byDits::operator()(double x){
    return log10(x);
}

double byNats::operator()(double x){
    return log(x);
}