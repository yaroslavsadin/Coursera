#include "test_runner.h"
#include "http_request.h"
#include "stats.h"

#include <map>
#include <string_view>
using namespace std;

extern Stats ServeRequests(istream& input);

HttpRequest ParseRequest(string_view line) {
  HttpRequest res;
  size_t pos = line.find_first_not_of(' ');
  line.remove_prefix(pos);
  // Find the first whitespace and take method name
  pos = line.find(' ');
  res.method = line.substr(0,pos);
  line.remove_prefix(pos+1);
  pos = line.find(' ');
  res.uri = line.substr(0,pos);
  line.remove_prefix(pos+1);
  res.protocol = line;
  return res;
}
const array<string,4> Stats::methods_ = {"POST","PUT","DELETE","GET"};
const string Stats::default_method_ = "UNKNOWN";
const array<string,5> Stats::uris_ = {"/", "/order", "/product", "/basket", "/help"};
const string Stats::default_uri_ = "unknown";

void Stats::AddMethod(string_view method) {
  for(auto& i : methods_) {
    if (method == i) {
      method_stats[i]++;
      return;
    }
  }
  method_stats[default_method_]++;
}

void Stats::AddUri(string_view uri) {
  for(auto& i : uris_) {
    if (uri == i) {
      uri_stats[i]++;
      return;
    }
  }
  uri_stats[default_uri_]++;
}

Stats::Stats() {
  for(auto& i : methods_) {
    method_stats[i];
  }
  method_stats[default_method_];
  for(auto& i : uris_) {
    uri_stats[i];
  }
  uri_stats[default_uri_];
}
#if 0
bool operator!=(HttpRequest lhs, HttpRequest rhs) {
  return tie(lhs.method,lhs.protocol,lhs.uri) != tie(rhs.method,rhs.protocol,rhs.uri);
}
ostream& operator<<(ostream& os, HttpRequest lhs) {
  return os << lhs.method << ", " <<  lhs.uri << "," << lhs.protocol;
}
void TestHttpRequest(void) {
    {
      const string input = "GET / HTTP/1.1";
      const HttpRequest expected = {
        .method = "GET",
        .uri = "/",
        .protocol = "HTTP/1.1"
      };
      ASSERT_EQUAL(ParseRequest(input),expected);
    }
    {
      const string input = "POST /product HTTP/1.1";
      const HttpRequest expected = {
        .method = "POST",
        .uri = "/product",
        .protocol = "HTTP/1.1"
      };
      ASSERT_EQUAL(ParseRequest(input),expected);
    }
    {
      const string input = "DELETE /product HTTP/1.1";
      const HttpRequest expected = {
        .method = "DELETE",
        .uri = "/product",
        .protocol = "HTTP/1.1"
      };
      ASSERT_EQUAL(ParseRequest(input),expected);
    }
    {
      const string input = "HEAD / HTTP/1.1";
      const HttpRequest expected = {
        .method = "HEAD",
        .uri = "/",
        .protocol = "HTTP/1.1"
      };
      ASSERT_EQUAL(ParseRequest(input),expected);
    }
}
#endif
void TestBasic() {
  const string input =
    R"(GET / HTTP/1.1
    POST /order HTTP/1.1
    POST /product HTTP/1.1
    POST /product HTTP/1.1
    POST /product HTTP/1.1
    GET /order HTTP/1.1
    PUT /product HTTP/1.1
    GET /basket HTTP/1.1
    DELETE /product HTTP/1.1
    GET / HTTP/1.1
    GET / HTTP/1.1
    GET /help HTTP/1.1
    GET /upyachka HTTP/1.1
    GET /unexpected HTTP/1.1
    HEAD / HTTP/1.1)";

  const map<string_view, int> expected_method_count = {
    {"GET", 8},
    {"PUT", 1},
    {"POST", 4},
    {"DELETE", 1},
    {"UNKNOWN", 1},
  };
  const map<string_view, int> expected_url_count = {
    {"/", 4},
    {"/order", 2},
    {"/product", 5},
    {"/basket", 1},
    {"/help", 1},
    {"unknown", 2},
  };

  istringstream is(input);
  const Stats stats = ServeRequests(is);

  ASSERT_EQUAL(stats.GetMethodStats(), expected_method_count);
  ASSERT_EQUAL(stats.GetUriStats(), expected_url_count);
}

void TestAbsentParts() {
  // Методы GetMethodStats и GetUriStats должны возвращать словари
  // с полным набором ключей, даже если какой-то из них не встречался

  const map<string_view, int> expected_method_count = {
    {"GET", 0},
    {"PUT", 0},
    {"POST", 0},
    {"DELETE", 0},
    {"UNKNOWN", 0},
  };
  const map<string_view, int> expected_url_count = {
    {"/", 0},
    {"/order", 0},
    {"/product", 0},
    {"/basket", 0},
    {"/help", 0},
    {"unknown", 0},
  };
  const Stats default_constructed;

  ASSERT_EQUAL(default_constructed.GetMethodStats(), expected_method_count);
  ASSERT_EQUAL(default_constructed.GetUriStats(), expected_url_count);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestBasic);
  RUN_TEST(tr, TestAbsentParts);
  // RUN_TEST(tr, TestHttpRequest);
  return 0;
}
