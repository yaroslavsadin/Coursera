#include "ini.h"

namespace Ini {

size_t Document::SectionCount(void) const {
    return sections.size();
}

const Section& Document::GetSection(const std::string& name) const {
    auto it = sections.find(name);
    if(it != sections.end()) {
        return it->second;
    } else {
        throw std::out_of_range("No such section");
    }
}

Section& Document::AddSection(const std::string& name) {
    if(sections.count(name)) {
        return sections.at(name);
    }
    else {
        return sections[name] = Section();
    }
}

bool IsSection(std::string s) {
    if(s[0] == '[') return true;
    return false;
}

std::string GetSectionName(std::string_view s) {
    s.remove_prefix(1);
    s.remove_suffix(1);
    return std::string(s);
}

std::pair<std::string, std::string> Split(std::string_view line, char by) {
  size_t pos = line.find(by);
  std::string_view left = line.substr(0, pos);

  if (pos < line.size() && pos + 1 < line.size()) {
    return {std::string(left), std::string(line.substr(pos + 1))};
  } else {
    return {std::string(left), std::string()};
  }
}

std::string Lstrip(std::string_view line) {
  while (!line.empty() && isspace(line[0])) {
    line.remove_prefix(1);
  }
  return std::string(line);
}

std::pair<std::string,std::string> GetKeyValue(std::string_view s) {
    return Split(s,'=');
}

Document Load(std::istream& is) {
    Document doc;
    std::string s;
    Section* current_section;
    while(getline(is,s)) {
        s = Lstrip(s);
        if (s.empty()) continue;
        if(IsSection(s)) {
            current_section = &doc.AddSection(GetSectionName(s));
        } else {
            current_section->insert(GetKeyValue(s));
        }
    }
    return doc;
}

}