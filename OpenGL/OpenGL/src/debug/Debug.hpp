#pragma once
#include <iostream>
#include <iomanip>
#include <chrono>
#include <glm/glm.hpp>

class DebugTimer {

  static std::chrono::time_point<std::chrono::high_resolution_clock> start;
  static float timeElapsed;

public:
  static void debug_timer_start();
  static void debug_timer_end();
  static void debug_timer_add();
  static float debug_timer_time();
};

#define TIME(exec)                                                           \
  {                                                                          \
    DebugTimer::debug_timer_start();                                         \
  }                                                                          \
  exec {                                                                     \
    DebugTimer::debug_timer_end();                                           \
    std::cout << #exec << " took " << DebugTimer::debug_timer_time() << "ms" \
              << std::endl;                                                  \
  }

#define TIME_ADD(exec)                                                       \
  {                                                                          \
    DebugTimer::debug_timer_start();                                         \
  }                                                                          \
  exec { DebugTimer::debug_timer_add(); }

#define TIME_ADD_RESULT()                                                     \
  {                                                                           \
    std::cout << "operations took " << DebugTimer::debug_timer_time() << "ms" \
              << std::endl;                                                   \
  }

#ifndef DEBUG
#define ASSERT_GL_MAIN_THREAD()
#else

#include <thread>
#include <csignal>

static const std::thread::id mainThread = std::this_thread::get_id();

#define ASSERT_GL_MAIN_THREAD() { \
  const std::thread::id thisThread = std::this_thread::get_id(); \
  if(mainThread != thisThread) { \
    std::cout << "[CRITICAL] " << "gl called on worker thread " << thisThread \
              << " while main thread is " << mainThread << "." << std::endl; \
    std::raise(SIGINT); \
  } \
}
#endif

template<typename T>
void debug_bindump(T val) {
  for(int i = 8 * sizeof(T) -1; i >= 0; i--) {
    std::cout << (val >> i & 1);
  }
  std::cout << std::endl;
}

template<typename T, glm::length_t N>
std::ostream& operator<<(std::ostream& os, glm::vec<N, T> const& v) {
  // os << std::setprecision(5);
  os << "vec" << N << "(";
  for(size_t i = 0; i < N-1; i++)
    os << v[i] << ", ";
  os << v[N-1] << ")";
  return os;
}
