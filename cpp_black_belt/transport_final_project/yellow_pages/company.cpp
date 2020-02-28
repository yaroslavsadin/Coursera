#include "company.h"

namespace YP {
    Address& Company::AddAddress() {
        return address.emplace();
    }
    void Company::AddName(const std::string& name, Name::Type type) {
        names.push_back({name,type});
    }
    Phone& Company::AddPhone() {
        return phones.emplace_back();
    }
    void Company::AddUrl(const std::string& url) {
        urls.emplace_back(url);
    }
    void Company::AddRubric(size_t rubric) {
        rubrics.push_back(rubric);
    }
    WorkingTime& Company::AddWorkingTime() {
        return working_time.emplace();
    }
    void Company::AddNearbyStop(const std::string& name, uint32_t meters) {
        nearby_stops.push_back({name,meters});
    }
}