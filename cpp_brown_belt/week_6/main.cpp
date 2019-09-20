#include <iostream>
#include "test_runner.h"
#include "misc.h"
#include "io.h"
#include "json.h"
#include <string>

using namespace std;

int main(void) {
#if 0
    BusDatabaseHandler handler;
    int num;
    try {
        cin >> num;
        handler.RequestsFromStream(num,cin);
        cin >> num;
        auto responses = handler.RequestsFromStream(num,cin).ProcessRequests().GetResponses();
        for(const auto& resp : responses) {
            cout << resp << '\n';
        }
    } catch(exception& e) {
        cerr << e.what() << endl;
    }
#else
    
#endif
    using namespace Json;
    istringstream is {R"(
{        
    "stat_requests": [
    {
      "type": "Bus",
      "name": "256",
      "id": 1965312327
    },
    {
      "type": "Bus",
      "name": "750",
      "id": 519139350
    },
    {
      "type": "Bus",
      "name": "751",
      "id": 194217464
    },
    {
      "type": "Stop",
      "name": "Samara",
      "id": 746888088
    },
    {
      "type": "Stop",
      "name": "Prazhskaya",
      "id": 65100610
    },
    {
      "type": "Stop",
      "name": "Biryulyovo Zapadnoye",
      "id": 1042838872
    }
  ]
}
    )"};

    Document doc = Load(is);
    try {
        auto& as_map = doc.GetRoot().AsMap();
        for(const auto& [k,v] : as_map) {
            cout << k << ":\n";
            for(const auto& node : v.AsArray()) {
                const auto as_map1 = node.AsMap();
                cout << as_map1.at("type").AsString() << endl;
                cout << as_map1.at("name").AsString() << endl;
                cout << as_map1.at("id").AsInt() << endl;
                cout << holds_alternative<int>(static_cast<variant<vector<Node>, map<string, Node>, int, string>>(as_map1.at("id"))) << endl;
            }
        }
    } catch (exception& e) {
        cerr << e.what() << endl;
    }
    return 0;
}