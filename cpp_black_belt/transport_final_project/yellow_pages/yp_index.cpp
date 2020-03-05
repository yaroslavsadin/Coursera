#include "yp_index.h"

namespace YP {
    YellowPagesIndex::YellowPagesIndex(const YellowPages::Database& proto_db)
    {
        std::unordered_map<size_t,std::string> rubrics_;
        for(const auto& [id,rubric] : proto_db.rubrics()) {
            // Ignoring keywords
            rubrics_[id] = rubric.name();
        }
        company_names.reserve(proto_db.companies_size());
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
            auto main_idx = company_names.size() - 1;

            // ------- Index names;
            for(const auto& name : proto_company_names) {
                names[name.value()].push_back(main_idx);
            }
            // ------- Index urls;
            for(const auto& name : company.urls()) {
                urls[name.value()].push_back(main_idx);
            }
            // ------- Index rubsrics;
            for(size_t id : company.rubrics()) {
                rubrics[rubrics_.at(id)].push_back(main_idx);
            }
            
            // ------- Index phone
            for(const auto& phone : company.phones()) {
                phone_country_code[phone.type()][phone.country_code()].push_back(main_idx);
                phone_local_code[phone.type()][phone.local_code()].push_back(main_idx);
                phone_number[phone.type()][phone.number()].push_back(main_idx);
                phone_extension[phone.type()][phone.extension()].push_back(main_idx);
            }
        }
    }

    std::set<size_t> YellowPagesIndex::Search(const std::vector<Item>& requests) {
        std::unordered_map<Item::Type,std::set<size_t>> candidates;

        auto f_process_simple = [&candidates](const Index& index, const Item& item) {
            for(const auto& name : std::get<std::vector<std::string>>(item.data)) {
                if(index.count(name)) {
                    for(auto idx : index.at(name)) {
                        candidates[item.type].insert(idx);
                    }
                }
            }
        };

        for(const Item& item : requests) {
            switch(item.type) {
            case Item::Type::NAMES:
                f_process_simple(names,item);
                break;
            case Item::Type::URLS:
                f_process_simple(urls,item);
                break;
            case Item::Type::RUBRICS:
                f_process_simple(rubrics,item);
                break;
            default: // PHONES
                // for(const PhoneTemplate& phone_template : std::get<std::vector<PhoneTemplate>>(item.data)) {
                //     size_t type_idx = (phone_template.GetType() == "PHONE") ? 0 : 1;
                //     std::vector<std::set<size_t>> phone_candidates;
                //     if(phone_template.HasCountryCode()) {
                //         phone_candidates.push_back(phone_country_code[type_idx].at(phone_template.GetCountryCode()));
                //     }
                //     if(phone_template.HasLocalCode()) {
                //         phone_candidates.push_back(phone_local_code[type_idx].at(phone_template.GetLocalCode()));
                //     }
                //     if(phone_template.HasNumber()) {
                //         phone_candidates.push_back(phone_number[type_idx].at(phone_template.GetNumber()));
                //     }
                //     if(phone_template.HasExtension()) {
                //         phone_candidates.push_back(phone_extension[type_idx].at(phone_template.GetExtension()));
                //     }
                //     candidates.push_back(intersection(phone_candidates));
                // }
                break;
            }
        }
        std::vector<std::set<size_t>> candidates_;
        candidates_.reserve(candidates.size());
        for(auto& [k,v] : candidates) {
            candidates_.push_back(std::move(v));
        }
        return intersection(candidates_.begin(),candidates_.end());
    }

    PhoneTemplate& PhoneTemplate::SetCountryCode(std::string s) {
        country_code = std::move(s);
        return *this;
    }
    const std::string& PhoneTemplate::GetCountryCode() const {
        return *country_code;
    }
    PhoneTemplate& PhoneTemplate::SetLocalCode(std::string s) {
        local_code = std::move(s);
        return *this;
    }
    const std::string& PhoneTemplate::GetLocalCode() const {
        return *local_code;
    }
    PhoneTemplate& PhoneTemplate::SetNumber(std::string s) {
        number = std::move(s);
        return *this;
    }
    const std::string& PhoneTemplate::GetNumber() const {
        return *number;
    }
    PhoneTemplate& PhoneTemplate::SetExtension(std::string s) {
        extenstion = std::move(s);
        return *this;
    }
    const std::string& PhoneTemplate::GetExtension() const {
        return *extenstion;
    }
}