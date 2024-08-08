#include <array>
#include <stdexcept>
#pragma once
// This is a wrapper to the array class. It is a FixedStack with all the stack
// functionality but with the class keeping the index of the top

template <typename T, std::size_t N> class FixedStack {
private:
  std::array<T, N> array;
  uint top = 0;

public:
  FixedStack() = default;
  ~FixedStack() = default;
  T pop() {
    if (top <= 0) {
      throw std::out_of_range("Not enough elements in array");
    } else {
      return array[--top];
    }
  }
  void push(const T &item) {
    if (top >= N) {
      throw std::out_of_range("Cannot add more elements to stack");
    } else {
      array[top] = item;
      top++;
    }
  }
  T &peek() {
    if (top >= 1) {
      return array[top - 1];
    } else {
      throw std::out_of_range("Nothing in the stack yet");
    }
  }
  bool isEmpty() { return (top == 0); }
  std::size_t size() { return top; }
  T operator[](int index) { return array[index]; }

  void swap(int index1, int index2) {
    T temp = array[index1];
    array[index1] = array[index2];
    array[index2] = temp;
  }
  void clear() { top = 0; }
  // class Iterator {
  // public:
  //   using difference_type = std::ptrdiff_t;
  //   using value_type = T;
  //   Iterator(T* ptr) : m_ptr(ptr) {};                              // default-initializable
  //   bool operator==(const Iterator& other) const {return m_ptr == other.m_ptr;}; // equality 
  //   bool operator!=(const Iterator& other) const {return m_ptr != other.m_ptr;}; // equality 

  //   T &operator*() const {return m_ptr;};                    // dereferenceable
  //   Iterator &operator++()                   // pre-incrementable
  //   {                                        /*do stuff...*/
  //     return *this;
  //   }
  //   void operator++(int) // post-incrementable
  //   {
  //     ++*this;
  //   }

  // private:
  //   // implementation...
  //   T* m_ptr;
  // };
  // Iterator begin() { return Iterator(array); } 
  // Iterator end() { return Iterator(array + top); }

};