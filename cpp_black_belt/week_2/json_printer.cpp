#include <test_runner.h>

#include <cassert>
#include <cmath>
#include <stdexcept>
#include <sstream>
#include <stack>
#include <string>
#include <variant>
#include <memory>
#include "json.h"
#include "misc.h"

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
  else if (std::holds_alternative<int>(node)) {
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
    data.push_back(static_cast<int>(x));
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

template<typename Object>
class JsonValue {
public:
  using ThisType = Object;
  // friend class PrevType;

  JsonValue(ThisType& parent) : obj(parent) {}

  ThisType& Number(int64_t x)  {
    *value = static_cast<int>(x);
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
    return JsonArray<ThisType>(obj,value->AsMap());
  }

  void SetValue(Json::Node* val) {
    value = val;
  }

private:
  Json::Node* value;
  ThisType& obj;
};

template<typename PrevType>
class JsonObject {
public:
  JsonObject(PrevType& prev, Object& data) : data(data), temp(*this), prev(prev) {}
  
  JsonValue<JsonObject<PrevType>>& Key(const std::string& key) {
    data[key] = Json::Node();
    temp.SetValue(&data[key]);
    return temp;
  }

  PrevType& EndObject() {
    return *prev;
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
    data.push_back(static_cast<int>(x));
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
  
  JsonValue<JsonObject<void>>& Key(const std::string& key) {
    data[key] = Json::Node();
    temp.SetValue(&data[key]);
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
    ostringstream os;
    PrintJsonString(os, "Hello, \"world\"");
    ASSERT_EQUAL(os.str(),R"("Hello, \"world\"")");
  }
  {
    ostringstream os;
    PrintJsonString(os, R"(")");
    ASSERT_EQUAL(os.str(),R"("\"")");
  }
  {
    ostringstream os;
    PrintJsonString(os, R"(\)");
    ASSERT_EQUAL(os.str(),R"("\\")");
  }
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestPrintJsonString);
  RUN_TEST(tr, TestArray);
  RUN_TEST(tr, TestObject);
  RUN_TEST(tr, TestAutoClose);

  return 0;
}
