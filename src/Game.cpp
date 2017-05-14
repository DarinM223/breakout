#include "Game.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

Game::Game(int width, int height, ResourceManager &manager)
    : manager_(manager), width_(width), height_(height) {}

void Game::init() {
  this->manager_.loadShader("./shaders/sprite.vs", "./shaders/sprite.frag",
                            "sprite");
  auto width = static_cast<GLfloat>(this->width_);
  auto height = static_cast<GLfloat>(this->height_);
  auto projection = glm::ortho(0.0f, width, height, 0.0f, -1.0f, 1.0f);

  auto &shader = this->manager_.getShader("sprite");
  shader.use();
  shader.setInteger("image", 0);
  shader.setMatrix4("projection", projection);

  this->manager_.loadTexture("./textures/awesomeface.png",
                             Dimensions{DimensionsType::Image}, true, "face");
  this->renderer = std::make_unique<SpriteRenderer>(shader);
}

void Game::processInput() {}

void Game::update() {}

void Game::render() {
  auto &texture = this->manager_.getTexture("face");
  RendererOptions options{};
  this->renderer->drawSprite(texture, options);
}

void Game::updateTime(GLfloat time) {
  this->dt_ = time - this->lastTime_;
  this->lastTime_ = time;
}
