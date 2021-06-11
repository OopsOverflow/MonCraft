#include "entity/Entity.hpp"
#include "../common/Identifier.hpp"


class ServerEntity : public Entity {

public:
  ServerEntity(Identifier uid, glm::vec3 pos, Hitbox box);

  Identifier getIdentifier() const;
  
private:
  void render() override;
  Identifier uid;
};
