#include <iostream>
#include <sstream>
#include <fstream>
#include "database.pb.h"
#include "yp_index.h"
#include "yp_serialize.h"
#include "json.h"

std::stringstream input{
R"(
{
"yellow_pages": {
        "rubrics": {
            "1": {
                "name": "Парк"
            }
        },
        "companies": [
            {
                "names": [
                    {
                        "value": "Дендрарий"
                    }
                ],
                "urls": [
                    {
                        "value": "http://dendrarium.ru"
                    }
                ],
                "rubrics": [
                    1
                ],
                "address": {
                    "coords": {
                        "lat": 43.573226,
                        "lon": 39.742947
                    }
                },
                "nearby_stops": [
                    {
                        "name": "Цирк",
                        "meters": 180
                    }
                ],
                "phones": [
                    {
                        "type": "PHONE",
                        "country_code": "7",
                        "local_code": "862",
                        "number": "2671646"
                    },
                    {
                        "type": "PHONE",
                        "country_code": "7",
                        "local_code": "862",
                        "number": "2671842"
                    }
                ]
            },
            {
                "names": [
                    {
                        "value": "им. Фрунзе"
                    }
                ],
                "rubrics": [
                    1
                ],
                "address": {
                    "coords": {
                        "lat": 43.567998,
                        "lon": 39.734131
                    }
                },
                "nearby_stops": [
                    {
                        "name": "Пансионат Светлана",
                        "meters": 580
                    },
                    {
                        "name": "Цирк",
                        "meters": 700
                    },
                    {
                        "name": "Театральная",
                        "meters": 1000
                    }
                ]
            }
        ]
    }
}
)"
};

int main(void) {
    Json::Document doc = Json::Load(input);
    {
        std::fstream ser_file("ser.bin", std::ios::binary | std::ios::out);
        YP::Serialize(doc, ser_file);
    }
    {
        std::fstream ser_file("ser.bin", std::ios::binary | std::ios::in);
        YellowPages::Database db;
        db.ParseFromIstream(&ser_file);
        YP::YellowPagesIndex index(db);
    }
    return 0;
}