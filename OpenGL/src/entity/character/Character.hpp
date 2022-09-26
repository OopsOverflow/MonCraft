#pragma once

#include <glm/glm.hpp>

#include "entity/Entity.hpp"
#include "entity/Node.hpp"
#include "blocks/Block.hpp"



/**
 * The main character.
 */

class Character : public Entity {

public:

  /**
   * Creates the character at the given position.
   */
  Character();

  virtual ~Character();	

  void handleAction(Action action) override;

  void updateProperties() override;

  /**
   * Breaks the block in line of sight if within reach of the character.
   */
  void leftClick();

  /**
   * Place a block in front of the player, next to the nearest block face in
   * line of sight, if any.
   */
  void rightClick();

  /**
   * Choose the block in line of sight to be placed.
   */
  void middleClick();

  /**
   * Set the selected block type (in hand).
   */
  void setCurrentBlock(BlockType type);

  /**
   * Return the selected block type (in hand).
   */
  BlockType getCurrentBlock() const;

  sf::Packet& operator<<(sf::Packet& packet) override;
	sf::Packet& operator>>(sf::Packet& packet) const override;
	void consume(sf::Packet& packet) override;

protected:
  Node rootNode;

  float animState;

  BlockType currentBlock;

  bool dab;
  bool sprint;

};
