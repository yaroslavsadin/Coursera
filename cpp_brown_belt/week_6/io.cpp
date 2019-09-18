#include "misc.h"
#include "io.h"
#include <sstream>

using namespace std;

const string MODIFY_DELIMITER {":"};

Request::Type TypeFromString(string_view str) {
    bool is_modify_request = str.find(MODIFY_DELIMITER);
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
    while(count--) {
        string request = GetObjectFromStream<string>(is).value_or("");
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

std::string BusRequest::Process(const BusDatabase& db) const { return {}; }
void AddBusRequest::Process(BusDatabase& db) const {}
void AddStopRequest::Process(BusDatabase& db) const {}