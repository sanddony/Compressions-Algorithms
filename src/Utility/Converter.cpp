#include "Converter.h"
#define INT_BYTE_COUNT 4

std::vector<std::pair<QString, qreal>>
    Converter::ToVectorOfPairQStringQreal(std::vector<std::pair<std::string, double>>& value)
{
    std::vector<std::pair<QString, qreal>> result;
    for (auto& _pair: value) {
        result.push_back(std::make_pair(QString::fromStdString((_pair.first)),
                                        static_cast<qreal>(_pair.second)));
    }
    return result;
}

std::vector<std::pair<std::string, double> >
    Converter::ToVectorOfPairStringDouble(std::map<std::vector<std::byte>, double> value)
{
    std::vector<std::pair<std::string, double>> result;
    for (auto _pair: value) {
        result.push_back(std::make_pair(Converter::toString(_pair.first),_pair.second));
    }
    return result;
}

std::vector<std::pair<std::string, double> >
    Converter::ToVectorOfPairStringDouble(std::map<std::array<std::vector<std::byte>, 2>, double> value)
{
    std::vector<std::pair<std::string, double>> result;
    for (auto _pair: value) {
        result.push_back(std::make_pair(Converter::toString(_pair.first),_pair.second));
    }
    return result;
}



std::string Converter::toString(std::vector<std::vector<std::byte> > value)
{
    std::string result;
    for (auto& vec: value)
    {
        result+=toString(vec);
    }
    return result;
}

std::string Converter::toString(std::vector<std::byte> value)
{
    std::string result;

    for (auto unicode_byte: value)
    {
        result.push_back(static_cast<char>(unicode_byte));
    }

    return result;
}

std::string Converter::toDemonstrativeString(std::byte value){
    std::string result = "[";
    std::byte mask{0b10000000};
    for (int i = 0; i < 8; ++i)
    {
        result += ((mask & value) != std::byte{0}) + 48;
        mask >>=1;
    }
    result+=']';
    return result;

}

std::vector<std::string> Converter::toVectorOfString(QList<FileController *> value)
{
    std::vector<std::string> result;
    for (auto& files: value) {
        result.push_back(files->filePath().toStdString());
    }
    return result;
}

std::string Converter::toDemonstrativeString(std::vector<std::byte> value){
    std::string result;
    for(auto& byte_: value){
        result+=toDemonstrativeString(byte_);
    }
    return result;
}

std::string Converter::toString(std::array<std::vector<std::byte>, 2> value)
{
    std::string result;

    for (auto& pair_unicode_byte: value){
        result += Converter::toString(pair_unicode_byte);
    }

    return result;
}

std::vector<std::byte> Converter::ToVectorOfByte(int value)
{
    int mask = 0xFF000000;
    std::vector<std::byte> result;
    for (int idx = 0; idx < INT_BYTE_COUNT; ++idx) {
        result.push_back(std::byte{value & mask});
        mask >>=8;
    }
    return result;
}

