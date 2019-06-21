#include "test_runner.h"

#include <forward_list>
#include <iterator>
#include <algorithm>

using namespace std;

template <typename Type, typename Hasher>
class HashSet {
public:
  using BucketList = forward_list<Type>;

public:
  explicit HashSet(
      size_t num_buckets,
      const Hasher& hasher = {}
  ) : buckets(num_buckets), h(hasher) {}

  void Add(const Type& value);
  bool Has(const Type& value) const;
  void Erase(const Type& value);
  const BucketList& GetBucket(const Type& value) const;
private:
  Hasher h;
  vector<BucketList> buckets;
};

template <typename Type, typename Hasher>
void HashSet<Type,Hasher>::Add(const Type& value) {
  size_t bucket = h(value) % buckets.size();
  BucketList& b = buckets[bucket];
  auto it =  find(b.begin(),b.end(),value);
  
  if(it == b.end()) {
    b.push_front(value);
  }
}

template <typename Type, typename Hasher>
bool HashSet<Type,Hasher>::Has(const Type& value) const {
  size_t bucket = h(value) % buckets.size();
  const BucketList& b = buckets[bucket];
  auto it =  find(b.begin(),b.end(),value);
  if(it == b.end()) {
    return false;
  }
  return true;
}

template <typename Type, typename Hasher>
void HashSet<Type,Hasher>::Erase(const Type& value) {
  size_t bucket = h(value) % buckets.size();
  BucketList& b = buckets[bucket]; 
  auto it =  find(b.begin(),b.end(),value);
  if(it == b.end()) {
    return;
  }
  b.remove(value);
}

template <typename Type, typename Hasher>
const typename HashSet<Type,Hasher>::BucketList& HashSet<Type,Hasher>::GetBucket(const Type& value) const {
  return buckets[h(value) % buckets.size()];
}

struct IntHasher {
  size_t operator()(int value) const {
    // Это реальная хеш-функция из libc++, libstdc++.
    // Чтобы она работала хорошо, std::unordered_map
    // использует простые числа для числа бакетов
    return value;
  }
};

struct TestValue {
  int value;

  bool operator==(TestValue other) const {
    return value / 2 == other.value / 2;
  }
};

struct TestValueHasher {
  size_t operator()(TestValue value) const {
    return value.value / 2;
  }
};

void TestSmoke() {
  HashSet<int, IntHasher> hash_set(2);
  hash_set.Add(3);
  hash_set.Add(4);

  ASSERT(hash_set.Has(3));
  ASSERT(hash_set.Has(4));
  ASSERT(!hash_set.Has(5));

  hash_set.Erase(3);

  ASSERT(!hash_set.Has(3));
  ASSERT(hash_set.Has(4));
  ASSERT(!hash_set.Has(5));

  hash_set.Add(3);
  hash_set.Add(5);

  ASSERT(hash_set.Has(3));
  ASSERT(hash_set.Has(4));
  ASSERT(hash_set.Has(5));
}

void TestEmpty() {
  HashSet<int, IntHasher> hash_set(10);
  for (int value = 0; value < 10000; ++value) {
    ASSERT(!hash_set.Has(value));
  }
}

void TestIdempotency() {
  HashSet<int, IntHasher> hash_set(10);
  hash_set.Add(5);
  ASSERT(hash_set.Has(5));
  hash_set.Add(5);
  ASSERT(hash_set.Has(5));
  hash_set.Erase(5);
  ASSERT(!hash_set.Has(5));
  hash_set.Erase(5);
  ASSERT(!hash_set.Has(5));
}

void TestEquivalence() {
  HashSet<TestValue, TestValueHasher> hash_set(10);
  hash_set.Add(TestValue{2});
  hash_set.Add(TestValue{3});

  ASSERT(hash_set.Has(TestValue{2}));
  ASSERT(hash_set.Has(TestValue{3}));

  const auto& bucket = hash_set.GetBucket(TestValue{2});
  const auto& three_bucket = hash_set.GetBucket(TestValue{3});
  ASSERT_EQUAL(&bucket, &three_bucket);

  ASSERT_EQUAL(1, distance(begin(bucket), end(bucket)));
  ASSERT_EQUAL(2, bucket.front().value);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSmoke);
  RUN_TEST(tr, TestEmpty);
  RUN_TEST(tr, TestIdempotency);
  RUN_TEST(tr, TestEquivalence);
  return 0;
}
