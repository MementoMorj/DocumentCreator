#pragma once
#include <string>

class DocumentException {
private:
	std::string message_;

public:
	DocumentException(const std::string& message);

	const std::string& GetMessage() const;
};

