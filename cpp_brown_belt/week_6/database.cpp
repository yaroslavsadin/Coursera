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

Graph::Router<EdgeWeight> BusDatabase::InitRouter(void) const {
    // Create a graph which uses stops as vertices and buses as edges
     if(graph_.GetVertexCount() == 0) {
        size_t num_stops = accumulate(buses_.begin(),buses_.end(),0,
        [](const size_t acc, const Buses::value_type& rhs){
            return acc + ((rhs.second.route_type == Bus::RouteType::LINEAR) ? 
            rhs.second.stops : rhs.second.stops + 1);
        });

        graph_ = Graph::DirectedWeightedGraph<EdgeWeight>(num_stops);
        
        // vector<Graph::Edge<EdgeWeight>> edges;
        Graph::VertexId stop_vertex_id = 0;
        for(const auto& [name,bus] : buses_) {
            if(bus.route_type == Bus::RouteType::LINEAR) {
                for(const auto& stop : bus.route) {
                    stop_to_id_list_[stop].push_back(stop_vertex_id++);
                }
            } else {
                for(const auto& stop : Range(bus.route.begin(),bus.route.end()-1)) {
                    stop_to_id_list_[stop].push_back(stop_vertex_id++);
                }
            }
            for(auto it = bus.route.begin() + 1; it < bus.route.end(); it++) {
                const auto& stop_from = *prev(it);
                const auto& stop_to = *it;
                if(bus.route_type == Bus::RouteType::CIRCULAR && next(it) == bus.route.end()) {
                    stop_to_id_list_[stop_to].push_back(stop_vertex_id++);
                }
                edges.push_back(
                    Graph::Edge<EdgeWeight> {
                        stop_to_id_list_[stop_from].back(),
                        stop_to_id_list_[stop_to].back(),
                        EdgeWeight(EdgeType::RIDE, stops_.at(stop_from).distance_to_stop_.at(stop_to) 
                        / 1000. / route_settings_.bus_velocity_ * 60, name)
                    }
                );
                if(bus.route_type == Bus::RouteType::LINEAR) {
                    edges.push_back(
                        Graph::Edge<EdgeWeight> {
                            stop_to_id_list_[stop_to].back(),
                            stop_to_id_list_[stop_from].back(),
                            EdgeWeight(EdgeType::RIDE, stops_.at(stop_to).distance_to_stop_.at(stop_from) 
                            / 1000. / route_settings_.bus_velocity_ * 60, name)
                        }
                    );
                }
            }
        }
        for(const auto& stop_vertices : stop_to_id_list_) {
            const auto& vertex_indices = stop_vertices.second;
            if(vertex_indices.size() > 1) {
                for(auto it_from = vertex_indices.begin(); it_from < vertex_indices.end(); it_from++) {
                    for(auto it_to = vertex_indices.begin(); it_to < vertex_indices.end(); it_to++)
                        if(it_from != it_to) {
                            edges.push_back(
                                Graph::Edge<EdgeWeight> {
                                    *it_from,
                                    *it_to,
                                    EdgeWeight(EdgeType::CHANGE, route_settings_.bus_wait_time_, "NONE")
                                }
                            );
                        }
                }
            }
        }
        for(const auto& edge : edges) {
            graph_.AddEdge(edge);
        }
    }
    return Graph::Router(graph_);
}

#include "test_runner.h"

std::optional<Graph::Router<EdgeWeight>::RouteInfo> 
BusDatabase::BuildRoute(const string& from, const string& to) const {
    cerr << "Entered BuildRoute" << endl;
    auto router = InitRouter();
    std::optional<Graph::Router<EdgeWeight>::RouteInfo> route_info;
    for(const auto& stop_from : stop_to_id_list_.at(from)) {
        for(const auto& stop_to : stop_to_id_list_.at(to)) {
            if(auto route_info_new = router.BuildRoute(stop_from, stop_to)) {
                if(!route_info || route_info->weight.time_ > route_info_new->weight.time_) {
                    route_info = route_info_new;
                }
            }
        }
    }
    if(route_info) {
        cout << route_info->weight.time_ << endl;
        for(size_t i = 0; i < route_info->edge_count; i++) {
            cout << "Edge: " << router.GetRouteEdge(route_info->id,i) << " " 
            << "Bus: " << edges[router.GetRouteEdge(route_info->id,i)].weight.bus_name_  << " " 
            << "Time: " << edges[router.GetRouteEdge(route_info->id,i)].weight.time_ << endl;
        }
    } else {
        cout << "Route not found\n";
    }
    cout << endl;
    return nullopt;
}