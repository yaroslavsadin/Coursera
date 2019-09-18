#include <iostream>
#include "database.h"
#include <cmath>
#include <variant>

using namespace std;

double toRad(double degree) {
    constexpr double PI = 3.1415926535;
    return degree/180 * PI;
}

double CalcDistance(const Stop& from, const Stop& to) {
    double dist;
    dist = sin(toRad(from.latitude)) * sin(toRad(to.latitude)) + cos(toRad(from.latitude)) 
            * cos(toRad(to.latitude)) * cos(toRad(from.longtitude - to.longtitude));
    dist = acos(dist);
    //got dist in radian, no need to change back to degree and convert to rad again.
    dist = 6371 * 1000 * dist;
    return dist;
}

void BusDatabase::AddStop(std::string_view name, double latitude, double longtitude) {
    stops_[string(name)] = {latitude,longtitude};
}

double BusDatabase::ComputeDistance(const Bus& bus) const {
    if(bus.route.size() < 2) return 0;
    double distance {.0};
    for(auto it = bus.route.begin() + 1; it < bus.route.end(); it++) {
        distance += CalcDistance(stops_.at(*(it-1)), stops_.at(*it));
    }
    if(bus.route_type == Bus::RouteType::LINEAR) {
        distance *= 2;
    }
    return distance;
}

void BusDatabase::AddBus(size_t route, StopsRange stops, bool is_circular) {
    std::vector<std::string> bus_temp;
    bus_temp.reserve(stops.size());

    for(const auto& stop : stops) {
        bus_temp.push_back(stop);
    }

    buses_[route] = {
        .stops = is_circular ? bus_temp.size() : bus_temp.size() * 2 - 1,
        .unique_stops = is_circular ? bus_temp.size() - 1 : bus_temp.size(),
        .route_type = is_circular ? Bus::RouteType::CIRCULAR : Bus::RouteType::LINEAR,
        .route = move(bus_temp)
    };
}

optional<const Bus*>  BusDatabase::GetBusInfo (size_t route) const {
    if(buses_.count(route))
        return &buses_.at(route);
    else
        return nullopt;
}

double  BusDatabase::GetBusDistance(size_t bus_route) const {
    if(!bus_to_distance_.count(bus_route)) {
        bus_to_distance_[bus_route] = ComputeDistance(buses_.at(bus_route));
    }
    return bus_to_distance_.at(bus_route);
}