#include "misc.h"
#include "io.h"
#include <sstream>
#include <iomanip>

using namespace std;

static const string MODIFY_DELIMITER {":"};
static const string COMMA(", ");
static const string STOP_DISTANCES_DELIM("m to ");
static const string CICULAR_ROUTE_DELIM(" > ");
static const string LINEAR_ROUTE_DELIM(" - ");

Request::Type TypeFromString(string_view str) {
    bool is_modify_request = (str.find(MODIFY_DELIMITER) == str.npos) ? false : true;
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
    } else {
        throw invalid_argument("Unknown request type");
    }
}

BusDatabaseHandler& BusDatabaseHandler::RequestsFromStream(int count, std::istream& is) {
    string request;
    getline(is,request);
    while(count--) {
        getline(is,request);
        Request::Type request_type = TypeFromString(request);

        switch(request_type) {
        case Request::Type::ADD_BUS:
            requests_.push_back(make_unique<AddBusRequest>(request));
            break;
        case Request::Type::ADD_STOP:
            requests_.push_back(make_unique<AddStopRequest>(request));
            break;
        case Request::Type::GET_BUS_INFO:
            requests_.push_back(make_unique<BusRequest>(request));
            break;
        case Request::Type::GET_STOP_INFO:
            requests_.push_back(make_unique<StopRequest>(request));
            break;
        default:
            break;
        }
    }
    return *this;
}

BusDatabaseHandler& BusDatabaseHandler::ProcessRequests() {
    for(const auto& request : requests_) {
        if(request->type_ == Request::Type::GET_BUS_INFO || 
           request->type_ == Request::Type::GET_STOP_INFO) {
            const auto& request_ = static_cast<const ReadReqeust<string>&>(*request);
            responses_.push_back(request_.Process(db));
        } else {
            const auto& request_ = static_cast<const ModifyReqeust&>(*request);
            request_.Process(db);
        }
    }
    requests_.clear();
    return *this;
}

/************************** 
    REQUEST CONSTRUCTORS  *
***************************/

BusRequest::BusRequest(std::string_view from_string) 
: ReadReqeust<std::string>(Request::Type::GET_BUS_INFO) 
{
    // Reading "Bus "
    ReadToken(from_string);
    // Getting route number into the field
    bus_name_ = string(ReadToken(from_string,"\n"));
}

StopRequest::StopRequest(std::string_view from_string) 
: ReadReqeust<std::string>(Request::Type::GET_STOP_INFO) 
{
    // Reading "Stop "
    ReadToken(from_string);
    // Getting route number into the field
    stop_name_ = string(ReadToken(from_string,"\n"));
}

Bus::RouteType AddBusRequest::GetRouteType(string_view request) {
    if(request.find(">") != request.npos) {
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

/******************************* 
    REQUEST PROCESS FUNCTIONS  *
********************************/

std::string BusRequest::Process(const BusDatabase& db) const {
    auto info = db.GetBusInfo(bus_name_);
    if(info.has_value()) {
        ostringstream os;
        os << "Bus " << bus_name_ << ": " <<
        (*info)->stops << " stops on route, " <<
        (*info)->unique_stops << " unique stops, " <<
        db.GetBusDistance(bus_name_).road_distance << " route length, " << setprecision(6) << 
        db.GetBusDistance(bus_name_).road_distance / db.GetBusDistance(bus_name_).linear_distance << " curvature";
        return os.str();
    } else {
        return "Bus " + bus_name_ + ": " + "not found";
    }
}
std::string StopRequest::Process(const BusDatabase& db) const {
    auto info = db.GetStopInfo(stop_name_);
    if(info.has_value()) {
        if((*info)->buses.size()) {
            ostringstream os;
            os << "Stop " << stop_name_ << ": buses ";
            for(const auto& bus : (*info)->buses) {
                os << bus << ' ';
            }
            return os.str();
        } else {
            return "Stop " + stop_name_ + ": " + "no buses";
        }
    } else {
        return "Stop " + stop_name_ + ": " + "not found";
    }
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