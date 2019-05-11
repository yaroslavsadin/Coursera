#pragma once

#include <chrono>
#include <iostream>
#include <string>
#include "ostream_color.h"

using namespace std;
using namespace std::chrono;

class LogDuration {
public:
  explicit LogDuration(const string& func, const string& msg = "")
    : message(msg + ": ")
    , function(func)
    , start(steady_clock::now())
  {
  }

  ~LogDuration() {
    auto finish = steady_clock::now();
    auto dur = finish - start;
    cerr << MAGENTA_TEXT_START << "Profile of function "
       << function << "()" << endl << message
       << duration_cast<milliseconds>(dur).count()
       << " ms" << TEXT_COLOR_RESET << endl;
  }
private:
  string message;
  string function;
  steady_clock::time_point start;
};

#define UNIQ_ID_IMPL(lineno) _a_local_var_##lineno
#define UNIQ_ID(lineno) UNIQ_ID_IMPL(lineno)

#define LOG_DURATION(message) \
  LogDuration UNIQ_ID(__LINE__)(__FUNCTION__, message);
