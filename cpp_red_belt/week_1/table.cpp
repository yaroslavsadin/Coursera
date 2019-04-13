#include "test_runner.h"

using namespace std;

// Реализуйте здесь шаблонный класс Table
template<typename T>
class Table {
public:
  Table(size_t rows, size_t columns) : table(vector<vector<T>>(rows, vector<T>(columns))) {}
  void Resize(size_t rows, size_t columns);
  pair<size_t,size_t> Size() const {
    if(table.empty()) {
      return pair<size_t,size_t>(0,0);
    } else {
      return pair<size_t,size_t>(table.size(), table[0].size());
    }
  }
  vector<T>& operator[](size_t index) {
    if((table.size() - 1) >= index) {
      return table[index];
    } else {
      throw invalid_argument("Index exceed the table boundaries");
    }
  }
  const vector<T>& operator[](size_t index) const {
    if((table.size() - 1) >= index) {
      return table[index];
    } else {
      throw invalid_argument("Index exceed the table boundaries");
    }
  }
private:
  vector<vector<T>> table;
};

template <typename T>
void Table<T>::Resize(size_t rows, size_t columns) {
  table.resize(rows);
  for(auto& col : table) {
    col.resize(columns);
  }
}

void TestTable() {
  Table<int> t(1, 1);
  ASSERT_EQUAL(t.Size().first, 1u);
  ASSERT_EQUAL(t.Size().second, 1u);
  t[0][0] = 42;
  ASSERT_EQUAL(t[0][0], 42);
  t.Resize(3, 4);
  ASSERT_EQUAL(t.Size().first, 3u);
  ASSERT_EQUAL(t.Size().second, 4u);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestTable);
  return 0;
}
