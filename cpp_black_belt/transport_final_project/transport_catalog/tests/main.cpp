#include <iostream>
#include "test_runner.h"
#include "misc.h"
#include "../io.h"
#include "json.h"
#include <string>
#include <iomanip>
#include "graph.h"
#include "router.h"

using namespace std;

 stringstream input_base{R"(
{
    "serialization_settings": {
        "file": "ser.bin"
    },
    "routing_settings": {
        "bus_wait_time": 2,
        "bus_velocity": 30
    },
    "render_settings": {
        "width": 1200,
        "height": 500,
        "padding": 50,
        "outer_margin": 150,
        "stop_radius": 5,
        "line_width": 14,
        "bus_label_font_size": 20,
        "bus_label_offset": [
            7,
            15
        ],
        "stop_label_font_size": 18,
        "stop_label_offset": [
            7,
            -3
        ],
        "underlayer_color": [
            255,
            255,
            255,
            0.85
        ],
        "underlayer_width": 3,
        "color_palette": [
            "green",
            [
                255,
                160,
                0
            ],
            "red"
        ],
        "layers": [
            "bus_lines",
            "bus_labels",
            "stop_points",
            "stop_labels"
        ]
    },
    "base_requests": [
        {
            "type": "Bus",
            "name": "14",
            "stops": [
                "Улица Лизы Чайкиной",
                "Электросети",
                "Ривьерский мост",
                "Гостиница Сочи",
                "Кубанская улица",
                "По требованию",
                "Улица Докучаева",
                "Улица Лизы Чайкиной"
            ],
            "is_roundtrip": true
        },
        {
            "type": "Bus",
            "name": "24",
            "stops": [
                "Улица Докучаева",
                "Параллельная улица",
                "Электросети",
                "Санаторий Родина"
            ],
            "is_roundtrip": false
        },
        {
            "type": "Bus",
            "name": "114",
            "stops": [
                "Морской вокзал",
                "Ривьерский мост"
            ],
            "is_roundtrip": false
        },
        {
            "type": "Stop",
            "name": "Улица Лизы Чайкиной",
            "latitude": 43.590317,
            "longitude": 39.746833,
            "road_distances": {
                "Электросети": 4300,
                "Улица Докучаева": 2000
            }
        },
        {
            "type": "Stop",
            "name": "Морской вокзал",
            "latitude": 43.581969,
            "longitude": 39.719848,
            "road_distances": {
                "Ривьерский мост": 850
            }
        },
        {
            "type": "Stop",
            "name": "Электросети",
            "latitude": 43.598701,
            "longitude": 39.730623,
            "road_distances": {
                "Санаторий Родина": 4500,
                "Параллельная улица": 1200,
                "Ривьерский мост": 1900
            }
        },
        {
            "type": "Stop",
            "name": "Ривьерский мост",
            "latitude": 43.587795,
            "longitude": 39.716901,
            "road_distances": {
                "Морской вокзал": 850,
                "Гостиница Сочи": 1740
            }
        },
        {
            "type": "Stop",
            "name": "Гостиница Сочи",
            "latitude": 43.578079,
            "longitude": 39.728068,
            "road_distances": {
                "Кубанская улица": 320
            }
        },
        {
            "type": "Stop",
            "name": "Кубанская улица",
            "latitude": 43.578509,
            "longitude": 39.730959,
            "road_distances": {
                "По требованию": 370
            }
        },
        {
            "type": "Stop",
            "name": "По требованию",
            "latitude": 43.579285,
            "longitude": 39.733742,
            "road_distances": {
                "Улица Докучаева": 600
            }
        },
        {
            "type": "Stop",
            "name": "Улица Докучаева",
            "latitude": 43.585586,
            "longitude": 39.733879,
            "road_distances": {
                "Параллельная улица": 1100
            }
        },
        {
            "type": "Stop",
            "name": "Параллельная улица",
            "latitude": 43.590041,
            "longitude": 39.732886,
            "road_distances": {}
        },
        {
            "type": "Stop",
            "name": "Санаторий Родина",
            "latitude": 43.601202,
            "longitude": 39.715498,
            "road_distances": {}
        }
    ]
}
  )"};
  stringstream input_stats{R"(
{
    "serialization_settings": {
        "file": "ser.bin"
    },
    "stat_requests": [
        {
            "id": 218563507,
            "type": "Bus",
            "name": "14"
        },
        {
            "id": 508658276,
            "type": "Stop",
            "name": "Электросети"
        },
        {
            "id": 1964680131,
            "type": "Route",
            "from": "Морской вокзал",
            "to": "Параллельная улица"
        },
        {
            "id": 1359372752,
            "type": "Map"
        }
    ]
}
  )"};

  string expected{R"([{"curvature": 1.60481,"request_id": 218563507,"route_length": 11230,"stop_count": 8,"unique_stop_count": 7},{"buses": ["14","24"],"request_id": 508658276},{"items": [{"stop_name": "Морской вокзал","time": 2,"type": "WaitBus"},{"bus": "114","span_count": 1,"time": 1.7,"type": "RideBus"},{"stop_name": "Ривьерский мост","time": 2,"type": "WaitBus"},{"bus": "14","span_count": 4,"time": 6.06,"type": "RideBus"},{"stop_name": "Улица Докучаева","time": 2,"type": "WaitBus"},{"bus": "24","span_count": 1,"time": 2.2,"type": "RideBus"}],"map": "<?xml version=\"1.0\" encoding=\"UTF-8\" ?><svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"2000\" height=\"1000\"><polyline fill=\"none\" stroke=\"green\" stroke-width=\"14\" stroke-linecap=\"round\" stroke-linejoin=\"round\" points=\"270,450 50,183.333 270,450 \"/><polyline fill=\"none\" stroke=\"rgb(255,160,0)\" stroke-width=\"14\" stroke-linecap=\"round\" stroke-linejoin=\"round\" points=\"1150,383.333 270,116.667 50,183.333 270,250 490,316.667 710,383.333 930,450 1150,383.333 \"/><polyline fill=\"none\" stroke=\"red\" stroke-width=\"14\" stroke-linecap=\"round\" stroke-linejoin=\"round\" points=\"930,450 490,383.333 270,116.667 50,50 270,116.667 490,383.333 930,450 \"/><text fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" x=\"270\" y=\"450\" dx=\"7\" dy=\"15\" font-size=\"20\" font-family=\"Verdana\"  font-weight=\"bold\" >114</text><text fill=\"green\" stroke=\"none\" stroke-width=\"1\" x=\"270\" y=\"450\" dx=\"7\" dy=\"15\" font-size=\"20\" font-family=\"Verdana\"  font-weight=\"bold\" >114</text><text fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" x=\"50\" y=\"183.333\" dx=\"7\" dy=\"15\" font-size=\"20\" font-family=\"Verdana\"  font-weight=\"bold\" >114</text><text fill=\"green\" stroke=\"none\" stroke-width=\"1\" x=\"50\" y=\"183.333\" dx=\"7\" dy=\"15\" font-size=\"20\" font-family=\"Verdana\"  font-weight=\"bold\" >114</text><text fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" x=\"1150\" y=\"383.333\" dx=\"7\" dy=\"15\" font-size=\"20\" font-family=\"Verdana\"  font-weight=\"bold\" >14</text><text fill=\"rgb(255,160,0)\" stroke=\"none\" stroke-width=\"1\" x=\"1150\" y=\"383.333\" dx=\"7\" dy=\"15\" font-size=\"20\" font-family=\"Verdana\"  font-weight=\"bold\" >14</text><text fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" x=\"930\" y=\"450\" dx=\"7\" dy=\"15\" font-size=\"20\" font-family=\"Verdana\"  font-weight=\"bold\" >24</text><text fill=\"red\" stroke=\"none\" stroke-width=\"1\" x=\"930\" y=\"450\" dx=\"7\" dy=\"15\" font-size=\"20\" font-family=\"Verdana\"  font-weight=\"bold\" >24</text><text fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" x=\"50\" y=\"50\" dx=\"7\" dy=\"15\" font-size=\"20\" font-family=\"Verdana\"  font-weight=\"bold\" >24</text><text fill=\"red\" stroke=\"none\" stroke-width=\"1\" x=\"50\" y=\"50\" dx=\"7\" dy=\"15\" font-size=\"20\" font-family=\"Verdana\"  font-weight=\"bold\" >24</text><circle fill=\"white\" stroke=\"none\" stroke-width=\"1\" cx=\"270\" cy=\"250\" r=\"5\" /><circle fill=\"white\" stroke=\"none\" stroke-width=\"1\" cx=\"490\" cy=\"316.667\" r=\"5\" /><circle fill=\"white\" stroke=\"none\" stroke-width=\"1\" cx=\"270\" cy=\"450\" r=\"5\" /><circle fill=\"white\" stroke=\"none\" stroke-width=\"1\" cx=\"490\" cy=\"383.333\" r=\"5\" /><circle fill=\"white\" stroke=\"none\" stroke-width=\"1\" cx=\"710\" cy=\"383.333\" r=\"5\" /><circle fill=\"white\" stroke=\"none\" stroke-width=\"1\" cx=\"50\" cy=\"183.333\" r=\"5\" /><circle fill=\"white\" stroke=\"none\" stroke-width=\"1\" cx=\"50\" cy=\"50\" r=\"5\" /><circle fill=\"white\" stroke=\"none\" stroke-width=\"1\" cx=\"930\" cy=\"450\" r=\"5\" /><circle fill=\"white\" stroke=\"none\" stroke-width=\"1\" cx=\"1150\" cy=\"383.333\" r=\"5\" /><circle fill=\"white\" stroke=\"none\" stroke-width=\"1\" cx=\"270\" cy=\"116.667\" r=\"5\" /><text fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" x=\"270\" y=\"250\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >Гостиница Сочи</text><text fill=\"black\" stroke=\"none\" stroke-width=\"1\" x=\"270\" y=\"250\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >Гостиница Сочи</text><text fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" x=\"490\" y=\"316.667\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >Кубанская улица</text><text fill=\"black\" stroke=\"none\" stroke-width=\"1\" x=\"490\" y=\"316.667\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >Кубанская улица</text><text fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" x=\"270\" y=\"450\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >Морской вокзал</text><text fill=\"black\" stroke=\"none\" stroke-width=\"1\" x=\"270\" y=\"450\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >Морской вокзал</text><text fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" x=\"490\" y=\"383.333\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >Параллельная улица</text><text fill=\"black\" stroke=\"none\" stroke-width=\"1\" x=\"490\" y=\"383.333\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >Параллельная улица</text><text fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" x=\"710\" y=\"383.333\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >По требованию</text><text fill=\"black\" stroke=\"none\" stroke-width=\"1\" x=\"710\" y=\"383.333\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >По требованию</text><text fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" x=\"50\" y=\"183.333\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >Ривьерский мост</text><text fill=\"black\" stroke=\"none\" stroke-width=\"1\" x=\"50\" y=\"183.333\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >Ривьерский мост</text><text fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" x=\"50\" y=\"50\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >Санаторий Родина</text><text fill=\"black\" stroke=\"none\" stroke-width=\"1\" x=\"50\" y=\"50\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >Санаторий Родина</text><text fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" x=\"930\" y=\"450\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >Улица Докучаева</text><text fill=\"black\" stroke=\"none\" stroke-width=\"1\" x=\"930\" y=\"450\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >Улица Докучаева</text><text fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" x=\"1150\" y=\"383.333\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >Улица Лизы Чайкиной</text><text fill=\"black\" stroke=\"none\" stroke-width=\"1\" x=\"1150\" y=\"383.333\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >Улица Лизы Чайкиной</text><text fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" x=\"270\" y=\"116.667\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >Электросети</text><text fill=\"black\" stroke=\"none\" stroke-width=\"1\" x=\"270\" y=\"116.667\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >Электросети</text><rect fill=\"rgba(255,255,255,0.85)\" stroke=\"none\" stroke-width=\"1\" x=\"-150\" y=\"-150\" height=\"800\" width=\"1500\" /><polyline fill=\"none\" stroke=\"green\" stroke-width=\"14\" stroke-linecap=\"round\" stroke-linejoin=\"round\" points=\"270,450 50,183.333 \"/><polyline fill=\"none\" stroke=\"rgb(255,160,0)\" stroke-width=\"14\" stroke-linecap=\"round\" stroke-linejoin=\"round\" points=\"50,183.333 270,250 490,316.667 710,383.333 930,450 \"/><polyline fill=\"none\" stroke=\"red\" stroke-width=\"14\" stroke-linecap=\"round\" stroke-linejoin=\"round\" points=\"930,450 490,383.333 \"/><text fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" x=\"270\" y=\"450\" dx=\"7\" dy=\"15\" font-size=\"20\" font-family=\"Verdana\"  font-weight=\"bold\" >114</text><text fill=\"green\" stroke=\"none\" stroke-width=\"1\" x=\"270\" y=\"450\" dx=\"7\" dy=\"15\" font-size=\"20\" font-family=\"Verdana\"  font-weight=\"bold\" >114</text><text fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" x=\"50\" y=\"183.333\" dx=\"7\" dy=\"15\" font-size=\"20\" font-family=\"Verdana\"  font-weight=\"bold\" >114</text><text fill=\"green\" stroke=\"none\" stroke-width=\"1\" x=\"50\" y=\"183.333\" dx=\"7\" dy=\"15\" font-size=\"20\" font-family=\"Verdana\"  font-weight=\"bold\" >114</text><text fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" x=\"930\" y=\"450\" dx=\"7\" dy=\"15\" font-size=\"20\" font-family=\"Verdana\"  font-weight=\"bold\" >24</text><text fill=\"red\" stroke=\"none\" stroke-width=\"1\" x=\"930\" y=\"450\" dx=\"7\" dy=\"15\" font-size=\"20\" font-family=\"Verdana\"  font-weight=\"bold\" >24</text><circle fill=\"white\" stroke=\"none\" stroke-width=\"1\" cx=\"270\" cy=\"450\" r=\"5\" /><circle fill=\"white\" stroke=\"none\" stroke-width=\"1\" cx=\"50\" cy=\"183.333\" r=\"5\" /><circle fill=\"white\" stroke=\"none\" stroke-width=\"1\" cx=\"50\" cy=\"183.333\" r=\"5\" /><circle fill=\"white\" stroke=\"none\" stroke-width=\"1\" cx=\"270\" cy=\"250\" r=\"5\" /><circle fill=\"white\" stroke=\"none\" stroke-width=\"1\" cx=\"490\" cy=\"316.667\" r=\"5\" /><circle fill=\"white\" stroke=\"none\" stroke-width=\"1\" cx=\"710\" cy=\"383.333\" r=\"5\" /><circle fill=\"white\" stroke=\"none\" stroke-width=\"1\" cx=\"930\" cy=\"450\" r=\"5\" /><circle fill=\"white\" stroke=\"none\" stroke-width=\"1\" cx=\"930\" cy=\"450\" r=\"5\" /><circle fill=\"white\" stroke=\"none\" stroke-width=\"1\" cx=\"490\" cy=\"383.333\" r=\"5\" /><text fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" x=\"270\" y=\"450\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >Морской вокзал</text><text fill=\"black\" stroke=\"none\" stroke-width=\"1\" x=\"270\" y=\"450\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >Морской вокзал</text><text fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" x=\"50\" y=\"183.333\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >Ривьерский мост</text><text fill=\"black\" stroke=\"none\" stroke-width=\"1\" x=\"50\" y=\"183.333\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >Ривьерский мост</text><text fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" x=\"930\" y=\"450\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >Улица Докучаева</text><text fill=\"black\" stroke=\"none\" stroke-width=\"1\" x=\"930\" y=\"450\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >Улица Докучаева</text><text fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" x=\"490\" y=\"383.333\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >Параллельная улица</text><text fill=\"black\" stroke=\"none\" stroke-width=\"1\" x=\"490\" y=\"383.333\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >Параллельная улица</text></svg>","request_id": 1964680131,"total_time": 15.96},{"map": "<?xml version=\"1.0\" encoding=\"UTF-8\" ?><svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"2000\" height=\"1000\"><polyline fill=\"none\" stroke=\"green\" stroke-width=\"14\" stroke-linecap=\"round\" stroke-linejoin=\"round\" points=\"270,450 50,183.333 270,450 \"/><polyline fill=\"none\" stroke=\"rgb(255,160,0)\" stroke-width=\"14\" stroke-linecap=\"round\" stroke-linejoin=\"round\" points=\"1150,383.333 270,116.667 50,183.333 270,250 490,316.667 710,383.333 930,450 1150,383.333 \"/><polyline fill=\"none\" stroke=\"red\" stroke-width=\"14\" stroke-linecap=\"round\" stroke-linejoin=\"round\" points=\"930,450 490,383.333 270,116.667 50,50 270,116.667 490,383.333 930,450 \"/><text fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" x=\"270\" y=\"450\" dx=\"7\" dy=\"15\" font-size=\"20\" font-family=\"Verdana\"  font-weight=\"bold\" >114</text><text fill=\"green\" stroke=\"none\" stroke-width=\"1\" x=\"270\" y=\"450\" dx=\"7\" dy=\"15\" font-size=\"20\" font-family=\"Verdana\"  font-weight=\"bold\" >114</text><text fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" x=\"50\" y=\"183.333\" dx=\"7\" dy=\"15\" font-size=\"20\" font-family=\"Verdana\"  font-weight=\"bold\" >114</text><text fill=\"green\" stroke=\"none\" stroke-width=\"1\" x=\"50\" y=\"183.333\" dx=\"7\" dy=\"15\" font-size=\"20\" font-family=\"Verdana\"  font-weight=\"bold\" >114</text><text fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" x=\"1150\" y=\"383.333\" dx=\"7\" dy=\"15\" font-size=\"20\" font-family=\"Verdana\"  font-weight=\"bold\" >14</text><text fill=\"rgb(255,160,0)\" stroke=\"none\" stroke-width=\"1\" x=\"1150\" y=\"383.333\" dx=\"7\" dy=\"15\" font-size=\"20\" font-family=\"Verdana\"  font-weight=\"bold\" >14</text><text fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" x=\"930\" y=\"450\" dx=\"7\" dy=\"15\" font-size=\"20\" font-family=\"Verdana\"  font-weight=\"bold\" >24</text><text fill=\"red\" stroke=\"none\" stroke-width=\"1\" x=\"930\" y=\"450\" dx=\"7\" dy=\"15\" font-size=\"20\" font-family=\"Verdana\"  font-weight=\"bold\" >24</text><text fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" x=\"50\" y=\"50\" dx=\"7\" dy=\"15\" font-size=\"20\" font-family=\"Verdana\"  font-weight=\"bold\" >24</text><text fill=\"red\" stroke=\"none\" stroke-width=\"1\" x=\"50\" y=\"50\" dx=\"7\" dy=\"15\" font-size=\"20\" font-family=\"Verdana\"  font-weight=\"bold\" >24</text><circle fill=\"white\" stroke=\"none\" stroke-width=\"1\" cx=\"270\" cy=\"250\" r=\"5\" /><circle fill=\"white\" stroke=\"none\" stroke-width=\"1\" cx=\"490\" cy=\"316.667\" r=\"5\" /><circle fill=\"white\" stroke=\"none\" stroke-width=\"1\" cx=\"270\" cy=\"450\" r=\"5\" /><circle fill=\"white\" stroke=\"none\" stroke-width=\"1\" cx=\"490\" cy=\"383.333\" r=\"5\" /><circle fill=\"white\" stroke=\"none\" stroke-width=\"1\" cx=\"710\" cy=\"383.333\" r=\"5\" /><circle fill=\"white\" stroke=\"none\" stroke-width=\"1\" cx=\"50\" cy=\"183.333\" r=\"5\" /><circle fill=\"white\" stroke=\"none\" stroke-width=\"1\" cx=\"50\" cy=\"50\" r=\"5\" /><circle fill=\"white\" stroke=\"none\" stroke-width=\"1\" cx=\"930\" cy=\"450\" r=\"5\" /><circle fill=\"white\" stroke=\"none\" stroke-width=\"1\" cx=\"1150\" cy=\"383.333\" r=\"5\" /><circle fill=\"white\" stroke=\"none\" stroke-width=\"1\" cx=\"270\" cy=\"116.667\" r=\"5\" /><text fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" x=\"270\" y=\"250\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >Гостиница Сочи</text><text fill=\"black\" stroke=\"none\" stroke-width=\"1\" x=\"270\" y=\"250\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >Гостиница Сочи</text><text fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" x=\"490\" y=\"316.667\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >Кубанская улица</text><text fill=\"black\" stroke=\"none\" stroke-width=\"1\" x=\"490\" y=\"316.667\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >Кубанская улица</text><text fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" x=\"270\" y=\"450\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >Морской вокзал</text><text fill=\"black\" stroke=\"none\" stroke-width=\"1\" x=\"270\" y=\"450\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >Морской вокзал</text><text fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" x=\"490\" y=\"383.333\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >Параллельная улица</text><text fill=\"black\" stroke=\"none\" stroke-width=\"1\" x=\"490\" y=\"383.333\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >Параллельная улица</text><text fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" x=\"710\" y=\"383.333\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >По требованию</text><text fill=\"black\" stroke=\"none\" stroke-width=\"1\" x=\"710\" y=\"383.333\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >По требованию</text><text fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" x=\"50\" y=\"183.333\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >Ривьерский мост</text><text fill=\"black\" stroke=\"none\" stroke-width=\"1\" x=\"50\" y=\"183.333\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >Ривьерский мост</text><text fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" x=\"50\" y=\"50\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >Санаторий Родина</text><text fill=\"black\" stroke=\"none\" stroke-width=\"1\" x=\"50\" y=\"50\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >Санаторий Родина</text><text fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" x=\"930\" y=\"450\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >Улица Докучаева</text><text fill=\"black\" stroke=\"none\" stroke-width=\"1\" x=\"930\" y=\"450\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >Улица Докучаева</text><text fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" x=\"1150\" y=\"383.333\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >Улица Лизы Чайкиной</text><text fill=\"black\" stroke=\"none\" stroke-width=\"1\" x=\"1150\" y=\"383.333\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >Улица Лизы Чайкиной</text><text fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" x=\"270\" y=\"116.667\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >Электросети</text><text fill=\"black\" stroke=\"none\" stroke-width=\"1\" x=\"270\" y=\"116.667\" dx=\"7\" dy=\"-3\" font-size=\"18\" font-family=\"Verdana\" >Электросети</text></svg>","request_id": 1359372752}])"};


void TestSerialize() {
    Json::Document doc = Json::Load(input_base);
    TransportCatalog handler(doc);
    handler.ProcessRequests();
    handler.Serialize();
}

void TestDeserialize() {
    ostringstream os;
    Json::Document doc = Json::Load(input_stats);
    TransportCatalog handler(doc);
    auto responses = handler.Deserialize().ProcessRequests().GetResponses();
    cout << setprecision(6);
    Json::Print(responses, os);
    ASSERT_EQUAL(os.str(), expected);
}

int main(void) {
    TestRunner tr;
    RUN_TEST(tr,TestSerialize);
    RUN_TEST(tr,TestDeserialize);
    return 0;
}