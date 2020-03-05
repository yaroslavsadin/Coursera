#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <array>
#include <variant>
#include <optional>
#include "../../misc.h"
#include "database.pb.h"

namespace YP {
    template<typename It>
    auto intersection (It begin_, It end_) {
        using Container = typename It::value_type;
        if(begin_ == end_) {
            return Container{};
        }

        Container last_intersection = *begin_;
        Container curr_intersection;

        for (auto it = begin_ + 1; it != end_; ++it) {
            std::set_intersection(last_intersection.begin(), last_intersection.end(),
                it->begin(), it->end(),
                std::inserter(curr_intersection, curr_intersection.end()));
            std::swap(last_intersection, curr_intersection);
            curr_intersection.clear();
        }
        return last_intersection;
    }

    class PhoneTemplate {
    public:
        PhoneTemplate(const std::string& type_)
        : type(type_) {}
        const std::string& GetType() const { return type; }
        PhoneTemplate& SetCountryCode(std::string s);
        const std::string& GetCountryCode() const;
        bool HasCountryCode() const { return country_code.has_value(); }
        PhoneTemplate& SetLocalCode(std::string s);
        const std::string& GetLocalCode() const;
        bool HasLocalCode() const { return local_code.has_value(); }
        PhoneTemplate& SetNumber(std::string s);
        const std::string& GetNumber() const;
        bool HasNumber() const { return number.has_value(); }
        PhoneTemplate& SetExtension(std::string s);
        const std::string& GetExtension() const;
        bool HasExtension() const { return extension.has_value(); }
    private:    
        std::string type; // PHONE or FAX
        std::optional<std::string> country_code;
        std::optional<std::string> local_code;
        std::optional<std::string> number;
        std::optional<std::string> extension;
    };

    struct RequestItem {
        enum class Type {
            NAMES, URLS, RUBRICS, PHONES
        };
        Type type;
        std::variant<
            std::vector<std::string>,
            std::vector<PhoneTemplate>
        > data;
    };

    class YellowPagesIndex {
    public:
        YellowPagesIndex(const YellowPages::Database& proto_db);
        std::set<size_t> Search(const std::vector<RequestItem>& requests);
    private:
        using Index = std::unordered_map<std::string,std::vector<size_t>>;

        std::vector<std::string> company_names;

        Index names;
        Index urls;
        Index rubrics;

        // First for PHONE type, second for FAX type
        std::array<Index,2> phone_country_code;
        std::array<Index,2> phone_local_code;
        std::array<Index,2> phone_number;
        std::array<Index,2> phone_extension;
    };
}
