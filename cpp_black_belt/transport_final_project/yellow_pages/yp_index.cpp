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
            for(const auto& proto_phone : company.phones()) {
                auto& phone = company_to_phones[main_idx].emplace_back();
                phone.type = (proto_phone.type() == YellowPages::Phone_Type::Phone_Type_PHONE) ? 
                                            Phone::Type::PHONE : Phone::Type::FAX;
                phone.number = proto_phone.number();
                phone.extension = proto_phone.extension();
                phone.local_code = proto_phone.local_code();
                phone.country_code = proto_phone.country_code();
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
                auto& phone_candidates = candidates[item.type];

                for(const Phone& phone_template : std::get<std::vector<Phone>>(item.data)) {
                    for(const auto& [company,phones] : company_to_phones) {
                        for(const auto& phone : phones) {
                            if(DoesPhoneMatch(phone_template,phone)) {
                                phone_candidates.insert(company);
                                break;
                            }
                        }
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

    const std::string& YellowPagesIndex::CompanyNameByIdx(size_t idx) const {
        return company_names[idx];
    }
}