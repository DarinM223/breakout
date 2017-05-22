#ifndef OBJECT_H
#define OBJECT_H

#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "Texture.h"
#include "glm/glm.hpp"

template <typename D, typename B>
class IsDerivedFrom {
  static void Constraints(D* p) {
    B* pb = p;
    (void)pb;
  }

 protected:
  IsDerivedFrom() {
    void (*p)(D*) = Constraints;
    (void)p;
  }
};

struct Drawable {
  virtual void draw(SpriteRenderer& renderer) = 0;
};

struct GameObject : public Drawable {
  RendererOptions options;
  glm::vec2 velocity;
  GLboolean isSolid;
  GLboolean destroyed;
  Texture& sprite;

  GameObject() = delete;

  GameObject(glm::vec2 pos, glm::vec2 size, Texture& sprite,
             glm::vec3 color = glm::vec3{1.0f},
             glm::vec2 velocity = glm::vec2{0.0f, 0.0f})
      : options(pos, size, 0.0f, color),
        velocity(velocity),
        isSolid(false),
        destroyed(false),
        sprite(sprite) {}
  GameObject(const GameObject&) = default;
  GameObject& operator=(GameObject&& other) {
    options = other.options;
    velocity = other.velocity;
    isSolid = other.isSolid;
    destroyed = other.destroyed;
    sprite = std::move(other.sprite);
    return *this;
  }

  void draw(SpriteRenderer& renderer) override {
    if (!this->destroyed) {
      renderer.drawSprite(this->sprite, this->options);
    }
  }
};

const glm::vec2 POWERUP_SIZE{60, 20};
const glm::vec2 POWERUP_VELOCITY{0.0f, 150.0f};

struct Powerup : public GameObject {
  enum class Type {
    Speed,
    Sticky,
    PassThrough,
    PadSizeIncrease,
    Confuse,
    Chaos
  };

  Type type;
  GLfloat duration;
  bool activated;

  Powerup(Type type, glm::vec3 color, GLfloat duration, glm::vec2 position,
          Texture& texture)
      : GameObject(position, POWERUP_SIZE, texture, color, POWERUP_VELOCITY),
        type(type),
        duration(duration),
        activated(false) {}
  Powerup(Powerup&&) = default;
  Powerup& operator=(Powerup&& other) {
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
};

GameObject blockToDrawable(ResourceManager& manager, GLuint tile, GLuint x,
                           GLuint y, GLfloat unitWidth, GLfloat unitHeight);

#endif
