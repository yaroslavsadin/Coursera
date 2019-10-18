#pragma once
#include <vector>
#include <string>
#include <set>
#include <unordered_map>

struct Stop;
struct Bus;

using Stops = std::unordered_map< std::string , Stop >;
using Buses = std::unordered_map< std::string , Bus >;

struct Stop {
    double latitude;
    double longtitude;
    std::set<std::string> buses;
    std::unordered_map<std::string,int> distance_to_stop_;
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