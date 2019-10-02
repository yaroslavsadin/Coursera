#include "misc.h"
#include "io.h"
#include <sstream>
#include <iomanip>
#include <cassert>
#include "router.h"

using namespace std;

static const string MODIFY_DELIMITER {":"};
static const string COMMA(", ");
static const string STOP_DISTANCES_DELIM("m to ");
static const string CICULAR_ROUTE_DELIM(" > ");
static const string LINEAR_ROUTE_DELIM(" - ");

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
        default:
            break;
        }
    throw invalid_argument("Unknown request type");
}

BusDatabaseHandler& BusDatabaseHandler::ReadRequests(int count, std::istream& is) {
    string request;
    getline(is,request);
    while(count--) {
        getline(is,request);
        Request::Type request_type = TypeFromString(request,
                                    (request.find(MODIFY_DELIMITER) == request.npos) ? false : true);
        requests_.push_back(MakeRequest(request_type,string_view(request)));
    }
    return *this;
}

BusDatabaseHandler& BusDatabaseHandler::ReadRequests(Json::Document doc) {
    const auto& root_ = doc.GetRoot().AsMap();

    /* Setting route settings */
    const auto& route_settings = root_.at("routing_settings").AsMap();
    db.SetBusVelocity(route_settings.at("bus_velocity").AsInt());
    db.SetBusWaitTime(route_settings.at("bus_wait_time").AsInt());

    /* Read Base Requests*/
    const auto& base_requests = root_.at("base_requests").AsArray();
    for(const auto& node : base_requests) {
        const auto& request = node.AsMap();
        Request::Type request_type = TypeFromString(request.at("type").AsString(),true);
        requests_.push_back(MakeRequest(request_type,node));
    }

    /* Read Stats Requests*/
    const auto& stats_requests = root_.at("stat_requests").AsArray();
    for(const auto& node : stats_requests) {
        const auto& request = node.AsMap();
        Request::Type request_type = TypeFromString(request.at("type").AsString(),false);
        requests_.push_back(MakeRequest(request_type,node));
    }

    return *this;
}

