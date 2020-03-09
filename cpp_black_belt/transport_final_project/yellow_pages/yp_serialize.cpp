#include "yp_serialize.h"

namespace YP {
    void Serialize(const Json::Document& doc, YellowPages::Database& db) {
        const auto& root = doc.GetRoot().AsMap();
        assert(root.count("yellow_pages"));
        const auto& top = root.at("yellow_pages").AsMap();
        assert(top.count("companies"));
        assert(top.count("rubrics"));

        const auto& companies = top.at("companies").AsArray();
        const auto& rubrics = top.at("rubrics").AsMap();

        auto& proto_companies = *db.mutable_companies();
        auto& proto_rubrics = *db.mutable_rubrics();

        for(auto [k,v] : rubrics) {
            proto_rubrics[atoi(k.c_str())].set_name(v.AsMap().at("name").AsString());
        }

        for(auto& company_node : companies) {
            const auto& company_map = company_node.AsMap();
            auto& proto_company = *proto_companies.Add();

            if(company_map.count("names")) {
                auto& proto_names = *proto_company.mutable_names();
                for(const auto& name_node : company_map.at("names").AsArray()) {
                    const auto& name_map = name_node.AsMap();
                    auto& proto_name = *proto_names.Add();
                    if(name_map.count("type")) {
                        const auto& type = name_map.at("type").AsString();
                        if(type == "MAIN") {
                            proto_name.set_type(YellowPages::Name_Type::Name_Type_MAIN);
                        } else if(type == "SYNONYM") {
                            proto_name.set_type(YellowPages::Name_Type::Name_Type_SYNONYM);
                        } else {
                            proto_name.set_type(YellowPages::Name_Type::Name_Type_SHORT);
                        }
                    } else {
                        proto_name.set_type(YellowPages::Name_Type::Name_Type_MAIN);
                    }
                    proto_name.set_value(name_map.at("value").AsString());
                }
            }

            if(company_map.count("address")) {
                auto& proto_address = *proto_company.mutable_address();
                const auto& address_map = company_map.at("address").AsMap();
                if(address_map.count("components")) {
                    /// TODO:
                }
                if(address_map.count("coords")) {
                    const auto& coords = address_map.at("coords").AsMap();
                    auto& proto_coords = *proto_address.mutable_coords();
                    proto_coords.set_lat(atof(coords.at("lat").AsString().c_str()));
                    proto_coords.set_lon(atof(coords.at("lon").AsString().c_str()));
                }
            }

            if(company_map.count("urls")) {
                auto& proto_urls = *proto_company.mutable_urls();
                for(const auto& url_node : company_map.at("urls").AsArray()) {
                    const auto& url_map = url_node.AsMap();
                    auto& proto_url = *proto_urls.Add();
                    proto_url.set_value(url_map.at("value").AsString());
                }
            }

            if(company_map.count("rubrics")) {
                auto& proto_rubrics = *proto_company.mutable_rubrics();
                for(auto rubrics_node : company_map.at("rubrics").AsArray()) {
                    auto& proto_rubric = *proto_rubrics.Add();
                    proto_rubric = rubrics_node.AsInt();
                }
            }

            if(company_map.count("nearby_stops")) {
                auto& proto_nearby = *proto_company.mutable_nearby_stops();
                for(const auto& nearby_node : company_map.at("nearby_stops").AsArray()) {
                    const auto& nearby_map = nearby_node.AsMap();
                    auto& proto_nearby_stop = *proto_nearby.Add();
                    proto_nearby_stop.set_name(nearby_map.at("name").AsString());
                    proto_nearby_stop.set_meters(nearby_map.at("meters").AsInt());
                }
            }

            if(company_map.count("phones")) {
                auto& proto_phones = *proto_company.mutable_phones();
                for(const auto& phone_node : company_map.at("phones").AsArray()) {
                    const auto& phone_map = phone_node.AsMap();
                    auto& proto_phone = *proto_phones.Add();
                    if(phone_map.count("type")) {
                        if(phone_map.at("type").AsString() == "PHONE") {
                            proto_phone.set_type(YellowPages::Phone_Type_PHONE);
                        } else {
                            proto_phone.set_type(YellowPages::Phone_Type_FAX);
                        }
                    } else {
                        proto_phone.set_type(YellowPages::Phone_Type_PHONE);
                    }
                    if(phone_map.count("country_code")) {
                         proto_phone.set_country_code(phone_map.at("country_code").AsString());
                    }
                    if(phone_map.count("local_code")) {
                        proto_phone.set_local_code(phone_map.at("local_code").AsString());
                    }
                    if(phone_map.count("number")) {
                        proto_phone.set_number(phone_map.at("number").AsString());
                    }
                    if(phone_map.count("extension")) {
                        proto_phone.set_extension(phone_map.at("extension").AsString());
                    }
                    if(phone_map.count("description")) {
                        proto_phone.set_description(phone_map.at("description").AsString());
                    }
                }
            }
        }
    }
}