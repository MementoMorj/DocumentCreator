#include "Block.h"
#include "Converter.h"
#include "String.h"
#include "WrongFormatException.h"
#include <iterator>
#include <algorithm>
#include <numeric>

#define TOP 0
#define RIGHT 1
#define BOTTOM 2
#define LEFT 3
#define SIZE 0

void Block::FillDefaultAttributeValues()
{
    attributes_ = {
        {"align", {"left"}},
        {"border", {"0", " "}},
        {"margin", {"0", "0", "0", "0"}},
        {"padding", {"0", "0", "0", "0"}},
        {"mask", {""}},
        {"width", {"0"}},
    };
}

Block::Block()
{
    FillDefaultAttributeValues();
}

size_t Block::GetWidth() const
{
    std::vector<size_t> margins(4);
    {
        std::vector<std::string> string_margins(attributes_.at("margin"));
        for (size_t side = 0; side < 4; side++)
        {
            margins[side] = Converter::StringToUInt64(string_margins[side]);
        }
    }
    std::vector<size_t> paddings(4);
    {
        std::vector<std::string> string_paddings(attributes_.at("padding"));
        for (size_t side = 0; side < 4; side++)
        {
            paddings[side] = Converter::StringToUInt64(string_paddings[side]);
        }
    }
    size_t border_size = Converter::StringToUInt64(attributes_.at("border")[SIZE]);
    size_t max_width = std::max(
        Converter::StringToUInt64(attributes_.at("width")[0]),
        std::accumulate(elements_.begin(), elements_.end(), 0ull,
        [](const size_t size, const Element* element)
        {
            return std::max(size, element->GetWidth());
        }));
    size_t result = border_size * 2 + max_width + margins[LEFT] + margins[RIGHT];
    if (!Empty())
    {
        result += paddings[LEFT] + paddings[RIGHT];
    }
    return result;
}

size_t Block::GetHeight() const
{
    std::vector<size_t> margins(4);
    {
        std::vector<std::string> string_margins(attributes_.at("margin"));
        for (size_t side = 0; side < 4; side++)
        {
            margins[side] = Converter::StringToUInt64(string_margins[side]);
        }
    }
    std::vector<size_t> paddings(4);
    {
        std::vector<std::string> string_paddings(attributes_.at("padding"));
        for (size_t side = 0; side < 4; side++)
        {
            paddings[side] = Converter::StringToUInt64(string_paddings[side]);
        }
    }
    size_t border_size = Converter::StringToUInt64(attributes_.at("border")[SIZE]);
    size_t total_height = std::accumulate(elements_.begin(), elements_.end(), 0ull,
        [&paddings](const size_t size, const Element* element)
        {
            return size + element->GetHeight();
        });
    return border_size * 2 + total_height + margins[TOP] + margins[BOTTOM] + (!Empty() ? paddings[TOP] + paddings[BOTTOM] : 0);
}

bool Block::IsBlock() const
{
    return true;
}

Block* Block::Copy() const
{
    Block* block_copy = new Block();
    block_copy->attributes_ = attributes_;
    std::vector<Element*> elements_copy(elements_.size());
    for (size_t i = 0; i < elements_.size(); i++) {
        elements_copy[i] = elements_[i]->Copy();
    }
    block_copy->elements_ = elements_copy;
    return block_copy;
}

void Block::Read(std::istream& stream)
{
    std::string current_chunk;
    while (true)
    {
        getline(stream, current_chunk);
        if (current_chunk == "}")
        {
            break;
        }
        if (current_chunk[0] == '#')
        {
            std::stringstream string_stream(current_chunk.substr(1));
            std::string type;
            getline(string_stream, type, ' ');
            auto type_iterator = attributes_.find(type);
            if (type_iterator == attributes_.end())
            {
                throw WrongFormatException(std::string("unknown type of attribute: \"") + type + "\"");
            }
            std::vector<std::string> parameters;
            while (getline(string_stream, current_chunk, ' '))
            {
                parameters.push_back(current_chunk);
            }
            std::vector<std::string> old_parameters = type_iterator->second;
            if (parameters.size() != old_parameters.size())
            {
                throw WrongFormatException(std::string("wrong count of parameters: expected ") + std::to_string(old_parameters.size()) + ", got " + std::to_string(parameters.size()));
            }
            attributes_[type] = parameters;
            continue;
        }
        Element* element;
        do
        {
            if (current_chunk == "{")
            {
                element = new Block();
                element->Read(stream);
                break;
            }
            if (current_chunk[0] == '$')
            {
                element = new String();
                std::stringstream string_stream(current_chunk.substr(1));
                element->Read(string_stream);
                break;
            }
            throw WrongFormatException(std::string("unknown type of element \"") + current_chunk + "\"");
        } while (true);
        elements_.push_back(element);
    }
}

