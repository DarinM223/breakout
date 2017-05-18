#ifndef PARTICLE_H
#define PARTICLE_H

#include <GL/glew.h>
#include <vector>
#include "Object.h"
#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"

struct Particle {
  glm::vec2 position;
  glm::vec2 velocity;
  glm::vec4 color;
  GLfloat life;

  Particle() : position(0.0f), velocity(0.0f), color(1.0f), life(0.0f) {}
};

class ParticleGenerator {
 public:
  ParticleGenerator(Shader& shader, Texture& texture, GLuint amount);
  ~ParticleGenerator() { this->release(); }

  ParticleGenerator() = delete;
  ParticleGenerator(const ParticleGenerator&) = delete;
  ParticleGenerator& operator=(const ParticleGenerator&) = delete;

  ParticleGenerator(ParticleGenerator&& other)
      : shader_(other.shader_), texture_(other.texture_) {
    particles_ = std::move(other.particles_);
    amount_ = other.amount_;
    vao_ = other.vao_;
    other.valid_ = false;
  }

  ParticleGenerator& operator=(ParticleGenerator&& other) {
    this->release();
    shader_ = std::move(other.shader_);
    texture_ = std::move(other.texture_);
    particles_ = std::move(other.particles_);
    amount_ = other.amount_;
    vao_ = other.vao_;
    other.valid_ = false;
    return *this;
  }

  void update(GLfloat dt, GameObject& object, GLuint newParticles,
              glm::vec2 offset);

  void draw();

 private:
  void release() {
    if (valid_) {
      glDeleteVertexArrays(1, &vao_);
    }
  }

  GLuint firstUnusedParticle();
  void respawnParticle(Particle& particle, GameObject& object,
                       glm::vec2 offset);

  Shader& shader_;
  Texture& texture_;
  std::vector<Particle> particles_;
  GLuint amount_;
  GLuint lastUsedParticle_;

  GLuint vao_;
  bool valid_{true};
};

#endif
