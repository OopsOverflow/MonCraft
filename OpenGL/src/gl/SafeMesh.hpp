#pragma once

#include <memory>
#include <mutex>
#include <vector>

class Mesh;

class SafeMesh {

public:
  SafeMesh(std::unique_ptr<Mesh> mesh);
  ~SafeMesh();

  Mesh* get();

  static void update();

private:
  std::unique_ptr<Mesh> mesh;
  static std::mutex mutex;
  static std::vector<std::unique_ptr<Mesh>> destructions;
};
