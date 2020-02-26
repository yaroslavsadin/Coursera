#pragma once
#include <vector>
#include <string>
#include <set>
#include <map>
#include <unordered_map>
#include <deque>

struct Stop;
struct Bus;

using Stops = std::map< std::string , Stop >;
using Buses = std::map< std::string , Bus >;

struct Stop {
    double latitude;
    double longtitude;
    std::set<std::string> buses;
    std::unordered_map<std::string,int> distance_to_stop_;
};

struct Bus {
    enum class RouteType {
        ONEWAY,
        ROUNDTRIP
    };
    size_t stops;
    size_t unique_stops;
    RouteType route_type;
    std::vector<std::string> route;
};

enum class EdgeType {
    CHANGE,
    RIDE
};

struct EdgeInfo {
    EdgeType type_;
    std::string item_name_;
    int span_count_;
    
    struct RouteRange {
        int start;
        int end;
    };
    std::optional<RouteRange> route;

    EdgeInfo(EdgeType type, std::string bus_name) 
    : type_(type), item_name_(bus_name) 
    {}
    EdgeInfo(EdgeType type, std::string bus_name, 
    int span_count, int start_, int end_ ) 
    : type_(type), item_name_(bus_name), 
        span_count_(span_count) , route(RouteRange{start_,end_})
    {}
};