#include <list>
#include <memory>
#include <stdexcept>


/**
 * A FIFO container that allows moving items to the back.
 *
 * The container supports back-insertions, front-deletions and moving an element
 * to the back. elements are accessed through iterators returned by push().
 */

template<typename T>
class PriorityList : private std::list<T> {
  using list_t = std::list<T>;
  using iter_t = typename list_t::iterator;

public:
  using std::list<T>::list;
  using std::list<T>::begin;
  using std::list<T>::end;
  using std::list<T>::size;

  iter_t push(T const& val) {
    return list_t::insert(list_t::end(), val);
  }

  iter_t promote(iter_t it) {
    T val = *it;
    list_t::erase(it);
    return push(std::move(val));
  }

  void pop() {
    list_t::pop_front();
  }
};
