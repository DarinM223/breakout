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
    pb = p;
    (void)pb;
  }

 protected:
  IsDerivedFrom() {
    void (*p)(D*) = Constraints;
    p = Constraints;
    (void)p;
  }
};

template <typename D>
class IsDerivedFrom<D, void> {
  IsDerivedFrom() { char* p = (int*)0; }
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

  void draw(SpriteRenderer& renderer) override {
    if (!this->destroyed) {
      renderer.drawSprite(this->sprite, this->options);
    }
  }
};

GameObject blockToDrawable(ResourceManager& manager, GLuint tile, GLuint x,
                           GLuint y, GLfloat unitWidth, GLfloat unitHeight);

#endif
