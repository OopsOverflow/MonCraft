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

template <typename T>
class Grid {
public:
  /**
   * An unitialized 2D data store.
   */
  Grid(size_t width, size_t height)
      : width(width), height(height), map(new T[width * height]) {}

  ~Grid() {
    delete[] map;
  }

  /**
   * Gets the data stored at pos.
   * @throws std::out_of_range
   */
  T &at(glm::ivec2 pos) {
    if (pos.x > (int)width || pos.y > (int)height) {
      throw std::out_of_range("bitmap coordinates are out of range");
    }

    return map[pos.x + pos.y * width];
  }

  /**
   * Calls Fn for each data point stored.
   * @param fn() //  TODO
   */
  template <class Fn>
  Fn for_each(Fn f) {
    for (size_t i = 0; i < width; i++) {
      for (size_t j = 0; j < height; j++) {
      	glm::ivec2 pos(i, j);
      	f(pos, at(pos));
      }
    }

    return f;
  }

  /**
   * Calls Fn for each data point stored asynchronously (multithreaded).
   * @param fn()
   */
  template <class Fn>
  void for_each_parallel(Fn f) {
    int end = width * height;

    #pragma omp parallel for
    for(int start = 0; start < end; start++) {
      glm::ivec2 pos(start % width, start / width);
      f(pos, at(pos));
    }
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

  glm::ivec2 size() {
    return glm::ivec2(width, height);
  }

  const size_t width;
  const size_t height;

private:
  T *map;
};

typedef glm::vec<3, uint8_t> pixel_t;
template class Grid<pixel_t>;
using Bitmap = Grid<pixel_t>;

template class Grid<glm::ivec2>;
using Grid2D = Grid<glm::ivec2>;
