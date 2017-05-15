#include "BallObject.h"

BallObject::BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity,
                       Texture& sprite)
    : GameObject(pos, glm::vec2{radius * 2, radius * 2}, sprite,
                 glm::vec3{1.0f}, velocity),
      radius(radius),
      stuck(true) {}

const glm::vec2& BallObject::move(GLfloat dt, GLuint windowWidth) {
  if (!this->stuck) {
    this->options.position += this->velocity * dt;
    // Reverse velocity if outside window bounds.
    if (this->options.position.x <= 0.0f) {
      this->velocity.x = -this->velocity.x;
      this->options.position.x = 0.0f;
    } else if (this->options.position.x + this->options.size.x >= windowWidth) {
      this->velocity.x = -this->velocity.x;
      this->options.position.x = windowWidth - this->options.size.x;
    }
    if (this->options.position.y <= 0.0f) {
      this->velocity.y = -this->velocity.y;
      this->options.position.y = 0.0f;
    }
  }

  return this->options.position;
}

void BallObject::reset(glm::vec2 position, glm::vec2 velocity) {
  this->options.position = position;
  this->velocity = velocity;
  this->stuck = true;
}

bool checkCollision(const BallObject& a, const GameObject& b) {
  glm::vec2 center{a.options.position + a.options.size};
  glm::vec2 aabbHalfExtents{b.options.size.x / 2, b.options.size.y / 2};
  glm::vec2 aabbCenter{b.options.position.x + aabbHalfExtents.x,
                       b.options.position.y + aabbHalfExtents.y};
  glm::vec2 difference = center - aabbCenter;
  glm::vec2 clamped = glm::clamp(difference, -aabbHalfExtents, aabbHalfExtents);
  glm::vec2 closest = aabbCenter + clamped;

  difference = closest - center;
  return glm::length(difference) < a.radius;
}
