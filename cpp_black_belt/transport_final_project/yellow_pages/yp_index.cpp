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

    std::set<size_t> YellowPagesIndex::Search(const std::vector<RequestItem>& requests) {
        std::unordered_map<RequestItem::Type,std::set<size_t>> candidates;

        auto f_process_simple = [&candidates](const Index& index, const RequestItem& item) {
            for(const auto& name : std::get<std::vector<std::string>>(item.data)) {
                candidates[item.type]; // create an entry for the Type anyway
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
                for(const PhoneTemplate& phone_template : std::get<std::vector<PhoneTemplate>>(item.data)) {
                    std::vector<std::set<size_t>> phone_candidates;
                    auto array_idx = (phone_template.GetType() == "PHONE") ? 0 : 1;
                    
                    auto f_process_phone_elem = [this,&phone_candidates]
                        (const std::string& criterion, const Index& index) {
                        if(index.count(criterion)) {
                            auto& s = phone_candidates.emplace_back();
                            for(auto idx : index.at(criterion)) {
                                s.insert(idx);
                            }
                        }
                    };

                    if(phone_template.HasExtension()) {
                        f_process_phone_elem(phone_template.GetExtension(), phone_extension[array_idx]);
                    }
                    if(phone_template.HasCountryCode()) {
                        // If has country code it must match
                        f_process_phone_elem(phone_template.GetCountryCode(), phone_country_code[array_idx]);
                    }
                    if(phone_template.HasLocalCode() || phone_template.HasCountryCode()) {
                        // If has country code OR has local code, the last must match
                        f_process_phone_elem(phone_template.GetLocalCode(), phone_local_code[array_idx]);
                    }
                    if(phone_template.HasNumber()) {
                        // Has number, look up for it
                        f_process_phone_elem(phone_template.GetNumber(), phone_number[array_idx]);
                    } else {
                        f_process_phone_elem("", phone_number[array_idx]);
                    }
                    // We need to at least create and entry in candidates 
                    // in case none companies were found for the phone template
                    candidates[item.type];
                    for(auto idx : intersection(phone_candidates.begin(),phone_candidates.end())) {
                        candidates[item.type].insert(idx);
                    }
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

    const std::string YellowPagesIndex::CompanyNameByIdx(size_t idx) {
        return company_names[idx];
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