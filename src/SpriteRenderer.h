#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"

struct RendererOptions {
  RendererOptions(glm::vec2 position, glm::vec2 size, GLfloat rotate,
                  glm::vec3 color)
      : position(position), size(size), rotate(rotate), color(color) {}
  glm::vec2 position{0, 0};
  glm::vec2 size{10, 10};
  GLfloat rotate{0.0f};
  glm::vec3 color{1.0f};
};

class SpriteRenderer {
 public:
  SpriteRenderer(Shader& shader);
  ~SpriteRenderer() { this->release(); }

  SpriteRenderer() = delete;
  SpriteRenderer(const SpriteRenderer&) = delete;
  SpriteRenderer& operator=(const SpriteRenderer&) = delete;

  SpriteRenderer(SpriteRenderer&& other) : shader_(other.shader_) {
    vao_ = other.vao_;
    other.valid_ = false;
  }
  SpriteRenderer& operator=(SpriteRenderer&& other) {
    this->release();
    shader_ = std::move(other.shader_);
    vao_ = other.vao_;
    other.valid_ = false;
    return *this;
  }

  void drawSprite(Texture& texture, const RendererOptions& options);

 private:
  void release() {
    if (valid_) {
      glDeleteVertexArrays(1, &vao_);
    }
  }

  Shader& shader_;
  GLuint vao_;
  bool valid_{true};
};

#endif
