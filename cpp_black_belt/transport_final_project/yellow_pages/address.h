#pragma once

#include <vector>
#include <string>

namespace YellowPages {
    struct Coords {
        double lon;
        double lat;
    };

    struct AddressComponent {
        enum class Type {
            COUNTRY,
            REGION,
            CITY,
            STREET,
            HOUSE
        };
        std::string name;
        Type type;
    };

    class Address {
    public:
    private:
        std::string formatted; // ?
        std::vector<AddressComponent> components;
        Coords coords;
        std::string comment; // ?
    };
}