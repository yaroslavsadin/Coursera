#include <iomanip>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <utility>
#include "profile.h"
#include "test_runner.h"

using namespace std;

template <typename Iterator>
class IteratorRange {
public:
  IteratorRange(Iterator begin, Iterator end)
    : first(begin)
    , last(end)
    , size_(distance(first, last))
  {
  }

  Iterator begin() const {
    return first;
  }

  Iterator end() const {
    return last;
  }

  size_t size() const {
    return size_;
  }

private:
  Iterator first, last;
  size_t size_;
};

template <typename Iterator>
IteratorRange<Iterator> MakeRange(Iterator i1, Iterator i2) {
  return IteratorRange(i1,i2);
}

class ReadingManager {
public:
  ReadingManager() {}

  // O(1)
  void Read(int user_id, int page_count) {
    if(user_to_pages.count(user_id)) {
        int user_page = user_to_pages[user_id];
        pages_to_users[user_page].erase(user_id);
        if (pages_to_users[user_page].empty()) {
            pages_to_users.erase(user_page);
        }
    }
    user_to_pages[user_id] = page_count;
    pages_to_users[page_count].insert(user_id);
  }

  // O(L)
  double Cheer(int user_id) const {
    if (!user_to_pages.count(user_id)) {
      return 0;
    }
    const int user_count = user_to_pages.size();
    if (user_count == 1) {
      return 1;
    }
    const int page_count = user_to_pages.at(user_id);
    int count = 0;
    // O(L)
    for(const auto& [key,value] : 
    MakeRange(pages_to_users.begin(), pages_to_users.find(page_count))) {
      count += value.size();
    }
    // По умолчанию деление целочисленное, поэтому
    // нужно привести числитель к типу double.
    // Простой способ сделать это — умножить его на 1.0.
    return ((count * 1.0) / (user_count - 1));
  }

private:
  // Статическое поле не принадлежит какому-то конкретному
  // объекту класса. По сути это глобальная переменная,
  // в данном случае константная.
  // Будь она публичной, к ней можно было бы обратиться снаружи
  // следующим образом: ReadingManager::MAX_USER_COUNT.
  static const int MAX_USER_COUNT_ = 100'000;

  map<int,set<int>> pages_to_users;
  map<int,int> user_to_pages;
};

void ReadingManagerTests() {
  {
    // Если для данного пользователя пока не было ни одного события READ, доля считается равной 0
    ReadingManager manager;
    constexpr int NONEXISTENT = 0;
    double result = manager.Cheer(NONEXISTENT);
    const double expected = 0;
    ASSERT_EQUAL(expected,result);
  }
  {
    // Если этот пользователь на данный момент единственный, доля считается равной 1.
    ReadingManager manager;
    manager.Read(1,666);
    double result = manager.Cheer(1);
    const double expected = 1;
    ASSERT_EQUAL(expected,result);
  }
  {
    ReadingManager manager;
    manager.Read(1,666);
    manager.Read(2,665);
    double result = manager.Cheer(1);
    const double expected = 1./1;
    ASSERT_EQUAL(expected,result);
  }
  {
    ReadingManager manager;
    manager.Read(1,666);
    manager.Read(2,665);
    manager.Read(3,664);
    manager.Read(4,663);
    double result = manager.Cheer(3);
    const double expected = 1./3;
    ASSERT_EQUAL(expected,result);
  }
  {
    ReadingManager manager;
    manager.Read(1,666);
    manager.Read(2,665);
    manager.Read(3,664);
    manager.Read(4,663);
    double result = manager.Cheer(4);
    const double expected = 0./3;
    ASSERT_EQUAL(expected,result);
  }
  {
    ReadingManager manager;
    manager.Read(1,666);
    manager.Read(2,665);
    manager.Read(3,664);
    manager.Read(4,663);
    manager.Read(2,666);
    manager.Read(4,666);
    manager.Read(1,776);
    manager.Read(4,777);
    double result = manager.Cheer(1);
    const double expected = 2./3;
    ASSERT_EQUAL(expected,result);
  }
  {
    ReadingManager manager;
    manager.Read(1,666);
    manager.Read(2,666);
    manager.Read(3,666);
    manager.Read(4,666);
    manager.Read(5,666);
    manager.Read(6,1000);
    manager.Read(7,1000);
    manager.Read(8,1000);
    double result = manager.Cheer(7);
    const double expected = 5./7;
    ASSERT_EQUAL(expected,result);
  }
}

int main() {
  // Для ускорения чтения данных отключается синхронизация
  // cin и cout с stdio,
  // а также выполняется отвязка cin от cout

#if 0
  cout << 50000 << endl;
  for(size_t i = 0; i <= 50000; i++) {
    cout << "READ " + to_string(i) + " " + to_string(i) << "\n";
  }
  return 0;
#endif

  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  TestRunner tr;
  RUN_TEST(tr,ReadingManagerTests);

#if 0
  ifstream oss{"C:\\Work\\Coursera\\cpp_red_belt\\week_2\\ebook.txt"};
  cin.rdbuf(oss.rdbuf());
#endif

  ReadingManager manager;

  int query_count;
  cin >> query_count;

  LOG_DURATION();
  for (int query_id = 0; query_id < query_count; ++query_id) {
    string query_type;
    cin >> query_type;
    int user_id;
    cin >> user_id;

    if (query_type == "READ") {
      int page_count;
      cin >> page_count;
      manager.Read(user_id, page_count);
    } else if (query_type == "CHEER") {
      cout << setprecision(6) << manager.Cheer(user_id) << "\n";
    }
  }

  // O(L*Q)
  return 0;
}