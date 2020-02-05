#include <cstddef>
#include <iostream>
#include <utility>
#include <memory>
#include <cassert>

template <typename T>
class Vector;

template<typename T>
class RawMem {
public:
  friend class Vector<T>;
  RawMem() : data(nullptr), cp(0) {}
  RawMem(size_t n) { // may throw
    // return static_cast<T*>(operator new(n * sizeof(T)));
    data = reinterpret_cast<T*>(new char[sizeof(T) * n]);
    cp = n;
  }
  T* Get() const {
    return data;
  }
  size_t Cp() const {
    return cp;
  }
  T* operator+(size_t i) {
    return data + i;
  }
  T& operator[](size_t i) {
    return *(data + i);
  }
  const T& operator[](size_t i) const {
    return *(data + i);
  }
  ~RawMem() {
    if(data) {  
      delete [] reinterpret_cast<char*>(data);
    }
  }
  void Swap(RawMem& other) noexcept {
    std::swap(cp,other.cp);
    std::swap(data,other.data);
  }
private:
  T* data;
  size_t cp;
};

template <typename T>
class Vector {
public:
  Vector() : sz(0) {}
  Vector(size_t n) : data(n) {
    std::uninitialized_default_construct_n(
        begin(), n
      );
    sz = n;
  }
  Vector(const Vector& other) : data(other.Size()) {
    std::uninitialized_copy(
      other.begin(), 
      (other.begin() + other.sz), 
      begin()
    );
    sz = other.Size();
  }
  Vector(Vector&& other) {
    data.Swap(other.data);
    sz = other.sz;
    other.sz = 0;
  }

  ~Vector() {
      std::destroy_n(begin(), sz);
  }

  Vector& operator = (const Vector& other) {
    if(data.Cp() >= other.Size()) {
      for(size_t i = 0; i < other.sz; i++) {
        data[i] = other.data[i];
      }
      if(sz > other.Size()) {
        std::destroy_n(begin(), sz - other.sz);
      }
    } else {
      RawMem<T> new_mem(other.Size());
      std::uninitialized_copy(
        other.begin(), 
        (other.begin() + other.sz), 
        new_mem.Get()
      );
      data.Swap(new_mem);
      std::destroy_n(new_mem.Get(), sz);
    }
    sz = other.Size();
    return *this;
  }

  Vector& operator = (Vector&& other) noexcept {
    data.Swap(other.data);
    std::swap(sz,other.sz);
    return *this;
  }

  void Reserve(size_t n) {
    if(data.Cp() < n) {
      RawMem<T> new_mem(n);
      std::uninitialized_move(
        begin(), 
        (begin() + sz), 
        new_mem.Get()
      );
      data.Swap(new_mem);
      std::destroy_n(new_mem.Get(), sz);
    }
  }

  void Resize(size_t n) {
    if(data.Cp() < n) {
      Reserve(n);
    }
    if(sz < n) {
      std::uninitialized_default_construct_n(
        (data + sz),
        n - sz
      );
    } else if(sz > n) {
      std::destroy_n(begin() + n, sz - n);
    }
    sz = n;
  }

  void PushBack(const T& elem) {
    Extend();
    new (data + sz) T(elem);
    sz++;
  }
  void PushBack(T&& elem) {
    Extend();
    new (data + sz) T(std::move(elem));
    sz++;
  }

  template <typename ... Args>
  T& EmplaceBack(Args&&... args) {
    Extend();
    new (data + sz) T(std::forward<Args>(args)...);
    sz++;
    return data[sz-1];
  }

  void PopBack() {
    data[sz - 1].~T();
    sz--;
  }

  size_t Size() const noexcept {
    return sz;
  }

  size_t Capacity() const noexcept {
    return data.Cp();
  }

  const T& operator[](size_t i) const {
    return data[i];
  }
  T& operator[](size_t i) {
    return data[i];
  }

  // В данной части задачи реализуйте дополнительно эти функции:
  using iterator = T*;
  using const_iterator = const T*;

