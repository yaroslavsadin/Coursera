#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <exception>
#include <sstream>
#include <deque>
using namespace std;

char PrevOp(deque<string>& a) {
    if(a.size() > 1) {    
        constexpr int op_position = 1;
        return ((*(a.end()-1))[op_position]);
    } else {
        return '0';
    }
}

void BuildArithmeticEquation(deque<string>& a, int num, char op) {
    char prev_op = PrevOp(a);
    if((prev_op == '+' || prev_op == '-') &&
            (op == '*' || op == '/')) {
        a.push_front("(");
        a.push_back(")");
    }
    string new_op = " " + string(1,op) + " " + to_string(num);
    a.push_back(new_op);
}

int main(void) {
    char op;
    int num;
    cin >> num;
    deque<string> arithmetic_equation = {to_string(num)};
    int num_ops;
    cin >> num_ops;
    while(num_ops--) {
        cin >> op >> num ;
        BuildArithmeticEquation(arithmetic_equation, num, op);
    }
    for(auto& i : arithmetic_equation) {
        cout << i;
    }
    cout << endl;
    return 0;
}