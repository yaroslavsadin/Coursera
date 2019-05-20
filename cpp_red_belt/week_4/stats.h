#pragma once

#include "http_request.h"

#include <string_view>
#include <map>
using namespace std;

class Stats {
public:
  Stats();
  void AddMethod(string_view method);
  void AddUri(string_view uri);
  const map<string_view, int>& GetMethodStats() const { return method_stats; }
  const map<string_view, int>& GetUriStats() const { return uri_stats; };
private:
  map<string_view, int> method_stats;
  map<string_view, int> uri_stats;
  
  static const array<string,4> methods_;
  static const string default_method_;
  static const array<string,5> uris_;
  static const string default_uri_;
};

HttpRequest ParseRequest(string_view line);
