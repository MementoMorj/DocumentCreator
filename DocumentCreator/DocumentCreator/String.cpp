#include "String.h"

size_t String::GetWidth() const {
	return content_.size();
}

size_t String::GetHeight() const
{
	return 1ull;
}

bool String::IsBlock() const
{
	return false;
}

std::string String::Render() const
{
	return content_;
}

String* String::Copy() const {
	String* string = new String();
	string->content_ = content_;
	return string;
}

void String::Read(std::istream& stream)
{
	getline(stream, content_);
}
