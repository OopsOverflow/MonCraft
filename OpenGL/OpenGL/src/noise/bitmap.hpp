#pragma once

#include "../util/DataStore.hpp"

/**
 * A 2D data store.
 *
 * Stores data of type T in a 2D array of given dimensions.
 * Manages memory and allows iterating the stored points.
 * Defines Bitmap that holds an RGB8 image.
 */

template <typename T>
class Grid : public DataStore<T, 2> {
public:
  /**
   * An unitialized 2D data store.
   */
  Grid(glm::ivec2 size)
      : DataStore<T, 2>(size)
  {}
  Grid(int size)
      : DataStore<T, 2>(size)
  {}

  /**
   * Calls Fn for each data point stored.
   * @param fn() //  TODO
   */
  template <class Fn>
  Fn for_each(Fn f) {
    for (int i = 0; i < this->size.x; i++) {
      for (int j = 0; j < this->size.y; j++) {
      	glm::ivec2 pos(i, j);
      	f(pos, this->at(pos));
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
    int end = this->size.x * this->size.y;

    #pragma omp parallel for
    for(int start = 0; start < end; start++) {
      glm::ivec2 pos(start % this->size.x, start / this->size.y);
      f(pos, this->at(pos));
    }
  }
};

typedef glm::vec<3, uint8_t> pixel_t;
template class Grid<pixel_t>;
using Bitmap = Grid<pixel_t>;

template class Grid<glm::ivec2>;
using Grid2D = Grid<glm::ivec2>;
