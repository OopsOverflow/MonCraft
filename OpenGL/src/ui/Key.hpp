#pragma once
#include <cstdint>

class Key {

public:
  Key(int32_t keycode);

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
