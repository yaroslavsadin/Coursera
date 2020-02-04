#include <cstddef>

template <typename T>
class Vector {
public:
  // Эти функции были реализованы в первой части задачи
  Vector();
  Vector(size_t n);
  Vector(const Vector& other);
  Vector(Vector&& other);

  ~Vector();

  Vector& operator = (const Vector& other);
  Vector& operator = (Vector&& other) noexcept;

  void Reserve(size_t n);

  void Resize(size_t n);

  void PushBack(const T& elem);
  void PushBack(T&& elem);

  template <typename ... Args>
  T& EmplaceBack(Args&&... args);

  void PopBack();

  size_t Size() const noexcept;

  size_t Capacity() const noexcept;

  const T& operator[](size_t i) const;
  T& operator[](size_t i);


  // В данной части задачи реализуйте дополнительно эти функции:
  using iterator = T*;
  using const_iterator = const T*;

  iterator begin() noexcept;
  iterator end() noexcept;

  const_iterator begin() const noexcept;
  const_iterator end() const noexcept;

  // Тут должна быть такая же реализация, как и для константных версий begin/end
  const_iterator cbegin() const noexcept;
  const_iterator cend() const noexcept;

  // Вставляет элемент перед pos
  // Возвращает итератор на вставленный элемент
  iterator Insert(const_iterator pos, const T& elem);
  iterator Insert(const_iterator pos, T&& elem);

  // Конструирует элемент по заданным аргументам конструктора перед pos
  // Возвращает итератор на вставленный элемент
  template <typename ... Args>
  iterator Emplace(const_iterator it, Args&&... args);

  // Удаляет элемент на позиции pos
  // Возвращает итератор на элемент, следующий за удалённым
  iterator Erase(const_iterator it);
};
