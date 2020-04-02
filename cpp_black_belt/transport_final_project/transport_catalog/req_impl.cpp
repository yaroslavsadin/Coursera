#include "req_impl.h"
#include "time_interval.h"

RouteRequestImpl::RouteRequestImpl(
    std::string from,
    std::string to, 
    const BusDatabase& db, 
    const TransportRouter& router , 
    const SvgRender& renderer
)
: from_(from), to_(to), db(db), router(router), renderer(renderer)
{}

std::map<std::string,Json::Node> RouteRequestImpl::Build(
    const std::vector<std::string_view> companies, const Time& current_time, const YP::Companies& companies_descriptions
) const {
    std::vector<RouterT::RouteInfo> route_ids;
    for(auto company_name : companies) {
        auto route = router.BuildRouteToCompany(db.GetBuses(),db.GetStops(),from_,company_name);
        if(route) {
            route_ids.push_back(std::move(*route));
        }
    }
    if(route_ids.empty()) {
        std::map<std::string,Json::Node> res;
        res["error_message"] = Json::Node(std::string("not found"));
        return res;
    } else {
        auto it = std::min_element(route_ids.begin(),route_ids.end(),
        [this,current_time,&companies_descriptions](const auto& lhs, const auto& rhs){
            auto lhs_company_name = router.GetEdgeInfo(router.GetRouteEdgeId(lhs.id,lhs.edge_count-1)).company_name_;
            auto rhs_company_name = router.GetEdgeInfo(router.GetRouteEdgeId(rhs.id,rhs.edge_count-1)).company_name_;
            return 
            lhs.weight + GetWaitingTime(current_time + lhs.weight, companies_descriptions.at(lhs_company_name).intervals) 
            < rhs.weight + GetWaitingTime(current_time + rhs.weight, companies_descriptions.at(rhs_company_name).intervals);
        });
        return BuildResponse(*it);
    }
}

std::map<std::string,Json::Node> RouteRequestImpl::Build() const {
    auto route = router.BuildRoute(db.GetBuses(),db.GetStops(),from_,to_);
    if(route) {
        return BuildResponse(*route);
    } else {
        std::map<std::string,Json::Node> res;
        res["error_message"] = Json::Node(std::string("not found"));
        return res;
    }
}

std::map<std::string,Json::Node> RouteRequestImpl::BuildResponse(const RouterT::RouteInfo& route) const {
    std::map<std::string,Json::Node> res;
    size_t route_id = route.id;
    size_t num_edges = route.edge_count;

    SvgRender::RouteMap route_map;
    if(!num_edges) {
        res["total_time"] = 0;
        res["items"] = std::vector<Json::Node>();
    } else {
        res["total_time"] = Json::Node(route.weight);

        std::vector<Json::Node> items;
        for(size_t i = 0; i < num_edges;i++) {
            const size_t edge_info_idx = router.GetRouteEdgeId(route_id,i);
            const EdgeInfo& edge_info = router.GetEdgeInfo(edge_info_idx);
            switch(edge_info.type_) {
            case EdgeType::CHANGE:
                items.push_back(std::map<std::string,Json::Node> {
                    {"stop_name", edge_info.item_name_},
                    {"time", router.GetEdgeWeight(edge_info_idx)},
                    {"type", std::string("WaitBus")}
                });
                break;
            case EdgeType::RIDE:
                items.push_back(std::map<std::string,Json::Node> {
                    {"bus", edge_info.item_name_},
                    {"time", router.GetEdgeWeight(edge_info_idx)},
                    {"span_count", edge_info.span_count_},
                    {"type", std::string("RideBus")}
                });
                route_map.push_back(&edge_info);
                break;
                case EdgeType::WALK:
                items.push_back(std::map<std::string,Json::Node> {
                    {"company", std::string(edge_info.company_name_)},
                    {"time", router.GetEdgeWeight(edge_info_idx)},
                    {"stop_name", edge_info.item_name_},
                    {"type", std::string("WalkToCompany")}
                });
                route_map.push_back(&edge_info);
                break;
            default:
                throw std::runtime_error("Wrong edge type");
                break;
            }
        }
        res["items"] = move(items);
    }
    std::stringstream ss;
    renderer.RenderRoute(std::move(route_map)).Render(ss);
    res["map"] = Json::Node(ss.str());
    return res;
}

FindCompaniesRequestImpl::FindCompaniesRequestImpl(const Json::Node& from_json_node, const std::optional<YP::YellowPagesIndex>& index)
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

std::vector<std::string_view> FindCompaniesRequestImpl::FilterCompanies() const {
    return index->Search(requests);
}

std::map<std::string,Json::Node> FindCompaniesRequestImpl::BuildResponse() const {
    std::map<std::string,Json::Node> res;
    std::vector<Json::Node> companies;
    auto companies_filtered = index->Search(requests);
    companies.reserve(companies_filtered.size());
    for(auto company : companies_filtered) {
        companies.emplace_back(std::string(company));
    }
    res["companies"] = std::move(companies);
    return res;
}