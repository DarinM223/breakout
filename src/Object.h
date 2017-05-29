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
             glm::vec2 velocity = glm::vec2{0.0f, 0.0f});
  GameObject(const GameObject&) = default;
  GameObject& operator=(GameObject&& other);

  void draw(SpriteRenderer& renderer) override;
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
          Texture& texture);
  Powerup(Powerup&&) = default;
  Powerup& operator=(Powerup&& other);
};

GameObject blockToDrawable(ResourceManager& manager, GLuint tile, GLuint x,
                           GLuint y, GLfloat unitWidth, GLfloat unitHeight);

#endif
