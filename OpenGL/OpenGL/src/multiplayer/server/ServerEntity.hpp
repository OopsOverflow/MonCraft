#include "entity/Entity.hpp"
#include "../common/Identifier.hpp"


class ServerEntity : public Entity {

public:
  ServerEntity(Identifier uid, glm::vec3 pos, Hitbox box);

  void render() override;

private:
  Identifier uid;
};
