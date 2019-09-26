#include <iostream>
#include "database.h"
#include <cmath>
#include <variant>
#include <cassert>

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
    stops_[name].id_ = current_stop_id_++;
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

void BusDatabase::AddEdgesToGraph(const string& bus_name, const string& stop_from, const string& stop_to,
map<pair<string_view,string_view>,int>& existing_edges, vector<Graph::Edge<EdgeWeight>>& edges) const {
    if(existing_edges.count({stop_from,stop_to})) {
        edges[existing_edges.at({stop_from,stop_to})].weight.buses_on_edge.insert(bus_name);
    } else {
        double distance = stops_.at(stop_from).distance_to_stop_.at(stop_to) 
                                / 1000. / route_settings_.bus_velocity_ * 60;
        edges.push_back( {  stops_.at(stop_from).id_,
                            stops_.at(stop_to).id_,
                            EdgeWeight( bus_name, distance, route_settings_.bus_wait_time_)
                        } );
        existing_edges[{stop_from,stop_to}] = edges.size() - 1;
    }
}

Graph::Router<EdgeWeight> BusDatabase::InitRouter(void) const {
    // Create a graph which uses stops as vertices and buses as edges
     if(graph_.GetVertexCount() == 0) {
        graph_ = Graph::DirectedWeightedGraph<EdgeWeight>(stops_.size());
        map<pair<string_view,string_view>,int> existing_edges;
        vector<Graph::Edge<EdgeWeight>> edges;
        for(const auto& [name,bus] : buses_) {
            for(auto it = bus.route.begin() + 1; it < bus.route.end(); it++) {
                const auto& stop_from = *prev(it);
                const auto& stop_to = *it;
                AddEdgesToGraph(name, stop_from, stop_to,existing_edges, edges);
                if(bus.route_type == Bus::RouteType::LINEAR) {
                    AddEdgesToGraph(name, stop_to, stop_from,existing_edges, edges);
                }
            }
        }
        for(const auto& edge : edges) {
            graph_.AddEdge(edge);
        }
    }
    return Graph::Router(graph_);
}

std::optional<Graph::Router<EdgeWeight>::RouteInfo> 
BusDatabase::BuildRoute(const string& from, const string& to) const {
    cerr << "Entered BuildRoute" << endl;
    auto router = InitRouter();
    if(auto route_info = router.BuildRoute(stops_.at(from).id_, stops_.at(to).id_); 
    route_info.has_value()) {
        cout << "Time: " << route_info->weight.time_ << endl;
        for(size_t i = 0; i < route_info->edge_count; i++) {
            auto edge = router.GetRouteEdge(route_info->id,i);
            cout << edge_id_to_bus_name_[edge] << endl;
        }
    } else {
        cout << "Route not found\n";
    }
    // cerr << "Buses edge IDs:\n------------------------\n";
    // for(size_t it = 0; it < edge_id_to_bus_name_.size(); it++) {
    //     cerr << it << ": " << edge_id_to_bus_name_[it] << endl;
    // }
    // cerr << "Stop IDs:\n------------------------\n";
    // for(const auto& [stop,data] : stops_) {
    //     cerr << data.id_ << ": " << stop << endl;
    // }
    cout << endl;
    return nullopt;
}