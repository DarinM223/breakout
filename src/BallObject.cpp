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
