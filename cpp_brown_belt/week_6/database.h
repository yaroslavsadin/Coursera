#pragma once

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <map>
#include <memory>
#include <algorithm>
#include "misc.h"
#include "router.h"

struct Stop;
struct Bus;

using Stops = std::unordered_map< std::string , Stop >;
using StopsRange = std::vector<std::string>;
using Buses = std::unordered_map< std::string , Bus >;

struct StopId {
    Graph::VertexId change;
    Graph::VertexId board;
};

struct Stop {
    double latitude;
    double longtitude;
    StopId id_;
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

double CalcDistance(const Stop& from, const Stop& to);

enum class EdgeType {
    CHANGE,
    RIDE,
    AUX
};

struct EdgeWeight {
    EdgeType type_;
    double time_;
    std::string item_name_;
    int span_count_;

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

    EdgeWeight(double time) : time_(time) {}
    EdgeWeight(EdgeType type, double time, std::string bus_name) 
    : type_(type), time_(time), item_name_(bus_name) {}
    EdgeWeight(EdgeType type, double time, std::string bus_name, int span_count) 
    : type_(type), time_(time), item_name_(bus_name), span_count_(span_count) {}
};

class BusDatabase {
public:
    struct RouteSettings {
        int bus_wait_time_ = 0;
        int bus_velocity_ = 0;
    };
    struct Distances {
        unsigned int road_distance;
        double linear_distance;
    };
    void AddStop(std::string name, double latitude, double longtitude, 
                    std::vector< std::pair< std::string, unsigned int > >);
    void AddBus(const std::string& name, StopsRange stops, bool is_circular);
    std::optional<const Bus*>  GetBusInfo (const std::string& name) const;
    std::optional<const Stop*>  GetStopInfo (const std::string& name) const;
    const Distances& GetBusDistance(const std::string& name) const;
    void SetBusWaitTime(int x);
    void SetBusVelocity(int x);
    std::optional<Graph::Router<EdgeWeight>::RouteInfo> BuildRoute(const std::string& from, const std::string& to) const;
    const RouteSettings& GetRouteSettings() const { return route_settings_; };
    const auto GetRouteEdge(size_t route_id, size_t edge_id) const { 
        return graph_.GetEdge(router_->GetRouteEdge(route_id,edge_id)); 
    }
private:
    Distances ComputeDistance(const Bus& bus) const;
    double GetRideTime(const std::string& stop_from, const std::string& stop_to) const { 
        return stops_.at(stop_from).distance_to_stop_.at(stop_to) / 1000. / route_settings_.bus_velocity_ * 60;
    }
    Graph::VertexId current_stop_id {0};
    Stops stops_;
    Buses buses_;
    RouteSettings route_settings_;

    mutable std::unordered_map<std::string,std::vector<size_t>> stop_to_vertex_;
    mutable Graph::DirectedWeightedGraph<EdgeWeight> graph_ = Graph::DirectedWeightedGraph<EdgeWeight>(0);
    mutable std::optional<Graph::Router<EdgeWeight>> router_;
    mutable std::unordered_map< std::string_view , Distances > bus_to_distance_;
    mutable std::vector<int> routes;

    Graph::Router<EdgeWeight> InitRouter(void) const;
};