#include <iostream>
#include "database.pb.h"
#include "yp_index.h"

int main(void) {
    YellowPages::Database db;

    auto& companies = *db.mutable_companies();
    {
        auto* name = companies.Add()->mutable_names()->Add();
        name->set_type(YellowPages::Name_Type::Name_Type_MAIN);
        name->set_value("One");
    }
    {
        auto* name = companies.Add()->mutable_names()->Add();
        name->set_type(YellowPages::Name_Type::Name_Type_MAIN);
        name->set_value("Two");
    }
    {
        auto* comp = companies.Add()->mutable_names();

        auto* name = comp->Add();
        name->set_type(YellowPages::Name_Type::Name_Type_MAIN);
        name->set_value("Three");

        auto* name1 = comp->Add();
        name1->set_type(YellowPages::Name_Type::Name_Type_SYNONYM);
        name1->set_value("kek");
    }
    {
        auto* company = companies.Add();
        
        auto* names = company->mutable_names();
        auto* phones = company->mutable_phones();

        auto* phone = phones->Add();
        phone->set_type(YellowPages::Phone_Type::Phone_Type_PHONE);
        phone->set_country_code("7");
        // phone->set_local_code("812");
        phone->set_number("3633659");

        auto* name = names->Add();
        name->set_type(YellowPages::Name_Type::Name_Type_MAIN);
        name->set_value("Four");

        auto* name1 = names->Add();
        name1->set_type(YellowPages::Name_Type::Name_Type_SYNONYM);
        name1->set_value("lol");
    }
    
    YP::YellowPagesIndex index(db);
    std::vector<YP::RequestItem> items;
    items.push_back(
        YP::RequestItem{
            YP::RequestItem::Type::NAMES,
            std::vector<std::string>{
                "lol", "kek", "rofl"
            }
        }
    );
    items.push_back(
        YP::RequestItem{
            YP::RequestItem::Type::PHONES,
            std::vector<YP::PhoneTemplate>{
                YP::PhoneTemplate{"PHONE"}.SetNumber("3633659")
            }
        }
    );
    for(const auto& found : index.Search(items)) {
        std::cout << found << ' ';
    }
    std::vector<size_t> test;
    auto it = std::inserter(test,test.end());
    *it = 0;
    *it = 1;
    return 0;
}