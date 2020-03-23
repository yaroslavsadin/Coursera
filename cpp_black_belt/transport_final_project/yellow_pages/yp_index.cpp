#include "yp_index.h"

namespace YP {
    static bool DoesPhoneMatch(const Phone& query, const Phone& object) {
        // const Phone& query_phone = query.phone;
        if (!query.extension.empty() && query.extension != object.extension) {
            return false;
        }
        if (query.type.has_value() && query.type != object.type) {
            return false;
        }
        if (!query.country_code.empty() && query.country_code != object.country_code) {
            return false;
        }
        if (
            (!query.local_code.empty() || !query.country_code.empty())
            && query.local_code != object.local_code
        ) {
            return false;
        }
        return query.number == object.number;
    }

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

            auto& nearby_stops_ = nearby_stops.emplace_back();
            for(const auto& proto_nearby_stop : company.nearby_stops()) {
                nearby_stops_.emplace_back(proto_nearby_stop.name(),proto_nearby_stop.meters());
            }
        }
    }

    std::set<size_t> YellowPagesIndex::Search(const std::vector<RequestItem>& requests) const {
        std::vector<std::set<size_t>> candidates;

        auto f_process_simple = [&candidates](const Index& index, const RequestItem& item) {
            const auto& requests = std::get<std::vector<std::string>>(item.data);
            if(!requests.empty()) {
                auto& candidate = candidates.emplace_back();
                for(const auto& name : requests) {
                    if(index.count(name)) {
                        for(auto idx : index.at(name)) {
                            candidate.insert(idx);
                        }
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
                const auto& requests = std::get<std::vector<Phone>>(item.data);
                if(!requests.empty()) {
                    auto& phone_candidates = candidates.emplace_back();
                    for(const Phone& phone_template : requests) {
                        for(const auto& [company,phones] : company_to_phones) {
                            for(const auto& phone : phones) {
                                if(DoesPhoneMatch(phone_template,phone)) {
                                    phone_candidates.insert(company);
                                    break;
                                }
                            }
                        }
                    }
                }
                break;
            }
        }
        return intersection(candidates.begin(),candidates.end());
    }

    const std::string& YellowPagesIndex::CompanyNameByIdx(size_t idx) const {
        return company_names[idx];
    }
    const auto& YellowPagesIndex::CompanyNearbyStopsByIdx(size_t idx) const {
        return nearby_stops[idx];
    }
}