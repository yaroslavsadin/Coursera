#include "misc.h"
#include "io.h"
#include <sstream>
#include <iomanip>
#include <cassert>
#include "router.h"

using namespace std;

static const string STOP_DISTANCES_DELIM {"m to "};

Request::Type TypeFromString(string_view str, bool is_modify_request) {
    string_view type_str = ReadToken(str);
    if(type_str == "Bus") {
        if(is_modify_request) {
            return Request::Type::ADD_BUS;
        } else {
            return Request::Type::GET_BUS_INFO;
        }
    } else if (type_str == "Stop") {
        if(is_modify_request) {
            return Request::Type::ADD_STOP;
        } else {
            return Request::Type::GET_STOP_INFO;
        }
    }
    else if (type_str == "Route") {
        return Request::Type::GET_ROUTE;
    }else if (type_str == "Map") {
        return Request::Type::GET_MAP;
    } else {
        throw invalid_argument("Unknown request type");
    }
}

template<typename From>
unique_ptr<Request> MakeRequest(Request::Type request_type, const From& request) {
    switch(request_type) {
        case Request::Type::ADD_BUS:
            return make_unique<AddBusRequest>(request);
            break;
        case Request::Type::ADD_STOP:
            return make_unique<AddStopRequest>(request);
            break;
        case Request::Type::GET_BUS_INFO:
            return make_unique<BusRequest>(request);
            break;
        case Request::Type::GET_STOP_INFO:
            return make_unique<StopRequest>(request);
            break;
        case Request::Type::GET_ROUTE:
            return make_unique<RouteRequest>(request);
            break;
        case Request::Type::GET_MAP:
            return make_unique<MapRequest>(request);
            break;
        default:
            break;
        }
    throw invalid_argument("Unknown request type");
}

// Ugly because visitor doesn't work with vector<variant>... 
Svg::Color ColorFromJsonNode(const Json::Node& underlayer_color_) {
    if(holds_alternative<string>(underlayer_color_)) {
        return Svg::Color(underlayer_color_.AsString());
    } else {
        const auto& underlayer_colors_array = underlayer_color_.AsArray();
        if(underlayer_colors_array.size() == 4) {
            return(Svg::Color(
                Svg::Rgb (
                static_cast<uint8_t>(underlayer_colors_array[0].AsInt()),
                static_cast<uint8_t>(underlayer_colors_array[1].AsInt()),
                static_cast<uint8_t>(underlayer_colors_array[2].AsInt()),
                underlayer_colors_array[3].AsDouble()
                )
            ));
        } else {
            return (Svg::Color(
                Svg::Rgb (
                static_cast<uint8_t>(underlayer_colors_array[0].AsInt()),
                static_cast<uint8_t>(underlayer_colors_array[1].AsInt()),
                static_cast<uint8_t>(underlayer_colors_array[2].AsInt())
                )
            ));
        }
    }
}

TransportCatalog::TransportCatalog(Json::Document doc)
: db(), router(), renderer(db.GetBuses(),db.GetStops()) 
{
    const auto& root_ = doc.GetRoot().AsMap();

    /* Serializing settings */
    if(root_.count("serialization_settings")) {
        const auto& serialization_settings = root_.at("serialization_settings").AsMap();
        serial_file = serialization_settings.at("file").AsString();
    }


    /* Setting route settings */
    if(root_.count("routing_settings")) {
        const auto& route_settings = root_.at("routing_settings").AsMap();
        router.SetBusVelocity(route_settings.at("bus_velocity").AsInt());
        router.SetBusWaitTime(route_settings.at("bus_wait_time").AsInt());

        /* Setting rendering settings */
        const auto& render_settings = root_.at("render_settings").AsMap();
        const auto& stop_label_offset_array = render_settings.at("stop_label_offset").AsArray();
        const auto& bus_label_offset_array = render_settings.at("bus_label_offset").AsArray();
        renderer.SetWidth(render_settings.at("width").AsDouble())
                .SetHeight(render_settings.at("height").AsDouble())
                .SetPadding(render_settings.at("padding").AsDouble())
                .SetStopRadius(render_settings.at("stop_radius").AsDouble())
                .SetLineWidth(render_settings.at("line_width").AsDouble())
                .SetStopLabelFontSize(render_settings.at("stop_label_font_size").AsInt())
                .SetStopLabelOffset({
                                    stop_label_offset_array[0].AsDouble(),
                                    stop_label_offset_array[1].AsDouble()
                                    })
                .SetUnderlayerColor(ColorFromJsonNode(render_settings.at("underlayer_color")))
                .SetUnderlayerWidth(render_settings.at("underlayer_width").AsDouble())
                .SetBusLabelFontSize(render_settings.at("bus_label_font_size").AsInt())
                .SetBusLabelOffset({
                                    bus_label_offset_array[0].AsDouble(),
                                    bus_label_offset_array[1].AsDouble()
                                    })
                .SetOuterMargin(render_settings.at("outer_margin").AsDouble());

        vector<Svg::Color> colors;
        colors.reserve(render_settings.at("color_palette").AsArray().size());
        for(const auto& color_node : render_settings.at("color_palette").AsArray()) {
            colors.push_back(ColorFromJsonNode(color_node));
        }
        renderer.SetColorPalette(move(colors));

        vector<string> layers;
        colors.reserve(render_settings.at("layers").AsArray().size());
        for(const auto& layer_node : render_settings.at("layers").AsArray()) {
            layers.push_back(layer_node.AsString());
        }
        renderer.SetLayers(move(layers));
    }

    /* Read Base Requests*/
    if(root_.count("base_requests")) {
        const auto& base_requests = root_.at("base_requests").AsArray();
        for(const auto& node : base_requests) {
            const auto& request = node.AsMap();
            Request::Type request_type = TypeFromString(request.at("type").AsString(),true);
            requests_.push_back(MakeRequest(request_type,node));
        }
    }

    /* Read Stats Requests */
    if(root_.count("stat_requests")) {
        const auto& stats_requests = root_.at("stat_requests").AsArray();
        for(const auto& node : stats_requests) {
            const auto& request = node.AsMap();
            Request::Type request_type = TypeFromString(request.at("type").AsString(),false);
            requests_.push_back(MakeRequest(request_type,node));
        }
    }
}

