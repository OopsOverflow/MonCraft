#pragma once

#include <vector>
#include <string>
#include <chrono>
#include <algorithm>


class Bench {

public:
  using cap_id_t = std::string;
  static Bench* getInst();
  void capStart(cap_id_t id);
  void capEnd(cap_id_t id);
  void toCSV(std::string const& filename) const;
  void clear();
  void setEnabled(bool enabled);
  void toggle();

private:
  Bench();

  struct cap_t {
    cap_id_t id;
    std::chrono::time_point<std::chrono::high_resolution_clock> timer;
    std::vector<long> values;
  };

  std::vector<cap_t> captures;
  bool enabled;
};
