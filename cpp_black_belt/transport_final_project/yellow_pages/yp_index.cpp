#include "yp_index.h"

namespace YP {
    YellowPagesIndex::YellowPagesIndex(const YellowPages::Database& proto_db) 
    {
        std::unordered_map<size_t,std::string> rubrics_;
        for(const auto& pair__ : proto_db.rubrics()) {
            // Ignoring keywords
            rubrics_[pair__.first] = pair__.second.name();
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
                names[name.value()].insert(main_idx);
            }
            // ------- Index urls;
            for(const auto& name : company.urls()) {
                urls[name.value()].insert(main_idx);
            }
            // ------- Index rubsrics;
            for(size_t id : company.rubrics()) {
                rubrics[rubrics_.at(id)].insert(main_idx);
            }
            
            // ------- Index phone
            for(const auto& phone : company.phones()) {
                if(phone.type() == YellowPages::Phone_Type::Phone_Type_PHONE) {
                    phone_index[phone.number()].companies.insert(main_idx);
                    phone_index[phone.number()].local_to_companies[phone.local_code()].insert(main_idx);
                    phone_index[phone.number()].country_to_companies[phone.country_code()].insert(main_idx);
                    phone_index[phone.number()].extension_to_companies[phone.extension()].insert(main_idx);
                } else {
                    fax_index[phone.number()].companies.insert(main_idx);
                    fax_index[phone.number()].local_to_companies[phone.local_code()].insert(main_idx);
                    fax_index[phone.number()].country_to_companies[phone.country_code()].insert(main_idx);
                    fax_index[phone.number()].extension_to_companies[phone.extension()].insert(main_idx);
                }
            }
        }
    }

    std::set<size_t> YellowPagesIndex::Search(const std::vector<RequestItem>& requests) const {
        std::unordered_map<RequestItem::Type,std::set<size_t>> candidates;

        auto f_process_simple = [&candidates](const Index& index, const RequestItem& item) {
            for(const auto& name : std::get<std::vector<std::string>>(item.data)) {
                candidates[item.type];
                if(index.count(name)) {
                    for(auto idx : index.at(name)) {
                        candidates[item.type].insert(idx);
                    }
                }
            }
        };

        for(const RequestItem& item : requests) {
            switch(item.type) {
            case RequestItem::Type::NAMES:
                f_process_simple(names,item);
                break;
            case RequestItem::Type::URLS:
                f_process_simple(urls,item);
                break;
            case RequestItem::Type::RUBRICS:
                f_process_simple(rubrics,item);
                break;
            default: // PHONES
                std::vector<std::set<size_t>> phone_candidates;

                auto f_search_subindex = [&phone_candidates]
                (const auto& index, const auto& criterion) {
                    auto& s = phone_candidates.emplace_back();
                    if(index.count(criterion)) {
                        for(const auto& company : index.at(criterion)) {
                            s.insert(company);
                        }
                    }
                };

                auto f_search_index = [&phone_candidates,&f_search_subindex]
                (const auto& index, const auto& phone_template) {
                    const auto& phone_name = (phone_template.HasNumber()) ? phone_template.GetNumber() : "";
                    if(!index.count(phone_name)) return;

                    const auto& phone_data = index.at(phone_name);

                    auto& s = phone_candidates.emplace_back();
                    for(const auto& company : phone_data.companies) {
                        s.insert(company);
                    }
                    if(phone_template.HasCountryCode()) {
                        f_search_subindex(phone_data.country_to_companies, phone_template.GetCountryCode());
                    } 
                    if(phone_template.HasLocalCode() || phone_template.HasCountryCode()) {
                        f_search_subindex(phone_data.local_to_companies, 
                            (phone_template.HasLocalCode()) ? phone_template.GetLocalCode() : ""
                        );
                    }
                    if(phone_template.HasExtension()) {
                        f_search_subindex(phone_data.extension_to_companies, phone_template.GetExtension());
                    }
                };
                    
                for(const PhoneTemplate& phone_template : std::get<std::vector<PhoneTemplate>>(item.data)) {
                    if(!phone_template.HasType() || phone_template.GetType() == PhoneTemplate::Type::PHONE) {
                        f_search_index(phone_index,phone_template);
                    }
                    if(!phone_template.HasType() || phone_template.GetType() == PhoneTemplate::Type::FAX) {
                        f_search_index(fax_index,phone_template);
                    }
                    
                    candidates[item.type]; // Create entry for type in case intersection returns none
                    for(auto idx : intersection(phone_candidates.begin(),phone_candidates.end())) {
                        candidates[item.type].insert(idx);
                    }
                    phone_candidates.clear();
                }
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

    const std::string& YellowPagesIndex::CompanyNameByIdx(size_t idx) const {
        return company_names[idx];
    }

    PhoneTemplate& PhoneTemplate::SetType(std::string s) {
        if(s == "PHONE") {
            type = Type::PHONE;
        } else {
            type = Type::FAX;
        }
        return *this;
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
        extension = std::move(s);
        return *this;
    }
    const std::string& PhoneTemplate::GetExtension() const {
        return *extension;
    }
}