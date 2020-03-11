#pragma once
#include <string>

namespace YP {
    struct Phone {
        enum class Type {
            PHONE,
            FAX
        };
        Type type;
        std::string formatted;
        std::string country_code;
        std::string local_code;
        std::string number;
        std::string extension;
        std::string description;
    };
}