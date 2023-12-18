#pragma once

#include <glm/glm.hpp>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <rtc/configuration.hpp>
#include <sstream>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <fmt/ostream.h>
#include <fmt/std.h>

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
}

template<typename T, glm::length_t N>
struct fmt::formatter<glm::vec<N, T>> : fmt::ostream_formatter {};

template<typename T>
struct fmt::formatter<glm::tmat4x4<T>> : fmt::ostream_formatter {};


#include <rtc/rtc.hpp>

template<>
struct fmt::formatter<rtc::Configuration> : fmt::ostream_formatter {};

template<>
struct fmt::formatter<rtc::Description> : fmt::ostream_formatter {};

template<>
struct fmt::formatter<rtc::Candidate> : fmt::ostream_formatter {};
