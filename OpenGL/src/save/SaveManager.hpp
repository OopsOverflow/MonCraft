#pragma once

#include <glm/glm.hpp>
#include <iosfwd>
#include <memory>
#include <string>

#include "blocks/Block.hpp"
#include "entity/Entity.hpp"
#include "terrain/ChunkImpl.hpp"
#include "multiplayer/Packet.hpp"
#include "util/Identifier.hpp"

class Node;

class SaveManager
{
public:
    static std::unique_ptr<ChunkImpl> loadChunk(glm::ivec3 chunkPos);
    static bool saveChunk(ChunkImpl const& chunk);

    static std::unique_ptr<Entity> loadEntity(Identifier uid);
    static bool saveEntity(Entity& entity, EntityType type, Identifier uid);

    static std::string chunkSaveDir, entitySaveDir, configSaveDir;

private:
    SaveManager() = delete;
};

namespace serde {
    std::ostream &operator<<(std::ostream &stream, const glm::vec3 &vec);
    std::istream &operator>>(std::istream &stream, glm::vec3 &vec);

    std::ostream &operator<<(std::ostream &stream, State state);
    std::istream &operator>>(std::istream &stream, State &state);

    std::ostream &operator<<(std::ostream &stream, Facing facing);
    std::istream &operator>>(std::istream &stream, Facing &facing);

    std::ostream &operator<<(std::ostream &stream, EntityType type);
    std::istream &operator>>(std::istream &stream, EntityType &type);

    std::ostream &operator<<(std::ostream &stream, const Node &node);
    std::istream &operator>>(std::istream &stream, Node &node);

    std::ostream &operator<<(std::ostream &stream, BlockType type);
    std::istream &operator>>(std::istream &stream, BlockType &type);

    std::istream &operator>>(std::istream &stream, ChunkImpl &chunk);
    std::ostream &operator<<(std::ostream &stream, ChunkImpl const &chunk);
}
