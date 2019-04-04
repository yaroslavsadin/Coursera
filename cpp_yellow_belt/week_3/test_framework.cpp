#include "test_runner.h"

void Assert(bool b, string msg, int line, string file) {
  AssertEqual(b, true, msg, line, file);
}

TestRunner::~TestRunner() {
  if (fail_count > 0) {
      cerr << RED_TEXT_START << fail_count << " unit tests failed. Terminate" << TEXT_COLOR_RESET << endl;
      exit(1);
  }
}