#include "Particle.h"

ParticleGenerator::ParticleGenerator(Shader &shader, Texture &texture,
                                     GLuint amount)
    : shader_(shader), texture_(texture), amount_(amount) {
  GLuint vbo;
  GLfloat particleQuad[] = {0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
                            0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                            1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f};
  glGenVertexArrays(1, &vao_);
  glGenBuffers(1, &vbo);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(particleQuad), particleQuad,
               GL_STATIC_DRAW);

  glBindVertexArray(vao_);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                        (GLvoid *)0);
  glBindVertexArray(0);

  for (GLuint i = 0; i < amount_; i++) {
    particles_.emplace_back();
  }
}

ParticleGenerator::ParticleGenerator(ParticleGenerator &&other)
    : shader_(other.shader_), texture_(other.texture_) {
  particles_ = std::move(other.particles_);
  amount_ = other.amount_;
  vao_ = other.vao_;
  other.valid_ = false;
}

ParticleGenerator &ParticleGenerator::operator=(ParticleGenerator &&other) {
  this->release();
  shader_ = std::move(other.shader_);
  texture_ = std::move(other.texture_);
  particles_ = std::move(other.particles_);
  amount_ = other.amount_;
  vao_ = other.vao_;
  other.valid_ = false;
  return *this;
}

void ParticleGenerator::update(GLfloat dt, GameObject &object,
                               GLuint newParticles, glm::vec2 offset) {
  // For every new particle, find a particle that is dead and respawn it.
  for (GLuint i = 0; i < newParticles; i++) {
    auto unusedParticle = this->firstUnusedParticle();
    this->respawnParticle(particles_[unusedParticle], object, offset);
  }

  // Update all particles.
  for (GLuint i = 0; i < amount_; i++) {
    Particle &p = particles_[i];
    p.life -= dt;
    if (p.life > 0.0f) {
      p.position -= p.velocity * dt;
      p.color.a -= dt * 2.5;
    }
  }
}

void ParticleGenerator::draw() {
  // Uses additive blending to give 'glow' effect.
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  shader_.use();

  for (const auto &particle : particles_) {
    if (particle.life > 0.0f) {
      shader_.setVector2("offset", particle.position);
      shader_.setVector4("color", particle.color);
      texture_.bind();
      glBindVertexArray(vao_);
      glDrawArrays(GL_TRIANGLES, 0, 6);
      glBindVertexArray(0);
    }
  }

  // Reset blending.
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

GLuint ParticleGenerator::firstUnusedParticle() {
  for (GLuint i = lastUsedParticle_; i < amount_; i++) {
    if (particles_[i].life <= 0.0f) {
      lastUsedParticle_ = i;
      return i;
    }
  }

  for (GLuint i = 0; i < lastUsedParticle_; i++) {
    if (particles_[i].life <= 0.0f) {
      lastUsedParticle_ = i;
      return i;
    }
  }

  lastUsedParticle_ = 0;
  return 0;
}

void ParticleGenerator::respawnParticle(Particle &particle, GameObject &object,
                                        glm::vec2 offset) {
  GLfloat random = ((std::rand() % 100) - 50) / 10.0f;
  GLfloat rColor = 0.5 + ((std::rand() % 100) / 100.0f);
  particle.position = object.options.position + random + offset;
  particle.color = glm::vec4{rColor, rColor, rColor, 1.0f};
  particle.life = 1.0f;
  particle.velocity = object.velocity * 0.1f;
}
