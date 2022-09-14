#pragma once

#include <glm/glm.hpp>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <fmt/ostream.h>

class DebugTimer {

  static std::chrono::time_point<std::chrono::high_resolution_clock> start;
  static float timeElapsed;

public:
  static void debug_timer_start();
  static void debug_timer_end();
  static void debug_timer_add();
  static float debug_timer_time();
};

#ifndef DEBUG
#define ASSERT_GL_MAIN_THREAD()
#else

#include <csignal>
#include <thread>

static const std::thread::id mainThread = std::this_thread::get_id();

#define ASSERT_GL_MAIN_THREAD() { \
  const std::thread::id thisThread = std::this_thread::get_id(); \
  if(mainThread != thisThread) { \
    spdlog::critical("gl called on worker thread {} while main thread is {}.", thisThread, mainThread); \
    std::raise(SIGINT); \
  } \
}
#endif

template<typename T>
void debug_bindump(T val) {
  std::istringstream ss;
  for(int i = 8 * sizeof(T) -1; i >= 0; i--) {
    ss << (val >> i & 1);
  }
  spdlog::debug(ss.str());
}

namespace glm {
  template<typename T, length_t N>
  std::ostream& operator<<(std::ostream& os, vec<N, T> const& v) {
    // os << std::setprecision(5);
    os << "vec" << N << "(";
    for(length_t i = 0; i < N-1; i++)
      os << v[i] << ", ";
    os << v[N-1] << ")";
    return os;
  }

  template<typename T>
  std::ostream& operator<<(std::ostream& os, tmat4x4<T> const& mat) {
      os << std::fixed << std::setprecision(5);
      os << "mat: ";
      for (length_t i = 0; i < 4; i+=1) {
           os  << mat[i] << std::endl << "     ";
      }
      return os;
  }

  #if FMT_VERSION >= 90000 // fmt version >= 9 requires explicit formatter definitions.
    template<typename T, length_t N>
    struct fmt::formatter<vec<N, T>> : ostream_formatter{};
    template<typename T>
    struct fmt::formatter<tmat4x4<T>> : ostream_formatter{};
  #endif 
}

