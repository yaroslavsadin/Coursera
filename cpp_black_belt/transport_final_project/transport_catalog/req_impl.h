#pragma once

#include "descriptions.h"
#include "database.h"
#include "transport_router.h"
#include "svg_render.h"
#include "json.h"
#include "yp_index.h"

class RouteRequestImpl {
public:
    RouteRequestImpl(
        std::string from,
        std::string to, 
        const BusDatabase& db, 
        const TransportRouter& router , 
        const SvgRender& renderer
    );
    std::map<std::string,Json::Node> Build() const;
    std::map<std::string,Json::Node> Build(const std::vector<std::string_view> companies) const;
private:
    std::string from_;
    std::string to_;
    const BusDatabase& db;
    const TransportRouter& router;
    const SvgRender& renderer;

    std::map<std::string,Json::Node> BuildResponse(const RouterT::RouteInfo& route) const;
};

class FindCompaniesRequestImpl {
public:
    FindCompaniesRequestImpl(const Json::Node& from_json_node, const std::optional<YP::YellowPagesIndex>& index);
    std::vector<std::string_view> FilterCompanies() const;
    std::map<std::string,Json::Node> BuildResponse() const;
private:
    std::vector<YP::RequestItem> requests;
    const std::optional<YP::YellowPagesIndex>& index;
};