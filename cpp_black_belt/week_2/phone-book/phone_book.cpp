#include "contact.pb.h"
#include "phone_book.h"
using namespace std;

void PhoneBook::SaveTo(std::ostream& output) const {
  PhoneBookSerialize::ContactList list;
  for(const auto& item : contacts_) {
    auto& contact = *list.add_contact();
    contact.set_name(item.name);
    if(item.birthday) {
      PhoneBookSerialize::Date date;
      date.set_day(item.birthday->day);
      date.set_month(item.birthday->month);
      date.set_year(item.birthday->year);
      *contact.mutable_birthday() = date;
    }
    for(const auto& phone : item.phones) {
      contact.add_phone_number(phone);
    }
  }
  list.SerializeToOstream(&output);
}

PhoneBook DeserializePhoneBook(std::istream& input) {
  PhoneBookSerialize::ContactList list;
  list.ParseFromIstream(&input);
  vector<Contact> data;
  data.reserve(list.contact_size());
  for(size_t i = 0; i < list.contact_size(); i++) {
    Contact contact;
    contact.name = list.contact(i).name();
    if(list.contact(i).has_birthday()) {
      contact.birthday = {
        list.contact(i).birthday().year(),
        list.contact(i).birthday().month(),
        list.contact(i).birthday().day()
      };
    }
    vector<string> phones;
    phones.reserve(list.contact(i).phone_number_size());
    for(size_t j = 0; j < list.contact(i).phone_number_size(); j++) {
      phones.push_back(list.contact(i).phone_number(j));
    }
    contact.phones = move(phones);
    data.push_back(contact);
  }
  return PhoneBook(move(data));
}