TransportCatalog& TransportCatalog::ProcessRequests() {
    vector<Json::Node> responses;
    for(const auto& request : requests_) {
        if(request->type_ == Request::Type::GET_BUS_INFO || 
           request->type_ == Request::Type::GET_STOP_INFO ||
           request->type_ == Request::Type::GET_ROUTE ||
           request->type_ == Request::Type::GET_MAP) {
            const auto& request_ = static_cast<const ReadReqeust<Json::Node>&>(*request);
            responses.push_back(request_.Process(db,router,renderer));
        } else {
            const auto& request_ = static_cast<const ModifyReqeust&>(*request);
            request_.Process(db,router,renderer);
        }
    }
    responses_ = Json::Node(move(responses));
    requests_.clear();
    return *this;
}

/************************** 
    REQUEST CONSTRUCTORS  *
***************************/

inline size_t GetReqestId(const Json::Node& node) {
    return node.AsMap().at("id").AsInt();
}

BusRequest::BusRequest(const Json::Node& from_json_node) 
: ReadReqeust<Json::Node>(GetReqestId(from_json_node), Request::Type::GET_BUS_INFO) 
{
    const auto& as_map = from_json_node.AsMap();
    bus_name_ = as_map.at("name").AsString();
}

StopRequest::StopRequest(const Json::Node& from_json_node) 
: ReadReqeust<Json::Node>(GetReqestId(from_json_node), Request::Type::GET_STOP_INFO) 
{
    const auto& as_map = from_json_node.AsMap();
    stop_name_ = as_map.at("name").AsString();
}

Bus::RouteType AddBusRequest::GetRouteType(string_view request) {
    if(request.find(">") != request.npos) {
        return Bus::RouteType::ROUNDTRIP;
    } else {
        return Bus::RouteType::ONEWAY;
    }
}

Bus::RouteType AddBusRequest::GetRouteType(const Json::Node& json_node) {
    const auto& as_map = json_node.AsMap();
    if(as_map.at("is_roundtrip").AsBool()) {
        return Bus::RouteType::ROUNDTRIP;
    } else {
        return Bus::RouteType::ONEWAY;
    }
}

AddBusRequest::AddBusRequest(const Json::Node& json_node)
: ModifyReqeust(Request::Type::ADD_BUS), route_type_(GetRouteType(json_node))
{
    const auto& as_map = json_node.AsMap();
    bus_name_ = as_map.at("name").AsString();
    for(const auto& stop : as_map.at("stops").AsArray()) {
        stops_.push_back(stop.AsString());
    }
}

pair<string,unsigned int> ParseStopsDistance(string_view str) {
    unsigned int distance = StringToOther<unsigned int>(ReadToken(str,STOP_DISTANCES_DELIM));
    string name = string(ReadToken(str,STOP_DISTANCES_DELIM));
    return {name,distance};
}

StopDistances GetStopDistances(const vector<string>& distances) {
    StopDistances res;
    res.reserve(distances.size());
    for(string_view str : distances) {
        res.push_back(ParseStopsDistance(str));
    }
    return res;
}

AddStopRequest::AddStopRequest(const Json::Node& json_node)
: ModifyReqeust(Request::Type::ADD_STOP)
{
    const auto& as_map = json_node.AsMap();
    name_ = as_map.at("name").AsString();
    
    if(holds_alternative<int>(as_map.at("latitude"))) {
        latitude = static_cast<double>(as_map.at("latitude").AsInt());
    } else if(holds_alternative<double>(as_map.at("latitude"))) {
        latitude = as_map.at("latitude").AsDouble();
    }

    if(holds_alternative<int>(as_map.at("longitude"))) {
        longtitude = static_cast<double>(as_map.at("longitude").AsInt());
    } else if(holds_alternative<double>(as_map.at("longitude"))) {
        longtitude = as_map.at("longitude").AsDouble();
    }
    StopDistances res;
    if(as_map.count("road_distances")) {
        const auto& distances = as_map.at("road_distances").AsMap();
        res.reserve(distances.size());
        for(const auto& distance : distances) {
            res.push_back({distance.first,distance.second.AsInt()});
        }
    }
    distances_to_stops_ = move(res);
}

