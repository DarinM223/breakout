#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"

struct RendererOptions {
  glm::vec2 position{0, 0};
  glm::vec2 size{10, 10};
  GLfloat rotate{0.0f};
  glm::vec3 color{1.0f};
};

class SpriteRenderer {
 public:
  SpriteRenderer();
  ~SpriteRenderer() { this->release(); }

  SpriteRenderer(const SpriteRenderer&) = delete;
  SpriteRenderer& operator=(const SpriteRenderer&) = delete;

  SpriteRenderer(SpriteRenderer&& other) {
    this->vao_ = other.vao_;
    other.vao_ = -1;
  }
  SpriteRenderer& operator=(SpriteRenderer&& other) {
    this->release();
    this->vao_ = other.vao_;
    other.vao_ = -1;
    return *this;
  }

  void drawSprite(const Shader& shader, const Texture& texture,
                  const RendererOptions& options) const;

 private:
  void release() { glDeleteVertexArrays(1, &this->vao_); }

  GLuint vao_;
};

#endif
