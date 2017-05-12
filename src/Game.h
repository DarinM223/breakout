#ifndef GAME_H
#define GAME_H

#include <GL/glew.h>
#include <array>

class Game {
 public:
  enum class State : char {
    Active,
    Menu,
    Win,
  };

  Game(int width, int height);
  ~Game();
  void init();
  void processInput();
  void update();
  void render();

  void setDT(GLfloat dt) { this->dt_ = dt; }
  void setKey(int key, bool value) { this->keys_[key] = value; }

 private:
  std::array<bool, 1024> keys_{};
  State state_{State::Active};
  GLfloat dt_;
  int width_;
  int height_;
};

#endif
