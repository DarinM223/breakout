#ifndef GAME_H
#define GAME_H

#include <GL/glew.h>
#include <array>
#include <memory>
#include "Level.h"
#include "Object.h"
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
  void render();
  void updateTime(GLfloat time);

  void setKey(int key, bool value) { this->keys_[key] = value; }

 private:
  std::array<bool, 1024> keys_{};
  State state_{State::Active};

  std::vector<Level<GameObject>> levels_;
  GLuint level_;

  GLfloat dt_{0.0f};
  GLfloat lastTime_{0.0f};

  ResourceManager &manager_;
  std::unique_ptr<SpriteRenderer> renderer{nullptr};
  int width_;
  int height_;
};

#endif