RouteRequest::RouteRequest(const Json::Node& from_json_node)
: ReadReqeust<Json::Node>(GetReqestId(from_json_node), Request::Type::GET_ROUTE) 
{
    const auto& as_map = from_json_node.AsMap();
    from_ = as_map.at("from").AsString();
    to_ = as_map.at("to").AsString();
}

MapRequest::MapRequest(const Json::Node& from_json_node)
: ReadReqeust<Json::Node>(GetReqestId(from_json_node), Request::Type::GET_MAP) 
{
}

/******************************* 
    REQUEST PROCESS FUNCTIONS  *
********************************/

Json::Node BusRequest::Process(const BusDatabase& db, const TransportRouter& router, const SvgRender& renderer) const {
    auto info = db.GetBusInfo(bus_name_);
    map<string,Json::Node> res;
    if(info.has_value()) {
        res["route_length"] = Json::Node(static_cast<int>(db.GetBusDistance(bus_name_).road_distance));
        res["request_id"] = Json::Node(*id_);
        res["curvature"] = Json::Node(db.GetBusDistance(bus_name_).road_distance / 
                                    db.GetBusDistance(bus_name_).linear_distance);
        res["stop_count"] = Json::Node(static_cast<int>((*info)->stops));
        res["unique_stop_count"] = Json::Node(static_cast<int>((*info)->unique_stops));
    } else {
        res["request_id"] = Json::Node(*id_);
        res["error_message"] = Json::Node(string("not found"));
    }
    return Json::Node(res);
}

Json::Node StopRequest::Process(const BusDatabase& db, const TransportRouter& router, const SvgRender& renderer) const {
    auto info = db.GetStopInfo(stop_name_);
    map<string,Json::Node> res;
    if(info.has_value()) {
        res["buses"] = Json::Node(
            [&]() {
                vector<Json::Node> res;
                res.reserve((*info)->buses.size());
                for(string bus : (*info)->buses) {
                    res.push_back(bus);
                }
                return move(res);
            }()
        );
        res["request_id"] = Json::Node(*id_);
    } else {
        res["request_id"] = Json::Node(*id_);
        res["error_message"] = Json::Node(string("not found"));
    }
    return Json::Node(res);
}
Json::Node RouteRequest::Process(const BusDatabase& db, const TransportRouter& router, const SvgRender& renderer) const {
    auto route = router.BuildRoute(db.GetBuses(),db.GetStops(),from_,to_);
    map<string,Json::Node> res;
    res["request_id"] = Json::Node(*id_);
    if(route) {
        size_t route_id = route->id;
        size_t num_edges = route->edge_count;

        SvgRender::RouteMap route_map;
        if(!num_edges) {
            res["total_time"] = 0;
            res["items"] = vector<Json::Node>();
        } else {
            res["total_time"] = Json::Node(route->weight.time_ );

            vector<Json::Node> items;
            for(size_t i = 0; i < num_edges;i++) {
                const EdgeWeight& edge_info = router.GetRouteEdge(route_id,i).weight;
                switch(edge_info.type_) {
                case EdgeType::CHANGE:
                    items.push_back(map<string,Json::Node> {
                        {"stop_name", edge_info.item_name_},
                        {"time", edge_info.time_},
                        {"type", string("Wait")}
                    });
                    break;
                case EdgeType::RIDE:
                    items.push_back(map<string,Json::Node> {
                        {"bus", edge_info.item_name_},
                        {"time", edge_info.time_},
                        {"span_count", edge_info.span_count_},
                        {"type", string("Bus")}
                    });
                    route_map.push_back(&edge_info);
                    break;
                default:
                    throw runtime_error("Wrong edge type");
                    break;
                }
            }
            res["items"] = move(items);
        }
        // stringstream ss;
        // renderer.RenderRoute(std::move(route_map)).Render(ss);
        // res["map"] = Json::Node(ss.str());
    } else {
        res["error_message"] = Json::Node(string("not found"));
    }
    return Json::Node(move(res));
}

Json::Node MapRequest::Process(const BusDatabase& db, const TransportRouter& router, const SvgRender& renderer) const {
    std::map<string,Json::Node> res;
    stringstream ss;
    renderer.Render().Render(ss);
    res["map"] = Json::Node(ss.str());
    res["request_id"] = Json::Node(*id_);
    return move(res);
}

void AddBusRequest::Process(BusDatabase& db, TransportRouter& router, SvgRender& renderer) const {
    db.AddBus(
        move(bus_name_),move(stops_), (route_type_ == Bus::RouteType::ROUNDTRIP)
    );
}
void AddStopRequest::Process(BusDatabase& db, TransportRouter& router, SvgRender& renderer) const {
    db.AddStop(move(name_),latitude,longtitude,move(distances_to_stops_));
}