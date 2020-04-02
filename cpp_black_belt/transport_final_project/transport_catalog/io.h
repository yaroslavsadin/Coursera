#pragma once

#include <iostream>
#include <fstream>
#include "database.h"
#include "transport_router.h"
#include "misc.h"
#include "json.h"
#include "svg_render.h"
#include "yp_index.h"
#include <stdexcept>
#include "req_impl.h"
#ifdef DEBUG
#include "profile_advanced.h"
#endif

#include "db.pb.h"
#include "router.pb.h"
#include "renderer.pb.h"
#include "transport_catalog.pb.h"

/******************************* 
    REQUEST BASE CLASS         *
********************************/

struct Request {
  enum class Type {
    ADD_BUS,
    ADD_STOP,
    GET_BUS_INFO,
    GET_STOP_INFO,
    GET_ROUTE,
    GET_MAP,
    FIND_COMPANIES,
    ROUTE_TO_COMPANY
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
  virtual void Process() const = 0;
};

/******************************* 
    READ REQUEST BASE CLASS    *
********************************/

template<typename ResT>
struct ReadReqeust : public Request {
  ReadReqeust(Request::Type type_) : Request(type_) {}
  ReadReqeust(size_t id, Request::Type type_) : Request(type_), id_(id) {}
  virtual ResT Process() const = 0;
  std::optional<int> id_;
};

/******************************* 
        MODIFY REQUESTS        *
********************************/

struct AddBusRequest : public ModifyReqeust {
  AddBusRequest(const Json::Node& json_node, BusDatabase& db);
  static Bus::RouteType GetRouteType(std::string_view request);
  static Bus::RouteType GetRouteType(const Json::Node& json_node);
  void Process() const override;
  std::string bus_name_;
  std::vector<std::string> stops_;
  Bus::RouteType route_type_;
private:
  BusDatabase& db;
};

using StopDistances = std::vector< std::pair< std::string, unsigned int > >;
struct AddStopRequest : public ModifyReqeust {
  AddStopRequest(const Json::Node& json_node, BusDatabase& db);
  void Process() const override;
  std::string name_;
  double latitude;
  double longtitude;
  StopDistances distances_to_stops_;
private:
  BusDatabase& db;
};

/******************************* 
         READ REQUESTS         *
********************************/

struct BusRequest : public ReadReqeust<Json::Node> {
  BusRequest(const Json::Node& from_json_node, const BusDatabase& db);
  std::string bus_name_;
  Json::Node Process() const override;
private:
  const BusDatabase& db;
};

struct StopRequest : public ReadReqeust<Json::Node> {
  StopRequest(const Json::Node& from_json_node, const BusDatabase& db);
  std::string stop_name_;
  Json::Node Process() const override;
private:
  const BusDatabase& db;
};

struct RouteRequest : public ReadReqeust<Json::Node> {
  RouteRequest(const Json::Node& from_json_node, const BusDatabase& db, const TransportRouter& router, const SvgRender& renderer);
  Json::Node Process() const override;
private:
  RouteRequestImpl impl;
};

struct MapRequest : public ReadReqeust<Json::Node> {
  MapRequest(const Json::Node& from_json_node, const BusDatabase& db, const SvgRender& renderer);
  Json::Node Process() const override;
private:
  const BusDatabase& db;
  const SvgRender& renderer;
};

struct FindCompaniesRequest : public ReadReqeust<Json::Node> {
  FindCompaniesRequest(const Json::Node& from_json_node, const std::optional<YP::YellowPagesIndex>& index);
  Json::Node Process() const override;
private:
  FindCompaniesRequestImpl impl;
};

struct RouteToCompanyRequest : public ReadReqeust<Json::Node> {
  RouteToCompanyRequest(const Json::Node& from_json_node, const std::optional<YP::YellowPagesIndex>& index,
                const BusDatabase& db, const TransportRouter& router, const SvgRender& renderer);
  Json::Node Process() const override;
private:
  FindCompaniesRequestImpl companies_impl;
  RouteRequestImpl route_impl;
  Time current_time;
};

/******************************* 
       DATABASE WRAPPER        *
********************************/

class TransportCatalog {
public:
  using Requests = std::vector<std::unique_ptr<Request>>;
  using Respones = Json::Node;

  TransportCatalog(Json::Document doc);
  TransportCatalog& ProcessRequests();
  Respones GetResponses() {
    return move(responses_);
  }
  TransportCatalog& Serialize();
  TransportCatalog& Deserialize();
private:
  Requests requests_;
  Respones responses_;
  
  BusDatabase db;
  TransportRouter router;
  std::optional<SvgRender> renderer;
  std::optional<YP::YellowPagesIndex> index;

  ProtoTransport::TransportCatalog proto_catalog;

  std::string serial_file;

  template<typename From>
  std::unique_ptr<Request> MakeRequest(Request::Type request_type, const From& request) {
      switch(request_type) {
          case Request::Type::ADD_BUS:
              return std::make_unique<AddBusRequest>(request,db);
              break;
          case Request::Type::ADD_STOP:
              return std::make_unique<AddStopRequest>(request,db);
              break;
          case Request::Type::GET_BUS_INFO:
              return std::make_unique<BusRequest>(request,db);
              break;
          case Request::Type::GET_STOP_INFO:
              return std::make_unique<StopRequest>(request,db);
              break;
          case Request::Type::GET_ROUTE:
              return std::make_unique<RouteRequest>(request,db,router,*renderer);
              break;
          case Request::Type::GET_MAP:
              return std::make_unique<MapRequest>(request,db,*renderer);
              break;
          case Request::Type::FIND_COMPANIES:
              return std::make_unique<FindCompaniesRequest>(request,index);
              break;
          case Request::Type::ROUTE_TO_COMPANY:
              return std::make_unique<RouteToCompanyRequest>(request,index,db,router,*renderer);
              break;
          default:
              break;
          }
      throw std::invalid_argument("Unknown request type");
  }
};
