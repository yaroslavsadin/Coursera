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
  
  static const string method_get_;
  static const string method_post_;
  static const string method_put_;
  static const string method_delete_;
  static const string method_unknown_;

  static const string uri_empty_;
  static const string uri_order_;
  static const string uri_product_;
  static const string uri_basket_;
  static const string uri_help_;
  static const string uri_unknown_;
};

HttpRequest ParseRequest(string_view line);
