#pragma once
#include <cstdint>

class Key {

public:
  Key(int32_t keycode);
  Key& operator=(int32_t);
  bool operator==(Key const&);

  bool isLower();
  bool isUpper();
  bool isLetter();
  bool isDigit();
  bool isAlphaNumeric();
  bool isPrintable();
  bool isAscii();
  bool isBackspace();
  bool isEsc();

  int32_t asKeycode();
  char asChar();

private:
  int32_t keycode;
};
