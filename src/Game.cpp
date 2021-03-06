#include "Game.h"
#include <GLFW/glfw3.h>
#include <algorithm>
#include "Collision.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

const glm::vec2 PLAYER_SIZE{100, 20};
const GLfloat PLAYER_VELOCITY{500.0f};

const glm::vec2 BALL_VELOCITY{100.0f, -350.0f};
const GLfloat BALL_RADIUS = 12.5f;

bool shouldSpawn(GLuint chance) {
  GLuint random = std::rand() % chance;
  return random == 0;
}

bool isOtherPowerupActive(const std::vector<Powerup> &powerups,
                          Powerup::Type type) {
  for (const auto &powerup : powerups) {
    if (powerup.activated && powerup.type == type) {
      return true;
    }
  }
  return false;
}

Game::Game(int width, int height, ResourceManager &manager)
    : manager_(manager), width_(width), height_(height) {}

void Game::init() {
  manager_.loadShader("./shaders/particle.vs", "./shaders/particle.frag",
                      "particle");
  manager_.loadShader("./shaders/sprite.vs", "./shaders/sprite.frag", "sprite");
  manager_.loadShader("./shaders/postprocessor.vs",
                      "./shaders/postprocessor.frag", "postprocessor");
  auto width = static_cast<GLfloat>(width_);
  auto height = static_cast<GLfloat>(height_);
  auto projection = glm::ortho(0.0f, width, height, 0.0f, -1.0f, 1.0f);

  auto &spriteShader = manager_.getShader("sprite");
  spriteShader.use();
  spriteShader.setInteger("image", 0);
  spriteShader.setMatrix4("projection", projection);

  auto &particleShader = manager_.getShader("particle");
  particleShader.use();
  particleShader.setInteger("sprite", 0);
  particleShader.setMatrix4("projection", projection);

  // Load textures.
  manager_.loadTexture("./textures/awesomeface.png", {}, true, "face");
  manager_.loadTexture("./textures/background.jpg", {}, false, "background");
  manager_.loadTexture("./textures/block.png", {}, false, "block");
  manager_.loadTexture("./textures/block_solid.png", {}, false, "block_solid");
  manager_.loadTexture("./textures/paddle.png", {}, true, "paddle");
  manager_.loadTexture("./textures/particle.png", {}, true, "particle");
  manager_.loadTexture("./textures/powerup_chaos.png", {}, true,
                       "powerup_chaos");
  manager_.loadTexture("./textures/powerup_confuse.png", {}, true,
                       "powerup_confuse");
  manager_.loadTexture("./textures/powerup_increase.png", {}, true,
                       "powerup_increase");
  manager_.loadTexture("./textures/powerup_passthrough.png", {}, true,
                       "powerup_passthrough");
  manager_.loadTexture("./textures/powerup_speed.png", {}, true,
                       "powerup_speed");
  manager_.loadTexture("./textures/powerup_sticky.png", {}, true,
                       "powerup_sticky");

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
  level_ = 0;

  // Load player.
  glm::vec2 playerPos{width_ / 2 - PLAYER_SIZE.x / 2, height_ - PLAYER_SIZE.y};
  player_ = std::make_unique<GameObject>(playerPos, PLAYER_SIZE,
                                         manager_.getTexture("paddle"));

  // Load ball.
  glm::vec2 ballPos{
      playerPos + glm::vec2{PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2}};
  ball_ = std::make_unique<BallObject>(ballPos, BALL_RADIUS, BALL_VELOCITY,
                                       manager_.getTexture("face"));

  // Load particle generator.
  generator_ = std::make_unique<ParticleGenerator>(
      particleShader, manager_.getTexture("particle"), 500);

  renderer_ = std::make_unique<SpriteRenderer>(spriteShader);
  effects_ = std::make_unique<PostProcessor>(
      manager_.getShader("postprocessor"), width_, height_);
}

