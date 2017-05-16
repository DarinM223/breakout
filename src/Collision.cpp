#include "Collision.h"
#include <stdexcept>

Direction vectorDirection(glm::vec2 target) {
  glm::vec2 compass[] = {glm::vec2{0.0f, 1.0f}, glm::vec2{1.0f, 0.0f},
                         glm::vec2{0.0f, -1.0f}, glm::vec2{-1.0f, 0.0f}};

  GLfloat max = 0.0f;
  GLuint maxIdx = -1;
  for (GLuint i = 0; i < 4; i++) {
    GLfloat dot = glm::dot(glm::normalize(target), compass[i]);
    if (dot > max) {
      max = dot;
      maxIdx = i;
    }
  }

  switch (maxIdx) {
    case 0:
      return Direction::Up;
    case 1:
      return Direction::Right;
    case 2:
      return Direction::Down;
    case 3:
      return Direction::Left;
    default:
      return Direction::Up;
  }
}

Collision checkCollision(const BallObject& a, const GameObject& b) {
  glm::vec2 center{a.options.position + a.options.size};
  glm::vec2 aabbHalfExtents{b.options.size.x / 2, b.options.size.y / 2};
  glm::vec2 aabbCenter{b.options.position.x + aabbHalfExtents.x,
                       b.options.position.y + aabbHalfExtents.y};
  glm::vec2 difference = center - aabbCenter;
  glm::vec2 clamped = glm::clamp(difference, -aabbHalfExtents, aabbHalfExtents);
  glm::vec2 closest = aabbCenter + clamped;

  difference = closest - center;
  if (glm::length(difference) <= a.radius) {
    return Collision{true, vectorDirection(difference), difference};
  }
  return Collision{false, Direction::Up, glm::vec2{0, 0}};
}

bool checkCollision(const GameObject& a, const GameObject& b) {
  bool collisionX =
      a.options.position.x + a.options.size.x >= b.options.position.x &&
      b.options.position.x + b.options.size.x >= a.options.position.x;

  bool collisionY =
      a.options.position.y + a.options.size.y >= b.options.position.y &&
      b.options.position.y + b.options.size.y >= a.options.position.y;

  return collisionX && collisionY;
}
