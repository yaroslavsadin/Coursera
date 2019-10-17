#include <iostream>
#include "database.h"
#include <cmath>
#include <variant>
#include <cassert>
#include <numeric>

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

void BusDatabase::AddStop(std::string name, double latitude, double longtitude,
                                std::vector< std::pair< std::string, unsigned int > > stop_distances) {
    stops_[name].latitude = latitude;
    stops_[name].longtitude = longtitude;
    stops_[name].id_.change = current_stop_id++;
    stops_[name].id_.board = current_stop_id++;

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

BusDatabase::Distances BusDatabase::ComputeDistance(const Bus& bus) const {
    if(bus.route.size() < 2) return {0,0};
    double distance_linear {.0};
    unsigned int distance_road {0};
    for(auto it = bus.route.begin() + 1; it < bus.route.end(); it++) {
        const auto& prev_stop = stops_.at(*(it-1));
        const auto& cur_stop = stops_.at(*it);
        distance_linear += CalcDistance(prev_stop, cur_stop);
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

void BusDatabase::AddBus(const std::string& name, StopsRange stops, bool is_circular) {
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

const BusDatabase::Distances& BusDatabase::GetBusDistance(const std::string& name) const {
    if(!bus_to_distance_.count(name)) {
        bus_to_distance_[name] = ComputeDistance(buses_.at(name));
    }
    return bus_to_distance_.at(name);
}

void BusDatabase::SetBusWaitTime(int x) {
    route_settings_.bus_wait_time_ = x;
}

void BusDatabase::SetBusVelocity(int x) {
    route_settings_.bus_velocity_ = x;
}

Graph::Router<EdgeWeight> BusDatabase::InitRouter(void) const {
    // Create a graph which uses stops as vertices and buses as edges
    graph_ = Graph::DirectedWeightedGraph<EdgeWeight>(current_stop_id * 2);
    
    for(const auto& [bus_name,bus_data] : buses_) {
        for(auto it_from = bus_data.route.begin(); it_from < bus_data.route.end(); it_from++) {
            const auto& stop_from = *it_from;
            graph_.AddEdge(
                Graph::Edge<EdgeWeight> {
                    stops_.at(stop_from).id_.change,
                    stops_.at(stop_from).id_.board,
                    EdgeWeight(EdgeType::CHANGE, route_settings_.bus_wait_time_, stop_from)
                }
            );
            double cumulative_distance_straight = 0.;
            double cumulative_distance_reverse = 0.;
            for(auto it_to = it_from; it_to < bus_data.route.end(); it_to++) {
                const auto& stop_to = *it_to;
                if(stop_from != stop_to) {
                    const auto& prev_stop = *prev(it_to);
                    cumulative_distance_straight += GetRideTime(prev_stop, stop_to);
                    cumulative_distance_reverse += GetRideTime(stop_to, prev_stop);
                }
                graph_.AddEdge(
                    Graph::Edge<EdgeWeight> {
                        stops_.at(stop_from).id_.board,
                        stops_.at(stop_to).id_.change,
                        EdgeWeight(
                            EdgeType::RIDE, 
                            cumulative_distance_straight, 
                            bus_name,
                            it_to - it_from
                        )
                    }
                );
                if(bus_data.route_type == Bus::RouteType::LINEAR) {
                    graph_.AddEdge(
                    Graph::Edge<EdgeWeight> {
                        stops_.at(stop_to).id_.board,
                        stops_.at(stop_from).id_.change,
                        EdgeWeight(
                            EdgeType::RIDE, 
                            cumulative_distance_reverse, 
                            bus_name,
                            it_to - it_from
                        )
                    }
                );
                }
            }
        }
    }
    return Graph::Router(graph_);
}

std::optional<Graph::Router<EdgeWeight>::RouteInfo> 
BusDatabase::BuildRoute(const string& from, const string& to) const {
    if(!router_) router_.emplace(InitRouter());

    optional<Graph::Router<EdgeWeight>::RouteInfo> route_info = 
    router_->BuildRoute(stops_.at(from).id_.change, stops_.at(to).id_.change);
    routes.push_back(route_info->id);
    return route_info;
}