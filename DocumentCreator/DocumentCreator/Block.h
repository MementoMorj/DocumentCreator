#pragma once
#include "Element.h"
#include <string>
#include <vector>
#include <map>

class Block : public Element {
private:
	std::map<std::string, std::vector<std::string>> attributes_;
	std::vector<Element*> elements_;

	void FillDefaultAttributeValues();
public:
	Block();

	size_t GetWidth() const override;
	size_t GetHeight() const override;
	bool IsBlock() const override;
	Block* Copy() const override;
	void Read(std::istream& stream) override;

	size_t Count() const;
	bool Empty() const;

	void Remove(size_t index);

	std::string Render() const override;

	const std::vector<std::string>& GetParameters(const std::string& type) const;
};

