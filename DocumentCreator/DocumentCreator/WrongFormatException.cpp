#include "WrongFormatException.h"

WrongFormatException::WrongFormatException(const std::string& message) :
	DocumentException(message)
{
}
