#pragma once
#include <vector>
#include <unordered_map>
#include <optional>
#include <string>
#include "graph.h"
#include "router.h"
#include "distances.h"
#include "descriptions.h"
#include "router.pb.h"

struct RouteSettings {
    int bus_wait_time_ = 0;
    int bus_velocity_ = 0;
};

using RouterT = Graph::Router<EdgeWeight>;
using GraphT = Graph::DirectedWeightedGraph<EdgeWeight>;

class TransportRouter {
public:
    struct StopVertices {
        size_t change;
        size_t board;
    };

    void InitRouter(const Buses& buses_, const Stops& stops_) const;

    void SetBusWaitTime(int x) {
        route_settings_.bus_wait_time_ = x;
    }
    void SetBusVelocity(int x) {
        route_settings_.bus_velocity_ = x;
    }
    
    const auto& GetRouteEdge(size_t route_id, size_t edge_id) const { 
        return graph_.GetEdge(router_->GetRouteEdge(route_id,edge_id)); 
    }

    std::optional<RouterT::RouteInfo> BuildRoute(const Buses& buses_, const Stops& stops_, 
                                                    const std::string& from, const std::string& to) const;
    
    void Serialize(ProtoTransport::Router& r) {
        r.set_bus_velocity(route_settings_.bus_velocity_);
        r.set_bus_wait_time(route_settings_.bus_wait_time_);
    }
    void Deserialize(const ProtoTransport::Router& r) {
        route_settings_.bus_velocity_ = r.bus_velocity();
        route_settings_.bus_wait_time_ = r.bus_wait_time();
    }
private:
    double GetRideTime(const Stops& stops_, const std::string& stop_from, const std::string& stop_to) const { 
        return stops_.at(stop_from).distance_to_stop_.at(stop_to) / 1000. / route_settings_.bus_velocity_ * 60;
    }
    RouteSettings route_settings_;
    // Bad but as router was designed...
    mutable GraphT graph_ = GraphT(0);
    mutable std::optional<RouterT> router_;
    mutable std::unordered_map<std::string,StopVertices> stop_to_vertices_;
    
    mutable std::vector<int> routes;
};