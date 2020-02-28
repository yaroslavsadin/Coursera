#include "address.h"

namespace YP {
    Address& Address::AddComponent(const std::string& name, AddressComponent::Type type) {
        components.emplace_back(AddressComponent{name,type});
        return *this;
    }
    Address& Address::SetCoords(Coords coords_) {
        coords = coords_;
        return *this;
    }
}