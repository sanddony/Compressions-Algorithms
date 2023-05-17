#ifndef CONVERTER_H
#define CONVERTER_H

#include "FileController.h"
#include <vector>
#include <map>
#include <QString>

class Converter{

public:
    static std::vector<std::pair<QString, qreal>>
        ToVectorOfPairQStringQreal(std::vector<std::pair<std::string, double>>&);

    static std::vector<std::pair<std::string, double>>
        ToVectorOfPairStringDouble(std::map<std::vector<std::byte>, double>);

    static std::vector<std::pair<std::string, double>>
        ToVectorOfPairStringDouble(std::map<std::array<std::vector<std::byte>,2>, double>);

    static std::string toString(std::vector<std::vector<std::byte>> value);
    static std::string toString(std::vector<std::byte> value);

    static std::string toString(std::array<std::vector<std::byte>,2> value);
    static std::vector<std::byte> ToVectorOfByte(int value);

    static std::string toDemonstrativeString(std::vector<std::byte> value);
    static std::string toDemonstrativeString(std::byte value);

    static std::vector<std::string> toVectorOfString(QList<FileController*>);
};

#endif // CONVERTER_H
