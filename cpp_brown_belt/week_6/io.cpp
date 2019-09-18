#include "misc.h"
#include "io.h"
#include <sstream>

using namespace std;

const string MODIFY_DELIMITER {":"};

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
        return Request::Type::ADD_STOP;
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
        default:
            break;
        }
    }
    return *this;
}

  BusDatabaseHandler& BusDatabaseHandler::ProcessRequests() {
      for(const auto& request : requests_) {
          if(request->type_ == Request::Type::GET_BUS_INFO) {
            const auto& request_ = static_cast<const BusRequest&>(*request);
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
    stringstream ss(string(ReadToken(from_string)));
    bus_route_ = GetObjectFromStream<size_t>(ss).value();
}

bool AddBusRequest::IsCircularRoute(string_view request) {
    if(request.find(">") != request.npos) {
        return true;
    } else {
        return false;
    }
}

AddBusRequest::AddBusRequest(std::string_view from_string)
: ModifyReqeust(Request::Type::ADD_BUS)
{
    // Reading "Bus "
    ReadToken(from_string);
    // Getting route number into the field
    stringstream ss(string(ReadToken(from_string,MODIFY_DELIMITER)));
    bus_route_ = GetObjectFromStream<size_t>(ss).value();
    // Extra space
    ReadToken(from_string," ");

    string stops_delimiter = IsCircularRoute(from_string) ? " > " : " - ";

    vector<string> reversed;
    while(!from_string.empty()) {
        stops_.push_back(string(ReadToken(from_string,stops_delimiter)));
    }
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
    
    stop_.latitude = StringToDouble(ReadToken(from_string, ", "));
    stop_.longtitude = StringToDouble(ReadToken(from_string));
}

/******************************* 
    REQUEST PROCESS FUNCTIONS  *
********************************/

std::string BusRequest::Process(const BusDatabase& db) const {
    auto info = db.GetBusInfo(bus_route_);
    if(info.has_value()) {
        return "Bus " + to_string(bus_route_) + ": " +
        to_string((*info)->stops) + " stops on route, " +
        to_string((*info)->stops) + " unique stops, " +
        to_string((*info)->route_length) + " route length";
    } else {
        return "Bus " + to_string(bus_route_) + ": " + "not found";
    }
}
void AddBusRequest::Process(BusDatabase& db) const {
    db.AddBus(bus_route_,move(stops_));
}
void AddStopRequest::Process(BusDatabase& db) const {
    db.AddStop(name_,stop_.latitude,stop_.longtitude);
}