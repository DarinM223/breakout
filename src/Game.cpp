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

  // Load textures.
  manager_.loadTexture("./textures/awesomeface.png",
                       Dimensions{DimensionsType::Image}, true, "face");
  manager_.loadTexture("./textures/block.png",
                       Dimensions{DimensionsType::Image}, false, "block");
  manager_.loadTexture("./textures/block_solid.png",
                       Dimensions{DimensionsType::Image}, false, "block_solid");

  // Load levels.
  Level<GameObject> level1{blockToDrawable}, level2{blockToDrawable},
      level3{blockToDrawable}, level4{blockToDrawable};
  level1.load(manager_, "./levels/one.lvl", width_, height_ * 0.5);
  level2.load(manager_, "./levels/two.lvl", width_, height_ * 0.5);
  level3.load(manager_, "./levels/three.lvl", width_, height_ * 0.5);
  level4.load(manager_, "./levels/four.lvl", width_, height_ * 0.5);

  this->levels_.emplace_back(std::move(level1));
  this->levels_.emplace_back(std::move(level2));
  this->levels_.emplace_back(std::move(level3));
  this->levels_.emplace_back(std::move(level4));
  this->level_ = 2;

  this->renderer = std::make_unique<SpriteRenderer>(shader);
}

void Game::processInput() {}

void Game::update() {}

void Game::render() {
  if (this->state_ == State::Active) {
    auto &texture = this->manager_.getTexture("face");
    RendererOptions options{{200, 200}, {300, 400}, 45.0f, {0.0f, 1.0f, 0.0f}};
    this->renderer->drawSprite(texture, options);

    this->levels_[this->level_].draw(*this->renderer);
  }
}

void Game::updateTime(GLfloat time) {
  this->dt_ = time - this->lastTime_;
  this->lastTime_ = time;
}
