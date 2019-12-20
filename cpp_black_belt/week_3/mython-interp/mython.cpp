#include "object.h"
#include "object_holder.h"
#include "statement.h"
#include "lexer.h"
#include "parse.h"

#include <test_runner.h>

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

void TestAll();

void RunMythonProgram(istream& input, ostream& output) {
  Ast::Print::SetOutputStream(output);

  // std::stringstream sstream;
  // sstream << input.rdbuf();
  // std::cerr << sstream.str() << endl;

  Parse::Lexer lexer(input);
  auto program = ParseProgram(lexer);

  Runtime::Closure closure;
  program->Execute(closure);
}

int main() {
  TestAll();

  RunMythonProgram(cin, cout);

  return 0;
}

void TestSimplePrints() {
  istringstream input(R"(
print 57
print 10, 24, -8
print 'hello'
print "world"
print True, False
print
print None
)");

  ostringstream output;
  RunMythonProgram(input, output);

  ASSERT_EQUAL(output.str(), "57\n10 24 -8\nhello\nworld\nTrue False\n\nNone\n");
}

void TestAssignments() {
  istringstream input(R"(
x = 57
print x
x = 'C++ black belt'
print x
y = False
x = y
print x
x = None
print x, y
)");

  ostringstream output;
  RunMythonProgram(input, output);

  ASSERT_EQUAL(output.str(), "57\nC++ black belt\nFalse\nNone False\n");
}

void TestArithmetics() {
  istringstream input(
    "print 1+2+3+4+5, 1*2*3*4*5, 1-2-3-4-5, 36/4/3, 2*5+10/2"
  );

  ostringstream output;
  RunMythonProgram(input, output);

  ASSERT_EQUAL(output.str(), "15 120 -13 3 15\n");
}

void TestVariablesArePointers() {
  istringstream input(R"(
class Counter:
  def __init__():
    self.value = 0

  def add():
    self.value = self.value + 1

class Dummy:
  def do_add(counter):
    counter.add()

x = Counter()
y = x

x.add()
y.add()

print x.value

d = Dummy()
d.do_add(x)

print y.value
)");

  ostringstream output;
  RunMythonProgram(input, output);

  ASSERT_EQUAL(output.str(), "2\n3\n");
}

void TestCase8() {
  istringstream input(R"(
a = 1
b = 2
c = 3

result1 = a + b > c and a + c > b and b + c > a

a = False
b = False
c = True

result2 = not a and b or c
result3 = not a and (b or c)
result4 = not(not a and (b or c))

a = 'this'
b = 'is'
c = 'test'

result5 = a > b and a > c and c > b
result6 = a < b or a < c or c < b

a = ''
b = ""
c = 0

result7 = a > b
result8 = a or b or c

a = 0
b = 100
c = ''

result9 = a or b or c

a = None
b = None

result10 = a and b
result11 = not a or b

a = 1
b = 1
c = 2

result12 = a == b and a != c

a = '1'
b = '1'
c = "2"

result13 = a == b and a != c

print result1, result2, result3, result4, result5, result6, result7, result8, result9, result10, result11, result12, result13
)");

  ostringstream output;
  RunMythonProgram(input, cerr);
}


void TestAll() {
  TestRunner tr;
  Runtime::RunObjectHolderTests(tr);
  Runtime::RunObjectsTests(tr);
  Ast::RunUnitTests(tr);
  Parse::RunLexerTests(tr);
  TestParseProgram(tr);

  RUN_TEST(tr, TestSimplePrints);
  RUN_TEST(tr, TestAssignments);
  RUN_TEST(tr, TestArithmetics);
  RUN_TEST(tr, TestVariablesArePointers);
  RUN_TEST(tr, TestCase8);
}
