#include "Document.h"
#include "WrongFormatException.h"

Document::Document()
{

}

Document::Document(const Document& document)
{
	this->root_ = *document.root_.Copy();
}

Document::Document(std::istream& stream)
{
	std::string chunk;
	getline(stream, chunk);
	if (chunk != "{")
	{
		throw WrongFormatException("every document must have root block");
	}
	root_.Read(stream);
}

Document& Document::operator=(const Document& document) {
	root_ = *document.root_.Copy();
	return *this;
}

bool Document::Empty() const {
	return root_.Empty();
}

size_t Document::Count() const {
	return root_.Count();
}

std::string Document::Render() const
{
	return root_.Render();
}
