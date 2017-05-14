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
    this->vao_ = other.vao_;
    other.vao_ = -1;
  }
  SpriteRenderer& operator=(SpriteRenderer&& other) {
    this->release();
    this->shader_ = std::move(other.shader_);
    this->vao_ = other.vao_;
    other.vao_ = -1;
    return *this;
  }

  void drawSprite(Texture& texture, const RendererOptions& options);

 private:
  void release() { glDeleteVertexArrays(1, &this->vao_); }

  Shader& shader_;
  GLuint vao_;
};

#endif
