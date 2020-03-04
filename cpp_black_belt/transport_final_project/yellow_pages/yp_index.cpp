#include "yp_index.h"

namespace YP {
    std::set<std::string_view> intersection (const std::vector<std::set<std::string_view>> &vecs) {
        if(vecs.empty()) {
            return std::set<std::string_view>{};
        }

        std::set<std::string_view> last_intersection = vecs[0];
        std::set<std::string_view> curr_intersection;

        for (std::size_t i = 1; i < vecs.size(); ++i) {
            std::set_intersection(last_intersection.begin(), last_intersection.end(),
                vecs[i].begin(), vecs[i].end(),
                std::inserter(curr_intersection, curr_intersection.end()));
            std::swap(last_intersection, curr_intersection);
            curr_intersection.clear();
        }
        return last_intersection;
    }

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
                names[name.value()].insert(main_name);
            }
            // ------- Index urls;
            for(const auto& name : company.urls()) {
                urls[name.value()].insert(main_name);
            }
            // ------- Index rubsrics;
            for(size_t id : company.rubrics()) {
                rubrics[rubrics_.at(id)].insert(main_name);
            }
            
            // ------- Index phone
            for(const auto& phone : company.phones()) {
                phone_country_code[phone.type()][phone.country_code()].insert(main_name);
                phone_local_code[phone.type()][phone.local_code()].insert(main_name);
                phone_number[phone.type()][phone.number()].insert(main_name);
                phone_extension[phone.type()][phone.extension()].insert(main_name);
            }
        }
    }

    std::set<std::string_view> YellowPagesIndex::Search(const std::vector<Item>& requests) {
        std::vector<std::set<std::string_view>> candidates;
        for(const Item& item : requests) {
            switch(item.type) {
            case Item::Type::NAMES:
                for(const auto& name : std::get<std::vector<std::string>>(item.data)) {
                    if(names.count(name)) {
                        candidates.push_back(names.at(name));
                    }
                }
                break;
            case Item::Type::URLS:
                for(const auto& url : std::get<std::vector<std::string>>(item.data)) {
                    if(urls.count(url)) {
                        candidates.push_back(urls.at(url));
                    }
                }
                break;
            case Item::Type::RUBRICS:
                for(const auto& rubric : std::get<std::vector<std::string>>(item.data)) {
                    if(rubrics.count(rubric)) {
                        candidates.push_back(rubrics.at(rubric));
                    }
                }
                break;
            default: // PHONES
                for(const PhoneTemplate& phone : std::get<std::vector<PhoneTemplate>>(item.data)) {
                    size_t type_idx = (phone.GetType() == "PHONE") ? 0 : 1;
                    std::vector<std::set<std::string_view>> phone_candidates;
                    if(phone.HasCountryCode()) {
                        phone_candidates.push_back(phone_country_code[type_idx].at(phone.GetCountryCode()));
                    }
                    if(phone.HasLocalCode()) {
                        phone_candidates.push_back(phone_local_code[type_idx].at(phone.GetLocalCode()));
                    }
                    if(phone.HasNumber()) {
                        phone_candidates.push_back(phone_number[type_idx].at(phone.GetNumber()));
                    }
                    if(phone.HasExtension()) {
                        phone_candidates.push_back(phone_extension[type_idx].at(phone.GetExtension()));
                    }
                    candidates.push_back(intersection(phone_candidates));
                }
                break;
            }
        }
        return intersection(candidates);
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