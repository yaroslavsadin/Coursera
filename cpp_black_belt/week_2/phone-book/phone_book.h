#pragma once

#include "iterator_range.h"

#include <optional>
#include <string>
#include <string_view>
#include <vector>
#include <iosfwd>

struct Date {
  uint32_t year, month, day;
};

struct Contact {
  std::string name;
  std::optional<Date> birthday;
  std::vector<std::string> phones;
};

class PhoneBook {
public:
  using ContactRange = IteratorRange<std::vector<Contact>::const_iterator>;
  explicit PhoneBook(std::vector<Contact> contacts) 
  : contacts_(move(contacts))
  {
    std::sort(contacts_.begin(),contacts_.end(),[](const Contact& lhs, const Contact& rhs){
      return lhs.name < rhs.name;
    });
  }

  ContactRange FindByNamePrefix(std::string_view name_prefix) const {
    Contact dummy;
    dummy.name = name_prefix;
    auto range = std::equal_range(contacts_.begin(),contacts_.end(), dummy,
    [name_prefix](const Contact& contact1, const Contact& contact2){
      std::string_view cropped1(
        contact1.name.c_str(),
        (contact1.name.size() < name_prefix.size()) ? contact1.name.size() : name_prefix.size()
      );
      std::string_view cropped2(
        contact2.name.c_str(),
        (contact2.name.size() < name_prefix.size()) ? contact2.name.size() : name_prefix.size()
      );
      return cropped1 < cropped2;
    });
    return {range.first,range.second};
  }

  void SaveTo(std::ostream& output) const;
private:
  std::vector<Contact> contacts_;
};

PhoneBook DeserializePhoneBook(std::istream& input);

