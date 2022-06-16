#pragma once
#include "Element.h"
#include <string>

class String : public Element {
private:
	std::string content_;

public:
	size_t GetWidth() const override;
	size_t GetHeight() const override;
	bool IsBlock() const override;
	std::string Render() const override;
	String* Copy() const override;
	void Read(std::istream& stream) override;
};
