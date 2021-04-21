#pragma once

#include <stddef.h>
#include <glm/glm.hpp>
#include <stdexcept>
#include <iostream>


/**
 * A 2D data store.
 *
 * Stores data of type T in a 2D array of given dimensions.
 * Manages memory and allows iterating the stored points.
 * Defines Bitmap that holds an RGB8 image.
 */

template<typename T, int N>
class DataStore {
public:
  /**
   * An unitialized 2D data store.
   */
  DataStore(glm::vec<N, int> size)
      : size(size), map(nullptr)
  {
    int mapSize = 1;
    for(int i = 0; i < N; i++)
      mapSize *= size[i];
    map = new T[mapSize];
  }

  DataStore(int size) : DataStore(glm::vec<N, int>(size))
  { }

  ~DataStore() {
    delete[] map;
  }

  DataStore(DataStore const&) = delete;
  DataStore& operator=(DataStore const&) = delete;

  DataStore(DataStore&& other) noexcept
    : size(std::move(other.size)), map(std::move(other.map))
  {
    other.map = nullptr;
  }

  DataStore& operator=(DataStore&& other) {
    if (this != &other) {
      delete[] map;
      map = other.map;
      size = other.size;
    }
    return *this;
  }

  /**
   * Gets the data stored at pos.
   * @throws std::out_of_range
   */
  T &at(glm::vec<N, int> pos) {
    for(int i = 0; i < N; i++)
      if(pos[i] > size[i])
        throw std::out_of_range("DataStore coordinates are out of range");

    return this->operator[](pos);
  }

  /**
   * Gets the data stored at pos. (unsafe)
   */
  T &operator[](glm::vec<N, int> pos) {
    auto index = 0;
    auto acc = 1;
    for(auto i = 0; i < N - 1; i++) {
      index += pos[i] * acc;
      acc *= size[i];
    }
    index += pos[N-1] * acc;
    return map[index];
  }

  /**
   * Gets the raw pointer data
   */
  const T* c_ptr() const {
    return map;
  }

  T* mut_c_ptr() {
    return map;
  }

  const glm::vec<N, int> size;

protected:
  T *map;
};
