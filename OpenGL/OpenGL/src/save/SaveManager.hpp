#pragma once

#include <glm/glm.hpp>
#include "terrain/AbstractChunk.hpp"
#include "entity/Entity.hpp"

class Entity;

enum class EntityClass{Character};

class SaveManager
{
public:
    static std::unique_ptr<AbstractChunk> loadChunk(glm::ivec3 chunkPos);
    static bool saveChunk(AbstractChunk const& chunk);

    // TODO
    // static std::unique_ptr<Entity> loadEntity(Identifier uid);
    // static bool saveEntity(const Entity& entity);

    static std::string chunkSaveDir, entitySaveDir, configSaveDir;

private:
    SaveManager() = delete;
};

namespace Serde {
    std::ostream &operator<<(std::ostream &stream, const glm::vec3 &vec);
    std::istream &operator>>(std::istream &stream, glm::vec3 &vec);

    std::ostream &operator<<(std::ostream &stream, State state);
    std::istream &operator>>(std::istream &stream, State &state);

    std::ostream &operator<<(std::ostream &stream, Facing facing);
    std::istream &operator>>(std::istream &stream, Facing &facing);

    std::ostream &operator<<(std::ostream &stream, EntityClass entityClass);
    std::istream &operator>>(std::istream &stream, EntityClass &entityClass);

    std::ostream &operator<<(std::ostream &stream, const Node &node);
    std::istream &operator>>(std::istream &stream, Node &node);

    std::ostream &operator<<(std::ostream &stream, const Entity &entity);
    std::istream &operator>>(std::istream &stream, Entity &entity);

    std::ostream &operator<<(std::ostream &stream, BlockType type);
    std::istream &operator>>(std::istream &stream, BlockType &type);

    std::istream &operator>>(std::istream &stream, AbstractChunk &chunk);
    std::ostream &operator<<(std::ostream &stream, AbstractChunk const &chunk);
}
