#pragma once

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include "misc.h"

struct Stop {
    double latitude;
    double longtitude;
};

using Stops = std::unordered_map< std::string , Stop >;
using StopsRange = Range<std::vector<std::string>::iterator>;

struct Bus {
    explicit Bus(size_t stops_, size_t unique_stops_, double route_length_, std::vector<Stops::const_iterator> route_) :
    stops(stops_),
    unique_stops(unique_stops_),
    route_length(route_length_),
    route(route_)
    {}

    size_t stops;
    size_t unique_stops;
    double route_length;
    std::vector<Stops::const_iterator> route;
};

using Buses = std::unordered_map< size_t , std::unique_ptr< Bus > >;

double CalcDistance(const Stop& from, const Stop& to);

class BusDB {
public:
    void AddStop(std::string_view name, double latitude, double longtitude);
    void AddBus(size_t route, StopsRange stops);
    std::optional<const Bus*>  BusQuery (size_t route) const;
private:
    Stops stops_;
    Buses bus_to_stops_;
};