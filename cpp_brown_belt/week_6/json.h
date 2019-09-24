#pragma once

#include <istream>
#include <map>
#include <string>
#include <variant>
#include <vector>

namespace Json {

  class Node : public std::variant<std::vector<Node>,
                            std::map<std::string, Node>,
                            int,
                            std::string,
                            double,
                            bool> {
  public:
    using variant::variant;
    Node(int x) : variant(x) {}
    Node(bool x) : variant(x) {}
    Node(double x) : variant(x) {}
    Node(std::string x) : variant(x) {}
    Node(std::vector<Node> x) : variant(x) {}
    Node(std::map<std::string, Node> x) : variant(x) {}

    const auto& AsArray() const {
      return std::get<std::vector<Node>>(*this);
    }
    const auto& AsMap() const {
      return std::get<std::map<std::string, Node>>(*this);
    }
    int AsInt() const {
      return std::get<int>(*this);
    }
    double AsDouble() const {
      return std::get<double>(*this);
    }
    bool AsBool() const {
      return std::get<bool>(*this);
    }
    const auto& AsString() const {
      return std::get<std::string>(*this);
    }
  };

  class Document {
  public:
    explicit Document(Node root);

    const Node& GetRoot() const;

  private:
    Node root;
  };

  Document Load(std::istream& input);

  void Print(const Node& top, std::ostream& os);
}
