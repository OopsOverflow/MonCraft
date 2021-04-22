#pragma once

#include <array>
#include <mutex>
#include <stdexcept>

template<class T, int N>
class AtomicCyclicList {
public:
  AtomicCyclicList()
    : array(), head(0), tail(0), size_(0)
  { }

  int size() {
    std::lock_guard<std::mutex> lck(mutex);
    return size_;
  }

  void clear() {
    head = 0;
    tail = 0;
    size_ = 0;
  }

  T pop() {
    std::lock_guard<std::mutex> lck(mutex);
    if(size_ == 0) throw std::out_of_range("cyclic list overflow (pop)");
    auto lastTail = tail;
    tail = (tail + 1) % N;
    size_--;
    return array[lastTail];
  }

  void push(T val) {
    std::lock_guard<std::mutex> lck(mutex);
    if(size_ == N) throw std::out_of_range("cyclic list overflow (push)");
    array[head] = val;
    head = (head + 1) % N;
    size_++;
  }

private:
  std::array<T, N> array;
  int head, tail, size_;
  std::mutex mutex;
};
