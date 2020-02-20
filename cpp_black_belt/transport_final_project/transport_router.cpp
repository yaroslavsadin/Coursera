#include "transport_router.h"
using namespace std;

#ifdef DEBUG
#include "profile_advanced.h"
#endif

void TransportRouter::InitRouter(const Buses& buses_, const Stops& stops_) const {
    if(!router_) {
#ifdef DEBUG
        TotalDuration init_router("TransportRouter::InitRouter()");
        ADD_DURATION(init_router);
#endif
        Graph::VertexId current_vertex_id {0};
        for(const auto& [stop_name,_] : stops_) {
            stop_to_vertices_[stop_name].board = current_vertex_id++;
            stop_to_vertices_[stop_name].change = current_vertex_id++;
        }

        graph_ = Graph::DirectedWeightedGraph<double>(current_vertex_id);
        
        for(const auto& [bus_name,bus_data] : buses_) {
            for(size_t it_from = 0; it_from < bus_data.route.size(); it_from++) {
                const auto& stop_from = bus_data.route[it_from];
                graph_.AddEdge(
                    Graph::Edge<double> {
                        stop_to_vertices_.at(stop_from).change,
                        stop_to_vertices_.at(stop_from).board,
                        static_cast<double>(route_settings_.bus_wait_time_)
                    }
                );
                edges_info.push_back(EdgeInfo(EdgeType::CHANGE, stop_from));
                double cumulative_distance_straight = 0.;
                double cumulative_distance_reverse = 0.;
                for(size_t it_to = it_from; it_to < bus_data.route.size(); it_to++) {
                    const auto& stop_to = bus_data.route[it_to];
                    if(stop_from != stop_to) {
                        const auto& prev_stop = bus_data.route[it_to-1];
                        cumulative_distance_straight += GetRideTime(stops_, prev_stop, stop_to);
                        cumulative_distance_reverse += GetRideTime(stops_, stop_to, prev_stop);
                    }
                    graph_.AddEdge(
                        Graph::Edge<double> {
                            stop_to_vertices_.at(stop_from).board,
                            stop_to_vertices_.at(stop_to).change,
                            cumulative_distance_straight
                        }
                    );
                    edges_info.push_back(EdgeInfo(
                                EdgeType::RIDE, 
                                bus_name,
                                it_to - it_from,
                                it_from,it_to
                            ));
                    if(bus_data.route_type == Bus::RouteType::ONEWAY) {
                        graph_.AddEdge(
                            Graph::Edge<double> {
                                stop_to_vertices_.at(stop_to).board,
                                stop_to_vertices_.at(stop_from).change,
                                cumulative_distance_reverse
                            }
                        );
                        edges_info.push_back(EdgeInfo(
                            EdgeType::RIDE,
                            bus_name,
                            it_to - it_from,
                            it_to,it_from
                        ));
                    }
                }
            }
        }
        {
            #ifdef DEBUG
                TotalDuration router("router_.emplace(graph_)");
                cerr << graph_.GetEdgeCount() << ' ' << graph_.GetVertexCount() << endl;
                ADD_DURATION(router);
            #endif
            router_.emplace(graph_);
        }
    }
}

    void TransportRouter::Serialize(ProtoTransport::Router& r) const {
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
        for(size_t i = 0; i < graph_.GetEdgeCount(); i++) {
            ProtoTransport::EdgeInfo* proto_edge_info = r.mutable_edges_info()->Add();
            const auto& edge_info = edges_info[i];
            proto_edge_info->set_item_name(edge_info.item_name_);
            if(edge_info.type_ == EdgeType::RIDE) {
                proto_edge_info->set_span_count(edge_info.span_count_);
                proto_edge_info->set_stop_from(edge_info.route->start);
                proto_edge_info->set_stop_to(edge_info.route->end);
            }
            proto_edge_info->set_type((edge_info.type_ == EdgeType::CHANGE) ? true : false);
        }
        const RouterT::RoutesInternalData& router_internal_data = router_->GetRouteInternalData();
        if(!router_internal_data.empty()) {
            r.set_routes_col_size(router_internal_data[0].size());
        }
        for(size_t i = 0; i < router_internal_data.size(); i++) {
            auto* col = r.mutable_routes_data()->Add();
            for(size_t j = 0; j < router_internal_data[i].size(); j++) {
                const optional<RouterT::RouteInternalData>& data = router_internal_data[i][j];
                if(data.has_value()) {
                    auto* proto_route_internal_data = col->mutable_data()->Add();
                    if(data->prev_edge.has_value()) {
                        proto_route_internal_data->set_prev_edge(*data->prev_edge);
                    } else {
                        proto_route_internal_data->set_prev_edge(-1);
                    }
                    proto_route_internal_data->set_weight(data->weight);
                    proto_route_internal_data->set_idx(j);
                }
            }
        }
    }
    void TransportRouter::Deserialize(const ProtoTransport::Router& r, const Stops& s, const Buses& b) {
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
                if(proto_edge_info.type()) {    
                    edges_info.push_back(
                        EdgeInfo(
                            EdgeType::CHANGE,
                            proto_edge_info.item_name()
                        )
                    );
                } else {
                    edges_info.push_back(
                        EdgeInfo(
                            EdgeType::RIDE,
                            proto_edge_info.item_name(),
                            proto_edge_info.span_count(),
                            proto_edge_info.stop_from(),
                            proto_edge_info.stop_to()
                        )
                    );
                }
            }
        }

#ifdef DEBUG
        TotalDuration router_init("RouterDeser");
        ADD_DURATION(router_init);
#endif
        RouterT::RoutesInternalData router_internal_data;
        router_internal_data.resize(r.routes_data_size());
        for(int i = 0; i < r.routes_data_size(); i++) {
            std::vector<std::optional<RouterT::RouteInternalData>> col;
            col.resize(r.routes_col_size());
            for(int j = 0; j < r.routes_data(i).data_size(); j++) {
                const ProtoTransport::RouteData& data = r.routes_data(i).data(j);
                    RouterT::RouteInternalData temp;
                    temp.weight = data.weight();
                    if(data.prev_edge() != -1) {
                        temp.prev_edge = data.prev_edge();
                    }
                    col[data.idx()] = std::move(temp);
            }
            router_internal_data[i] = std::move(col);
        }
        router_.emplace(graph_,std::move(router_internal_data));
        // router_.emplace(graph_);
    }

std::optional<RouterT::RouteInfo> 
TransportRouter::BuildRoute(const Buses& buses_, const Stops& stops_, const string& from, const string& to) const {
    // InitRouter(buses_,stops_);
    std::optional<RouterT::RouteInfo> route_info = 
    router_->BuildRoute(stop_to_vertices_.at(from).change, stop_to_vertices_.at(to).change);
    if(route_info) {
        routes.push_back(route_info->id);
    }
    return route_info;
}