#include "DocumentException.h"

DocumentException::DocumentException(const std::string& message)
{
    message_ = message;
}

const std::string& DocumentException::GetMessage() const
{
    return message_;
}
