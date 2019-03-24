#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <string>
#include <memory>
#include <cmath>
using namespace std;

constexpr double pi = 3.14;

class Figure {
public:
    Figure() {}
    virtual string Name() = 0;
    virtual double Perimeter() = 0;
    virtual double Area() = 0;
private:
    // double perimeter_;
    // double area_;
};

class Rect : public Figure {
public:
    Rect(int a, int b) : a_(a), b_(b) {}
    string Name() override {
        return "RECT";
    }
    double Perimeter() override {
        return (static_cast<double>(a_) + b_) * 2;
    }
    double Area() override {
        return (static_cast<double>(a_) * b_);
    }
private:
    int a_,b_;
};

class Triangle : public Figure {
public:
    Triangle(int a, int b, int c) : a_(a), b_(b), c_(c) {}
    string Name() override {
        return "TRIANGLE";
    }
    double Perimeter() override {
        return (static_cast<double>(a_) + b_ + c_);
    }
    double Area() override {
        double p = (static_cast<double>(a_) + b_ + c_) / 2;
        return sqrt(p*(p-a_)*(p-b_)*(p-c_));
    }
private:
    int a_,b_,c_;
};

class Circle : public Figure {
public:
    Circle(int r) : r_(r) {}
    string Name() override {
        return "CIRCLE";
    }
    double Perimeter() override {
        return (2 * pi * r_);
    }
    double Area() override {
        return pi * r_ * r_;
    }
private:
    int r_;
};

shared_ptr<Figure> CreateFigure(istringstream& is) {
    string name;
    is >> name;
    if (name == "RECT") {
        int a,b;
        is >> a >> b;
        return make_shared<Rect>(a,b);
    } else if (name == "TRIANGLE") {
        int a,b,c;
        is >> a >> b >> c;
        return make_shared<Triangle>(a,b,c);
    } else {
        int r;
        is >> r;
        return make_shared<Circle>(r);
    }
}

int main() {
  vector<shared_ptr<Figure>> figures;
  for (string line; getline(cin, line); ) {
    istringstream is(line);

    string command;
    is >> command;
    if (command == "ADD") {
      figures.push_back(CreateFigure(is));
    } else if (command == "PRINT") {
      for (const auto& current_figure : figures) {
        cout << fixed << setprecision(3)
             << current_figure->Name() << " "
             << current_figure->Perimeter() << " "
             << current_figure->Area() << endl;
      }
    }
  }
  return 0;
}