#pragma once
#include <vector>
#include <string>
#include <map>
#include <unordered_map>

#include "company.h"
#include "rubrics.h"

namespace YellowPages {
    using Companies = std::vector<Company>;
    using Rubrics = std::unordered_map<size_t,Rubric>;

    class Database {
    public:
        Database() = default;
        Database(Companies companies_, Rubrics rubrics_)
        : companies(std::move(companies_)), rubrics(std::move(rubrics_))
        {}
    private:
        Companies companies;
        Rubrics rubrics;
    };
}