#include <iostream>
#include "database.h"
#include <cmath>
#include <variant>
#include <cassert>
#include <numeric>

using namespace std;

void BusDatabase::AddStop(std::string name, double latitude, double longtitude,
                                std::vector< std::pair< std::string, unsigned int > > stop_distances) {
    stops_[name].latitude = latitude;
    stops_[name].longtitude = longtitude;

    for(const auto& stop : stop_distances) {
        stops_[name].distance_to_stop_[stop.first] = stop.second;
        if(stops_.count(stop.first)) {
            if(!stops_.at(stop.first).distance_to_stop_.count(name)) {
                stops_[stop.first].distance_to_stop_.insert({name,stop.second});
            }
        } else {
            stops_[stop.first].distance_to_stop_.insert({name,stop.second});
        }
    }
}

Distances BusDatabase::ComputeDistance(const Bus& bus) const {
    if(bus.route.size() < 2) return {0,0};
    double distance_linear {.0};
    unsigned int distance_road {0};
    for(auto it = bus.route.begin() + 1; it < bus.route.end(); it++) {
        const auto& prev_stop = stops_.at(*(it-1));
        const auto& cur_stop = stops_.at(*it);
        distance_linear += CalcDistance(prev_stop.latitude, prev_stop.longtitude, 
                                        cur_stop.latitude, cur_stop.longtitude);
        distance_road += prev_stop.distance_to_stop_.at(*it);
        if (bus.route_type == Bus::RouteType::LINEAR) {
            distance_road += cur_stop.distance_to_stop_.at(*(it-1));
        }
    }
    if(bus.route_type == Bus::RouteType::LINEAR) {
        distance_linear *= 2;
    }
    return {distance_road, distance_linear};
}

void BusDatabase::AddBus(const std::string& name, std::vector<std::string> stops, bool is_circular) {
    std::vector<std::string> bus_temp;
    bus_temp.reserve(stops.size());
    unordered_set<string> unique_stops;

    for(const auto& stop : stops) {
        bus_temp.push_back(stop);
        unique_stops.insert(stop);
        stops_[stop].buses.insert(name);
    }

    buses_[name] = {
        .stops = is_circular ? bus_temp.size() : bus_temp.size() * 2 - 1,
        .unique_stops = unique_stops.size(),
        .route_type = is_circular ? Bus::RouteType::CIRCULAR : Bus::RouteType::LINEAR,
        .route = move(bus_temp)
    };
}

optional<const Bus*>  BusDatabase::GetBusInfo (const std::string& name) const {
    if(buses_.count(name))
        return &buses_.at(name);
    else
        return nullopt;
}

optional<const Stop*>  BusDatabase::GetStopInfo (const std::string& name) const {
    if(stops_.count(name))
        return &stops_.at(name);
    else
        return nullopt;
}

const Distances& BusDatabase::GetBusDistance(const std::string& name) const {
    if(!bus_to_distance_cache.count(name)) {
        bus_to_distance_cache[name] = ComputeDistance(buses_.at(name));
    }
    return bus_to_distance_cache.at(name);
}
