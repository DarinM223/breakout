#ifndef BALL_OBJECT_H
#define BALL_OBJECT_H

#include "Object.h"

struct BallObject : public GameObject {
  GLfloat radius;
  GLboolean stuck;

  BallObject() = delete;
  BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity,
             Texture& sprite);

  const glm::vec2& move(GLfloat dt, GLuint windowWidth);
  void reset(glm::vec2 position, glm::vec2 velocity);
};

bool checkCollision(const BallObject& a, const GameObject& b);

#endif
