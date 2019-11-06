#include <test_runner.h>

#include <cassert>
#include <cmath>
#include <stdexcept>
#include <sstream>
#include <stack>
#include <string>
#include <variant>
#include <memory>

namespace Json {

  class Node : public std::variant<std::monostate,
                            std::vector<Node>,
                            std::map<std::string, Node>,
                            int64_t,
                            std::string,
                            double,
                            bool> {
  public:
    using variant::variant;
    Node(std::monostate x) : variant(x) {}
    Node(int64_t x) : variant(x) {}
    Node(bool x) : variant(x) {}
    Node(double x) : variant(x) {}
    Node(std::string x) : variant(x) {}
    Node(std::vector<Node> x) : variant(x) {}
    Node(std::map<std::string, Node> x) : variant(x) {}

    const auto& AsArray() const {
      return std::get<std::vector<Node>>(*this);
    }
    auto& AsArray() {
      return std::get<std::vector<Node>>(*this);
    }
    const auto& AsMap() const {
      return std::get<std::map<std::string, Node>>(*this);
    }
    auto& AsMap() {
      return std::get<std::map<std::string, Node>>(*this);
    }
    int AsInt() const {
      return std::get<int64_t>(*this);
    }
    double AsDouble() const {
      if(std::holds_alternative<int64_t>(*this)) {
        return std::get<int64_t>(*this);
      } else {
        return std::get<double>(*this);
      }
    }
    bool AsBool() const {
      return std::get<bool>(*this);
    }
    const auto& AsString() const {
      return std::get<std::string>(*this);
    }
  };
}

template<typename It>
class Range {
public:
  Range(It begin, It end) : begin_(begin), end_(end) {}
  It begin() const { return begin_; }
  It end() const { return end_; }
  size_t size() { return std::distance(begin_,end_); }

private:
  It begin_;
  It end_;
};

using Array = std::vector<Json::Node>;
using Object = std::map<std::string,Json::Node>;

bool IsEscape(char c) {
  switch (c) {
    case '\b':
    case '\f':
    case '\n':
    case '\r':
    case '\t':
    case '\"':
    case '\\':
      return true;
    default:
      return false;
  }
  return false;
}

void PrintJsonString(std::ostream& out, std::string_view str) {
  // реализуйте фукнцию
  out << "\"";
  for(size_t i = 0; i < str.size(); i++) {
    if(IsEscape(str[i])) 
      out << R"(\)";
    out << str[i];
  }
  out << "\"";
}

void JsonPrint(const Json::Node& node, std::ostream& stream) {
  if (std::holds_alternative<std::monostate>(node)) {
    stream << "null";
  }
  else if (std::holds_alternative<int64_t>(node)) {
    stream << node.AsInt();
  }
  else if (std::holds_alternative<double>(node)) {
    stream << node.AsDouble();
  }
  else if (std::holds_alternative<bool>(node)) {
    stream << std::boolalpha << node.AsBool();
  }
  else if (std::holds_alternative<std::string>(node)) {
    PrintJsonString(stream,node.AsString());
  }
  else if (std::holds_alternative<Array>(node)) {
    stream << "[";
    const auto& x = node.AsArray();
    if(x.size()) { 
      for(const auto& node : Range(x.begin(),prev(x.end()))) {
        JsonPrint(node, stream);
        stream << ',';
      }
    
      JsonPrint(*prev(x.end()),stream);
    }
    stream << "]";
  }
  else {
    stream << "{";
    const auto& x = node.AsMap();
    if(x.size()) {
      for(const auto& pair_ : Range(x.begin(),prev(x.end()))) {
        JsonPrint(pair_.first, stream);
        stream << ":";
        JsonPrint(pair_.second, stream);
        stream << ',';
      } 
      JsonPrint(prev(x.end())->first, stream);
      stream << ":";
      JsonPrint(prev(x.end())->second, stream);
    }
    stream << "}";
  }
};

template<typename PrevType>
class JsonArray;
template<typename PrevType>
class JsonObject;

template<typename PrevType>
class JsonArray {
public:
  using ThisType = JsonArray<PrevType>;

  JsonArray(PrevType& prev, Array& data) : data(data), prev(prev) {}

  ThisType& Number(int64_t x)  {
    data.push_back(x);
    return *this;
  }
  ThisType& String(std::string_view x)  {
    data.push_back(std::string(x));
    return *this;
  }
  ThisType& Boolean(bool x)  {
    data.push_back(x);
    return *this;
  }
  ThisType& Null()  {
    data.push_back(Json::Node());
    return *this;
  }

  JsonArray<ThisType> BeginArray() {
    data.push_back(Json::Node(Array()));
    return JsonArray<ThisType>(*this,data.back().AsArray());
  }

  JsonObject<ThisType> BeginObject() {
    data.push_back(Json::Node(Object()));
    return JsonObject<ThisType>(*this,data.back().AsMap());
  }

  PrevType& EndArray() {
    return prev;
  }
private:
  Array& data;
  PrevType& prev;
};

template<typename Parent>
class JsonValue {
public:
  using ThisType = Parent;
  template<typename PrevType> friend class JsonObject;

  JsonValue(ThisType& parent) : obj(parent) {}

  ThisType& Number(int64_t x)  {
    *value = x;
    return obj;
  }
  ThisType& String(std::string_view x)  {
    *value = std::string(x);
    return obj;
  }
  ThisType& Boolean(bool x)  {
    *value = x;
    return obj;
  }
  ThisType& Null()  {
    return obj;
  }

