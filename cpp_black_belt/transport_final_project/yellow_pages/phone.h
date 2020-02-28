#pragma once
#include <string>

namespace YP {
    class Phone {
    public:
        enum class Type {
            PHONE,
            FAX
        };
        Phone() = default;
    private:
        Type type;
        std::string formatted;
        std::string country_code;
        std::string local_code;
        std::string number;
        std::string extension;
        std::string description;
    };
}