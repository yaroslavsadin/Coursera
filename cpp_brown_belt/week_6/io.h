#pragma once

#include <iostream>
#include "database.h"
#include "misc.h"

/******************************* 
    REQUEST BASE CLASS         *
********************************/

struct Request {
  enum class Type {
    ADD_BUS,
    ADD_STOP,
    GET_BUS_INFO,
    GET_STOP_INFO
  };
  Request(Type type) : type_(type) {}
  static std::unique_ptr<Request> MakeRequest(Type type);
  virtual ~Request() = default;
  const Type type_;
};

/******************************* 
    MODIFY REQUEST BASE CLASS  *
********************************/

struct ModifyReqeust : public Request {
  using Request::Request;
  virtual void Process(BusDatabase& db) const = 0;
};

/******************************* 
    READ REQUEST BASE CLASS    *
********************************/

template<typename ResT>
struct ReadReqeust : public Request {
  using Request::Request;
  virtual ResT Process(const BusDatabase& db) const = 0;
};

/******************************* 
        MODIFY REQUESTS        *
********************************/

struct AddBusRequest : public ModifyReqeust {
  AddBusRequest(std::string_view from_string);
  static Bus::RouteType GetRouteType(std::string_view request);
  void Process(BusDatabase& db) const override;
  std::string bus_name_;
  std::vector<std::string> stops_;
  Bus::RouteType route_type_;
};

using StopDistances = std::vector< std::pair< std::string, unsigned int > >;
struct AddStopRequest : public ModifyReqeust {
  AddStopRequest(std::string_view from_string);
  void Process(BusDatabase& db) const override;
  std::string name_;
  double latitude;
  double longtitude;
  StopDistances distances_to_stops_;
};

/******************************* 
         READ REQUESTS         *
********************************/

struct BusRequest : public ReadReqeust<std::string> {
  BusRequest(std::string_view from_string);
  std::string bus_name_;
  std::string Process(const BusDatabase& db) const override;
};

struct StopRequest : public ReadReqeust<std::string> {
  StopRequest(std::string_view from_string);
  std::string stop_name_;
  std::string Process(const BusDatabase& db) const override;
};

/******************************* 
       DATABASE WRAPPER        *
********************************/

class BusDatabaseHandler {
public:
  using Requests = std::vector<std::unique_ptr<Request>>;
  using Respones = std::vector<std::string>;

  BusDatabaseHandler() = default;
  BusDatabaseHandler& RequestsFromStream(int count, std::istream& is = std::cin);
  BusDatabaseHandler& ProcessRequests();
  Respones GetResponses() {
    return move(responses_);
  }
private:
  Requests requests_;
  Respones responses_;
  BusDatabase db;
};