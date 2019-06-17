#include "test_runner.h"

void Assert(bool b, string msg) {
  AssertEqual(b, true, msg);
}

TestRunner::~TestRunner() {
  if (fail_count > 0) {
      cerr << RED_TEXT_START << fail_count << " unit tests failed. Terminate" << TEXT_COLOR_RESET << endl;
      exit(1);
  }
}