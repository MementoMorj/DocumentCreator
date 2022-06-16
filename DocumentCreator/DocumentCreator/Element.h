#pragma once
#include <iostream>

class Element {
public:
	virtual size_t GetWidth() const = 0;
	virtual size_t GetHeight() const = 0;
	virtual bool IsBlock() const = 0;
	virtual std::string Render() const = 0;

	virtual Element* Copy() const = 0;

	virtual void Read(std::istream& stream) = 0;
};
