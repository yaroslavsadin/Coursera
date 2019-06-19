#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <utility>
#include <vector>

using namespace std;

template <typename T>
class PriorityCollection {
public:
  using Id = size_t/* тип, используемый для идентификаторов */;

  // Добавить объект с нулевым приоритетом
  // с помощью перемещения и вернуть его идентификатор
  Id Add(T object) {
    data_.push_back(move(object));
    Id id = data_.size()-1;
    prio_to_ids[0].insert(id);
    id_to_prio[id] = 0;
    ids.insert(id);
    return id;
  }

  // Добавить все элементы диапазона [range_begin, range_end)
  // с помощью перемещения, записав выданные им идентификаторы
  // в диапазон [ids_begin, ...)
  template <typename ObjInputIt, typename IdOutputIt>
  void Add(ObjInputIt range_begin, ObjInputIt range_end,
           IdOutputIt ids_begin) {
             for (auto i = range_begin; i != range_end; i++)
             {
               *ids_begin++ = Add(move(*i));
             }
           }

  // Определить, принадлежит ли идентификатор какому-либо
  // хранящемуся в контейнере объекту
  bool IsValid(Id id) const {
    return ids.count(id);
  }

  // Получить объект по идентификатору
  const T& Get(Id id) const {
    return data_[id];
  }

  // Увеличить приоритет объекта на 1
  void Promote(Id id) {
    auto current_prio = id_to_prio[id]++;
    prio_to_ids[current_prio].erase(id);
    if(prio_to_ids[current_prio].empty()) {
      prio_to_ids.erase(current_prio);
    }
    prio_to_ids[current_prio+1].insert(id);
  }

  // Получить объект с максимальным приоритетом и его приоритет
  pair<const T&, int> GetMax() const {
    auto max_ = GetMaxIdAndPrio();
    return {data_[max_.first],max_.second};
  }

  // Аналогично GetMax, но удаляет элемент из контейнера
  pair<T, int> PopMax() {
    auto max_ = GetMaxIdAndPrio();
    Delete(max_.first);
    return {move(data_[max_.first]),max_.second};
  }

private:
  // Приватные поля и методы
  vector<T> data_;
  map<size_t, set<Id>> prio_to_ids;
  map<Id, size_t> id_to_prio;
  set<Id> ids;

  pair<Id, int> GetMaxIdAndPrio() const {
    size_t current_max_prio = prev(prio_to_ids.end())->first;
    Id max_elem_id = *prev(prio_to_ids.at(current_max_prio).end());
    return {max_elem_id,current_max_prio};
  }

  void Delete(Id id) {
    ids.erase(id);
    auto prio = id_to_prio.at(id);
    prio_to_ids.at(prio).erase(id);
    if(prio_to_ids.at(prio).empty()) {
      prio_to_ids.erase(prio);
    }
    id_to_prio.erase(id);
  }
};


class StringNonCopyable : public string {
public:
  using string::string;  // Позволяет использовать конструкторы строки
  StringNonCopyable(const StringNonCopyable&) = delete;
  StringNonCopyable(StringNonCopyable&&) = default;
  StringNonCopyable& operator=(const StringNonCopyable&) = delete;
  StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

void TestNoCopy() {
  PriorityCollection<StringNonCopyable> strings;
  const auto white_id = strings.Add("white");
  const auto yellow_id = strings.Add("yellow");
  const auto red_id = strings.Add("red");

  strings.Promote(yellow_id);
  for (int i = 0; i < 2; ++i) {
    strings.Promote(red_id);
  }
  strings.Promote(yellow_id);
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "red");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "yellow");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "white");
    ASSERT_EQUAL(item.second, 0);
  }
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestNoCopy);
  return 0;
}
