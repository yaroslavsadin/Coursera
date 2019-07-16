#include "Common.h"
#include "test_runner.h"

#include <sstream>

using namespace std;

namespace Expr {
class Value : public Expression {
public:
  Value(int value) : val(value) {}
  // Вычисляет значение выражения
  int Evaluate() const override {
    return val;
  }

  // Форматирует выражение как строку
  // Каждый узел берётся в скобки, независимо от приоритета
  std::string ToString() const override {
    return std::to_string(val);
  }
private:
  int val;
};

class Sum : public Expression {
public:
  Sum(ExpressionPtr left_, ExpressionPtr right_) : left(move(left_)), right(move(right_)) {}
  // Вычисляет значение выражения
  int Evaluate() const override {
    return left->Evaluate() + right->Evaluate();
  }

  // Форматирует выражение как строку
  // Каждый узел берётся в скобки, независимо от приоритета
  std::string ToString() const override {
    return '(' + left->ToString() + ")+(" + right->ToString() + ')';
  }
private:
  ExpressionPtr left;
  ExpressionPtr right;
};

class Product : public Expression {
public:
  Product(ExpressionPtr left_, ExpressionPtr right_) : left(move(left_)), right(move(right_)) {}
  // Вычисляет значение выражения
  int Evaluate() const override {
    return left->Evaluate() * right->Evaluate();
  }

  // Форматирует выражение как строку
  // Каждый узел берётся в скобки, независимо от приоритета
  std::string ToString() const override {
    return '(' + left->ToString() + ")*(" + right->ToString() + ')';
  }
private:
  ExpressionPtr left;
  ExpressionPtr right;
};
}

ExpressionPtr Value(int value) {
  return make_unique<Expr::Value>(value);
}
ExpressionPtr Sum(ExpressionPtr left, ExpressionPtr right) {
  return make_unique<Expr::Sum>(move(left),move(right));
}
ExpressionPtr Product(ExpressionPtr left, ExpressionPtr right) {
  return make_unique<Expr::Product>(move(left),move(right));
}

string Print(const Expression* e) {
  if (!e) {
    return "Null expression provided";
  }
  stringstream output;
  output << e->ToString() << " = " << e->Evaluate();
  return output.str();
}

void Test() {
  ExpressionPtr e1 = Product(Value(2), Sum(Value(3), Value(4)));
  ASSERT_EQUAL(Print(e1.get()), "(2)*((3)+(4)) = 14");

  ExpressionPtr e2 = Sum(move(e1), Value(5));
  ASSERT_EQUAL(Print(e2.get()), "((2)*((3)+(4)))+(5) = 19");

  ASSERT_EQUAL(Print(e1.get()), "Null expression provided");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, Test);
  return 0;
}