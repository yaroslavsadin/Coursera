#pragma once
#include <vector>
#include <unordered_map>
#include <optional>
#include <string>
#include "graph.h"
#include "router.h"
#include "distances.h"
#include "descriptions.h"

struct RouteSettings {
    int bus_wait_time_ = 0;
    int bus_velocity_ = 0;
};

enum class EdgeType {
    CHANGE,
    RIDE
};

// Using not only as weight but also to keep useful info about the edge
struct EdgeWeight {
    EdgeType type_;
    double time_;
    std::string item_name_;
    int span_count_;
    std::deque<std::string_view> stops_;

    bool operator>(const EdgeWeight& other) const {
        return this->time_ > other.time_;
    }

    bool operator<(const EdgeWeight& other) const {
        return this->time_ < other.time_;
    }

    EdgeWeight operator+(const EdgeWeight& other) const {
        EdgeWeight tmp(*this);
        tmp.time_ += other.time_;
        return tmp;
    }

    EdgeWeight(double time) : time_(time) 
    {}
    EdgeWeight(EdgeType type, double time, std::string bus_name) 
    : type_(type), time_(time), item_name_(bus_name) 
    {}
    EdgeWeight(EdgeType type, double time, std::string bus_name, 
    int span_count, const std::deque<std::string_view>& stops ) 
    : type_(type), time_(time), item_name_(bus_name), 
        span_count_(span_count) , stops_(stops)
    {}
};

using RouterT = Graph::Router<EdgeWeight>;
using GraphT = Graph::DirectedWeightedGraph<EdgeWeight>;

class TransportRouter {
public:
    struct StopVertices {
        size_t change;
        size_t board;
    };

    RouterT InitRouter(const Buses& buses_, const Stops& stops_) const;

    void AddVerticesForStop(const std::string& stop_name) {
        stop_to_vertices_[stop_name].board = current_vertex_id++;
        stop_to_vertices_[stop_name].change = current_vertex_id++;
    }

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
private:
    double GetRideTime(const Stops& stops_, const std::string& stop_from, const std::string& stop_to) const { 
        return stops_.at(stop_from).distance_to_stop_.at(stop_to) / 1000. / route_settings_.bus_velocity_ * 60;
    }
    RouteSettings route_settings_;
    // Bad but as router was designed...
    mutable GraphT graph_ = GraphT(0);
    mutable std::optional<RouterT> router_;

    Graph::VertexId current_vertex_id {0};
    std::unordered_map<std::string,StopVertices> stop_to_vertices_;
    
    mutable std::vector<int> routes;
};