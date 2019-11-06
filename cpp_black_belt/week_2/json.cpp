#include "json.h"
#include "misc.h"
#include <cctype>
#include <cmath>

using namespace std;

namespace Json {

  Document::Document(Node root) : root(move(root)) {
  }

  const Node& Document::GetRoot() const {
    return root;
  }

  Node LoadNode(istream& input);

  Node LoadArray(istream& input) {
    vector<Node> result;

    for (char c; input >> c && c != ']'; ) {
      if (c != ',') {
        input.putback(c);
      }
      result.push_back(LoadNode(input));
    }

    return Node(move(result));
  }

  Node LoadNum(istream& input) {
    int result = 0;
    double fract = 0;
    int sign = 1;
    if(input.peek() == '-') {
      sign = -1;
      input.ignore(1);
    }
    while (isdigit(input.peek())) {
      result *= 10;
      result += input.get() - '0';
    }
    if(input.peek() == '.') {
      input.ignore(1);
      int count = 0;
      while (isdigit(input.peek())) {
        fract *= 10;
        fract += input.get() - '0';
        count++;
      }
      fract /= pow(10,count);
      return Node(sign*(result + fract));
    } else {
      return Node(sign*result);
    }
  }

  Node LoadBool(istream& input) {
    string str;
    while(isalpha(input.peek())) {
      str.push_back(input.get());
    }
    if(str == "false") {
      return Node(false);
    } else {
      return Node(true);
    }
  }

  Node LoadString(istream& input) {
    string line;
    getline(input, line, '"');
    return Node(move(line));
  }

  Node LoadDict(istream& input) {
    map<string, Node> result;

    for (char c; input >> c && c != '}'; ) {
      if (c == ',') {
        input >> c;
      }

      string key = LoadString(input).AsString();
      input >> c;
      result.emplace(move(key), LoadNode(input));
    }

    return Node(move(result));
  }

  Node LoadNode(istream& input) {
    char c;
    input >> c;

    if (c == '[') {
      return LoadArray(input);
    } else if (c == '{') {
      return LoadDict(input);
    } else if (c == '"') {
      return LoadString(input);
    } else if (isalpha(c)) {
      input.putback(c);
      return LoadBool(input);
    } else {
      input.putback(c);
      return LoadNum(input);
    }
  }

  Document Load(istream& input) {
    return Document{LoadNode(input)};
  }

  void Print(const Json::Node& top, ostream& os) {
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
      const auto& as_array = top.AsArray();
      if(as_array.size()) { 
        for(const auto& node : Range(as_array.begin(),prev(as_array.end()))) {
          Print(node,os);
          os << ',';
        }
      
        Print(*prev(as_array.end()),os);
      }
      os << "]";
    } else if (holds_alternative<std::map<std::string, Node>>(top)) {
      const auto& as_map = top.AsMap();
      os << "{";
      if(as_map.size()) {
        for(const auto& pair_ : Range(as_map.begin(),prev(as_map.end()))) {
          os << "\"" << pair_.first << "\": ";
          Print(pair_.second,os);
          os << ',';
        } 
        os << "\"" << prev(as_map.end())->first << "\": ";
        Print(prev(as_map.end())->second,os);
      }
      os << "}";
    } else {
      os << "none";
    }
  }

}
