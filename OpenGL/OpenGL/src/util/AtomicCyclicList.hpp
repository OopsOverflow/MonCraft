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
    std::lock_guard<std::mutex> lck(mutex);
    head = 0;
    tail = 0;
    size_ = 0;
  }

  bool pop(T& t) {
    std::lock_guard<std::mutex> lck(mutex);
    if(size_ == 0) return false;
    auto lastTail = tail;
    tail = (tail + 1) % N;
    size_--;
    t = array[lastTail];
    return true;
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
