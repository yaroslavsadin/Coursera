#include "test_runner.h"

#include <cstdint>
#include <iterator>
#include <numeric>
#include <list>

using namespace std;

#if 0
// O(N/2)
template <typename RandomIt>
void Permute(RandomIt first, RandomIt last, RandomIt to_permute) {
  typename RandomIt::value_type* p = new typename RandomIt::value_type(move(*to_permute));
  for(auto it = to_permute; it > first; it--) {
    *(it) = move(*prev(it));
  }
  *first = move(*p);
  delete p;
}

// O(N^2/2)
template <typename RandomIt>
void MakeJosephusPermutation(RandomIt first, RandomIt last, uint32_t step_size) {
  size_t count = last-first;
  size_t current = 0;
  // O(N)
  for(size_t i = count - 1; i > 0; i--) {
    auto it = first + current;
    // O(N/2)
    Permute(first++,last,it);
    current = (current + step_size - 1) % i;
  }
}
#endif

template <typename RandomIt>
void MakeJosephusPermutation(RandomIt first, RandomIt last, uint32_t step_size) {
  list<typename RandomIt::value_type> pool;
  for(auto it = first; it != last; it++) {
    pool.push_back(move(*it));
  }
  size_t current = 0;
  auto it = pool.begin();
  while(pool.size()) {
    *first++ = move(*it);
    it = pool.erase(it);
    if(!pool.size()) {
      break;
    }
    size_t next_ = (current + step_size - 1) % pool.size();
    if (pool.size() <= step_size) {
      it = next(pool.begin(),next_);
    } else
    {
      if (next_ > current) {
        it = next(it,step_size-1);
      } else {
        it = next(pool.begin(),next_);     
      }
    }

    current = next_;
  }
}

vector<int> MakeTestVector() {
  vector<int> numbers(10);
  iota(begin(numbers), end(numbers), 0);
  return numbers;
}

void TestIntVector() {
  const vector<int> numbers = MakeTestVector();
  {
    vector<int> numbers_copy = numbers;
    MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 1);
    ASSERT_EQUAL(numbers_copy, numbers);
  }
  {
    vector<int> numbers_copy = numbers;
    MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 3);
    ASSERT_EQUAL(numbers_copy, vector<int>({0, 3, 6, 9, 4, 8, 5, 2, 7, 1}));
                                      //                               
  }
}

// Это специальный тип, который поможет вам убедиться, что ваша реализация
// функции MakeJosephusPermutation не выполняет копирование объектов.
// Сейчас вы, возможно, не понимаете как он устроен, однако мы расскажем,
// почему он устроен именно так, далее в блоке про move-семантику —
// в видео «Некопируемые типы»

struct NoncopyableInt {
  int value;

  NoncopyableInt(const NoncopyableInt&) = delete;
  NoncopyableInt& operator=(const NoncopyableInt&) = delete;

  NoncopyableInt(NoncopyableInt&&) = default;
  NoncopyableInt& operator=(NoncopyableInt&&) = default;
};

bool operator == (const NoncopyableInt& lhs, const NoncopyableInt& rhs) {
  return lhs.value == rhs.value;
}

ostream& operator << (ostream& os, const NoncopyableInt& v) {
  return os << v.value;
}

void TestAvoidsCopying() {
  vector<NoncopyableInt> numbers;
  numbers.push_back({1});
  numbers.push_back({2});
  numbers.push_back({3});
  numbers.push_back({4});
  numbers.push_back({5});

  MakeJosephusPermutation(begin(numbers), end(numbers), 2);

  vector<NoncopyableInt> expected;
  expected.push_back({1});
  expected.push_back({3});
  expected.push_back({5});
  expected.push_back({4});
  expected.push_back({2});

  ASSERT_EQUAL(numbers, expected);
}

// void TestPermute() {
//   vector<int> result = {1,2,3,4,5};
//   vector<int> expected = {3,1,2,4,5};
//   Permute(result.begin(),result.end(),result.begin()+2);
//   ASSERT_EQUAL(expected,result);
//   Permute(result.begin(),result.end(),result.begin());
//   ASSERT_EQUAL(expected,result);
//   expected = {5,3,1,2,4};
//   Permute(result.begin(),result.end(),result.begin()+4);
//   ASSERT_EQUAL(expected,result);
// }

int main() {
  TestRunner tr;
  // RUN_TEST(tr, TestPermute);
  RUN_TEST(tr, TestIntVector);
  RUN_TEST(tr, TestAvoidsCopying);
  return 0;
}
