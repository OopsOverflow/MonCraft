#include "entity/Entity.hpp"
#include "../common/Identifier.hpp"


class ServerEntity : public Entity {

public:
  ServerEntity(Identifier uid, glm::vec3 pos, Hitbox box);

  Identifier getIdentifier() const;

private:
  Identifier uid;
};
