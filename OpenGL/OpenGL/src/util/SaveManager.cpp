#include <filesystem>
#include <fstream>
#include <SDL2/SDL_keyboard.h>

#include "SaveManager.hpp"
#include "Serde.hpp"
#include "blocks/AllBlocks.hpp"
#include "debug/Debug.hpp"
#include "entity/character/Character.hpp"
#include "zstr.hpp"

using Serde::Binary;

std::string SaveManager::chunkSaveDir = "save/defaultWorld/chunks";
std::string SaveManager::entitySaveDir = "save/defaultWorld/entities";
std::string SaveManager::configSaveDir = "save";
std::string SaveManager::configFilename = "config.txt";

SaveManager::SaveManager()
  : config(loadConfig())
{}

SaveManager::~SaveManager() {
  saveConfig();
}

SaveManager &SaveManager::getInst() {
  static SaveManager inst;
  return inst;
}

Config &SaveManager::getConfig() { return config; }

Config SaveManager::loadConfig() {
  std::ifstream openedFile(configSaveDir + "/" + configFilename);
  if (!openedFile.is_open())
    return Config();

  Config config;
  std::string readLine;
  std::string param;

#define KEY_PARAM(NAME)                                                        \
  if (param == #NAME ":") {                                                    \
    config.NAME = SDL_GetKeyFromName(Serde::remainder(ss).c_str());            \
    continue;                                                                  \
  }
#define PARAM(NAME)                                                            \
  if (param == #NAME ":") {                                                    \
    ss >> config.NAME;                                                         \
    continue;                                                                  \
  }

  while (getline(openedFile, readLine)) {
    std::stringstream ss;
    ss >> std::boolalpha;
    ss.str(readLine);
    ss >> param;

    PARAM(seed)
    PARAM(multiplayer)
    PARAM(serverAddr)
    PARAM(serverPort)
    PARAM(renderDistH)
    PARAM(renderDistV)
    PARAM(threadCount)
    PARAM(fov)
    KEY_PARAM(forward)
    KEY_PARAM(backward)
    KEY_PARAM(left)
    KEY_PARAM(right)
    KEY_PARAM(jump)
    KEY_PARAM(sneak)
    KEY_PARAM(view)
    KEY_PARAM(menu)
  }

#undef KEY_PARAM
#undef PARAM

  config.fov = std::min(180.0f, std::max(0.0f, config.fov));
  return config;
}

bool SaveManager::saveConfig() {
  std::filesystem::create_directories(configSaveDir);
  std::string filePath = configSaveDir + "/" + configFilename;
  std::ofstream openedFile(filePath, std::fstream::trunc);
  if (!openedFile) {
    std::cout << "[WARN] failed to open file: " << filePath << std::endl;
    return false;
  }
  openedFile << std::boolalpha << "MonCraft v1.1.0" << std::endl;

  #define KEY_PARAM(NAME)                                                      \
    openedFile << "\t" #NAME ": " << SDL_GetKeyName(config.NAME) << std::endl;
  #define PARAM(NAME)                                                          \
    openedFile << "\t" #NAME ": " << config.NAME << std::endl;

  PARAM(seed)
  PARAM(multiplayer)
  PARAM(serverAddr)
  PARAM(serverPort)
  PARAM(renderDistH)
  PARAM(renderDistV)
  PARAM(threadCount)
  PARAM(fov)
  KEY_PARAM(forward)
  KEY_PARAM(backward)
  KEY_PARAM(left)
  KEY_PARAM(right)
  KEY_PARAM(jump)
  KEY_PARAM(sneak)
  KEY_PARAM(view)
  KEY_PARAM(menu)

  #undef KEY_PARAM
  #undef PARAM

  return true;
}

std::ostream &operator<<(std::ostream &stream, const glm::vec3 &vec) {
  Binary<float> x = vec.x, y = vec.y, z = vec.z;
  stream << x << y << z;
  return stream;
}
std::istream &operator>>(std::istream &stream, glm::vec3 &vec) {
  Binary<float> x, y, z;
  stream >> x >> y >> z;
  vec.x = x.val;
  vec.y = y.val;
  vec.z = z.val;
  return stream;
}

std::ostream &operator<<(std::ostream &stream, State state) {
  Binary temp((uint8_t)state);
  stream << temp;
  return stream;
}
std::istream &operator>>(std::istream &stream, State &state) {
  Binary<uint8_t> temp;
  stream >> temp;
  state = (State)temp.val;
  return stream;
}

std::ostream &operator<<(std::ostream &stream, Facing facing) {
  Binary temp((uint8_t)facing);
  stream << temp;
  return stream;
}
std::istream &operator>>(std::istream &stream, Facing &facing) {
  Binary<uint8_t> temp;
  stream >> temp;
  facing = (Facing)temp.val;
  return stream;
}

std::ostream &operator<<(std::ostream &stream, EntityClass entityClass) {
  Binary temp((uint8_t)entityClass);
  stream << temp;
  return stream;
}
std::istream &operator>>(std::istream &stream, EntityClass &entityClass) {
  Binary<uint8_t> temp;
  stream >> temp;
  entityClass = (EntityClass)temp.val;
  return stream;
}

