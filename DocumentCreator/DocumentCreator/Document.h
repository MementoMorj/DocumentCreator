#pragma once
#include "Block.h"
#include <iostream>
#include <vector>

class Document
{
private:
	Block root_;

public:
	Document();
	Document(const Document& document);
	explicit Document(std::istream& stream);

	Document& operator=(const Document& document);

	bool Empty() const;
	size_t Count() const;

	std::string Render() const;
};
