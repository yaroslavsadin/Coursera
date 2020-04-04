#ifdef DEBUG
#include <iostream>
#include "test_runner.h"
#include "misc.h"
#include "io.h"
#include "json.h"
#include <string>
#include <iomanip>
#include "graph.h"
#include "router.h"

using namespace std;

int main(void) {
  stringstream input_base{R"(
{
    "base_requests": [
        {
            "is_roundtrip": true,
            "name": "13",
            "stops": [
                "Ulitsa Lizy Chaykinoy"
            ],
            "type": "Bus"
        },
        {
            "latitude": 43.5903,
            "longitude": 39.7468,
            "name": "Ulitsa Lizy Chaykinoy",
            "road_distances": {},
            "type": "Stop"
        }
    ],
    "render_settings": {
        "bus_label_font_size": 20,
        "bus_label_offset": [
            7,
            15
        ],
        "color_palette": [
            [
                255,
                0,
                255
            ],
            "red",
            "green",
            "blue"
        ],
        "company_line_width": 20,
        "company_radius": 3,
        "height": 2000,
        "layers": [
            "bus_lines",
            "company_lines",
            "bus_labels",
            "stop_points",
            "company_points",
            "stop_labels",
            "company_labels"
        ],
        "line_width": 14,
        "outer_margin": 150,
        "padding": 50,
        "stop_label_font_size": 20,
        "stop_label_offset": [
            7,
            -3
        ],
        "stop_radius": 5,
        "underlayer_color": [
            255,
            255,
            255,
            0.85
        ],
        "underlayer_width": 3,
        "width": 900
    },
    "routing_settings": {
        "bus_velocity": 30,
        "bus_wait_time": 2,
        "pedestrian_velocity": 100
    },
    "serialization_settings": {
        "file": "/tmp/tmpw_2_snm9"
    },
    "yellow_pages": {
        "companies": [
            {
                "address": {
                    "coords": {
                        "lat": "43.567998",
                        "lon": "39.734131"
                    }
                },
                "names": [
                    {
                        "value": "Dendrariy"
                    }
                ],
                "nearby_stops": [
                    {
                        "meters": 580,
                        "name": "Ulitsa Lizy Chaykinoy"
                    }
                ],
                "phones": [
                    {
                        "type": "FAX"
                    }
                ],
                "rubrics": [
                    1
                ],
                "working_time": {
                    "intervals": [
                        {
                            "day": "MONDAY",
                            "minutes_from": 0,
                            "minutes_to": 1440
                        },
                        {
                            "day": "TUESDAY",
                            "minutes_from": 0,
                            "minutes_to": 1440
                        },
                        {
                            "day": "THURSDAY",
                            "minutes_from": 0,
                            "minutes_to": 1440
                        },
                        {
                            "day": "FRIDAY",
                            "minutes_from": 0,
                            "minutes_to": 1440
                        },
                        {
                            "day": "SATURDAY",
                            "minutes_from": 0,
                            "minutes_to": 1440
                        },
                        {
                            "day": "SUNDAY",
                            "minutes_from": 0,
                            "minutes_to": 1440
                        }
                    ]
                }
            },
            {
                "address": {
                    "coords": {
                        "lat": "43.557998",
                        "lon": "39.734131"
                    }
                },
                "names": [
                    {
                        "value": "im. Frunze"
                    }
                ],
                "nearby_stops": [
                    {
                        "meters": 579,
                        "name": "Ulitsa Lizy Chaykinoy"
                    }
                ],
                "rubrics": [
                    1
                ],
                "working_time": {
                    "intervals": [
                        {
                            "day": "EVERYDAY",
                            "minutes_from": 60,
                            "minutes_to": 61
                        },
                        {
                            "day": "EVERYDAY",
                            "minutes_from": 120,
                            "minutes_to": 1200
                        },
                        {
                            "day": "EVERYDAY",
                            "minutes_from": 1201,
                            "minutes_to": 1439
                        }
                    ]
                }
            }
        ],
        "rubrics": {
            "1": {
                "name": "Park"
            },
            "2": {
                "name": "Vokzal"
            }
        }
    }
}
  )"};
  stringstream input_stats{R"(
{
    "serialization_settings": {
        "file": "/tmp/tmpqrfdop7w"
    },
    "stat_requests": [
        {
            "id": 1976763663,
            "type": "Bus",
            "name": "13"
        },
        {
            "id": 1208333195,
            "type": "Stop",
            "name": "Морской вокзал"
        },
        {
            "id": 275341641,
            "type": "FindCompanies",
            "rubrics": [
                "Парк"
            ]
        },
        {
            "id": 1142770959,
            "type": "Route",
            "from": "Краево-Греческая улица",
            "to": "Органный зал"
        },
        {
            "id": 940120142,
            "type": "RouteToCompany",
            "from": "Краево-Греческая улица",
            "companies": {
                "rubrics": [
                    "Парк"
                ]
            },
            "datetime": [
                6,
                8,
                35
            ]
        },
        {
            "id": 1054152420,
            "type": "RouteToCompany",
            "from": "Краево-Греческая улица",
            "companies": {
                "rubrics": [
                    "Парк"
                ]
            },
            "datetime": [
                6,
                8,
                40
            ]
        }
    ]
}
  )"};

    {
        Json::Document doc = Json::Load(input_base);
        TransportCatalog handler(doc);
        handler.ProcessRequests();
        handler.Serialize();
    }
    {
        Json::Document doc = Json::Load(input_stats);
        TransportCatalog handler(doc);
        auto responses = handler.Deserialize().ProcessRequests().GetResponses();
        cout << setprecision(6);
        Json::Print(responses, cout);
    }
    
    return 0;
}
#endif