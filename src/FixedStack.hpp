#include <array>
#include <stdexcept>
#pragma once
// This is a wrapper to the array class. It is a FixedStack with all the stack
// functionality but with the class keeping the index of the top

template <typename T, std::size_t N> class FixedStack {
private:
  std::array<T, N> array;
  int top = 0;

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
  T peek() {
    if (top >= 1) {
      return array[top - 1];
    }
    else{
      throw std::out_of_range("Nothing in the stack yet");
    }
  }
  bool isEmpty() { return (top == 0); }
  std::size_t size() { return top; }
  T operator[](int index) { return array[index]; }
  void clear() { top = 0; }
};