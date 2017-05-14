#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include "Shader.h"
#include "Texture.h"

struct RendererOptions {};

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

  void drawSprite(const Texture& texture, RendererOptions options) const;

 private:
  void release() { glDeleteVertexArrays(1, &this->vao_); }

  Shader& shader_;
  GLuint vao_;
};

#endif
