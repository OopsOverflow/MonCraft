#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

#include "SaveManager.hpp"
#include "blocks/AllBlocks.hpp"
#include "debug/Debug.hpp"
#include "entity/character/Character.hpp"
#include <SDL2/SDL_keyboard.h>

std::string SaveManager::chunkSaveDir = "save/defaultWorld/chunks";
std::string SaveManager::entitySaveDir = "save/defaultWorld/entities";
std::string SaveManager::configSaveDir = "save";
std::string SaveManager::configFilename = "config.txt";

template <typename T> class Binary {
public:
  Binary() {}
  Binary(T val) : val(val) {}
  T val;
};
template <typename T>
std::ostream &operator<<(std::ostream &stream, Binary<T> &bin) {
  stream.write((char *)&bin.val, sizeof(T));
  return stream;
}
template <typename T>
std::istream &operator>>(std::istream &stream, Binary<T> &bin) {
  stream.read((char *)&bin.val, sizeof(T));
  return stream;
}

std::string remainder(std::stringstream &ss) {
  std::string s;
  std::string res;
  ss >> res;
  while (ss >> s) {
    res += " " + s;
  }
  return res;
}

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
    config.NAME = SDL_GetKeyFromName(remainder(ss).c_str());                   \
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
  }
  openedFile >> *entity;

  return std::unique_ptr<Entity>(entity);
}

std::ostream &operator<<(std::ostream &stream, BlockType &type) {
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
  int maxIndex = chunk.size.x * chunk.size.y * chunk.size.z;
  uint16_t offset = 0;

  while (offset < maxIndex) {
    Binary<uint16_t> blockCount;
    BlockType type;
    stream >> blockCount >> type;

    for (uint16_t i = 0; i < blockCount.val; i += 1) {
      chunk.at(offset + i) = AllBlocks::create_static(type);
    }

    offset += blockCount.val;
  }

  return stream;
}
std::ostream &operator<<(std::ostream &stream, Chunk const &chunk) {
  int maxIndex = chunk.size.x * chunk.size.y * chunk.size.z;
  Binary<uint16_t> blockCount(1);
  BlockType last = chunk.at(0)->type;

  for (int i = 1; i < maxIndex; i += 1) {
    if (chunk.at(i)->type == last) {
      blockCount.val += 1;
    } else {
      stream << blockCount << last;
      blockCount.val = 1;
      last = chunk.at(i)->type;
    }
  }

  stream << blockCount << last;
  return stream;
}

std::unique_ptr<Chunk> SaveManager::getChunk(glm::ivec3 chunkPos) {
  std::string filePath = chunkSaveDir + "/chunk_" +
                         std::to_string(chunkPos.x) + "_" +
                         std::to_string(chunkPos.y) + "_" +
                         std::to_string(chunkPos.z) + ".chunk";

  std::ifstream openedFile(filePath, std::fstream::binary);
  if (!openedFile.is_open())
    return nullptr;

  Binary<uint8_t> chunkSize;
  openedFile >> chunkSize;

  if (chunkPos == glm::ivec3(0, 2, -1)) {
  }

  Chunk *newChunk = new Chunk(chunkPos, chunkSize.val);
  openedFile >> *newChunk;
  openedFile.close();

  return std::unique_ptr
<Chunk>(newChunk);
}

bool SaveManager::saveChunk(Chunk const &chunk) {
  std::filesystem::create_directories(chunkSaveDir);
  std::string filePath = chunkSaveDir + "/chunk_" +
                         std::to_string(chunk.chunkPos.x) + "_" +
                         std::to_string(chunk.chunkPos.y) + "_" +
                         std::to_string(chunk.chunkPos.z) + ".chunk";

  std::ofstream openedFile(filePath, std::fstream::trunc | std::fstream::binary);
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