size_t Block::Count() const
{
    return elements_.size();
}

bool Block::Empty() const
{
    return (Count() == 0);
}

void Block::Remove(size_t index) {
    
}

std::string Block::Render() const
{
    size_t width = GetWidth();
    size_t height = GetHeight();
    std::vector<size_t> margins(4);
    {
        std::vector<std::string> string_margins(attributes_.at("margin"));
        for (size_t side = 0; side < 4; side++)
        {
            margins[side] = Converter::StringToUInt64(string_margins[side]);
        }
    }
    std::vector<size_t> paddings(4);
    {
        std::vector<std::string> string_paddings(attributes_.at("padding"));
        for (size_t side = 0; side < 4; side++)
        {
            paddings[side] = Converter::StringToUInt64(string_paddings[side]);
        }
    }
    size_t border_size;
    char border_symbol;
    {
        std::vector<std::string> border(attributes_.at("border"));
        border_size = Converter::StringToUInt64(border[SIZE]);
        border_symbol = border[1][0];
    }
    std::vector<std::string> lines(height, std::string(width, ' '));
    //Draw border
    //Draw top and bottom border
    for (size_t i = 0; i < border_size; i++)
    {
        for (size_t j = margins[LEFT]; j < width - margins[RIGHT]; j++)
        {
            lines[i + margins[TOP]][j] = border_symbol;
            lines[height - 1 - i - margins[BOTTOM]][j] = border_symbol;
        }
    }
    //Draw left and right border
    for (size_t j = 0; j < border_size; j++)
    {
        for (size_t i = margins[TOP]; i < height - margins[BOTTOM]; i++)
        {
            lines[i][j + margins[LEFT]] = border_symbol;
            lines[i][width - 1 - j - margins[RIGHT]] = border_symbol;
        }
    }
    //Drawing elements in block
    const size_t kELementDistance = margins[TOP] + paddings[TOP];
    size_t current_x = margins[LEFT] + paddings[LEFT] + border_size;
    size_t current_y = kELementDistance + border_size;
    Block* last_block;
    for (size_t k = 0; k < elements_.size(); k++)
    {
        const size_t kInitialCurrentX = current_x;
        const Element* element = elements_[k];
        std::string element_render = element->Render();
        if (!element->IsBlock())
        {
            std::string align = attributes_.at("align")[0];
            size_t left_string_padding = 0;
            size_t current_width = width - border_size * 2 - paddings[LEFT] - paddings[RIGHT] - margins[LEFT] - margins[RIGHT];
            size_t element_width = element->GetWidth();
            if (align == "left")
            {
                goto RENDER_ELEMENT;
            }
            if (align == "right")
            {
                left_string_padding = current_width - element_width;
                goto RENDER_ELEMENT;
            }
            if (align == "center")
            {
                left_string_padding = (current_width - element_width + 1) / 2;
                goto RENDER_ELEMENT;
            }
            throw WrongFormatException("unknown parameter value of align");
        RENDER_ELEMENT:
            current_x += left_string_padding;
            std::string mask = attributes_.at("mask")[0];
            if (mask != "")
            {
                for (const char& symbol : element_render)
                {
                    lines[current_y][current_x] = mask[0];
                    current_x++;
                }
                goto UPDATE_CURRENT_POSITION;
            }
            for (const char& symbol : element_render)
            {
                lines[current_y][current_x] = symbol;
                current_x++;
            }
        UPDATE_CURRENT_POSITION:
            current_x = kInitialCurrentX;
            current_y++;
            continue;
        }
        for (const char& symbol : element_render)
        {
            if (symbol == '\n')
            {
                current_y += 1;
                current_x = kInitialCurrentX;
                continue;
            }
            lines[current_y][current_x] = symbol;
            current_x++;
        }
    }
    return std::accumulate(lines.begin(), lines.end(), std::string(),
        [](const std::string joined_string, const std::string line)
        {
            return joined_string + line + "\n";
        }
    );
}

const std::vector<std::string>& Block::GetParameters(const std::string& type) const
{
    return attributes_.at(type);
}

#undef TOP
#undef RIGHT
#undef BOTTOM
#undef LEFT
#undef SIZE
