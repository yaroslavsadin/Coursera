#pragma once
#include <cstdint>

using namespace std;

template <typename Iterator>
class Page {
public:
  Page(Iterator b, Iterator e)
  : first(b), second(e) {}
  Iterator begin() const {
    return first;
  }
  Iterator end() const {
    return second;
  }
  size_t size() const {
    return second - first;
  }
private:
  Iterator first;
  Iterator second;
};

// Реализуйте шаблон класса Paginator

template <typename Iterator>
class Paginator {
public:
  Paginator(Iterator b, Iterator e, size_t page_size) {
    Iterator current = b;
    while(e - current >= page_size) {
      pages.push_back({current,current+page_size});
      current = next(current,page_size);
    }
    if(current != e) {
      pages.push_back({current,e});
    }
  }
  // auto  = typename vector<Page<Iterator>>::const_iterator
  const auto begin() const {
    return pages.begin();
  }
  const auto end() const {
    return pages.end();
  }
  size_t size() const {
    return pages.size();
  }
private:
  vector<Page<Iterator>> pages;
};

template <typename C>
auto Paginate(C& c, size_t page_size) {
  // Реализуйте этот шаблон функции
  return Paginator(c.begin(),c.end(),page_size);
}