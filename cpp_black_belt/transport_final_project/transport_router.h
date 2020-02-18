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

    void AddVerticesForStop(const std::string& stop_name) {
        
    }

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
    
    void Serialize(ProtoTransport::Router& r) {
#ifdef DEBUG
        TotalDuration serialize("TransportRouter::Serialize");
        ADD_DURATION(serialize);
#endif
        r.set_vertex_count(graph_.GetVertexCount());
        for(size_t i = 0; i < graph_.GetEdgeCount(); i++) {
            ProtoTransport::Edge* proto_edge = r.mutable_edges()->Add();
            Graph::Edge<double>& edge = graph_.GetEdge(i);
            proto_edge->set_from(edge.from);
            proto_edge->set_to(edge.to);
            proto_edge->set_time(edge.weight);
        }
        for(const auto& edge_info : edges_info) {
            ProtoTransport::EdgeInfo* proto_edge_info = r.mutable_edges_info()->Add();
            proto_edge_info->set_item_name(edge_info.item_name_);
            proto_edge_info->set_span_count(edge_info.span_count_);
            proto_edge_info->set_type((edge_info.type_ == EdgeType::CHANGE) ? true : false);
            proto_edge_info->set_time(edge_info.time_);
        }
    }
    void Deserialize(const ProtoTransport::Router& r, const Stops& s) {
        {
#ifdef DEBUG            
            TotalDuration deserialize("TransportRouter::Deserialize");
            ADD_DURATION(deserialize);
#endif
            Graph::VertexId current_vertex_id {0};
            for(const auto& [stop_name,_] : s) {
                stop_to_vertices_[stop_name].board = current_vertex_id++;
                stop_to_vertices_[stop_name].change = current_vertex_id++;
            }

            const auto& edges = r.edges();
            graph_ = GraphT(r.vertex_count());
            for(int i = 0; i < edges.size(); i++) {
                Graph::Edge<double> edge {
                    edges[i].from(),
                    edges[i].to(),
                    edges[i].time()
                };
                graph_.AddEdge(std::move(edge));
            }
            for(const auto& proto_edge_info : r.edges_info()) {
                edges_info.push_back(
                    EdgeInfo(
                        (proto_edge_info.type()) ?EdgeType::CHANGE : EdgeType::RIDE,
                        proto_edge_info.time(),
                        proto_edge_info.item_name(),
                        proto_edge_info.span_count()
                    )
                );
            }
        }
#ifdef DEBUG
        TotalDuration router_init("Graph::Router(graph_)");
        ADD_DURATION(router_init);
#endif
        router_.emplace(graph_);
    }
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