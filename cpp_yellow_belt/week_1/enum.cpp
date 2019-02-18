#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <exception>
using namespace std;

enum class RequestType {
  ADD,
  REMOVE,
  NEGATE
};

void ProcessRequest(
    set<int>& numbers,
    RequestType request_type,
    int request_data) {
  switch (request_type) {
  case RequestType::ADD:
    numbers.insert(request_data);
    break;
  case RequestType::REMOVE:
    numbers.erase(request_data);
    break;
  case RequestType::NEGATE: {  // фигурные скобки обязательны
    bool contains = numbers.count(request_data) == 1;
    if (contains) {
      numbers.erase(request_data);
      numbers.insert(-request_data);
    }
    break;
  }
  default:
    cout << "Unknown request" << endl;
  }
}

int main(void) {
    set<int> numbers;
    ProcessRequest(numbers, RequestType::ADD, 8);
    ProcessRequest(numbers, RequestType::NEGATE, 8);
    ProcessRequest(numbers, RequestType::REMOVE, -8);
    return 0;
}