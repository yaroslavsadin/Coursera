#pragma once

#include "descriptions.h"
#include "database.h"
#include "router.h"
#include "svg_render.h"

template<typename FromType, typename ToType>
class RouteRequestImpl {
public:
    RouteRequestImpl(
        FromType from,
        ToType to, 
        const BusDatabase& db, 
        const TransportRouter& router , 
        const SvgRender& renderer
    )
    : from_(from), to_(to), db(db), router(router), renderer(renderer)
    {}

    auto BuildRoute() const {
        return router.BuildRoute(db.GetBuses(),db.GetStops(),from_,to_);
    }

    void SetTo(ToType to) {
        to_ = to;
    }

    std::map<std::string,Json::Node> Process() const {
        auto route = BuildRoute();
        map<string,Json::Node> res;
        if(route) {
            size_t route_id = route->id;
            size_t num_edges = route->edge_count;

            SvgRender::RouteMap route_map;
            if(!num_edges) {
                res["total_time"] = 0;
                res["items"] = vector<Json::Node>();
            } else {
                res["total_time"] = Json::Node(route->weight);

                vector<Json::Node> items;
                for(size_t i = 0; i < num_edges;i++) {
                    const size_t edge_info_idx = router.GetRouteEdgeId(route_id,i);
                    const EdgeInfo& edge_info = router.GetEdgeInfo(edge_info_idx);
                    switch(edge_info.type_) {
                    case EdgeType::CHANGE:
                        items.push_back(map<string,Json::Node> {
                            {"stop_name", edge_info.item_name_},
                            {"time", router.GetEdgeWeight(edge_info_idx)},
                            {"type", string("WaitBus")}
                        });
                        break;
                    case EdgeType::RIDE:
                        items.push_back(map<string,Json::Node> {
                            {"bus", edge_info.item_name_},
                            {"time", router.GetEdgeWeight(edge_info_idx)},
                            {"span_count", edge_info.span_count_},
                            {"type", string("RideBus")}
                        });
                        route_map.push_back(&edge_info);
                        break;
                     case EdgeType::WALK:
                        items.push_back(map<string,Json::Node> {
                            {"company", edge_info.item_name_},
                            {"time", router.GetEdgeWeight(edge_info_idx)},
                            {"stop_name", edge_info.item_name_},
                            {"type", string("WalkToCompany")}
                        });
                        route_map.push_back(&edge_info);
                        break;
                    default:
                        throw runtime_error("Wrong edge type");
                        break;
                    }
                }
                res["items"] = move(items);
            }
            // stringstream ss;
            // renderer.RenderRoute(std::move(route_map)).Render(ss);
            // res["map"] = Json::Node(ss.str());
        } else {
            res["error_message"] = Json::Node(string("not found"));
        }
        return res;
    }
private:
    FromType from_;
    ToType to_;
    const BusDatabase& db;
    const TransportRouter& router;
    const SvgRender& renderer;
};

class FindCompaniesRequestImpl {
public:
    FindCompaniesRequestImpl(const Json::Node& from_json_node, const std::optional<YP::YellowPagesIndex>& index)
    : index(index)
    {
        auto f_add_strings_request = [this](YP::RequestItem::Type type, const auto& strings) 
        {
            std::vector<std::string> data;
            data.reserve(strings.size());
            for(const auto& rubric : strings) {
                data.push_back(rubric.AsString());
            }
            requests.emplace_back(YP::RequestItem{type,std::move(data)});
        };

        const auto& as_map = from_json_node.AsMap();
        if(as_map.count("names")) {
            f_add_strings_request(YP::RequestItem::Type::NAMES,as_map.at("names").AsArray());
        }
        if(as_map.count("urls")) {
            f_add_strings_request(YP::RequestItem::Type::URLS,as_map.at("urls").AsArray());
        }
        if(as_map.count("rubrics")) {
            f_add_strings_request(YP::RequestItem::Type::RUBRICS,as_map.at("rubrics").AsArray());
        }
        if(as_map.count("phones")) {
            const auto& phones = as_map.at("phones").AsArray();
            std::vector<YP::Phone> data;
            data.reserve(phones.size());
            for(const auto& phone_node : phones) {
                const auto& phone_as_map = phone_node.AsMap();
                auto& phone = data.emplace_back();
                if(phone_as_map.count("type")) {
                    if(phone_as_map.at("type").AsString() == "PHONE") {
                        phone.type = YP::Phone::Type::PHONE;
                    } else {
                        phone.type = YP::Phone::Type::FAX;
                    }
                }
                if(phone_as_map.count("number")) {
                    phone.number = phone_as_map.at("number").AsString();
                }
                if(phone_as_map.count("country_code")) {
                    phone.country_code = phone_as_map.at("country_code").AsString();
                }
                if(phone_as_map.count("local_code")) {
                    phone.local_code =  phone_as_map.at("local_code").AsString();
                }
                if(phone_as_map.count("extension")) {
                    phone.extension = phone_as_map.at("extension").AsString();
                }
            }
            requests.emplace_back(YP::RequestItem{YP::RequestItem::Type::PHONES,std::move(data)});
        }
    }

    auto FilterCompanies() const {
        return index->Search(requests);
    }

    auto Process() const {
        std::map<string,Json::Node> res;
        std::vector<Json::Node> companies;
        auto indices = index->Search(requests);
        companies.reserve(indices.size());
        for(size_t idx : indices) {
            companies.emplace_back(index->CompanyNameByIdx(idx));
        }
        res["companies"] = std::move(companies);
        return res;
    }
private:
    std::vector<YP::RequestItem> requests;
    const std::optional<YP::YellowPagesIndex>& index;
};