void Game::processInput() {
  if (state_ == State::Active) {
    GLfloat velocity = PLAYER_VELOCITY * dt_;
    if (keys_[GLFW_KEY_A]) {
      if (player_->options.position.x >= 0) {
        player_->options.position.x -= velocity;
        if (ball_->stuck) {
          ball_->options.position.x -= velocity;
        }
      }
    }
    if (keys_[GLFW_KEY_D]) {
      if (player_->options.position.x <= width_ - player_->options.size.x) {
        player_->options.position.x += velocity;
        if (ball_->stuck) {
          ball_->options.position.x += velocity;
        }
      }
    }
    if (keys_[GLFW_KEY_SPACE]) {
      ball_->stuck = false;
    }
  }
}

void Game::update() {
  ball_->move(dt_, width_);
  this->handleCollisions();
  generator_->update(dt_, *ball_, 2, glm::vec2{ball_->radius / 2});
  this->updatePowerups(dt_);
  if (shakeTime_ > 0.0f) {
    shakeTime_ -= dt_;
    if (shakeTime_ <= 0.0f) {
      effects_->shake = false;
    }
  }
}

void Game::handleCollisions() {
  for (auto &tile : levels_[level_].blocks()) {
    if (!tile.destroyed) {
      auto collision = checkCollision(*ball_, tile);
      if (collision.occurred) {
        if (!tile.isSolid) {
          tile.destroyed = true;
          this->spawnPowerups(tile);
        } else {
          shakeTime_ = 0.05f;
          effects_->shake = true;
        }

        // Resolve collision.
        if (collision.direction == Direction::Left ||
            collision.direction == Direction::Right) {
          ball_->velocity.x = -ball_->velocity.x;
          GLfloat depth = ball_->radius - std::abs(collision.difference.x);
          if (collision.direction == Direction::Left) {
            ball_->options.position.x += depth;
          } else {
            ball_->options.position.x -= depth;
          }
        } else {
          ball_->velocity.y = -ball_->velocity.y;
          GLfloat depth = ball_->radius - std::abs(collision.difference.y);
          if (collision.direction == Direction::Up) {
            ball_->options.position.y -= depth;
          } else {
            ball_->options.position.y += depth;
          }
        }
      }
    }
  }

  for (auto &powerup : powerups_) {
    if (!powerup.destroyed) {
      if (powerup.options.position.y >= height_) {
        powerup.destroyed = true;
      }
      if (checkCollision(*player_, powerup)) {
        this->activatePowerup(powerup);
        powerup.destroyed = true;
        powerup.activated = true;
      }
    }
  }

  if (!ball_->stuck) {
    Collision result = checkCollision(*ball_, *player_);
    if (result.occurred) {
      GLfloat centerBoard =
          player_->options.position.x + player_->options.size.x / 2;
      GLfloat distance =
          (ball_->options.position.x + ball_->radius) - centerBoard;
      GLfloat percentage = distance / (player_->options.size.x / 2);
      GLfloat strength = 2.0f;
      glm::vec2 oldVelocity = ball_->velocity;
      ball_->velocity.x = BALL_VELOCITY.x * percentage * strength;
      ball_->velocity.y = -ball_->velocity.y;
      ball_->velocity =
          glm::normalize(ball_->velocity) * glm::length(oldVelocity);
      ball_->velocity.y = -1 * std::abs(ball_->velocity.y);
    }
  }
}

void Game::render() {
  if (state_ == State::Active) {
    {
      auto scopedRender = effects_->prepareRender();
      auto &texture = manager_.getTexture("background");
      RendererOptions options{
          {0, 0}, {width_, height_}, 0.0f, {1.0f, 1.0f, 1.0f}};
      renderer_->drawSprite(texture, options);

      player_->draw(*renderer_);
      for (auto &powerup : powerups_) {
        if (!powerup.destroyed) {
          powerup.draw(*renderer_);
        }
      }
      generator_->draw();
      ball_->draw(*renderer_);
      levels_[level_].draw(*renderer_);
    }
    effects_->render(glfwGetTime());
  }
}

