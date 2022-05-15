#pragma once

#include <glm/glm.hpp>
#include <stdexcept>
#include <iostream>
#include <numeric>
#include <vector>


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
      : _size(size), map(getElementCount(size))
  {}


  /**
   * Constructs a DataStore of given size along all dimensions.
   */
  DataStore(int size) : DataStore(glm::vec<N, int>(size))
  {}

  DataStore(DataStore const&) = delete;
  DataStore& operator=(DataStore const&) = delete;

  DataStore(DataStore&& other) noexcept
    : _size(std::move(other.size)), map(std::move(other.map))
  {
    other.map = nullptr;
  }

  DataStore& operator=(DataStore&& other) {
    if (this != &other) {
      delete[] map;
      map = other.map;
      _size = other.size;
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
      maxIndex *= _size[i];

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
      maxIndex *= _size[i];

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
      if(pos[i] < 0 || pos[i] >= _size[i])
        throw std::out_of_range("DataStore coordinates are out of range");

    return this->operator[](pos);
  }

  /**
   * Gets the data stored at pos.
   * @throws std::out_of_range
   */
  T const& at(glm::vec<N, int> pos) const {
    for(int i = 0; i < N; i++)
      if(pos[i] < 0 || pos[i] >= _size[i])
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
      acc *= _size[i];
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
      acc *= _size[i];
    }
    index += pos[N-1] * acc;
    return map[index];
  }

  ///**
  // * Gets the raw pointer data
  // */
  //const T* c_ptr() const {
  //  return map;
  //}

  ///**
  // * Gets the raw pointer data (mutable)
  // * /!\ proceed with care when manipulating the internal memory.
  // */
  //T* mut_c_ptr() {
  //  return map;
  //}

  /**
   * Gets the DataStore size
   */
  glm::vec<N, int> size() const {
    return _size;
  }

  /**
   * Grows the DataStore by amount rows & cols
   */
  void grow(glm::vec<N, int> amount) {
    _size += amount;
    int newMapSize = getElementCount(_size);
    map.resize(newMapSize);
  }

  /**
   * Shrinks the DataStore by amount rows & cols
   */
  void shrink(glm::vec<N, int> amount) {
    _size -= amount;

    if(glm::any(_size < 0))
      throw std::runtime_error("size is negative after shrink");

    int newMapSize = getElementCount(_size);
    map.resize(newMapSize);
  }

protected:
  static int getElementCount(glm::vec<N, int> size) {
    if(glm::any(glm::equal(size, glm::vec<N, int>(0)))) return 0;

    int mapSize = 1;
    for(int i = 0; i < N; i++)
      mapSize *= size[i];

    return mapSize;
  }

private:
  glm::vec<N, int> _size;
  std::vector<T> map;
};
