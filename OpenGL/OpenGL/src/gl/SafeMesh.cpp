#include "SafeMesh.hpp"

#include "debug/Debug.hpp"
#include "gl/Mesh.hpp"

std::vector<std::unique_ptr<Mesh>> SafeMesh::destructions;
std::mutex SafeMesh::mutex;

SafeMesh::SafeMesh(std::unique_ptr<Mesh> mesh)
  : mesh(std::move(mesh))
{}

SafeMesh::~SafeMesh() {
  std::lock_guard<std::mutex> lck(mutex);
  destructions.push_back(std::move(mesh));
}

void SafeMesh::update() {
  std::lock_guard<std::mutex> lck(mutex);
  ASSERT_GL_MAIN_THREAD();
  destructions.clear();
}

Mesh* SafeMesh::get() {
  return mesh.get();
}
