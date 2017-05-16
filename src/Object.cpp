#include "Object.h"
#include <stdexcept>

GameObject blockToDrawable(ResourceManager &manager, GLuint tile, GLuint x,
                           GLuint y, GLfloat unitWidth, GLfloat unitHeight) {
  glm::vec2 pos{unitWidth * x, unitHeight * y};
  glm::vec2 size{unitWidth, unitHeight};
  if (tile == 1) {  // Solid
    GameObject obj{pos, size, manager.getTexture("block_solid"),
                   glm::vec3{0.8f, 0.8f, 0.7f}};
    obj.isSolid = true;
    return obj;
  } else if (tile > 1) {  // Non-solid
    glm::vec3 color{1.0f};
    switch (tile) {
      case 2:
        color = {0.2f, 0.6f, 1.0f};
        break;
      case 3:
        color = {0.0f, 0.7f, 0.0f};
        break;
      case 4:
        color = {0.8f, 0.8f, 0.4f};
        break;
      case 5:
        color = {1.0f, 0.5f, 0.0f};
        break;
    }
    return {pos, size, manager.getTexture("block"), color};
  }

  throw std::runtime_error{"Invalid tile"};
}
