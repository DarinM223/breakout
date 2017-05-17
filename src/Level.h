#ifndef LEVEL_H
#define LEVEL_H

#include <GL/glew.h>
#include <fstream>
#include <functional>
#include <sstream>
#include <vector>
#include "Object.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"

template <typename T>
using BlockToDrawable = std::function<T(ResourceManager &, GLuint, GLuint,
                                        GLuint, GLfloat, GLfloat)>;

template <typename Object>
class Level : public IsDerivedFrom<Object, Drawable> {
 public:
  Level(BlockToDrawable<Object> toDrawable) : toDrawable_(toDrawable) {}
  void load(ResourceManager &manager, const char *file, GLuint width,
            GLuint height);
  void draw(SpriteRenderer &renderer);
  bool completed() const;
  std::vector<Object> &blocks() { return bricks_; }

 private:
  BlockToDrawable<Object> toDrawable_;
  void init(ResourceManager &manager, std::vector<std::vector<GLuint>> tileData,
            GLuint width, GLuint height);
  std::vector<Object> bricks_;
};

template <typename Object>
void Level<Object>::load(ResourceManager &manager, const char *file,
                         GLuint width, GLuint height) {
  bricks_.clear();
  std::vector<std::vector<GLuint>> tileData;
  std::ifstream fstream{file};
  if (!fstream) {
    return;  // TODO(DarinM223): fix to throw error.
  }

  std::string line;
  while (std::getline(fstream, line)) {
    std::istringstream sstream{line};
    std::vector<GLuint> row;

    GLuint tileCode;
    while (sstream >> tileCode) {
      row.emplace_back(tileCode);
    }
    tileData.emplace_back(std::move(row));
  }

  if (tileData.size() > 0) {
    this->init(manager, tileData, width, height);
  }
}

template <typename Object>
void Level<Object>::draw(SpriteRenderer &renderer) {
  for (auto &obj : bricks_) {
    obj.draw(renderer);
  }
}

template <typename Object>
bool Level<Object>::completed() const {
  return true;  // TODO(DarinM223): implement this.
}

template <typename Object>
void Level<Object>::init(ResourceManager &manager,
                         std::vector<std::vector<GLuint>> tileData,
                         GLuint levelWidth, GLuint levelHeight) {
  auto height = tileData.size();
  auto width = tileData[0].size();
  auto unitWidth = levelWidth / static_cast<GLfloat>(width);
  auto unitHeight = levelHeight / static_cast<GLfloat>(height);

  for (GLuint y = 0; y < height; y++) {
    for (GLuint x = 0; x < width; x++) {
      auto tile = tileData[y][x];
      if (tile > 0) {
        auto obj = toDrawable_(manager, tile, x, y, unitWidth, unitHeight);
        bricks_.emplace_back(std::move(obj));
      }
    }
  }
}

#endif
