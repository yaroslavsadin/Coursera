#include "Common.h"
#include <atomic>
#include <mutex>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include "test_runner.h"

using namespace std;

template <typename T>
class Synchronized {
public:
  template <typename U>
  struct Sync {
    U& ref_to_value;
    lock_guard<mutex> g;
  };

  explicit Synchronized(T initial = T()) : value(move(initial)) {}

  Sync<T> GetAccess() {
    return {value,lock_guard(m)};
  }
  Sync<const T> GetAccess() const{
    return {value,lock_guard(m)};
  }

private:
  T value;
  mutable mutex m;
};

class LruCache : public ICache {
private:
  struct BookKeeper {
    BookPtr ptr = nullptr;
    size_t lru = 0;
  };
public:
  LruCache(
      shared_ptr<IBooksUnpacker> books_unpacker,
      const Settings& settings
  ) : books_unpacker_(books_unpacker), settings_(settings)
  {
    // реализуйте метод
  }

  BookPtr GetBook(const string& book_name) override {
    // реализуйте метод
    
    /* Mutex lock inside */
    auto sync = cache_.GetAccess();
    auto& cache = sync.ref_to_value;

    if(cache.count(book_name)) {
      cache[book_name].lru = ++current_max_lru;
      return cache[book_name].ptr;
    } else {
      BookPtr book_content = books_unpacker_->UnpackBook(book_name);
      auto book_size = book_content->GetContent().size();
      if(mem_consumed + book_size <= settings_.max_memory) {
        cache[book_name] = BookKeeper{book_content,++current_max_lru};
        mem_consumed += book_size;
        return cache[book_name].ptr;
      } else {
        mem_consumed += book_size;
        cache[book_name] = {book_content,++current_max_lru};
        while(mem_consumed > settings_.max_memory && !cache.empty()) {
          auto it = min_element(cache.begin(),cache.end(),
          [](const pair<string , BookKeeper>& lhs, const pair<string , BookKeeper>& rhs){
            return lhs.second.lru < rhs.second.lru;
          });
          mem_consumed -= it->second.ptr->GetContent().size();
          cache.erase(it);
        }
        if(cache.empty()) {
          return nullptr;
        } else {
          return cache[book_name].ptr;
        }
      }
    }
  }
private:
  atomic<int> mem_consumed = 0;
  Synchronized< unordered_map< string , BookKeeper > > cache_;
  shared_ptr<IBooksUnpacker> books_unpacker_;
  Settings settings_;
  atomic<size_t> current_max_lru = 1;
};


unique_ptr<ICache> MakeCache(
    shared_ptr<IBooksUnpacker> books_unpacker,
    const ICache::Settings& settings
) {
  // реализуйте функцию
  return make_unique<LruCache>(move(books_unpacker),settings);
}
