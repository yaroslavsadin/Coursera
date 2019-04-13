#include "test_runner.h"

#include <ostream>
using namespace std;

#define PRINT_VALUES(out, x, y) \
  out << (x) << endl,           \
  out << (y) << endl

int main() {
  TestRunner tr;
  tr.RunTest([] {
    ostringstream output;
    PRINT_VALUES(output, 5, "red belt");
    ASSERT_EQUAL(output.str(), "5\nred belt\n");
  }, "PRINT_VALUES usage example");
  tr.RunTest([] {
    ostringstream output;
    int a = 5;
    PRINT_VALUES(output, a++, ++a);
    ASSERT_EQUAL(output.str(), "5\n7\n");
  }, "++ operator");
  tr.RunTest([] {
    ostringstream output;
    int a = 5;
    int b = 6;
    if(true)  
      PRINT_VALUES(output, a + b, b - a);
    else
      PRINT_VALUES(output, a + b, b - a);
    
    ASSERT_EQUAL(output.str(), "11\n1\n");
  }, "Expressions");
}
