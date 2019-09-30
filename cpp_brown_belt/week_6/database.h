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

struct Stop {
    double latitude;
    double longtitude;
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
    RIDE
};

struct EdgeWeight {
    EdgeType type_;
    double time_;
    std::string_view bus_name_;

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
    EdgeWeight(EdgeType type, double time, std::string_view bus_name) 
    : type_(type), time_(time), bus_name_(bus_name) {}
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
private:
    Distances ComputeDistance(const Bus& bus) const;
    Stops stops_;
    Buses buses_;
    RouteSettings route_settings_;

    mutable std::unordered_map<std::string_view,std::vector<size_t>> stop_to_id_list_;
    mutable Graph::DirectedWeightedGraph<EdgeWeight> graph_ = Graph::DirectedWeightedGraph<EdgeWeight>(0);
    mutable std::optional<Graph::Router<EdgeWeight>> router_;
    mutable std::unordered_map< std::string_view , Distances > bus_to_distance_;

    template<typename It>
    Graph::VertexId AddRoute(Graph::VertexId stop_vertex_id, const std::string name, 
    Range<It> bus_range, Bus::RouteType route_type) const;
    
    Graph::Router<EdgeWeight> InitRouter(void) const;
};