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
    
    void Serialize(ProtoTransport::Router& r) {
        r.set_vertex_count(graph_.GetVertexCount());
        for(size_t i = 0; i < graph_.GetEdgeCount(); i++) {
            ProtoTransport::Edge proto_edge;
            const Graph::Edge<EdgeWeight>& edge = graph_.GetEdge(i);
            proto_edge.set_from(edge.from);
            proto_edge.set_to(edge.to);
            proto_edge.set_item_name(edge.weight.item_name_);
            proto_edge.set_type(edge.weight.type_ == EdgeType::CHANGE);
            proto_edge.set_time(edge.weight.time_);
            proto_edge.set_span_count(edge.weight.span_count_);

            *r.mutable_edges()->Add() = std::move(proto_edge);
        }
    }
    void Deserialize(const ProtoTransport::Router& r) {
        const auto& edges = r.edges();
        graph_ = GraphT(r.vertex_count());
        for(size_t i = 0; i < edges.size(); i++) {
            Graph::Edge<EdgeWeight> edge {
                edges[i].from(),
                edges[i].to(),
                EdgeWeight (
                    (edges[i].type()) ?EdgeType::CHANGE : EdgeType::RIDE,
                    edges[i].time(),
                    edges[i].item_name(),
                    edges[i].span_count(),
                    {} // placeholder for deque
                )
            };

            if(edge.weight.type_ == EdgeType::CHANGE) {
                stop_to_vertices_[edge.weight.item_name_].board = edge.to;
                stop_to_vertices_[edge.weight.item_name_].change = edge.from;
            }

            graph_.AddEdge(std::move(edge));
        }
        router_.emplace(Graph::Router(graph_));
    }
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