std::ostream &operator<<(std::ostream &stream, const Node &node) {
  stream << node.loc << node.rot << node.sca;
  return stream;
}
std::istream &operator>>(std::istream &stream, Node &node) {
  glm::vec3 loc, rot, sca;

  stream >> loc >> rot >> sca;
  node.loc = loc;
  node.rot = rot;
  node.sca = sca;
  return stream;
}

std::ostream &operator<<(std::ostream &stream, const Entity &entity) {
  stream << entity.getNode() << entity.getHead() << entity.state;
  return stream;
}
std::istream &operator>>(std::istream &stream, Entity &entity) {
  Node head, node;
  stream >> node >> head >> entity.state;
  entity.setNode(node);
  entity.setHead(head);
  return stream;
}

std::ostream &operator<<(std::ostream &stream, const Character &character) {
  stream << EntityClass::Character << (const Entity &)character;
  return stream;
}

bool SaveManager::saveEntity(const Entity &entity) {
  std::filesystem::create_directories(entitySaveDir);
  std::string filePath =
      entitySaveDir + "/entity_" + std::to_string(entity.uid) + ".entity";
  std::ofstream openedFile(
      filePath, std::fstream::trunc | std::fstream::binary | std::fstream::out);
  if (!openedFile) {
    std::cout << "[WARN] failed to open file: " << filePath << std::endl;
    return false;
  }

  openedFile << entity;
  openedFile.close();
  return true;
}

std::unique_ptr<Entity> SaveManager::getEntity(Identifier uid) {
  std::string filePath =
      entitySaveDir + "/entity_" + std::to_string(uid) + ".entity";
  std::ifstream openedFile(filePath, std::fstream::binary | std::fstream::in);
  if (!openedFile.is_open())
    return std::unique_ptr<Entity>(nullptr);

  Entity *entity = nullptr;
  EntityClass entityClass;
  openedFile >> entityClass;
  switch (entityClass) {
  case EntityClass::Character:
    entity = new Character({});
    break;
  default :
      throw std::runtime_error("Unknown entity type");
  }
  openedFile >> *entity;

  return std::unique_ptr<Entity>(entity);
}

std::ostream &operator<<(std::ostream &stream, BlockType type) {
  Binary bin((uint8_t)type);
  stream << bin;
  return stream;
}
std::istream &operator>>(std::istream &stream, BlockType &type) {
  Binary<uint8_t> temp;
  stream >> temp;
  type = (BlockType)temp.val;
  return stream;
}

std::istream &operator>>(std::istream &stream, Chunk &chunk) {
  glm::vec3 pos(0);
  for (pos.y = 0; pos.y < chunk.size.y; pos.y++) {
    for (pos.z = 0; pos.z < chunk.size.z; pos.z++) {
      for (pos.x = 0; pos.x < chunk.size.x; pos.x++) {
        chunk.at(pos) = AllBlocks::deserialize(stream);
      }
    }
  }

  return stream;
}
std::ostream &operator<<(std::ostream &stream, Chunk const &chunk) {
  glm::vec3 pos(0);
  for (pos.y = 0; pos.y < chunk.size.y; pos.y++) {
    for (pos.z = 0; pos.z < chunk.size.z; pos.z++) {
      for (pos.x = 0; pos.x < chunk.size.x; pos.x++) {
        AllBlocks::serialize(stream, chunk.at(pos));
      }
    }
  }

  return stream;
}

std::unique_ptr<Chunk> SaveManager::getChunk(glm::ivec3 chunkPos) {
  std::string filePath = chunkSaveDir + "/chunk_" +
                         std::to_string(chunkPos.x) + "_" +
                         std::to_string(chunkPos.y) + "_" +
                         std::to_string(chunkPos.z) + ".chunk";

  zstr::ifstream openedFile(filePath, std::fstream::binary);
  if (!openedFile.is_open())
    return nullptr;

  Binary<uint8_t> chunkSize;
  openedFile >> chunkSize;

  Chunk *newChunk = new Chunk(chunkPos, chunkSize.val);
  openedFile >> *newChunk;
  openedFile.close();

  return std::unique_ptr<Chunk>(newChunk);
}

bool SaveManager::saveChunk(Chunk const &chunk) {
  std::filesystem::create_directories(chunkSaveDir);
  std::string filePath = chunkSaveDir + "/chunk_" +
                         std::to_string(chunk.chunkPos.x) + "_" +
                         std::to_string(chunk.chunkPos.y) + "_" +
                         std::to_string(chunk.chunkPos.z) + ".chunk";

  zstr::ofstream openedFile(filePath, std::fstream::trunc | std::fstream::binary);
  if (!openedFile) {
    std::cout << "[WARN] failed to open file: " << filePath << std::endl;
    return false;
  }

  Binary<uint8_t> chunkSize(chunk.size.x);
  openedFile << chunkSize << chunk;
  openedFile.close();
  return true;
}

sf::Packet &operator<<(sf::Packet &packet, Chunk const &chunk) {
  std::ostringstream stream;
  stream << chunk;
  packet << stream.str();
  return packet;
}

sf::Packet &operator>>(sf::Packet &packet, Chunk &chunk) {
  std::string str;
  packet >> str;
  std::stringstream stream(str);
  stream >> chunk;
  return packet;
}
