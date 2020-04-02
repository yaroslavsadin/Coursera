#include "misc.h"
#include "io.h"
#include <sstream>
#include <iomanip>
#include <cassert>
#include "router.h"
#include "yp_serialize.h"

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
    }else if (type_str == "FindCompanies") {
        return Request::Type::FIND_COMPANIES;
    }else if (type_str == "RouteToCompany") {
        return Request::Type::ROUTE_TO_COMPANY;
    } else {
        throw invalid_argument("Unknown request type");
    }
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
: db(), router()
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
        router.SetPedestrianVelocity(route_settings.at("pedestrian_velocity").AsInt());
    }

    /* Initialize Yellow Pages*/
    if(root_.count("yellow_pages")) {
        YP::Serialize(doc,*proto_catalog.mutable_yp());
        index.emplace(proto_catalog.yp());
    }

    /* Setting rendering settings */
    renderer.emplace(db.GetStops(),db.GetBuses(),index->GetCompanies());
    if(root_.count("render_settings")) {
        auto& renderer_ = *renderer;
        const auto& render_settings = root_.at("render_settings").AsMap();
        const auto& stop_label_offset_array = render_settings.at("stop_label_offset").AsArray();
        const auto& bus_label_offset_array = render_settings.at("bus_label_offset").AsArray();
        renderer_.SetWidth(render_settings.at("width").AsDouble())
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
                .SetOuterMargin(render_settings.at("outer_margin").AsDouble())
                .SetCompanyRadius(render_settings.at("company_radius").AsDouble())
                .SetCompanyLineWidth(render_settings.at("company_line_width").AsDouble());

        vector<Svg::Color> colors;
        colors.reserve(render_settings.at("color_palette").AsArray().size());
        for(const auto& color_node : render_settings.at("color_palette").AsArray()) {
            colors.push_back(ColorFromJsonNode(color_node));
        }
        renderer_.SetColorPalette(move(colors));

        vector<string> layers;
        colors.reserve(render_settings.at("layers").AsArray().size());
        for(const auto& layer_node : render_settings.at("layers").AsArray()) {
            layers.push_back(layer_node.AsString());
        }
        renderer_.SetLayers(move(layers));
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
           request->type_ == Request::Type::GET_MAP ||
           request->type_ == Request::Type::FIND_COMPANIES ||
           request->type_ == Request::Type::ROUTE_TO_COMPANY) {
            const auto& request_ = static_cast<const ReadReqeust<Json::Node>&>(*request);
            responses.push_back(request_.Process());
        } else {
            const auto& request_ = static_cast<const ModifyReqeust&>(*request);
            request_.Process();
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

BusRequest::BusRequest(const Json::Node& from_json_node, const BusDatabase& db) 
: ReadReqeust<Json::Node>(GetReqestId(from_json_node), Request::Type::GET_BUS_INFO), db(db)
{
    const auto& as_map = from_json_node.AsMap();
    bus_name_ = as_map.at("name").AsString();
}

StopRequest::StopRequest(const Json::Node& from_json_node, const BusDatabase& db) 
: ReadReqeust<Json::Node>(GetReqestId(from_json_node), Request::Type::GET_STOP_INFO), db(db) 
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

AddBusRequest::AddBusRequest(const Json::Node& json_node, BusDatabase& db)
: ModifyReqeust(Request::Type::ADD_BUS), route_type_(GetRouteType(json_node)), db(db)
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

AddStopRequest::AddStopRequest(const Json::Node& json_node, BusDatabase& db)
: ModifyReqeust(Request::Type::ADD_STOP),db(db)
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

RouteRequest::RouteRequest(const Json::Node& from_json_node, const BusDatabase& db, const TransportRouter& router , const SvgRender& renderer)
: ReadReqeust<Json::Node>(GetReqestId(from_json_node), Request::Type::GET_ROUTE), 
  impl(from_json_node.AsMap().at("from").AsString(),from_json_node.AsMap().at("to").AsString(),db,router,renderer)
{}

MapRequest::MapRequest(const Json::Node& from_json_node, const BusDatabase& db, const SvgRender& renderer)
: ReadReqeust<Json::Node>(GetReqestId(from_json_node), Request::Type::GET_MAP), db(db), renderer(renderer) 
{}

FindCompaniesRequest::FindCompaniesRequest(const Json::Node& from_json_node, const std::optional<YP::YellowPagesIndex>& index)
: ReadReqeust<Json::Node>(GetReqestId(from_json_node), Request::Type::FIND_COMPANIES), impl(from_json_node,index)
{}

RouteToCompanyRequest::RouteToCompanyRequest(const Json::Node& from_json_node, const std::optional<YP::YellowPagesIndex>& index,
                const BusDatabase& db, const TransportRouter& router, const SvgRender& renderer)
:   ReadReqeust<Json::Node>(GetReqestId(from_json_node), Request::Type::ROUTE_TO_COMPANY),
    companies_impl(from_json_node.AsMap().at("companies"),index),
    route_impl(from_json_node.AsMap().at("from").AsString(),"",db,router,renderer),
    current_time(Time(
        from_json_node.AsMap().at("datetime").AsArray()[0].AsInt(),
        from_json_node.AsMap().at("datetime").AsArray()[1].AsInt(),
        from_json_node.AsMap().at("datetime").AsArray()[2].AsInt())
    )
{}

/******************************* 
    REQUEST PROCESS FUNCTIONS  *
********************************/

Json::Node BusRequest::Process() const {
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

Json::Node StopRequest::Process() const {
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
                return res;
            }()
        );
        res["request_id"] = Json::Node(*id_);
    } else {
        res["request_id"] = Json::Node(*id_);
        res["error_message"] = Json::Node(string("not found"));
    }
    return Json::Node(res);
}
Json::Node RouteRequest::Process() const {
    auto res = impl.Build();
    res["request_id"] = Json::Node(*id_);
    return res;
}

