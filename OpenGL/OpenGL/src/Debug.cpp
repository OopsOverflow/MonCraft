#include "Debug.hpp"

std::chrono::time_point<std::chrono::high_resolution_clock> DebugTimer::start;
float  DebugTimer::timeElapsed;

void DebugTimer::debug_timer_start() {
  start = std::chrono::high_resolution_clock::now();
}

void DebugTimer::debug_timer_end() {
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
  timeElapsed = duration.count() / (float)1000;
}

float DebugTimer::debug_timer_time() {
  return timeElapsed;
}
