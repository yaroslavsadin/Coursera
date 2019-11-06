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

size_t FindEscape(std::string_view str) {
  for(size_t i; i < str.size(); i++) {
    switch (str[i]) {
      case '\\': return i; break;
      case '\"': return i; break;
      default:
          break;
    }
  }
  return str.npos;
}

void PrintEscape(char c, std::ostream& out) {
  out << R"(\)" << c;
}

void PrintJsonString(std::ostream& out, std::string_view str) {
  // реализуйте фукнцию
  size_t cur_pos = 0;
  while(cur_pos != str.npos) {
    cur_pos = FindEscape(str);
    out << str.substr(cur_pos-1);
    if(cur_pos != str.npos) {
      PrintEscape(str[cur_pos], out);
    }
  }
}

template<typename PrevType>
class JsonArray;
template<typename PrevType>
class JsonObject;

struct NullContext {};
static NullContext null_context;

using Array = std::vector<Json::Node>;
using Object = std::map<std::string,Json::Node>;

template<typename PrevType>
class JsonArray {
public:
  using ThisType = JsonArray<PrevType>;

  JsonArray() : data_storage(Array()), data(*data_storage), prev(null_context) {}
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

  JsonArray<ThisType> BeginArray(){
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
  std::optional<Array> data_storage;
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
  JsonObject() : data_storage(Object()), data(*data_storage), temp(*this), prev(null_context) {}
  JsonObject(PrevType& prev, Object& data) : data(data), temp(*this), prev(prev) {}
  
  JsonValue<JsonObject<PrevType>>& Key(std::string key) {
    data[key] = Json::Node();
    temp.SetValue(&data[key]);
    return temp;
  }

  PrevType& EndObject() {
    return *prev;
  }
protected:
  std::optional<Object> data_storage;
  Object& data;
  JsonValue<JsonObject<PrevType>> temp;
  PrevType& prev;
};



using ArrayContext = JsonArray<NullContext>;  // Замените это объявление на определение типа ArrayContext
ArrayContext PrintJsonArray(std::ostream& out) {
  // реализуйте функцию
  return ArrayContext();
}

using ObjectContext = JsonObject<NullContext>;  // Замените это объявление на определение типа ObjectContext
ObjectContext PrintJsonObject(std::ostream& out) {
  // реализуйте функцию
  return ObjectContext();
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

  ASSERT_EQUAL(output.str(), R"({"id1":1234,"id2":false,"":null,"\"":"\\"})");
}

void TestAutoClose() {
  std::ostringstream output;

  {
    auto json = PrintJsonArray(output);
    json.BeginArray().BeginObject();
  }

  ASSERT_EQUAL(output.str(), R"([[{}]])");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestArray);
  RUN_TEST(tr, TestObject);
  RUN_TEST(tr, TestAutoClose);

  return 0;
}
