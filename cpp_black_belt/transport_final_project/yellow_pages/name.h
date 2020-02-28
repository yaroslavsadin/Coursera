#pragma once
#include <string>

namespace YP {
    struct Name {
        enum class Type {
            MAIN,
            SYNONYM,
            SHORT
        };
        std::string value;
        Type type;
    };
}