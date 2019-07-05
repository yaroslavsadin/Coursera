#pragma once

#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include <unordered_set>

namespace Ini {

using Section = std::unordered_map<std::string,std::string>;

class Document
{
public:
    Document() : sections(0) {}
    size_t SectionCount(void) const;
    const Section& GetSection(const std::string& name) const;
    Section& AddSection(const std::string& name);
private:
    std::unordered_map<std::string,Section> sections;
};

Document Load(std::istream&);

}