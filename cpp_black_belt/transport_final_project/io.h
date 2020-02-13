#pragma once

#include <iostream>
#include <fstream>
#include "database.h"
#include "transport_router.h"
#include "misc.h"
#include "json.h"
#include "svg_render.h"
#include <stdexcept>

#include "database.pb.h"
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
    GET_MAP
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
  virtual void Process(BusDatabase& db, TransportRouter& router, SvgRender& renderer) const = 0;
};

/******************************* 
    READ REQUEST BASE CLASS    *
********************************/

template<typename ResT>
struct ReadReqeust : public Request {
  ReadReqeust(Request::Type type_) : Request(type_) {}
  ReadReqeust(size_t id, Request::Type type_) : Request(type_), id_(id) {}
  virtual ResT Process(const BusDatabase& db, const TransportRouter& router, const SvgRender& renderer) const = 0;
  std::optional<int> id_;
};

/******************************* 
        MODIFY REQUESTS        *
********************************/

struct AddBusRequest : public ModifyReqeust {
  AddBusRequest(const Json::Node& json_node);
  static Bus::RouteType GetRouteType(std::string_view request);
  static Bus::RouteType GetRouteType(const Json::Node& json_node);
  void Process(BusDatabase& db, TransportRouter& router, SvgRender& renderer) const override;
  std::string bus_name_;
  std::vector<std::string> stops_;
  Bus::RouteType route_type_;
};

using StopDistances = std::vector< std::pair< std::string, unsigned int > >;
struct AddStopRequest : public ModifyReqeust {
  AddStopRequest(const Json::Node& json_node);
  void Process(BusDatabase& db, TransportRouter& router, SvgRender& renderer) const override;
  std::string name_;
  double latitude;
  double longtitude;
  StopDistances distances_to_stops_;
};

/******************************* 
         READ REQUESTS         *
********************************/

struct BusRequest : public ReadReqeust<Json::Node> {
  BusRequest(const Json::Node& from_json_node);
  std::string bus_name_;
  Json::Node Process(const BusDatabase& db, const TransportRouter& router, const SvgRender& renderer) const override;
};

struct StopRequest : public ReadReqeust<Json::Node> {
  StopRequest(const Json::Node& from_json_node);
  std::string stop_name_;
  Json::Node Process(const BusDatabase& db, const TransportRouter& router, const SvgRender& renderer) const override;
};

struct RouteRequest : public ReadReqeust<Json::Node> {
  RouteRequest(const Json::Node& from_json_node);
  std::string from_;
  std::string to_;
  Json::Node Process(const BusDatabase& db, const TransportRouter& router, const SvgRender& renderer) const override;
};

struct MapRequest : public ReadReqeust<Json::Node> {
  MapRequest(const Json::Node& from_json_node);
  Json::Node Process(const BusDatabase& db, const TransportRouter& router, const SvgRender& renderer) const override;
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
  TransportCatalog& Serialize() {
    ProtoTransport::TransportCatalog t;
    std::ofstream serial(
        serial_file, std::ios::binary
    );
    db.Serialize(*t.mutable_db());
    router.InitRouter(db.GetBuses(),db.GetStops());
    router.Serialize(*t.mutable_router());
    t.SerializeToOstream(&serial);
    assert(!serial.bad());
    return *this;
  }
  TransportCatalog& Deserialize() {
    std::ifstream serial(
        serial_file, std::ios::binary
    );
    ProtoTransport::TransportCatalog t;
    t.ParseFromIstream(&serial);
    assert(!serial.bad());
    db.Deserialize(t.db());
    router.Deserialize(t.router());
    return *this;
  }
private:
  Requests requests_;
  Respones responses_;
  
  BusDatabase db;
  TransportRouter router;
  SvgRender renderer;

  std::string serial_file;
};

struct ColorVisitor {
    Svg::Color operator()(const std::vector<Json::Node>& underlayer_colors_array) const {
        if(underlayer_colors_array.size() == 4) {
            return (Svg::Color(
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
    Svg::Color operator()(const std::string& underlayer_colors_string) const {
        return Svg::Color(underlayer_colors_string);
    }
    Svg::Color operator()(bool) const {
        return Svg::Color();
    }
    Svg::Color operator()(int) const {
        return Svg::Color();
    }
    Svg::Color operator()(double) const {
        return Svg::Color();
    }
    Svg::Color operator()(std::map<std::string, Json::Node>&) const {
        return Svg::Color();
    }
};