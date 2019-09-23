#include <iostream>
#include "test_runner.h"
#include "misc.h"
#include "io.h"
#include "json.h"
#include <string>
#include <iomanip>

using namespace std;

template<typename From>
auto MakeRequest(const From& request) {
  return request;
}

void JsonPrint(const Json::Node& top, ostream& os) {
  if(holds_alternative<int>(top)) {
    os << top.AsInt();
  } else if (holds_alternative<double>(top)) {
    os << top.AsDouble();
  } else if (holds_alternative<string>(top)) {
    os << "\"" << top.AsString() << "\"";
  } else if (holds_alternative<bool>(top)) {
    os << top.AsBool();
  } else if (holds_alternative<std::vector<Json::Node>>(top)) {
    os << "[";
    for(const auto& node : top.AsArray()) {
      JsonPrint(node,os);
    }
    // os << '\b';
    os << "]";
  } else {
    os << "{";
    for(const auto& [key,value] : top.AsMap()) {
      os << "\"" << key << "\": ";
      JsonPrint(value,os);
    }
    // os << '\b';
    os << "}";
  }
  os << ',';
}

int main(void) {
#if 1
    BusDatabaseHandler handler;

    try {
        Json::Document doc = Json::Load(cin);
        auto responses = handler.ReadRequests(doc).ProcessRequests().GetResponses();
        cout << setprecision(6);
        JsonPrint(responses, cout);
    } catch(exception& e) {
        cerr << e.what() << endl;
    }
#else
    try {
      Json::Node node{0.5};
      double lol = node.AsDouble();
      cout << lol << endl;
    } catch(exception& e) {
        cerr << e.what() << endl;
    }
#endif
    
    return 0;
}