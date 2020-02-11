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
        if (bus.route_type == Bus::RouteType::ONEWAY) {
            distance_road += cur_stop.distance_to_stop_.at(*(it-1));
        }
    }
    if(bus.route_type == Bus::RouteType::ONEWAY) {
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
        .route_type = is_circular ? Bus::RouteType::ROUNDTRIP : Bus::RouteType::ONEWAY,
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

#include <cassert>
ProtoTransport::Database BusDatabase::Serialize() const {
    ProtoTransport::Database db;

    auto stops = db.mutable_stops();
    for(auto [name,stop] : stops_) {
        ProtoTransport::Stop item;
        item.set_lat(stop.latitude);
        item.set_lon(stop.longtitude);
        auto dist_to_stops = item.mutable_distance_to_stop_();
        for(auto& [k,v] : stop.distance_to_stop_) {
            (*dist_to_stops)[k] = v;
        }
        (*stops)[name] = move(item);
    }
    assert(db.stops_size() == stops_.size());

    auto buses = db.mutable_buses();
    for(auto [name,bus] : buses_) {
        ProtoTransport::Bus item;
        item.set_stops(bus.stops);
        item.set_unique_stops(bus.unique_stops);
        item.set_is_round_trip(bus.route_type == Bus::RouteType::ROUNDTRIP);
        auto route = item.mutable_route();
        for(const auto& stop : bus.route) {
            *route->Add() = std::distance(stops_.begin(),stops_.find(stop));
        }
        (*buses)[name] = move(item);
    }
    assert(db.buses_size() == buses_.size());

    return db;
}

void BusDatabase::Deserialize(const ProtoTransport::Database& db) {
    const auto& stops = db.stops();
    for(auto [name,stop] : stops) {
        stops_[name].latitude = stop.lat();
        stops_[name].longtitude = stop.lon();
        for(auto [k,v]  : stop.distance_to_stop_()) {
            stops_[name].distance_to_stop_[k] = v;
        }
    }
    assert(db.stops_size() == stops_.size());

    const auto& buses = db.buses();
    for(auto [name,bus] : buses) {
        buses_[name].stops = bus.stops();
        buses_[name].unique_stops = bus.unique_stops();
        buses_[name].route_type = (bus.is_round_trip()) ?
                                    Bus::RouteType::ROUNDTRIP :
                                    Bus::RouteType::ONEWAY;
        
        buses_[name].route.reserve(bus.route_size());
        for(uint32_t stop_idx : bus.route()) {
            buses_[name].route.push_back((next(stops_.begin(), stop_idx))->first);
            stops_[(next(stops_.begin(), stop_idx))->first].buses.insert(name);
        }
    }
    assert(db.buses_size() == buses_.size());
}
