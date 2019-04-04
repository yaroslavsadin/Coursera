#include "condition_parser.h"
#include "test_framework.h"

#include <sstream>
using namespace std;

void TestParseCondition() {
  {
    istringstream is("date != 2017-11-18");
    shared_ptr<Node> root = ParseCondition(is);
    Assert(root->Evaluate({2017, 1, 1}, ""), "Parse condition 1",__LINE__,__FILE__);
    Assert(!root->Evaluate({2017, 11, 18}, ""), "Parse condition 2",__LINE__,__FILE__);
  }
  {
    istringstream is(R"(event == "sport event")");
    shared_ptr<Node> root = ParseCondition(is);
    Assert(root->Evaluate({2017, 1, 1}, "sport event"), "Parse condition 3",__LINE__,__FILE__);
    Assert(!root->Evaluate({2017, 1, 1}, "holiday"), "Parse condition 4",__LINE__,__FILE__);
  }
  {
    istringstream is("date >= 2017-01-01 AND date < 2017-07-01");
    shared_ptr<Node> root = ParseCondition(is);
    Assert(root->Evaluate({2017, 1, 1}, ""), "Parse condition 5",__LINE__,__FILE__);
    Assert(root->Evaluate({2017, 3, 1}, ""), "Parse condition 6",__LINE__,__FILE__);
    Assert(root->Evaluate({2017, 6, 30}, ""), "Parse condition 7",__LINE__,__FILE__);
    Assert(!root->Evaluate({2017, 7, 1}, ""), "Parse condition 8",__LINE__,__FILE__);
    Assert(!root->Evaluate({2016, 12, 31}, ""), "Parse condition 9",__LINE__,__FILE__);
  }
  {
    istringstream is(R"(event != "sport event" AND event != "Wednesday")");
    shared_ptr<Node> root = ParseCondition(is);
    Assert(root->Evaluate({2017, 1, 1}, "holiday"), "Parse condition 10",__LINE__,__FILE__);
    Assert(!root->Evaluate({2017, 1, 1}, "sport event"), "Parse condition 11",__LINE__,__FILE__);
    Assert(!root->Evaluate({2017, 1, 1}, "Wednesday"), "Parse condition 12",__LINE__,__FILE__);
  }
  {
    istringstream is(R"(event == "holiday AND date == 2017-11-18")");
    shared_ptr<Node> root = ParseCondition(is);
    Assert(!root->Evaluate({2017, 11, 18}, "holiday"), "Parse condition 13",__LINE__,__FILE__);
    Assert(!root->Evaluate({2017, 11, 18}, "work day"), "Parse condition 14",__LINE__,__FILE__);
    Assert(root->Evaluate({1, 1, 1}, "holiday AND date == 2017-11-18"), "Parse condition 15",__LINE__,__FILE__);
  }
  {
    istringstream is(R"(((event == "holiday" AND date == 2017-01-01)))");
    shared_ptr<Node> root = ParseCondition(is);
    Assert(root->Evaluate({2017, 1, 1}, "holiday"), "Parse condition 16",__LINE__,__FILE__);
    Assert(!root->Evaluate({2017, 1, 2}, "holiday"), "Parse condition 17",__LINE__,__FILE__);
  }
  {
    istringstream is(R"(date > 2017-01-01 AND (event == "holiday" OR date < 2017-07-01))");
    shared_ptr<Node> root = ParseCondition(is);
    Assert(!root->Evaluate({2016, 1, 1}, "holiday"), "Parse condition 18",__LINE__,__FILE__);
    Assert(root->Evaluate({2017, 1, 2}, "holiday"), "Parse condition 19",__LINE__,__FILE__);
    Assert(root->Evaluate({2017, 1, 2}, "workday"), "Parse condition 20",__LINE__,__FILE__);
    Assert(!root->Evaluate({2018, 1, 2}, "workday"), "Parse condition 21",__LINE__,__FILE__);
  }
  {
    istringstream is(R"(date > 2017-01-01 AND event == "holiday" OR date < 2017-07-01)");
    shared_ptr<Node> root = ParseCondition(is);
    Assert(root->Evaluate({2016, 1, 1}, "event"), "Parse condition 22",__LINE__,__FILE__);
    Assert(root->Evaluate({2017, 1, 2}, "holiday"), "Parse condition 23",__LINE__,__FILE__);
    Assert(root->Evaluate({2017, 1, 2}, "workday"), "Parse condition 24",__LINE__,__FILE__);
    Assert(!root->Evaluate({2018, 1, 2}, "workday"), "Parse condition 25",__LINE__,__FILE__);
  }
  {
    istringstream is(R"(((date == 2017-01-01 AND event == "holiday")))");
    shared_ptr<Node> root = ParseCondition(is);
    Assert(root->Evaluate({2017, 1, 1}, "holiday"), "Parse condition 26",__LINE__,__FILE__);
    Assert(!root->Evaluate({2017, 1, 2}, "holiday"), "Parse condition 27",__LINE__,__FILE__);
  }
  {
    istringstream is(R"(((event == "2017-01-01" OR date > 2016-01-01)))");
    shared_ptr<Node> root = ParseCondition(is);
    Assert(root->Evaluate({1, 1, 1}, "2017-01-01"), "Parse condition 28",__LINE__,__FILE__);
    Assert(!root->Evaluate({2016, 1, 1}, "event"), "Parse condition 29",__LINE__,__FILE__);
    Assert(root->Evaluate({2016, 1, 2}, "event"), "Parse condition 30",__LINE__,__FILE__);
  }
}

int main(void) {
  TestRunner tr;
  tr.RunTest(TestParseCondition,"TestParseCondition");
  return 0;
}