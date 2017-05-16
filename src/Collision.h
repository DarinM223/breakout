#ifndef COLLISION_H
#define COLLISION_H

#include "BallObject.h"
#include "glm/glm.hpp"

enum class Direction { Up, Down, Left, Right };

struct Collision {
  bool occurred;
  Direction direction;
  glm::vec2 difference;

  Collision(bool occurred, Direction direction, glm::vec2 difference)
      : occurred(occurred), direction(direction), difference(difference) {}
};

Direction vectorDirection(glm::vec2 target);
Collision checkCollision(const BallObject& a, const GameObject& b);
bool checkCollision(const GameObject& a, const GameObject& b);

#endif
