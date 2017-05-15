#include "SpriteRenderer.h"
#include "glm/gtc/matrix_transform.hpp"

SpriteRenderer::SpriteRenderer(Shader& shader) : shader_(shader) {
  GLuint vbo;
  GLfloat vertices[] = {0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f};

  glGenVertexArrays(1, &vao_);
  glGenBuffers(1, &vbo);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindVertexArray(vao_);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                        (GLvoid*)0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void SpriteRenderer::drawSprite(Texture& texture,
                                const RendererOptions& options) {
  shader_.use();
  glm::mat4 model{};
  model = glm::translate(model, glm::vec3{options.position, 0.0f});

  model = glm::translate(
      model, glm::vec3{0.5f * options.size.x, 0.5f * options.size.y, 0.0f});
  model = glm::rotate(model, options.rotate, glm::vec3{0.0f, 0.0f, 1.0f});
  model = glm::translate(
      model, glm::vec3{-0.5f * options.size.x, -0.5f * options.size.y, 0.0f});

  model = glm::scale(model, glm::vec3{options.size, 1.0f});

  shader_.setMatrix4("model", model);
  shader_.setVector3("spriteColor", options.color);

  glActiveTexture(GL_TEXTURE0);
  texture.bind();

  glBindVertexArray(vao_);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}
