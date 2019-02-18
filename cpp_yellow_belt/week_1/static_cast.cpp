#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <exception>
#include <limits>
using namespace std;

enum class RequestType {
  ADD,
  REMOVE,
  NEGATE
};

ostream& operator<<(ostream& stream, RequestType req) {
    stream << static_cast<int>(req);
    return stream;
}

int main(void) {
    // Выведет 0
    cout << static_cast<int>(RequestType::ADD) << endl;

    // Выведет 1
    cout << static_cast<int>(RequestType::REMOVE) << endl;

    // Выведет 2
    cout << static_cast<int>(RequestType::NEGATE) << endl;

    int request_code;
    cin >> request_code;
    auto request_type = static_cast<RequestType>(request_code);
    cout << request_type;
    // Если ввести request_code = 7,
    // в переменной request_type окажется RequestType::NEGATE
    return 0;
}