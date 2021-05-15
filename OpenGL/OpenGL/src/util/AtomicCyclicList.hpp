#pragma once

#include <array>
#include <mutex>
#include <stdexcept>

/**
 * The atomic cyclic list is an efficient queue-like container.
 * It has a fixed size of N and stores elements of type T.
 * All operations are thread-safe (hence "atomic").
 * All operations are constant-time. (O(1))
 */

template<class T, int N>
class AtomicCyclicList {
public:
  AtomicCyclicList()
    : array(), head(0), tail(0), size_(0)
  { }

  /**
   * Gives the number of elements stored it the queue.
   * O(1)
   */
  int size() {
    std::lock_guard<std::mutex> lck(mutex);
    return size_;
  }

  /**
   * Removes all elements.
   * O(1)
   */
  void clear() {
    std::lock_guard<std::mutex> lck(mutex);
    head = 0;
    tail = 0;
    size_ = 0;
  }

  /**
   * Removes an element stored in the list and copies it in t, if it exists.
   * returns if the operation succeeded or not.
   * O(1)
   */
  bool pop(T& t) {
    std::lock_guard<std::mutex> lck(mutex);
    if(size_ == 0) return false;
    auto lastTail = tail;
    tail = (tail + 1) % N;
    size_--;
    t = array[lastTail];
    return true;
  }

  /**
   * Adds a new element to the head of the queue.
   * throws out_of_range if the list is full.
   */
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
