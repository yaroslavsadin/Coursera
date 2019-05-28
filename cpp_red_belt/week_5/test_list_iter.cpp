#include <iostream>
#include <set>
#include <list>
#include <queue>
#include <memory>
using namespace std;

class StringNonCopyable : public string {
public:
  using string::string;  // Позволяет использовать конструкторы строки
  StringNonCopyable(const StringNonCopyable&) = delete;
  StringNonCopyable(StringNonCopyable&&) = default;
  StringNonCopyable& operator=(const StringNonCopyable&) = delete;
  StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

class Logger {
public:
    Logger() {
        cout << "Default constructor "<< data << endl;
    }
    explicit Logger(int d) : data(d) {
        cout << "Value constructor "<< data << endl;
    }
    Logger(const Logger& l) {
        cout << "Copy constructor "<< data << endl;
    }
    Logger(Logger&& l) {
        cout << "Move constructor "<< data << endl;
    }
    ~Logger() {
        cout << "Destructor "<< data << endl;
    }
    bool operator<(const Logger& other) const {
        return data < other.data;
    }
    void operator==(const Logger& other) {
         cout << "Copy assignment "<< data << endl;
    }
    void operator==(Logger&& other) {
         cout << "Move assignment "<< data << endl;
    }
private:
    int data = 0;
};

int main(void) {
    StringNonCopyable str = "string";
    list<StringNonCopyable> l;
    l.push_back(std::move(str));
    StringNonCopyable* ptr = &l.back();
    cout << *ptr << endl;
    list<int> l1 = {1,2,3,4,5,6};
    auto rit = next(l1.rbegin(),4);
    cout << *rit << ' ' << *prev(rit.base()) << endl;
    return 0;
}