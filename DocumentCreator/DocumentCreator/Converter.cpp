#include "Converter.h"

uint64_t Converter::StringToUInt64(const std::string& value)
{
    std::stringstream stream(value);
    int64_t result;
    stream >> result;
    return result;
}
