#include "Object.h"
#include <stdexcept>

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture& sprite,
                       glm::vec3 color, glm::vec2 velocity)
    : options(pos, size, 0.0f, color),
      velocity(velocity),
      isSolid(false),
      destroyed(false),
      sprite(sprite) {}

GameObject& GameObject::operator=(GameObject&& other) {
  options = other.options;
  velocity = other.velocity;
  isSolid = other.isSolid;
  destroyed = other.destroyed;
  sprite = std::move(other.sprite);
  return *this;
}

void GameObject::draw(SpriteRenderer& renderer) {
  if (!this->destroyed) {
    renderer.drawSprite(this->sprite, this->options);
  }
}

Powerup::Powerup(Type type, glm::vec3 color, GLfloat duration,
                 glm::vec2 position, Texture& texture)
    : GameObject(position, POWERUP_SIZE, texture, color, POWERUP_VELOCITY),
      type(type),
      duration(duration),
      activated(false) {}

Powerup& Powerup::operator=(Powerup&& other) {
  options = other.options;
  velocity = other.velocity;
  isSolid = other.isSolid;
  destroyed = other.destroyed;
  sprite = std::move(other.sprite);
  type = other.type;
  duration = other.duration;
  activated = other.activated;
  return *this;
}

GameObject blockToDrawable(ResourceManager& manager, GLuint tile, GLuint x,
                           GLuint y, GLfloat unitWidth, GLfloat unitHeight) {
  glm::vec2 pos{unitWidth * x, unitHeight * y};
  glm::vec2 size{unitWidth, unitHeight};
  if (tile == 1) {  // Solid
    GameObject obj{pos, size, manager.getTexture("block_solid"),
                   glm::vec3{0.8f, 0.8f, 0.7f}};
    obj.isSolid = true;
    return obj;
  } else if (tile > 1) {  // Non-solid
    glm::vec3 color{1.0f};
    switch (tile) {
      case 2:
        color = {0.2f, 0.6f, 1.0f};
        break;
      case 3:
        color = {0.0f, 0.7f, 0.0f};
        break;
      case 4:
        color = {0.8f, 0.8f, 0.4f};
        break;
      case 5:
        color = {1.0f, 0.5f, 0.0f};
        break;
    }
    return {pos, size, manager.getTexture("block"), color};
  }

  throw std::runtime_error{"Invalid tile"};
}
