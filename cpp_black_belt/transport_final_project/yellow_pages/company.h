#pragma once
#include <optional>

#include "address.h"
#include "name.h"
#include "phone.h"
#include "working_time.h"

namespace YellowPages {
    struct NearbyStop {
        std::string name;
        uint32_t meters;
    };

    class Company {
    public:
        Company(std::string main_name)
        : names( {Name{ main_name , YellowPages::Name::Type::MAIN }} ) 
        {}
        void AddRubric(size_t rubric) {
            rubrics.push_back(rubric);
        }
    private:
        std::optional<Address> address;
        std::vector<Name> names;
        std::vector<Phone> phones;
        std::vector<std::string> urls;
        std::vector<size_t> rubrics;
        WorkingTime working_time;
        std::vector<NearbyStop> nearby_stops;
    };
}