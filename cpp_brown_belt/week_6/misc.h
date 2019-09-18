#pragma once

#include <sstream>
#include <string>
#include <utility>
#include <optional>
#include <iostream>

template<typename It>
class Range {
public:
  Range(It begin, It end) : begin_(begin), end_(end) {}
  It begin() const { return begin_; }
  It end() const { return end_; }
  size_t size() { return std::distance(begin_,end_); }

private:
  It begin_;
  It end_;
};

std::pair<std::string_view, std::optional<std::string_view>> SplitTwoStrict(std::string_view s, std::string_view delimiter = " ");
std::pair<std::string_view, std::string_view> SplitTwo(std::string_view s, std::string_view delimiter = " ");
std::string_view ReadToken(std::string_view& s, std::string_view delimiter = " ");

template<typename ObjType>
std::optional<ObjType> GetObjectFromStream(std::istream& is = std::cin) {
    ObjType var;
    is >> var;
    if(is) 
      return var;
    else 
      return std::nullopt;    
}

void ToNextLine(std::istream& is = std::cin);
double StringToDouble(std::string_view str);