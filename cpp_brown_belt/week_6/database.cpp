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
    dist = 6371 * dist;
    return dist;
}

void BusDatabase::AddStop(std::string_view name, double latitude, double longtitude) {
    stops_[string(name)] = {latitude,longtitude};
}

void BusDatabase::AddBus(size_t route, StopsRange stops) {
    std::vector<Stops::const_iterator> bus_temp;
    bus_temp.reserve(stops.size());
    unordered_set<string> unique_stops;
    double distance {.0};

#define CURRENT_STOP (**prev(bus_temp.end())).second
#define PREVIOUS_STOP (**prev(prev(bus_temp.end()))).second
    for(const auto& stop : stops) {
        bus_temp.push_back(stops_.find(stop));
        unique_stops.insert(stop);
        if(bus_temp.size() > 1) {
            distance += CalcDistance(CURRENT_STOP,PREVIOUS_STOP);
        }
    }
#undef CURRENT_STOP
#undef PREVIOUS_STOP

    bus_to_stops_[route] = {
        .stops = bus_temp.size(),
        .unique_stops = unique_stops.size(),
        .route_length = distance,
        .route = move(bus_temp)
    };
}

optional<const Bus*>  BusDatabase::GetBusInfo (size_t route) const {
    if(bus_to_stops_.count(route))    
        return &bus_to_stops_.at(route);
    else
        return nullopt;
}