#pragma once

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <set>
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
    std::set<std::string> buses;
    std::unordered_map<std::string,unsigned int> distance_to_stop_;
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
    void AddStop(std::string name, double latitude, double longtitude, 
                    std::vector< std::pair< std::string, unsigned int > >);
    void AddBus(const std::string& name, StopsRange stops, bool is_circular);
    std::optional<const Bus*>  GetBusInfo (const std::string& name) const;
    std::optional<const Stop*>  GetStopInfo (const std::string& name) const;
    std::pair<double,double> GetBusDistance(const std::string& name) const;
private:
    std::pair<double,double> ComputeDistance(const Bus& bus) const;
    Stops stops_;
    Buses buses_;
    mutable std::unordered_map<std::string_view,std::pair<double,double>> bus_to_distance_;
};