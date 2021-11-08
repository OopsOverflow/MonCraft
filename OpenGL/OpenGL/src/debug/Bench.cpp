#include "Bench.hpp"

#include <algorithm>
#include <numeric>
#include <iostream>
#include <fstream>

Bench::Bench() : enabled(true) {}

Bench* Bench::getInst() {
  static Bench inst;
  return &inst;
}

void Bench::capStart(Bench::cap_id_t id) {
  if(!enabled) return;
  auto now = std::chrono::high_resolution_clock::now();
  auto cap = std::find_if(captures.begin(), captures.end(), [&](cap_t const& cap) {
    return cap.id == id;
  });

  if(cap == captures.end()) {
    captures.emplace_back(cap_t{
      .id = id, .timer = now
    });
  }
  else {
    cap->timer = now;
  }
}

void Bench::capEnd(Bench::cap_id_t id) {
  if(!enabled) return;
  auto now = std::chrono::high_resolution_clock::now();
  auto cap = std::find_if(captures.begin(), captures.end(), [&](cap_t const& cap) {
    return cap.id == id;
  });

  if(cap != captures.end()) {
    auto delta = now - cap->timer;
    cap->timer = now;
    cap->values.push_back(std::chrono::duration_cast<std::chrono::microseconds>(delta).count());
  }
  else {
    std::cout << "[WARN] capEnd called but not started" << std::endl;
  }
}

void Bench::clear() {
  captures = {};
}

void Bench::toCSV(std::string const& filename) const {
  std::ofstream out;
  out.open(filename, std::ofstream::trunc);

  size_t maxLines = std::accumulate(captures.begin(), captures.end(), 0, [&](size_t acc, cap_t const& cap) {
    return std::max(acc, cap.values.size());
  });

  out << "\"index\" ";
  for(auto const& cap : captures) {
    out << '"' << cap.id << '"' << ' ';
  }
  out << std::endl;

  for (size_t i = 0; i < maxLines; i++) {
    out << i << " ";
    for(auto const& cap : captures) {
      if(cap.values.size() > i) {
        out << cap.values.at(i) << " ";
      }
      else {
        out << "\"\" ";
      }
    }
    out << std::endl;
  }
}

void Bench::setEnabled(bool enabled) {
  this->enabled = enabled;
}

void Bench::toggle() {
  enabled = !enabled;
}
