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
#ifdef DEBUG
#include "profile_advanced.h"
#endif

struct RouteSettings {
    int bus_wait_time_ = 0;
    int bus_velocity_ = 0;
};

using RouterT = Graph::Router<double>;
using GraphT = Graph::DirectedWeightedGraph<double>;

class TransportRouter {
public:
    struct StopVertices {
        size_t change;
        size_t board;
    };

    void InitRouter(const Buses& buses_, const Stops& stops_) const;
    
    const EdgeInfo& GetEdgeInfo(size_t idx) const {
        return edges_info[idx];
    }

    void SetBusWaitTime(int x) {
        route_settings_.bus_wait_time_ = x;
    }
    void SetBusVelocity(int x) {
        route_settings_.bus_velocity_ = x;
    }
    
    Graph::EdgeId GetRouteEdgeId(size_t route_id, size_t edge_id) const { 
        return router_->GetRouteEdge(route_id,edge_id); 
    }

    std::optional<RouterT::RouteInfo> BuildRoute(const Buses& buses_, const Stops& stops_, 
                                                    const std::string& from, const std::string& to) const;
    
    void Serialize(ProtoTransport::Router& r) const;
    void Deserialize(const ProtoTransport::Router& r, const Stops& s);
private:
    double GetRideTime(const Stops& stops_, const std::string& stop_from, const std::string& stop_to) const { 
        return stops_.at(stop_from).distance_to_stop_.at(stop_to) / 1000. / route_settings_.bus_velocity_ * 60;
    }
    RouteSettings route_settings_;
    // Bad but as router was designed...
    mutable GraphT graph_ = GraphT(0);
    mutable std::optional<RouterT> router_;
    mutable std::unordered_map<std::string_view,StopVertices> stop_to_vertices_;
    mutable std::vector<EdgeInfo> edges_info;
    
    mutable std::vector<int> routes;
};