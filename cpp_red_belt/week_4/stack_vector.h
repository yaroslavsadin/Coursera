#pragma once

#include <array>
#include <stdexcept>
using namespace std;

template <typename T, size_t N>
class StackVector {
public:
  explicit StackVector(size_t a_size = 0) : size(a_size) {
    if(a_size > N) {
      throw invalid_argument("a_size > N");
    }
  }

  T& operator[](size_t index) {
    return data_[index];
  }
  const T& operator[](size_t index) const{
    return data_[index];
  }

  auto begin() {return data_.begin();}
  auto end() {return data_.begin() + size;}
  auto begin() const {return data_.begin();}
  auto end() const {return data_.begin() + size;}

  size_t Size() const {
    return size;
  }
  size_t Capacity() const {
    return data_.size();
  }

  void PushBack(const T& value) {
    if (Size() == Capacity()) {
      throw overflow_error("Push_Back");
    }
    data_[size++] = value;
  }
  T PopBack() {
    if(size)  
      return(data_[--size]);
    else
      throw underflow_error("Pop_Back");
  }

private:
  array<T,N> data_;
  int size = 0;
};

