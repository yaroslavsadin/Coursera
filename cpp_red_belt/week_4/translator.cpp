#include "test_runner.h"
#include <string>
#include <string_view>
#include <set>
#include <vector>

using namespace std;

class Translator {
public:
  void Add(string_view source, string_view target) {
    auto it_src = words.insert(string(source)).first;
    auto it_dst = words.insert(string(target)).first;
    string_view src = *it_src;
    string_view dst = *it_dst;
    fwd[src]=dst;
    bwd[dst]=src;
  }
  string_view TranslateForward(string_view source) const {
    if(fwd.count(source)) {
      return fwd.at(source);
    } else {
      return empty;
    }
  }
  string_view TranslateBackward(string_view target) const {
    if(bwd.count(target)) {
      return bwd.at(target);
    } else {
      return empty;
    }
  }

private:
  // ???
  set<string> words;
  map<string_view,string_view> fwd;
  map<string_view,string_view> bwd;
  static const string empty;
};

const string Translator::empty = "";

void TestSimple() {
  Translator translator;
  translator.Add(string("okno"), string("window"));
  translator.Add(string("stol"), string("table"));

  ASSERT_EQUAL(translator.TranslateForward("okno"), "window");
  ASSERT_EQUAL(translator.TranslateBackward("table"), "stol");
  ASSERT_EQUAL(translator.TranslateBackward("stol"), "");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSimple);
  return 0;
}
