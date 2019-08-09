#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>
#include <unordered_map>

using namespace std;

class ReadingManager {
public:
  ReadingManager()
      : user_page_counts_(MAX_USER_COUNT_ + 1, 0),
        // sorted_users_(),
        // user_positions_(MAX_USER_COUNT_ + 1, -1),
        pages_status(MAX_PAGE_COUNT_ + 1, 0) {}

  void Read(int user_id, int page_count) {
    if (user_page_counts_[user_id] == 0) {
      AddUser(user_id);
    } else {
      pages_status[user_page_counts_[user_id]]--;
    }
    user_page_counts_[user_id] = page_count;
    pages_status[page_count]++;
    // cerr << "--------- Read " << user_id << ' ' << page_count << "-----------" << endl;
    // for(int i = 0; i < 20; i++) {
    //   cerr << pages_status[i] << endl;
    // }
    // cerr << "--------------------" << endl;
  }

  double Cheer(int user_id) const {
    if (user_page_counts_[user_id] == 0) {
      return 0;
    }
    const int user_count = GetUserCount();
    if (user_count == 1) {
      return 1;
    }
    const int user_page = user_page_counts_[user_id];
    int users_below = 0;
    for(int i = user_page - 1; i >= 0; i--) {
      users_below += pages_status[i];
    }
    return (users_below) * 1.0 / (user_count - 1);
  }

private:
  // Статическое поле не принадлежит какому-то конкретному
  // объекту класса. По сути это глобальная переменная,
  // в данном случае константная.
  // Будь она публичной, к ней можно было бы обратиться снаружи
  // следующим образом: ReadingManager::MAX_USER_COUNT.
  static const int MAX_USER_COUNT_ = 100'000;
  static const int MAX_PAGE_COUNT_ = 1000;

  vector<int> user_page_counts_;
  // vector<int> sorted_users_;   // отсортированы по убыванию количества страниц
  // vector<int> user_positions_; // позиции в векторе sorted_users_
  vector<int> pages_status;
  int user_count = 0;


  int GetUserCount() const {
    return user_count;
  }
  void AddUser(int user_id) {
    user_count++;
  }
  // void SwapUsers(int lhs_position, int rhs_position) {
  //   const int lhs_id = sorted_users_[lhs_position];
  //   const int rhs_id = sorted_users_[rhs_position];
  //   swap(sorted_users_[lhs_position], sorted_users_[rhs_position]);
  //   swap(user_positions_[lhs_id], user_positions_[rhs_id]);
  // }
};

int main() {
  // Для ускорения чтения данных отключается синхронизация
  // cin и cout с stdio,
  // а также выполняется отвязка cin от cout
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  ReadingManager manager;

  int query_count;
  cin >> query_count;

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

  return 0;
}