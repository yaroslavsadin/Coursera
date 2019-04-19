#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>
#include "profile.h"
#include "test_runner.h"

using namespace std;

class ReadingManager {
public:
  ReadingManager()
      : user_page_counts_(MAX_USER_COUNT_ + 1, 0),
        sorted_users_(),
        user_positions_(MAX_USER_COUNT_ + 1, -1) {}

  // O(N)
  void Read(int user_id, int page_count) {
    if (user_page_counts_[user_id] == 0) {    // O(1)
      AddUser(user_id);                       // O(1)
    }
    user_page_counts_[user_id] = page_count;  // O(1)
    int& position = user_positions_[user_id]; // O(1)
    // O(N)
    while (position > 0 && page_count > user_page_counts_[sorted_users_[position - 1]]) {
      SwapUsers(position, position - 1);
    }
  }

  // O(N)
  double Cheer(int user_id) const {
    if (user_page_counts_[user_id] == 0) {
      return 0;
    }
    const int user_count = GetUserCount();
    if (user_count == 1) {
      return 1;
    }
    const int page_count = user_page_counts_[user_id];
    int position = user_positions_[user_id];
    // O(N)
    while (position < user_count &&
      user_page_counts_[sorted_users_[position]] == page_count) {
      ++position;
    }
    if (position == user_count) {
        return 0;
    }
    // По умолчанию деление целочисленное, поэтому
    // нужно привести числитель к типу double.
    // Простой способ сделать это — умножить его на 1.0.
    return (user_count - position) * 1.0 / (user_count - 1);
  }

private:
  // Статическое поле не принадлежит какому-то конкретному
  // объекту класса. По сути это глобальная переменная,
  // в данном случае константная.
  // Будь она публичной, к ней можно было бы обратиться снаружи
  // следующим образом: ReadingManager::MAX_USER_COUNT.
  static const int MAX_USER_COUNT_ = 100'000;

  vector<int> user_page_counts_;
  vector<int> sorted_users_;   // отсортированы по убыванию количества страниц
  vector<int> user_positions_; // позиции в векторе sorted_users_

  int GetUserCount() const {
    return sorted_users_.size();
  }
  // O(1)
  void AddUser(int user_id) {
    sorted_users_.push_back(user_id);                     // O(1)
    user_positions_[user_id] = sorted_users_.size() - 1;  // O(1)
  }
  // O(1)
  void SwapUsers(int lhs_position, int rhs_position) {
    const int lhs_id = sorted_users_[lhs_position];
    const int rhs_id = sorted_users_[rhs_position];
    swap(sorted_users_[lhs_position], sorted_users_[rhs_position]);
    swap(user_positions_[lhs_id], user_positions_[rhs_id]);
  }
};

void ReadingManagerTests() {
  {
    // Если для данного пользователя пока не было ни одного события READ, доля считается равной 0
    ReadingManager manager;
    constexpr int NONEXISTENT = 0;
    int result = manager.Cheer(NONEXISTENT);
    const int expected = 0;
    ASSERT_EQUAL(expected,result);
  }
  {
    // Если этот пользователь на данный момент единственный, доля считается равной 1.
    ReadingManager manager;
    manager.Read(1,666);
    int result = manager.Cheer(1);
    const int expected = 1;
    ASSERT_EQUAL(expected,result);
  }
  {
    ReadingManager manager;
    manager.Read(1,666);
    manager.Read(2,665);
    int result = manager.Cheer(1);
    const int expected = 1./1;
    ASSERT_EQUAL(expected,result);
  }
  {
    ReadingManager manager;
    manager.Read(1,666);
    manager.Read(2,665);
    manager.Read(3,664);
    manager.Read(4,663);
    int result = manager.Cheer(3);
    const int expected = 1./3;
    ASSERT_EQUAL(expected,result);
  }
  {
    ReadingManager manager;
    manager.Read(1,666);
    manager.Read(2,665);
    manager.Read(3,664);
    manager.Read(4,663);
    int result = manager.Cheer(4);
    const int expected = 0./3;
    ASSERT_EQUAL(expected,result);
  }
  {
    ReadingManager manager;
    manager.Read(1,666);
    manager.Read(2,666);
    manager.Read(1,777);
    manager.Read(2,700);
    int result = manager.Cheer(1);
    const int expected = 1./1;
    ASSERT_EQUAL(expected,result);
  }
  {
    ReadingManager manager;
    manager.Read(1,666);
    manager.Read(2,666);
    manager.Read(3,666);
    manager.Read(4,666);
    manager.Read(5,666);
    manager.Read(6,1);
    manager.Read(7,2);
    manager.Read(8,3);
    int result = manager.Cheer(8);
    const int expected = 3./7;
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

  // O(N*Q)
  return 0;
}