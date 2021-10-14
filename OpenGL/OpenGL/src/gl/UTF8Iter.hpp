#pragma once

#include <string>

// thanks http://www.nubaria.com/en/blog/?p=371


class UTF8Iter {
public:
  UTF8Iter(std::string::const_iterator it)
    : it(it)
  {}

  char32_t operator*() {
    char32_t res = 0;
    char c1 = *it;

    if(c1 & m1) {
      if(c1 & m3) {
        if(c1 & m4) {
          res = (c1 & 0x07) << 18;
          char c2 = *(it + 1);
          res +=  (c2 & 0x3f) << 12;
          char c3 = *(it + 2);
          res +=  (c3 & 0x3f) << 6;
          char c4 = *(it + 3);
          res += (c4 & 0x3f);
        }
        else {
          res = (c1 & 0x0f) << 12;
          char c2 = *(it + 1);
          res += (c2 & 0x3f) << 6;
          char c3 = *(it + 2);
          res +=  (c3 & 0x3f);
        }
      }
      else {
        res = (c1 & 0x1f) << 6;
        char c2 = *(it + 1);
        res += (c2 & 0x3f);
      }
    }
    else res = c1;

    return res;
  }

  UTF8Iter operator++() {
    char c1 = *it;

    std::string::difference_type offset = 1;

    if(c1 & m1) {
      if(c1 & m3) {
        if(c1 & m4)
          offset = 4;
        else
          offset = 3;
      }
      else {
        offset = 2;
      }
    }

    it += offset;

    return *this;
  }

  bool operator==(UTF8Iter o) { return it == o.it; }
  bool operator!=(UTF8Iter o) { return !(operator==(o)); }
private:
  std::string::const_iterator it;
  static const unsigned char m1 = 128;
  static const unsigned char m2 = 64;
  static const unsigned char m3 = 32;
  static const unsigned char m4 = 16;
};

class UTF8StringAdaptator {
public:
  UTF8StringAdaptator(std::string const& str)
    : strbeg(str.cbegin()), strend(str.cend())
  {}

  UTF8Iter begin() { return UTF8Iter(strbeg); }

  UTF8Iter end() { return UTF8Iter(strend); }

private:
  const std::string::const_iterator strbeg;
  const std::string::const_iterator strend;
};
