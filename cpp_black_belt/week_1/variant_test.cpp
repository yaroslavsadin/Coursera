#include <iostream>
#include <variant>
#include <string>
#include <vector>

using namespace std;

class VariantDescendent : public variant<bool,int> {

};

class VariantDescendent2 : public variant<int,double,string,vector<VariantDescendent>>{
    using variant::variant;
};

struct Visitor {
    string operator()(int a) {
        cout << a << endl;
        return "int";
    }
    string operator()(double a) {
        cout << a << endl;
        return "double";
    }
    string operator()(string) {
        throw runtime_error("");
    }
    string operator()(vector<VariantDescendent>&) {
        throw runtime_error("");
    }
};

int main(void) {
    VariantDescendent2 test;
    test = 2.;
    auto res = visit(Visitor{},static_cast<variant<int,double,string,vector<VariantDescendent>>>(test));
}