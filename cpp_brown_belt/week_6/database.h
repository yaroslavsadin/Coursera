#pragma once

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include "misc.h"

struct Stop;
struct Bus;

using Stops = std::unordered_map< std::string , Stop >;
using StopsRange = std::vector<std::string>;
using Buses = std::unordered_map< std::string , Bus >;

struct Stop {
    double latitude;
    double longtitude;
};

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

double CalcDistance(const Stop& from, const Stop& to);

class BusDatabase {
public:
    void AddStop(std::string_view name, double latitude, double longtitude);
    void AddBus(const std::string& name, StopsRange stops, bool is_circular);
    std::optional<const Bus*>  GetBusInfo (const std::string& name) const;
    double GetBusDistance(const std::string& name) const;
private:
    double ComputeDistance(const Bus& bus) const;
    Stops stops_;
    Buses buses_;
    mutable std::unordered_map<std::string_view,double> bus_to_distance_;
};