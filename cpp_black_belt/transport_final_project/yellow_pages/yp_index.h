#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <array>
#include "database.pb.h"

namespace YP {
    struct YellowPagesRequest {
        
    };

    class YellowPagesIndex {
    public:
        YellowPagesIndex(const YellowPages::Database& proto_db);
    private:
        using Index = std::unordered_map<std::string,std::string_view>;

        std::vector<std::string> company_names;

        Index names;
        Index urls;
        Index rurics;

        std::array<Index,2> phone_country_code;
        std::array<Index,2> phone_local_code;
        std::array<Index,2> phone_number;
        std::array<Index,2> phone_extension;
    };
}
