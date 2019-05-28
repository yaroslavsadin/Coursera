#include "test_runner.h"
#include <algorithm>
#include <memory>
#include <vector>

using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
  // Напишите реализацию функции,
  // не копируя сортируемые элементы
  using T = typename RandomIt::value_type;
  if (std::distance(range_begin,range_end) < 2) {
    return;
  } else {
    vector<T> temp(make_move_iterator(range_begin),make_move_iterator(range_end));
    size_t delim = temp.size() / 3;
    MergeSort(range_begin,range_begin+delim);
    MergeSort(range_begin+delim,range_begin+2*delim);
    vector<T> temp1;
    merge(
      make_move_iterator(range_begin),
      make_move_iterator(range_begin+delim),
      make_move_iterator(range_begin+delim),
      make_move_iterator(range_begin+2*delim),
      back_inserter(temp1)
    );
    MergeSort(range_begin+2*delim,range_end);
    merge(
      make_move_iterator(temp1.begin()),
      make_move_iterator(temp1.end()),
      make_move_iterator(range_begin+2*delim),
      make_move_iterator(range_end),
      range_begin
    );
  }
}

void TestIntVector() {
  vector<int> numbers = {6, 1, 3, 9, 1, 9, 8, 12, 1};
  MergeSort(begin(numbers), end(numbers));
  ASSERT(is_sorted(begin(numbers), end(numbers)));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestIntVector);
  return 0;
}
