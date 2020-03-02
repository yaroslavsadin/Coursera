#include "yp_index.h"

namespace YP {
    YellowPagesIndex::YellowPagesIndex(const YellowPages::Database& proto_db) 
    : company_names(proto_db.companies_size())
    {
        std::unordered_map<size_t,std::string> rubrics_;
        for(const auto& [id,rubric] : proto_db.rubrics()) {
            // Ignoring keywords
            rubrics_[id] = rubric.name();
        }
        for(const auto& company : proto_db.companies()) {
            // Assuming at least one name
            const auto& proto_company_names = company.names();
            // Каждое описание компании — строка с основным названием компании, 
            // где под основным названием понимается value первого YellowPages.Name 
            // компании с type = MAIN.
            company_names.push_back(
                std::find_if(
                proto_company_names.begin(), proto_company_names.end(),
                    [](const auto& elem){
                        return elem.type() == YellowPages::Name_Type::Name_Type_MAIN;
                    }
                )->value()
            );
            std::string_view main_name = company_names.back();

            // ------- Index names;
            for(const auto& name : proto_company_names) {
                names[name.value()] = main_name;
            }
            // ------- Index urls;
            for(const auto& name : company.urls()) {
                urls[name.value()] = main_name;
            }
            // ------- Index rubsrics;
            for(size_t id : company.rubrics()) {
                rurics[rubrics_.at(id)] = main_name;
            }
            
            // ------- Index phone
            for(const auto& phone : company.phones()) {
                phone_country_code[phone.type()][phone.country_code()] = main_name;
                phone_local_code[phone.type()][phone.local_code()] = main_name;
                phone_number[phone.type()][phone.number()] = main_name;
                phone_extension[phone.type()][phone.extension()] = main_name;
            }
        }
    }
}