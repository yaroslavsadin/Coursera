#include "transport_router.h"
using namespace std;

void TransportRouter::InitRouter(const Buses& buses_, const Stops& stops_) const {
    if(!router_) {    
        Graph::VertexId current_vertex_id {0};
        for(const auto& [stop_name,_] : stops_) {
            stop_to_vertices_[stop_name].board = current_vertex_id++;
            stop_to_vertices_[stop_name].change = current_vertex_id++;
        }

        graph_ = Graph::DirectedWeightedGraph<EdgeWeight>(current_vertex_id * 2);
        
        for(const auto& [bus_name,bus_data] : buses_) {
            for(auto it_from = bus_data.route.begin(); it_from < bus_data.route.end(); it_from++) {
                const auto& stop_from = *it_from;
                graph_.AddEdge(
                    Graph::Edge<EdgeWeight> {
                        stop_to_vertices_.at(stop_from).change,
                        stop_to_vertices_.at(stop_from).board,
                        EdgeWeight(EdgeType::CHANGE, route_settings_.bus_wait_time_, stop_from)
                    }
                );
                double cumulative_distance_straight = 0.;
                double cumulative_distance_reverse = 0.;
                deque<string_view> stops_along_straight;
                deque<string_view> stops_along_reverse;
                for(auto it_to = it_from; it_to < bus_data.route.end(); it_to++) {
                    const auto& stop_to = *it_to;
                    if(stop_from != stop_to) {
                        const auto& prev_stop = *prev(it_to);
                        cumulative_distance_straight += GetRideTime(stops_, prev_stop, stop_to);
                        cumulative_distance_reverse += GetRideTime(stops_, stop_to, prev_stop);
                    }
                    stops_along_straight.push_back(stop_to);
                    stops_along_reverse.push_front(stop_to);
                    graph_.AddEdge(
                        Graph::Edge<EdgeWeight> {
                            stop_to_vertices_.at(stop_from).board,
                            stop_to_vertices_.at(stop_to).change,
                            EdgeWeight(
                                EdgeType::RIDE, 
                                cumulative_distance_straight, 
                                bus_name,
                                it_to - it_from,
                                (stop_from != stop_to) ? stops_along_straight : deque<string_view>{}
                            )
                        }
                    );
                    if(bus_data.route_type == Bus::RouteType::ONEWAY) {
                        graph_.AddEdge(
                        Graph::Edge<EdgeWeight> {
                            stop_to_vertices_.at(stop_to).board,
                            stop_to_vertices_.at(stop_from).change,
                            EdgeWeight(
                                EdgeType::RIDE, 
                                cumulative_distance_reverse, 
                                bus_name,
                                it_to - it_from,
                                (stop_from != stop_to) ? stops_along_reverse : deque<string_view>{}
                            )
                        }
                    );
                    }
                }
            }
        }
        router_.emplace(Graph::Router(graph_));
    }
}

std::optional<RouterT::RouteInfo> 
TransportRouter::BuildRoute(const Buses& buses_, const Stops& stops_, const string& from, const string& to) const {
    InitRouter(buses_,stops_);

    try {
        stop_to_vertices_.at(from);
        stop_to_vertices_.at(to);
    } catch(...) {
        cerr << "Here" << endl;
    }

    std::optional<RouterT::RouteInfo> route_info = 
    router_->BuildRoute(stop_to_vertices_.at(from).change, stop_to_vertices_.at(to).change);
    if(route_info) {
        routes.push_back(route_info->id);
    }
    return route_info;
}