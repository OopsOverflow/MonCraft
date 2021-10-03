#pragma once

#include <stddef.h>
#include <glm/glm.hpp>
#include <stdexcept>
#include <iostream>
#include <numeric>


/**
 * A 2D data store.
 *
 * Stores data of type T in a N-dimentional array of fixed size.
 * Manages memory and allows iterating the stored elements.
 */

template<typename T, int N>
class DataStore {
public:

  /**
   * An unitialized 2D data store of given size.
   */
  DataStore(glm::vec<N, int> size)
      : size(size), map(nullptr)
  {
    int mapSize = 1;
    for(int i = 0; i < N; i++)
      mapSize *= size[i];
    map = new T[mapSize];
  }


  /**
   * Constructs a DataStore of given size along all dimensions.
   */
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
  T &at(int index) {
    int maxIndex = 1;
    for(int i = 0; i < N; i++)
      maxIndex *= size[i];

    if(index < 0 || index >= maxIndex)
      throw std::out_of_range("DataStore index is out of range");

    return map[index];
  }

  /**
   * Gets the data stored at pos.
   * @throws std::out_of_range
   */
  T const& at(int index) const {
    int maxIndex = 1;
    for(int i = 0; i < N; i++)
      maxIndex *= size[i];

    if(index < 0 || index >= maxIndex)
      throw std::out_of_range("DataStore index is out of range");

    return map[index];
  }

  /**
   * Gets the data stored at pos.
   * @throws std::out_of_range
   */
  T &at(glm::vec<N, int> pos) {
    for(int i = 0; i < N; i++)
      if(pos[i] < 0 || pos[i] >= size[i])
        throw std::out_of_range("DataStore coordinates are out of range");

    return this->operator[](pos);
  }

  /**
   * Gets the data stored at pos.
   * @throws std::out_of_range
   */
  T const& at(glm::vec<N, int> pos) const {
    for(int i = 0; i < N; i++)
      if(pos[i] < 0 || pos[i] >= size[i])
        throw std::out_of_range("DataStore coordinates are out of range");

    return this->operator[](pos);
  }

  /**
   * Gets the data stored at pos. (unsafe)
   */
  T &operator[](glm::vec<N, int> pos) {
    int index = 0;
    int acc = 1;
    for(int i = 0; i < N - 1; i++) {
      index += pos[i] * acc;
      acc *= size[i];
    }
    index += pos[N-1] * acc;
    return map[index];
  }

  /**
   * Gets the data stored at pos. (unsafe)
   */
  T const& operator[](glm::vec<N, int> pos) const {
    int index = 0;
    int acc = 1;
    for(int i = 0; i < N - 1; i++) {
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

  /**
   * Gets the raw pointer data (motable)
   * /!\ proceed with care when manipulating the internal memory.
   */
  T* mut_c_ptr() {
    return map;
  }

  const glm::vec<N, int> size;

protected:
  T *map;
};
