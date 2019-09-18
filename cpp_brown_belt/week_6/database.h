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
using StopsRange = std::vector<std::string>;

struct Bus {
    enum class RouteType {
        LINEAR,
        CIRCULAR
    };
    size_t stops;
    size_t unique_stops;
    RouteType route_type;
    std::vector<std::string> route;
};

using Buses = std::unordered_map< size_t , Bus >;

double CalcDistance(const Stop& from, const Stop& to);

class BusDatabase {
public:
    void AddStop(std::string_view name, double latitude, double longtitude);
    void AddBus(size_t route, StopsRange stops, bool is_circular);
    std::optional<const Bus*>  GetBusInfo (size_t route) const;
    double GetBusDistance(size_t bus_route) const;
private:
    double ComputeDistance(const Bus& bus) const;
    Stops stops_;
    Buses buses_;
    mutable std::unordered_map<int,double> bus_to_distance_;
};