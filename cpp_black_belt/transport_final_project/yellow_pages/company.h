#pragma once
#include <optional>

#include "address.h"
#include "name.h"
#include "phone.h"
#include "working_time.h"

namespace YP {
    struct NearbyStop {
        std::string name;
        uint32_t meters;
    };

    class Company {
    public:
        Company(const std::string& main_name)
        : names( {Name{ main_name , Name::Type::MAIN }} ) 
        {}
        Address& AddAddress();
        void AddName(const std::string& name, Name::Type type);
        Phone& AddPhone();
        void AddUrl(const std::string& url);
        void AddRubric(size_t rubric);
        template<typename T,ENABLE_IF_TYPE_IS(T,std::vector<size_t>)>
        void SetRubrics(T&& rubrics_) {
            rubrics = std::forward<T>(rubrics_);
        }
        WorkingTime& AddWorkingTime();
        void AddNearbyStop(const std::string& name, uint32_t meters);
    private:
        std::optional<Address> address;
        std::vector<Name> names;
        std::vector<Phone> phones;
        std::vector<std::string> urls;
        std::vector<size_t> rubrics;
        std::optional<WorkingTime> working_time;
        std::vector<NearbyStop> nearby_stops;
    };
}