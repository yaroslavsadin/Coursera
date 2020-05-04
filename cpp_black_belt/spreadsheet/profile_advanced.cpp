#include "profile_advanced.h"

#include <iostream>
#include <sstream>

constexpr auto RED_TEXT_START = "\033[1;31m";
constexpr auto CYAN_TEXT_START = "\033[1;36m";
constexpr auto BLUE_TEXT_START = "\033[1;34m";
constexpr auto MAGENTA_TEXT_START = "\033[1;35m";
constexpr auto YELLOW_TEXT_START = "\033[1;33m";
constexpr auto TEXT_COLOR_RESET = "\033[0m";

TotalDuration::TotalDuration(const string& msg)
  : message(msg + ": ")
  , value(0)
{
}

TotalDuration::~TotalDuration() {
  ostringstream os;
  os << message
     << duration_cast<milliseconds>(value).count()
     << " ms" << endl;
  cerr << MAGENTA_TEXT_START << os.str() << TEXT_COLOR_RESET;
}

AddDuration::AddDuration(steady_clock::duration& dest)
  : add_to(dest)
  , start(steady_clock::now())
{
}

AddDuration::AddDuration(TotalDuration& dest)
  : AddDuration(dest.value)
{
}

AddDuration::~AddDuration() {
  add_to += steady_clock::now() - start;
}