Json::Node MapRequest::Process() const {
    std::map<string,Json::Node> res;
    stringstream ss;
    renderer.Render().Render(ss);
    res["map"] = Json::Node(ss.str());
    res["request_id"] = Json::Node(*id_);
    return res;
}

Json::Node RouteToCompanyRequest::Process() const {
    auto res = route_impl.Build(companies_impl.FilterCompanies());
    res["request_id"] = Json::Node(*id_);
    return res;
}

void AddBusRequest::Process() const {
    db.AddBus(
        move(bus_name_),move(stops_), (route_type_ == Bus::RouteType::ROUNDTRIP)
    );
}
void AddStopRequest::Process() const {
    db.AddStop(move(name_),latitude,longtitude,move(distances_to_stops_));
}

Json::Node FindCompaniesRequest::Process() const {
    auto res = impl.BuildResponse();
    res["request_id"] = Json::Node(*id_);
    return res;
}

TransportCatalog& TransportCatalog::Serialize() {
#ifdef DEBUG
    std::cerr << "--------------- SERIALIZATION ---------------" << std::endl;
#endif
    std::ofstream serial(
        serial_file, std::ios::binary
    );
    {
#ifdef DEBUG
      TotalDuration serialize("TransportCatalog& Serialize()");
      ADD_DURATION(serialize);
#endif
      db.Serialize(*proto_catalog.mutable_db());
      router.InitRouter(db.GetBuses(),db.GetStops(),
      /// TODO: This is temporary to let the old tests pass
        (index.has_value()) ? index->GetCompanies() : YP::Companies{}
      );
      router.Serialize(*proto_catalog.mutable_router());
      renderer->Serialize(*proto_catalog.mutable_renderer());
    }
    proto_catalog.SerializeToOstream(&serial);
    // assert(!serial.bad());
    return *this;
}

TransportCatalog& TransportCatalog::Deserialize() {
#ifdef DEBUG
    std::cerr << "-------------- DESERIALIZATION --------------" << std::endl;
#endif
    std::ifstream serial(
        serial_file, std::ios::binary
    );
    ProtoTransport::TransportCatalog t;
    t.ParseFromIstream(&serial);
    // assert(!serial.bad());
#ifdef DEBUG
    TotalDuration deserialize("TransportCatalog& Deserialize()");
    ADD_DURATION(deserialize);
#endif
    db.Deserialize(t.db());
    index.emplace(t.yp());
    router.Deserialize(t.router(),db.GetStops(),db.GetBuses(),
      /// TODO: This is temporary to let the old tests pass
        (index.has_value()) ? index->GetCompanies() : YP::Companies{}
      );
    renderer->Deserialize(t.renderer(),db.GetStops(),db.GetBuses(),
        /// TODO: This is temporary to let the old tests pass
            (index.has_value()) ? index->GetCompanies() : YP::Companies{}
        );
    return *this;
}

