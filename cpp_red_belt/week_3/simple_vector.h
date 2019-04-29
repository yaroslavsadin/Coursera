#pragma once

#include <cstdlib>
#include <algorithm>

// Реализуйте шаблон SimpleVector
template <typename T>
class SimpleVector {
public:
  SimpleVector() = default;
  explicit SimpleVector(size_t size) : capacity_(size) {
    begin_ = new T[capacity_];
    end_ = begin_ + size;
  }
  ~SimpleVector() {
    delete[] begin_;
  }

  T& operator[](size_t index) {
    return begin_[index];
  }

  T* begin() { return begin_;}
  T* end() { return end_;}
  const T* begin() const { return begin_;}
  const T* end() const { return end_;}

  size_t Size() const {
    return end_ - begin_;
  }
  size_t Capacity() const {
    return capacity_;
  }
  void PushBack(const T& value) {
    if (Capacity() == 0) {
      begin_ = new T[1];
      end_ = begin_;
      capacity_ = 1;
    } else if (Size() == Capacity()) {
      capacity_*=2;
      T* new_begin = new T[capacity_];
      std::copy(begin_,end_,new_begin);
      end_ = new_begin + Size();
      delete[] begin_;
      begin_ = new_begin;
    }
    *end_++ = value;
  }

private:
  // Добавьте поля для хранения данных вектора
  T* begin_ = nullptr;
  T* end_ = nullptr;
  size_t capacity_ = 0;
};