  iterator begin() noexcept {
    return data.Get();
  }
  iterator end() noexcept {
    return data.Get() + sz;
  }

  const_iterator begin() const noexcept {
    return data.Get();
  }
  const_iterator end() const noexcept {
    return data.Get() + sz;
  }

  std::reverse_iterator<iterator> rbegin() noexcept {
    return data.Get();
  }

  std::reverse_iterator<iterator> rend() noexcept {
    return data.Get() + sz;
  }
  // Тут должна быть такая же реализация, как и для константных версий begin/end
  const_iterator cbegin() const noexcept {
    return data.Get();
  }
  const_iterator cend() const noexcept {
    return data.Get() + sz;
  }
  
  std::reverse_iterator<const_iterator> crbegin() noexcept {
    return data.Get();
  }

  std::reverse_iterator<const_iterator> crend() noexcept {
    return data.Get() + sz;
  }
  // Вставляет элемент перед pos
  // Возвращает итератор на вставленный элемент
  iterator Insert(const_iterator pos_, const T& elem) {
    // PushBack + std::rotate
    if(pos_ < begin() || pos_ > end()) 
      throw std::runtime_error("Insert pos invalid");
    
    size_t pos_idx = pos_ - begin();

    Extend();

    iterator pos = begin() + pos_idx;
    auto num_after_elem = end() - pos;
    RawMem<T> buf(num_after_elem);

    std::uninitialized_move_n(
      pos, num_after_elem, buf.data
    );
    std::destroy_n(pos, num_after_elem);
    
    try {
      new (pos) T(elem);
    } catch(...) {
      std::uninitialized_move_n(
        buf.data, num_after_elem, pos
      );
      std::destroy_n(buf.data, num_after_elem);
      throw;
    }

    std::uninitialized_move_n(
      buf.data, num_after_elem, pos + 1
    );
    std::destroy_n(buf.data, num_after_elem);

    sz++;
    return pos;
  }
  iterator Insert(const_iterator pos_, T&& elem) {
    // PushBack + std::rotate
    if(pos_ < begin() || pos_ > end()) 
      throw std::runtime_error("Insert pos invalid");
    
    size_t pos_idx = pos_ - begin();

    Extend();

    iterator pos = begin() + pos_idx;
    auto num_after_elem = end() - pos;
    
    RawMem<T> buf(num_after_elem);

    std::uninitialized_move_n(
      pos, num_after_elem, buf.data
    );
    std::destroy_n(pos, num_after_elem);
    
    new (pos) T(std::move(elem));
    
    std::uninitialized_move_n(
      buf.data, num_after_elem, pos + 1
    );
    std::destroy_n(buf.data, num_after_elem);
    
    sz++;
    return pos;
  }

  // Конструирует элемент по заданным аргументам конструктора перед pos
  // Возвращает итератор на вставленный элемент
  template <typename ... Args>
  iterator Emplace(const_iterator it, Args&&... args) {
    return Insert(it,T(std::forward<Args>(args)...));
  }

  // Удаляет элемент на позиции pos
  // Возвращает итератор на элемент, следующий за удалённым
  iterator Erase(const_iterator it_) {
    if(it_ < begin() || it_ > end()) 
      throw std::runtime_error("Insert pos invalid");
    if(!sz || it_ == end()) {
      return end();
    }
    size_t pos_idx = it_ - begin();
    iterator it = begin() + pos_idx;

    auto num_after_elem = end() - it;
    RawMem<T> buf(num_after_elem);
    
    std::uninitialized_move_n(
      it + 1, num_after_elem - 1, buf.data
    );
    std::destroy_n(it, num_after_elem);
    
    std::uninitialized_move_n(
      buf.data, num_after_elem - 1, it
    );
    std::destroy_n(buf.data, num_after_elem - 1);
    
    sz--;
    return it;
  }
private:
  RawMem<T> data;
  size_t sz;

  void Extend() {
    if(data.Cp() == 0) {
      Reserve(1);
    }
    else if(sz == data.Cp()) {
      Reserve(sz * 2);
    }
  }
};
