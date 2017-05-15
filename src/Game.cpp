#include "Game.h"
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

Game::Game(int width, int height, ResourceManager &manager)
    : manager_(manager), width_(width), height_(height) {}

void Game::init() {
  manager_.loadShader("./shaders/sprite.vs", "./shaders/sprite.frag", "sprite");
  auto width = static_cast<GLfloat>(width_);
  auto height = static_cast<GLfloat>(height_);
  auto projection = glm::ortho(0.0f, width, height, 0.0f, -1.0f, 1.0f);

  auto &shader = manager_.getShader("sprite");
  shader.use();
  shader.setInteger("image", 0);
  shader.setMatrix4("projection", projection);

  // Load textures.
  manager_.loadTexture("./textures/awesomeface.png", {}, true, "face");
  manager_.loadTexture("./textures/background.jpg", {}, false, "background");
  manager_.loadTexture("./textures/block.png", {}, false, "block");
  manager_.loadTexture("./textures/block_solid.png", {}, false, "block_solid");
  manager_.loadTexture("./textures/paddle.png", {}, true, "paddle");

  // Load levels.
  Level<GameObject> level1{blockToDrawable}, level2{blockToDrawable},
      level3{blockToDrawable}, level4{blockToDrawable};
  level1.load(manager_, "./levels/one.lvl", width_, height_ * 0.5);
  level2.load(manager_, "./levels/two.lvl", width_, height_ * 0.5);
  level3.load(manager_, "./levels/three.lvl", width_, height_ * 0.5);
  level4.load(manager_, "./levels/four.lvl", width_, height_ * 0.5);

  levels_.emplace_back(std::move(level1));
  levels_.emplace_back(std::move(level2));
  levels_.emplace_back(std::move(level3));
  levels_.emplace_back(std::move(level4));
  level_ = 2;

  // Load player.
  glm::vec2 playerPos{width_ / 2 - PLAYER_SIZE.x / 2, height_ - PLAYER_SIZE.y};
  player_ = std::make_unique<GameObject>(playerPos, PLAYER_SIZE,
                                         manager_.getTexture("paddle"));

  renderer_ = std::make_unique<SpriteRenderer>(shader);
}

void Game::processInput() {
  if (state_ == State::Active) {
    GLfloat velocity = PLAYER_VELOCITY * dt_;
    if (keys_[GLFW_KEY_A]) {
      if (player_->options.position.x >= 0) {
        player_->options.position.x -= velocity;
      }
    }
    if (keys_[GLFW_KEY_D]) {
      if (player_->options.position.x <= width_ - player_->options.size.x) {
        player_->options.position.x += velocity;
      }
    }
  }
}

void Game::update() {}

void Game::render() {
  if (state_ == State::Active) {
    auto &texture = manager_.getTexture("background");
    RendererOptions options{
        {0, 0}, {width_, height_}, 0.0f, {1.0f, 1.0f, 1.0f}};
    renderer_->drawSprite(texture, options);

    player_->draw(*renderer_);
    levels_[level_].draw(*renderer_);
  }
}

void Game::updateTime(GLfloat time) {
  dt_ = time - lastTime_;
  lastTime_ = time;
}
