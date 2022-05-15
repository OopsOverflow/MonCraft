#include "Key.hpp"

#include <SDL2/SDL_keycode.h> // sdl is kept as a weak implementation detail

Key::Key(int32_t keycode)
  : keycode(keycode)
{ }

bool Key::isLower() { return 'a' <= keycode && keycode <= 'z'; }
bool Key::isUpper() { return 'A' <= keycode && keycode <= 'Z'; }
bool Key::isLetter() { return isLower() || isUpper(); }
bool Key::isDigit() { return '0' <= keycode && keycode <= '9'; }
bool Key::isAscii() { return 0 <= keycode && keycode <= 255; }
bool Key::isPrintable() { return 32 <= keycode && keycode <= 126; }
bool Key::isAlphaNumeric() { return isAscii() || isDigit(); }
bool Key::isBackspace() { return keycode == SDLK_BACKSPACE; }
bool Key::isEsc() { return keycode == SDLK_ESCAPE; }

int32_t Key::asKeycode() { return keycode; }
char Key::asChar() { return (char)keycode; }
