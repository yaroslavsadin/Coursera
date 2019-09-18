#pragma once

#include <iostream>
#include "database.h"
#include "misc.h"

struct Request {
  enum class Type {
    ADD_BUS,
    ADD_STOP,
    GET_BUS_INFO
  };
  Request(Type type) : type_(type) {}
  static std::unique_ptr<Request> MakeRequest(Type type);
  virtual ~Request() = default;
  const Type type_;
};

struct ModifyReqeust : public Request {
  using Request::Request;
  virtual void Process(BusDatabase& db) const = 0;
};

template<typename ResT>
struct ReadReqeust : public Request {
  using Request::Request;
  virtual ResT Process(const BusDatabase& db) const = 0;
};

struct AddBusRequest : public ModifyReqeust {
  AddBusRequest(std::string_view from_string);
  static Bus::RouteType GetRouteType(std::string_view request);
  void Process(BusDatabase& db) const override;
  std::string bus_name_;
  std::vector<std::string> stops_;
  Bus::RouteType route_type_;
};

struct AddStopRequest : public ModifyReqeust {
  AddStopRequest(std::string_view from_string);
  void Process(BusDatabase& db) const override;
  std::string name_;
  Stop stop_;
};

struct BusRequest : public ReadReqeust<std::string> {
  BusRequest(std::string_view from_string);
  std::string bus_name_;
  std::string Process(const BusDatabase& db) const override;
};

class BusDatabaseHandler {
public:
  using Requests = std::vector<std::unique_ptr<Request>>;
  using Respones = std::vector<std::string>;

  BusDatabaseHandler() = default;
  BusDatabaseHandler& RequestsFromStream(int count, std::istream& is = std::cin);
  BusDatabaseHandler& ProcessRequests();
  const Respones& GetResponses() {
    return responses_;
  }
private:
  Requests requests_;
  Respones responses_;
  BusDatabase db;
};