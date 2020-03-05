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
        auto* comp = companies.Add()->mutable_names();

        auto* name = comp->Add();
        name->set_type(YellowPages::Name_Type::Name_Type_MAIN);
        name->set_value("Four");

        auto* name1 = comp->Add();
        name1->set_type(YellowPages::Name_Type::Name_Type_SYNONYM);
        name1->set_value("lol");
    }
    
    YP::YellowPagesIndex index(db);
    std::vector<YP::Item> items;
    items.push_back(YP::Item{YP::Item::Type::NAMES,std::vector<std::string>{"lol", "kek", "rofl"}});
    for(const auto& found : index.Search(items)) {
        std::cout << found << ' ';
    }
    std::vector<size_t> test;
    auto it = std::inserter(test,test.end());
    *it = 0;
    *it = 1;
    return 0;
}