  JsonArray<ThisType> BeginArray() {
    *value = Json::Node(Array());
    return JsonArray<ThisType>(obj,value->AsArray());
  }

  JsonObject<ThisType> BeginObject() {
    *value = Json::Node(Object());
    return JsonObject<ThisType>(obj,value->AsMap());
  }

private:
  Json::Node* value;
  ThisType& obj;
  void SetValue(Json::Node* val) {
    value = val;
  }
};

template<typename PrevType>
class JsonObject {
public:
  JsonObject(PrevType& prev, Object& data) : data(data), temp(*this), prev(prev) {}
  
  JsonValue<JsonObject<PrevType>>& Key(std::string_view key) {
    data[std::string(key)] = Json::Node();
    temp.SetValue(&data[std::string(key)]);
    return temp;
  }

  PrevType& EndObject() {
    return prev;
  }
private:
  Object& data;
  JsonValue<JsonObject<PrevType>> temp;
  PrevType& prev;
};

template<>
class JsonArray<void> {
public:
  using ThisType = JsonArray<void>;

  JsonArray(std::ostream& stream) : stream(stream), data(Array()) {}

  ThisType& Number(int64_t x)  {
    data.push_back(x);
    return *this;
  }
  ThisType& String(std::string_view x)  {
    data.push_back(std::string(x));
    return *this;
  }
  ThisType& Boolean(bool x)  {
    data.push_back(x);
    return *this;
  }
  ThisType& Null()  {
    data.push_back(Json::Node());
    return *this;
  }

  JsonArray<ThisType> BeginArray(){
    data.push_back(Json::Node(Array()));
    return JsonArray<ThisType>(*this,data.back().AsArray());
  }

  JsonObject<ThisType> BeginObject() {
    data.push_back(Json::Node(Object()));
    return JsonObject<ThisType>(*this,data.back().AsMap());
  }

  void EndArray() {}
  
  ~JsonArray() {
    JsonPrint(Json::Node(data), stream);
  }
private:
  Array data;
  std::ostream& stream;
};

template<>
class JsonObject<void> {
public:
  JsonObject(std::ostream& stream) 
  : data(), temp(*this), stream(stream)
  {}
  
  JsonValue<JsonObject<void>>& Key(std::string_view key) {
    data[std::string(key)] = Json::Node();
    temp.SetValue(&data[std::string(key)]);
    return temp;
  }

  void EndObject() {}

  ~JsonObject() {
    JsonPrint(Json::Node(data), stream);
  }
private:
  Object data;
  JsonValue<JsonObject<void>> temp;
  std::ostream& stream;
};

using ArrayContext = JsonArray<void>;  // Замените это объявление на определение типа ArrayContext
ArrayContext PrintJsonArray(std::ostream& out) {
  // реализуйте функцию
  return ArrayContext(out);
}

using ObjectContext = JsonObject<void>;  // Замените это объявление на определение типа ObjectContext
ObjectContext PrintJsonObject(std::ostream& out) {
  // реализуйте функцию
  return ObjectContext(out);
}

void TestArray() {
  std::ostringstream output;

  {
    auto json = PrintJsonArray(output);
    json
      .Number(5)
      .Number(6)
      .BeginArray()
        .Number(7)
      .EndArray()
      .Number(8)
      .String("bingo!");
  }

  ASSERT_EQUAL(output.str(), R"([5,6,[7],8,"bingo!"])");
}

void TestObject() {
  std::ostringstream output;

  {
    auto json = PrintJsonObject(output);
    json
      .Key("id1").Number(1234)
      .Key("id2").Boolean(false)
      .Key("").Null()
      .Key("\"").String("\\");
  }

  ASSERT_EQUAL(output.str(), R"({"":null,"\"":"\\","id1":1234,"id2":false})");
}

void TestAutoClose() {
  std::ostringstream output;

  {
    auto json = PrintJsonArray(output);
    json.BeginArray().BeginObject();
  }

  ASSERT_EQUAL(output.str(), R"([[{}]])");
}

void TestPrintJsonString() {
  {
    std::ostringstream os;
    PrintJsonString(os, "Hello, \"world\"");
    ASSERT_EQUAL(os.str(),R"("Hello, \"world\"")");
  }
  {
    std::ostringstream os;
    PrintJsonString(os, R"(")");
    ASSERT_EQUAL(os.str(),R"("\"")");
  }
  {
    std::ostringstream os;
    PrintJsonString(os, R"(\)");
    ASSERT_EQUAL(os.str(),R"("\\")");
  }
}

void TestAll() {
  std::ostringstream output;

  {
    auto json = PrintJsonObject(std::cout);
    json
      .Key("id1").Number(1234)
      .Key("id2").Boolean(false)
      .Key("").Null()
      .Key("\"").String("\\")
      .Key("test\"")
      .BeginArray()
        .BeginObject()
          .Key("lol").Number(42)
          .Key("kek").Boolean(true)
        .EndObject()
        .Boolean(false)
        .String("\"keklol")
        .Number(43)
      .EndArray()
      .Key("1").Boolean(true);
  }

  // ASSERT_EQUAL(output.str(), R"({"":null,"\"":"\\","id1":1234,"id2":false})");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestPrintJsonString);
  RUN_TEST(tr, TestArray);
  RUN_TEST(tr, TestObject);
  RUN_TEST(tr, TestAutoClose);
  // RUN_TEST(tr, TestAll);

  return 0;
}
