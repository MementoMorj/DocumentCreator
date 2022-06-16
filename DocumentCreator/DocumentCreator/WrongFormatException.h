#pragma once
#include "DocumentException.h"

class WrongFormatException :
	public DocumentException
{
public:
	WrongFormatException(const std::string& message);
};