void Game::updateTime(GLfloat time) {
  dt_ = time - lastTime_;
  lastTime_ = time;
}

void Game::spawnPowerups(const GameObject &block) {
  if (shouldSpawn(75)) {
    powerups_.emplace_back(Powerup::Type::Speed, glm::vec3{0.5f, 0.5f, 1.0f},
                           0.0f, block.options.position,
                           manager_.getTexture("powerup_speed"));
  }
  if (shouldSpawn(75)) {
    powerups_.emplace_back(Powerup::Type::Sticky, glm::vec3{1.0f, 0.5f, 1.0f},
                           20.0f, block.options.position,
                           manager_.getTexture("powerup_sticky"));
  }
  if (shouldSpawn(75)) {
    powerups_.emplace_back(
        Powerup::Type::PassThrough, glm::vec3{0.5f, 1.0f, 0.5f}, 10.0f,
        block.options.position, manager_.getTexture("powerup_passthrough"));
  }
  if (shouldSpawn(75)) {
    powerups_.emplace_back(
        Powerup::Type::PadSizeIncrease, glm::vec3{1.0f, 0.6f, 0.4f}, 0.0f,
        block.options.position, manager_.getTexture("powerup_increase"));
  }
  if (shouldSpawn(15)) {
    powerups_.emplace_back(Powerup::Type::Confuse, glm::vec3{1.0f, 0.3f, 0.3f},
                           15.0f, block.options.position,
                           manager_.getTexture("powerup_confuse"));
  }
  if (shouldSpawn(15)) {
    powerups_.emplace_back(Powerup::Type::Chaos, glm::vec3{0.9f, 0.25f, 0.25f},
                           15.0f, block.options.position,
                           manager_.getTexture("powerup_chaos"));
  }
}

void Game::updatePowerups(GLfloat dt) {
  for (auto &powerup : powerups_) {
    powerup.options.position += powerup.velocity * dt;
    if (powerup.activated) {
      powerup.duration -= dt;
      if (powerup.duration <= 0.0f) {
        powerup.activated = false;
        if (!isOtherPowerupActive(powerups_, powerup.type)) {
          switch (powerup.type) {
            case Powerup::Type::Sticky: {
              ball_->sticky = false;
              player_->options.color = glm::vec3{1.0f};
            } break;
            case Powerup::Type::PassThrough: {
              ball_->passthrough = false;
              ball_->options.color = glm::vec3{1.0f};
            } break;
            case Powerup::Type::Confuse:
              effects_->confuse = false;
              break;
            case Powerup::Type::Chaos:
              effects_->chaos = false;
              break;
            default:
              break;
          }
        }
      }
    }
  }

  powerups_.erase(std::remove_if(powerups_.begin(), powerups_.end(),
                                 [](const auto &powerup) {
                                   return powerup.destroyed &&
                                          !powerup.activated;
                                 }),
                  powerups_.end());
}

void Game::activatePowerup(Powerup &powerup) {
  switch (powerup.type) {
    case Powerup::Type::Speed:
      ball_->velocity *= 1.2;
      break;
    case Powerup::Type::Sticky: {
      ball_->sticky = true;
      player_->options.color = glm::vec3{1.0f, 0.5f, 1.0f};
    } break;
    case Powerup::Type::PassThrough: {
      ball_->passthrough = true;
      ball_->options.color = glm::vec3{1.0f, 0.5f, 0.5f};
    } break;
    case Powerup::Type::PadSizeIncrease:
      player_->options.size.x += 50;
      break;
    case Powerup::Type::Confuse: {
      if (!effects_->chaos) {
        effects_->confuse = true;
      }
    } break;
    case Powerup::Type::Chaos: {
      if (!effects_->confuse) {
        effects_->chaos = true;
      }
    } break;
  }
}
