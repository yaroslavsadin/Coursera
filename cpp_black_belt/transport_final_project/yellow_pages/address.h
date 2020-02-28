#pragma once

#include <vector>
#include <string>
#include "misc.h"

namespace YP {
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
        Address() = default;
        Address& AddComponent(const std::string& name, AddressComponent::Type type);
        template<typename T,ENABLE_IF_TYPE_IS(T,std::vector<AddressComponent>)>
        void SetComponents(T&& components_) {
            components = std::forward<T>(components_);
        }
        Address& SetCoords(Coords coords);
    private:
        std::string formatted; // ?
        std::vector<AddressComponent> components;
        Coords coords;
        std::string comment; // ?
    };
}