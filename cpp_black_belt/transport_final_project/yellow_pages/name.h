#pragma once
#include <string>

namespace YellowPages {
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