BusDatabaseHandler& BusDatabaseHandler::ProcessRequests() {
    vector<Json::Node> responses;
    for(const auto& request : requests_) {
        if(request->type_ == Request::Type::GET_BUS_INFO || 
           request->type_ == Request::Type::GET_STOP_INFO ||
           request->type_ == Request::Type::GET_ROUTE) {
            const auto& request_ = static_cast<const ReadReqeust<Json::Node>&>(*request);
            responses.push_back(request_.Process(db));
        } else {
            const auto& request_ = static_cast<const ModifyReqeust&>(*request);
            request_.Process(db);
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

BusRequest::BusRequest(std::string_view from_string) 
: ReadReqeust<Json::Node>(Request::Type::GET_BUS_INFO) 
{
    // Reading "Bus "
    ReadToken(from_string);
    // Getting route number into the field
    bus_name_ = string(ReadToken(from_string,"\n"));
}

BusRequest::BusRequest(const Json::Node& from_json_node) 
: ReadReqeust<Json::Node>(GetReqestId(from_json_node), Request::Type::GET_BUS_INFO) 
{
    const auto& as_map = from_json_node.AsMap();
    bus_name_ = as_map.at("name").AsString();
}

StopRequest::StopRequest(std::string_view from_string) 
: ReadReqeust<Json::Node>(Request::Type::GET_STOP_INFO) 
{
    // Reading "Stop "
    ReadToken(from_string);
    // Getting route number into the field
    stop_name_ = string(ReadToken(from_string,"\n"));
}

StopRequest::StopRequest(const Json::Node& from_json_node) 
: ReadReqeust<Json::Node>(GetReqestId(from_json_node), Request::Type::GET_STOP_INFO) 
{
    const auto& as_map = from_json_node.AsMap();
    stop_name_ = as_map.at("name").AsString();
}

Bus::RouteType AddBusRequest::GetRouteType(string_view request) {
    if(request.find(">") != request.npos) {
        return Bus::RouteType::CIRCULAR;
    } else {
        return Bus::RouteType::LINEAR;
    }
}

Bus::RouteType AddBusRequest::GetRouteType(const Json::Node& json_node) {
    const auto& as_map = json_node.AsMap();
    if(as_map.at("is_roundtrip").AsBool()) {
        return Bus::RouteType::CIRCULAR;
    } else {
        return Bus::RouteType::LINEAR;
    }
}

AddBusRequest::AddBusRequest(std::string_view from_string)
: ModifyReqeust(Request::Type::ADD_BUS), route_type_(GetRouteType(from_string))
{
    // Reading "Bus "
    ReadToken(from_string);
    // Getting route number into the field
    bus_name_ = string(ReadToken(from_string,MODIFY_DELIMITER));
    // Extra space
    ReadToken(from_string," ");

    string stops_delimiter = (route_type_ == Bus::RouteType::CIRCULAR) ? CICULAR_ROUTE_DELIM 
                                                                        : LINEAR_ROUTE_DELIM;

    while(!from_string.empty()) {
        stops_.push_back(string(ReadToken(from_string,stops_delimiter)));
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

AddStopRequest::AddStopRequest(std::string_view from_string)
: ModifyReqeust(Request::Type::ADD_STOP)
{
    // Reading "Stop "
    ReadToken(from_string);
    // Stop name
    name_ = ReadToken(from_string,MODIFY_DELIMITER);
    // Extra space
    ReadToken(from_string," ");
    
    latitude = StringToOther<double>(ReadToken(from_string, COMMA));
    longtitude = StringToOther<double>(ReadToken(from_string, COMMA));

    vector<string> distances;
    while(!from_string.empty()) {
        distances.push_back(string(ReadToken(from_string,COMMA)));
    }
    distances_to_stops_ = GetStopDistances(distances);
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

RouteRequest::RouteRequest(std::string_view from_string)
: ReadReqeust(Request::Type::GET_ROUTE)
{
}

RouteRequest::RouteRequest(const Json::Node& from_json_node)
: ReadReqeust<Json::Node>(GetReqestId(from_json_node), Request::Type::GET_ROUTE) 
{
    const auto& as_map = from_json_node.AsMap();
    from_ = as_map.at("from").AsString();
    to_ = as_map.at("to").AsString();
}

/******************************* 
    REQUEST PROCESS FUNCTIONS  *
********************************/

Json::Node BusRequest::Process(const BusDatabase& db) const {
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

Json::Node StopRequest::Process(const BusDatabase& db) const {
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
Json::Node RouteRequest::Process(const BusDatabase& db) const {
    auto route = db.BuildRoute(from_,to_);
    map<string,Json::Node> res;
    res["request_id"] = Json::Node(*id_);
    if(route) {
        size_t route_id = route->id;
        size_t num_edges = route->edge_count;

        if(!num_edges) {
            res["total_time"] = 0;
            res["items"] = vector<Json::Node>();
            return res;
        }

        res["total_time"] = Json::Node(route->weight.time_ );

        vector<Json::Node> items;

        for(size_t i = 0; i < num_edges;i++) {
            auto edge_info = db.GetRouteEdge(route_id,i).weight;
            switch(edge_info.type_) {
            case EdgeType::CHANGE:
                items.push_back(map<string,Json::Node> {
                    {"stop_name", edge_info.item_name_},
                    {"time", db.GetRouteSettings().bus_wait_time_},
                    {"type", string("Wait")}
                });
                break;
            case EdgeType::RIDE: {
                items.push_back(map<string,Json::Node> {
                    {"bus", edge_info.item_name_},
                    {"time", edge_info.time_},
                    {"span_count", edge_info.span_count_},
                    {"type", string("Bus")}
                });
            }
                break;
            default:
                throw runtime_error("Wrong edge type");
                break;
            }
        }
        res["items"] = move(items);
    } else {
        res["error_message"] = Json::Node(string("not found"));
    }
    return Json::Node(move(res));
}
void AddBusRequest::Process(BusDatabase& db) const {
    db.AddBus(
        bus_name_,move(stops_), 
        (route_type_ == Bus::RouteType::CIRCULAR)
    );
}
void AddStopRequest::Process(BusDatabase& db) const {
    db.AddStop(move(name_),latitude,longtitude,move(distances_to_stops_));
}