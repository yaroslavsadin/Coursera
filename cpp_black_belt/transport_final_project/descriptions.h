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

// Using not only as weight but also to keep useful info about the edge
struct EdgeInfo {
    EdgeType type_;
    double time_;
    std::string item_name_;
    int span_count_;
    // std::deque<std::string_view> stops_;

    inline bool operator>(const EdgeInfo& other) const {
        return this->time_ > other.time_;
    }

    inline bool operator<(const EdgeInfo& other) const {
        return this->time_ < other.time_;
    }

    inline EdgeInfo operator+(const EdgeInfo& other) const {
        EdgeInfo tmp(*this);
        tmp.time_ += other.time_;
        return tmp;
    }

    EdgeInfo(double time) : time_(time) 
    {}
    EdgeInfo(EdgeType type, double time, std::string bus_name) 
    : type_(type), time_(time), item_name_(bus_name) 
    {}
    EdgeInfo(EdgeType type, double time, std::string bus_name, 
    int span_count/*, const std::deque<std::string_view>& stops*/ ) 
    : type_(type), time_(time), item_name_(bus_name), 
        span_count_(span_count)// , stops_(stops)
    {}
};