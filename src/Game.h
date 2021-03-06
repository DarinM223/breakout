#ifndef GAME_H
#define GAME_H

#include <GL/glew.h>
#include <array>
#include <memory>
#include <vector>
#include "BallObject.h"
#include "Level.h"
#include "Object.h"
#include "Particle.h"
#include "PostProcessor.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"

class Game {
 public:
  enum class State : char {
    Active,
    Menu,
    Win,
  };

  Game(int width, int height, ResourceManager &manager);
  void init();
  void processInput();
  void update();
  void handleCollisions();
  void render();
  void updateTime(GLfloat time);

  void setKey(int key, bool value) { keys_[key] = value; }
  void spawnPowerups(const GameObject &block);
  void updatePowerups(GLfloat dt);
  void activatePowerup(Powerup &powerup);

 private:
  std::array<bool, 1024> keys_{};
  State state_{State::Active};

  std::vector<Level<GameObject>> levels_;
  GLuint level_;

  GLfloat dt_{0.0f};
  GLfloat lastTime_{0.0f};

  ResourceManager &manager_;
  std::unique_ptr<SpriteRenderer> renderer_{nullptr};
  std::unique_ptr<ParticleGenerator> generator_{nullptr};

  std::unique_ptr<PostProcessor> effects_{nullptr};
  GLfloat shakeTime_{0.0f};

  std::unique_ptr<GameObject> player_{nullptr};
  std::unique_ptr<BallObject> ball_{nullptr};
  std::vector<Powerup> powerups_;
  int width_;
  int height_;
};

#endif
